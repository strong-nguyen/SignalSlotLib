#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H


#include <queue>
#include <condition_variable>
#include <mutex>


class ITask
{
public:
  virtual void execute() = 0;

  virtual ~ITask() = default;
};

template <typename Callable>
class TaskImpl : public ITask
{
public:
  TaskImpl(Callable&& callable) : _callable(std::forward<Callable>(callable))
  {

  }

  void execute() override
  {
    _callable();
  }

private:
  Callable _callable;
};


class TaskQueue
{
public:
  template <typename Callable>
  void pushTask(Callable&& callable)
  {
    _tasks.push(std::make_unique<TaskImpl<std::decay_t<Callable>>>(std::forward<Callable>(callable)));
    _cv.notify_one();
  }

  void run()
  {
    while (true)
    {
      std::unique_ptr<ITask> task;
      {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]()
          {
            return !_tasks.empty() || _isStop;
          });

        if (_isStop && _tasks.empty())
        {
          // Stop when run all of tasks
          return;
        }

        if (!_tasks.empty())
        {
          task = std::move(_tasks.front());
          _tasks.pop();
        }
      }

      if (task)
      {
        task->execute();
      }
    }
  }

  void stop()
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _isStop = true;
    _cv.notify_all();
  }


private:
  std::queue<std::unique_ptr<ITask>> _tasks;
  std::condition_variable _cv;
  std::mutex _mutex;
  bool _isStop = false;
};


#endif

