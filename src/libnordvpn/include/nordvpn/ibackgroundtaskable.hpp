#ifndef IBACKGROUNDTASKABLE_HPP
#define IBACKGROUNDTASKABLE_HPP

/**
 * @brief The BackgroundTaskable interface provides some kind of task that gets
 * periodically executed by a background thread.
 *
 * @details See the various `register*()` functions on how to set up one or more
 * background tasks that should be started upon #startBackgroundTasks().
 * A good place for calling at least one `register*()` function is the
 * constructor of the derived class.
 */
class IBackgroundTaskable {
  public:
    /**
     * @brief Start all registered background tasks.
     */
    virtual void startBackgroundTasks() = 0;

    /**
     * @brief Terminate the all background tasks after they finished the
     * (possibly) currently executing iteration.
     */
    virtual void stopBackgroundTasks() = 0;
};

#endif // IBACKGROUNDTASKABLE_HPP
