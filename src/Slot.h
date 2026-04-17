#ifndef SLOT_H
#define SLOT_H

#include "TaskQueue.h"


#include <memory>



enum class ConnectionType
{
  Direct = 0,
  Queued
};

template <typename... Args>
class Slot
{
public:
  virtual void call(Args... args) = 0;

  virtual ~Slot() = default;
};


template <typename Callable, typename... Args>
class SlotImpl : public Slot<Args...>
{
public:
  SlotImpl(Callable&& callable) : _callable(callable)
  {

  }

  SlotImpl(Callable&& callable, ConnectionType connType, TaskQueue* taskQueue)
    :
    _callable(callable),
    _connType(connType),
    _taskQueue(taskQueue)
  {

  }

  void call(Args... args) override
  {
    if (_connType == ConnectionType::Direct)
    {
      _callable(args...);
    }
    else if (_connType == ConnectionType::Queued && _taskQueue)
    {
      _taskQueue->pushTask([this, args...]()
        {
          _callable(args...);
        });
    }
  }

private:
  Callable _callable;

  ConnectionType _connType = ConnectionType::Direct;

  TaskQueue* _taskQueue = nullptr;
};


#endif