#ifndef BACKGROUNDTASKABLE_HPP
#define BACKGROUNDTASKABLE_HPP

#include <atomic>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "nordvpn/ibackgroundtaskable.hpp"

using namespace std::placeholders;

/**
 * @brief The BackgroundTaskable class provides an abstract class than
 * can be extended/implemented by any class, that wants to provide some kind of
 * task that gets periodically executed by a background thread.
 *
 * @details See the various `register*()` functions on how to set up one or more
 * background tasks that should be started upon #startBackgroundTasks().
 * A good place for calling at least one `register*()` function is the
 * constructor of the derived class.
 */
class BackgroundTaskable : virtual IBackgroundTaskable {
  public:
    void startBackgroundTasks() override {
        for (auto &_backgroundTask : this->_backgroundTasks) {
            _backgroundTask->startBackgroundTask();
        }
    }

    void stopBackgroundTasks() override {
        for (auto &_backgroundTask : this->_backgroundTasks) {
            _backgroundTask->stopBackgroundTask();
        }
    }

  protected:
    /**
     * @brief Register a new function to be executed as a background task after
     * #startBackgroundTasks() gets called.
     * @param task The task to executed. For member functions use
     * `std::bind(&MyClass::myFunc, this)` or a lamdba `[this] { myFunc() }`.
     * @param interval The time to wait between to executions of `task`.
     */
    void registerBackgroundTask(const std::function<void()> &task,
                                const std::chrono::milliseconds &interval) {
        this->_backgroundTasks.push_back(
            std::make_unique<BackgroundTask>(task, nullptr, interval));
    }

    /**
     * @brief Register a new function to be executed as a background task after
     * #startBackgroundTasks() gets called.
     * @param task The task to executed. For member functions use
     * `std::bind(&MyClass::myFunc, this)` or a lamdba `[this] { myFunc() }`.
     * @param init Function that gets called once after #startBackgroundTasks()
     * to initialize the recurring executions of `task`.
     * @param interval The time to wait between to executions of `task`.
     */
    void registerBackgroundTask(const std::function<void()> &task,
                                const std::function<void()> &init,
                                const std::chrono::milliseconds &interval) {
        this->_backgroundTasks.push_back(
            std::make_unique<BackgroundTask>(task, init, interval));
    }

    /**
     * @brief Register a new function to be executed as a background task after
     * #startBackgroundTasks() gets called.
     * @param task The task to executed. For member functions use
     * `std::bind(&MyClass::myFunc, this)` or a lamdba `[this] { myFunc() }`.
     * @param interval The time to wait between to executions of `task`.
     * @param specialTickInterval Second interval for special events, e.g. to
     * perform some extra work every n-th execution of `task`,
     * `specialTickInterval` should be `n * interval`. When the special tick
     * occurs, true is passed to `task`.
     * @param triggerSpecialTickFirstTime The special tick can be optionally
     * trigger the first time `task` is executed after #startBackgroundTasks().
     */
    void
    registerBackgroundTask(const std::function<void(bool)> &task,
                           const std::chrono::milliseconds &interval,
                           const std::chrono::milliseconds &specialTickInterval,
                           const bool triggerSpecialTickFirstTime = false) {
        this->_backgroundTasks.push_back(
            std::make_unique<BackgroundTaskWithSpecialTick>(
                task, nullptr, interval, specialTickInterval,
                triggerSpecialTickFirstTime));
    }

    /**
     * @brief Register a new function to be executed as a background task after
     * #startBackgroundTasks() gets called.
     * @param task The task to executed. For member functions use
     * `std::bind(&MyClass::myFunc, this)` or a lamdba `[this] { myFunc() }`.
     * @param init Function that gets called once after #startBackgroundTasks()
     * to initialize the recurring executions of `task`.
     * @param interval The time to wait between to executions of `task`.
     * @param specialTickInterval Second interval for special events, e.g. to
     * perform some extra work every n-th execution of `task`,
     * `specialTickInterval` should be `n * interval`. When the special tick
     * occurs, true is passed to `task`.
     * @param triggerSpecialTickFirstTime The special tick can be optionally
     * trigger the first time `task` is executed after #startBackgroundTasks().
     */
    void
    registerBackgroundTask(const std::function<void(bool)> &task,
                           const std::function<void()> &init,
                           const std::chrono::milliseconds &interval,
                           const std::chrono::milliseconds &specialTickInterval,
                           const bool triggerSpecialTickFirstTime = false) {
        this->_backgroundTasks.push_back(
            std::make_unique<BackgroundTaskWithSpecialTick>(
                task, init, interval, specialTickInterval,
                triggerSpecialTickFirstTime));
    }

