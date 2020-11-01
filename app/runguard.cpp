#include "runguard.h"

QString RunGuard::_hash(const QString &key, const QString &salt) {
    QByteArray data;

    data.append(key.toUtf8());
    data.append(salt.toUtf8());
    data = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();

    return data;
}

RunGuard::RunGuard(const QString &key)
    : _sharedMem(_hash(key, "_sharedMem")),
      _memLock(_hash(key, "_memLock"), 1) {
    _memLock.acquire();
    {
        QSharedMemory fix(
            this->_sharedMem
                .key()); // fix for *nix: http://habrahabr.ru/post/173281/
        fix.attach();
    }
    _memLock.release();
}

RunGuard::~RunGuard() { //
    this->_release();
}

bool RunGuard::_isAnotherRunning() {
    if (_sharedMem.isAttached())
        return false;

    _memLock.acquire();
    const bool isRunning = _sharedMem.attach();
    if (isRunning)
        _sharedMem.detach();
    _memLock.release();

    return isRunning;
}

bool RunGuard::tryToRun() {
    if (this->_isAnotherRunning())
        return false;

    _memLock.acquire();
    const bool result = _sharedMem.create(sizeof(quint64));
    _memLock.release();
    if (!result) {
        this->_release();
        return false;
    }

    return true;
}

void RunGuard::_release() {
    _memLock.acquire();
    if (_sharedMem.isAttached())
        _sharedMem.detach();
    _memLock.release();
}
