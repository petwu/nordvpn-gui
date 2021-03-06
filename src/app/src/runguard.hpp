#ifndef RUNGUARD_HPP
#define RUNGUARD_HPP

#include <QLocalServer>
#include <QObject>
#include <QSharedMemory>
#include <QString>
#include <QSystemSemaphore>
#include <functional>
#include <memory>

/**
 * @brief The RunGuard class provides a way to protect an application from
 * running more than once. In other words, it allows to create single instance
 * applications. Therefore this class should be used as one of the first things
 * inside main(), before the application is initialized (e.g. an instance if
 * QApplication or similar class is created).
 *
 * @details The guard is implemented with the help a shared memory block
 * (QSharedMemory). Additionally a server-socket (QLocalServer and QLocalSocket)
 * combination is used to notify the primary instance about any further
 * instantiation attempts.
 *
 * @note Base on:
 * https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection#answer-28172162
 */
class RunGuard : QObject {
  public:
    /**
     * @brief RunGuard constructor.
     * @param key Unique string that identified the application. Gets hashed and
     * then used as a key for a shared memory block and a system semaphore.
     */
    RunGuard(const QString &key);

    /**
     * @brief Destructor to release the shared memory.
     */
    ~RunGuard() override;

    /**
     * @brief Disable copy constructor.
     */
    RunGuard(const RunGuard &) = delete;

    /**
     * @brief Disable copy assignment.
     */
    auto operator=(const RunGuard &) -> RunGuard & = delete;

    /**
     * @brief Disable move constructor.
     */
    RunGuard(RunGuard &&) = delete;

    /**
     * @brief Disable move assignment.
     */
    auto operator=(RunGuard &&) -> RunGuard & = delete;

    /**
     * @brief Checks whether the application may be run.
     * @return true, if no other instance is currently running and the
     * application may run, false otherwise.
     */
    auto tryToRun() -> bool;

    /**
     * @brief Set a callback function that is called when someone tried to
     * create a second application instance but this attemp was blocked by the
     * RunGuard. This might be used to show and focus a minimized application.
     */
    void setOnSecondaryInstanceBlockedHandler(std::function<void()> callback);

  private:
    const QString _key;

    /** @brief
     * Shared memory for interprocess communication (IPC) to check if
     * another instance is already running.
     */
    QSharedMemory _sharedMem;
    /**
     * @brief Loch for accessing `_sharedMem`.
     */
    QSystemSemaphore _memLock;

    /**
     * @brief Local server for interprocess communication (IPC). Required to let
     * the primary instance know about any future instantiation attempts.
     */
    std::unique_ptr<QLocalServer> _ipcServer;

    /**
     * @brief _hash calculates a cryptographic hash based an the provided key
     * and salt.
     * @param key The key to identify the application.
     * @param salt Additional (random) data.
     * @return The hash value of key+salt.
     */
    static auto _hash(const QString &key, const QString &salt) -> QString;

    /**
     * @brief Check for running application instances.
     * @details The check is done by trying to aquire a shared memory block. If
     * that fails, the shared memory is alreay held by a primary isntance.
     * @return true, if another instance is arealy running, false otherwise.
     */
    auto _isAnotherRunning() -> bool;

    /**
     * @brief Release the shared memory and thereby allowing other instances to
     * be created.
     */
    void _releaseSharedMem();

    /**
     * @brief Callback that is called when someone tried to create a second
     * application instance but this attemp was blocked by the RunGuard. This
     * might be used to show and focus a minimized application.
     */
    std::function<void()> _onSecondaryInstanceBlocked = [] {};
};

#endif // RUNGUARD_HPP
