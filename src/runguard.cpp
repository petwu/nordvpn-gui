#include "runguard.h"

#include <QByteArray>
#include <QLocalSocket>
#include <QtCore>
#include <utility>

auto RunGuard::_hash(const QString &key, const QString &salt) -> QString {
    QByteArray data;

    data.append(key.toUtf8());
    data.append(salt.toUtf8());
    data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

    return data;
}

RunGuard::RunGuard(const QString &key)
    : _key(key), _sharedMem(_hash(key, "_sharedMem")),
      _memLock(_hash(key, "_memLock"), 1) {
    this->_memLock.acquire();
    {
        // Fix for Unix systems:
        // If the application terminated abnormal (e.g. crashed), the next call
        // to attach() will be successful for whatever reason ...
        // The solution is to simple attach and detach (when fix is out of
        // scope) to the shared memory once before being used seriously.
        // (http://habrahabr.ru/post/173281/)
        QSharedMemory fix(this->_sharedMem.key());
        fix.attach();
    }
    this->_memLock.release();
}

RunGuard::~RunGuard() {
    // release shared memory
    this->_releaseSharedMem();
    // delete the local IPC server (only primary instance)
    if (this->_ipcServer != nullptr) {
        this->_ipcServer->close();
    }
}

auto RunGuard::_isAnotherRunning() -> bool {
    if (_sharedMem.isAttached()) {
        return false;
    }
    this->_memLock.acquire();
    const bool isRunning = this->_sharedMem.attach();
    if (isRunning) {
        this->_sharedMem.detach();
    }
    this->_memLock.release();
    return isRunning;
}

auto RunGuard::tryToRun() -> bool {
    bool canRun = true;
    // check if another instance is running
    if (this->_isAnotherRunning()) {
        canRun = false;
    } else {
        // if no instance is running yet, create the shared memory to let any
        // future instances know, that a primary instance alreay exists
        this->_memLock.acquire();
        const bool result = this->_sharedMem.create(sizeof(quint64));
        this->_memLock.release();
        if (!result) {
            this->_releaseSharedMem();
            canRun = false;
        }
    }

    // take steps to notify or get notified by other instances
    if (canRun) {
        // If canRun is true, the the application is assumed to continue
        // running. In that case we start a local server that listes under _key
        // to get notified about any future instance creation attempt. Every
        // time this happens, onSecondaryInstanceBlocked gets invoked to let the
        // holder of the guard know about the failed instance creation.
        QLocalServer::removeServer(this->_key);
        if (this->_ipcServer == nullptr) {
            this->_ipcServer = std::make_unique<QLocalServer>();
        }
        this->_ipcServer->listen(this->_key);
        RunGuard::connect(this->_ipcServer.get(), &QLocalServer::newConnection,
                          [this] { this->_onSecondaryInstanceBlocked(); });
    } else {
        // If canRun is false this means that another instance is already
        // running. In that case we use a local socker to connect to the local
        // server of the primary instance (see if branch above) to notify it
        // about the instantiation attempt.
        QLocalSocket ipcSocket;
        ipcSocket.connectToServer(this->_key);
        ipcSocket.close();
    }
    return canRun;
}

void RunGuard::setOnSecondaryInstanceBlockedHandler(
    std::function<void()> callback) {
    this->_onSecondaryInstanceBlocked = std::move(callback);
}

void RunGuard::_releaseSharedMem() {
    this->_memLock.acquire();
    if (this->_sharedMem.isAttached()) {
        this->_sharedMem.detach();
    }
    this->_memLock.release();
}
