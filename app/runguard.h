#ifndef RUNGUARD_H
#define RUNGUARD_H

#include <QCryptographicHash>
#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>

/**
 * @brief The RunGuard class provides a way to protect an application from
 * running more than once. In other words, it allows to create single instance
 * applications. Therefore this class should be used as one of the first things
 * inside main(), before the application is initialized (e.g. an instance if
 * QApplication or similar class is created).
 *
 * Base on:
 * https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection#answer-28172162
 */
class RunGuard {
  public:
    RunGuard(const QString &key);
    ~RunGuard();

    /**
     * @brief Checks whether the application may be run.
     * @return true, if no other instance is currently running and the
     * application may run, false otherwise.
     */
    bool tryToRun();

  private:
    /** @brief
     * Shared memory for interprocess communication (IPC) to check if
     * another instance is already running.
     */
    QSharedMemory _sharedMem;
    /**
     * @brief Loch for accessing #_sharedMem.
     */
    QSystemSemaphore _memLock;

    /**
     * @brief _hash calculates a cryptographic hash based an the provided key
     * and salt.
     * @param key The key to identify the application.
     * @param salt Additional (random) data.
     * @return The hash value of key+salt.
     */
    QString _hash(const QString &key, const QString &salt);

    /**
     * @brief Check for running application instances.
     * @return true, if another instance is arealy running, false otherwise.
     */
    bool _isAnotherRunning();

    /**
     * @brief Release the shared memory and thereby allowing other instances to
     * be created.
     */
    void _release();

    // disable copying
    RunGuard(const RunGuard &) = delete;
    RunGuard &operator=(const RunGuard &) = delete;
};

#endif // RUNGUARD_H