  private:
    /**
     * @brief IBackgroundTask is an abstract class that can be implemented by
     * different types of background tasks providing different functionalities.
     * IBackgroundTask defines the common interface between these concrete
     * implementations.
     */
    class IBackgroundTask {
      public:
        IBackgroundTask() = default;
        IBackgroundTask(const IBackgroundTask &) = delete;
        auto operator=(const IBackgroundTask &) -> IBackgroundTask & = delete;
        IBackgroundTask(IBackgroundTask &&) = delete;
        auto operator=(IBackgroundTask &&) -> IBackgroundTask & = delete;
        virtual ~IBackgroundTask() = default;

        /**
         * @brief Start the background task in a separate daemon thread.
         */
        void startBackgroundTask() {
            if (this->_performBackgroundTask) {
                return;
            }
            this->_performBackgroundTask = true;
            std::thread(&IBackgroundTask::backgroundTask, this).detach();
        }

        /**
         * @brief Stop the background task. Currently run actions are finished
         * and not interrupted.
         */
        void stopBackgroundTask() { //
            this->_performBackgroundTask = false;
        }

      protected:
        /**
         * @brief The background task responsible for running in a loop while
         * #performBackgroundTask() returns true. Pauses between two interations
         * can be implemented using std::this_thread::sleep_for().
         */
        virtual void backgroundTask() = 0;

        /**
         * @brief Returns true after #startBackgroundTask() was called and as
         * long as #stopBackgroundTask() is not called.
         */
        auto performBackgroundTask() -> bool {
            return this->_performBackgroundTask;
        }

      private:
        /**
         * @brief Backing field for #performBackgroundTask().
         */
        std::atomic<bool> _performBackgroundTask = false;
    };

    /**
     * @brief The BackgroundTask class implements IBackgroundTask for a simple
     * void(void) function to be executed in a specific interval and with an
     * optional initializer function.
     */
    class BackgroundTask : public IBackgroundTask {
      public:
        BackgroundTask(std::function<void()> task, std::function<void()> init,
                       const std::chrono::milliseconds &interval)
            : _task(std::move(task)), _init(std::move(init)),
              _interval(interval) {}

      private:
        const std::function<void()> _task;
        const std::function<void()> _init;
        const std::chrono::milliseconds _interval;

        void backgroundTask() override {
            if (this->_init != nullptr) {
                this->_init();
            }
            while (this->performBackgroundTask()) {
                if (this->_task != nullptr) {
                    this->_task();
                }
                std::this_thread::sleep_for(this->_interval);
            }
        }
    };

    /**
     * @brief The BackgroundTaskWithSpecialTick class implements
     * IBackgroundTask. It provides similar functionality to BackgroundTask, but
     * extends it by "special ticks". A special tick is a event occuring in a
     * second interval that is bigger than the first one. This can be used to
     * time things like "every nth time the background task should perform some
     * additional action".
     */
    class BackgroundTaskWithSpecialTick : public IBackgroundTask {
      public:
        BackgroundTaskWithSpecialTick(
            std::function<void(bool)> task, std::function<void()> init,
            const std::chrono::milliseconds &interval,
            const std::chrono::milliseconds &specialTickInterval,
            const bool triggerSpecialTickFirstTime)
            : _task(std::move(task)), _init(std::move(init)),
              _interval(interval), _specialTickInterval(specialTickInterval),
              _triggerSpecialTickFirstTime(triggerSpecialTickFirstTime) {}

      private:
        const std::function<void(bool)> _task;
        const std::function<void()> _init;
        const std::chrono::milliseconds _interval;
        const std::chrono::milliseconds _specialTickInterval;
        const bool _triggerSpecialTickFirstTime;

        void backgroundTask() override {
            // init
            if (this->_init != nullptr) {
                this->_init();
            }
            // keep track of the time since the last special tick
            auto timeSinceLastSpecialTick =
                std::chrono::high_resolution_clock::now();
            bool triggerSpecialTickFirstTime =
                this->_triggerSpecialTickFirstTime;
            // event loop to periodically trigger the background task
            while (this->performBackgroundTask()) {
                // check if it is a special tick
                bool isSpecialTick = false;
                if (this->_specialTickInterval.count() > 0) {
                    if (triggerSpecialTickFirstTime) {
                        isSpecialTick = true;
                        triggerSpecialTickFirstTime = false;
                    } else if (std::chrono::duration_cast<
                                   std::chrono::milliseconds>(
                                   std::chrono::high_resolution_clock::now() -
                                   timeSinceLastSpecialTick) >=
                               this->_specialTickInterval) {
                        isSpecialTick = true;
                        // += not -=, because we have a time_point not a
                        // duration
                        timeSinceLastSpecialTick += this->_specialTickInterval;
                    }
                }
                // run the actual background task
                if (this->_task != nullptr) {
                    this->_task(isSpecialTick);
                }
                // sleep
                std::this_thread::sleep_for(this->_interval);
            }
        }
    };

    /**
     * @brief List of registered background tasks.
     */
    std::vector<std::unique_ptr<IBackgroundTask>> _backgroundTasks;
};

#endif // BACKGROUNDTASKABLE_HPP
