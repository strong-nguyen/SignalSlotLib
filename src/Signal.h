#ifndef SIGNAL_H
#define SIGNAL_H

#include "Slot.h"
#include "TaskQueue.h"

#include <functional>
#include <memory>
#include <mutex>
#include <atomic>
#include <map>


/*
* Generate unique id start from 1 and it is thread-safe
*/
class IdGenerator
{
public:
  static int64_t next()
  {
    static std::atomic<int64_t> id = 0;
    return ++id;
  }
};


/*
 * A Signal class can accept arbitrary number of arguments and type, then it is a good candidate to apply template
 */
template <typename... Args>
class Signal
{
public:
  ~Signal()
  {
    std::lock_guard<std::mutex> lock(_mutex);

    for (const auto &[_, slot] : _slots)
    {
      delete slot;
    }
    _slots.clear();
  }

  template <typename Callable>
  int64_t connect(Callable&& slot)
  {
    int64_t slotId = IdGenerator::next();

    // Use std::decay_t to remove Callable to the basic type one
    _slots.emplace(slotId, new SlotImpl<std::decay_t<Callable>, Args...>(
                               std::forward<Callable>(slot)));

    return slotId;
  }

  // Another overload accept a class member method
  template <typename T>
  int64_t connect(T *receiver, void (T::*method)(Args...))
  {
    if (!receiver)
    {
      return -1;
    }
    return connect(std::bind_front(method, receiver));
  }

  template <typename Callable>
  int64_t queueConnect(Callable&& slot, TaskQueue* taskQueue)
  {
    int64_t slotId = IdGenerator::next();

    // Use std::decay_t to remove Callable to the basic type one
    _slots.emplace(slotId, new SlotImpl<std::decay_t<Callable>, Args...>(
      std::forward<Callable>(slot),
      ConnectionType::Queued,
      taskQueue));

    return slotId;
  }

  // Another overload accept a class member method
  template <typename T>
  int64_t queueConnect(T* receiver, void (T::* method)(Args...), TaskQueue* taskQueue)
  {
    if (!receiver)
    {
      return -1;
    }
    return queueConnect(std::bind_front(method, receiver), taskQueue);
  }


  // Disconnect a slot using slot id
  // return true if disconnect sucess, else false
  bool disconnect(int64_t slotId)
  {
    std::lock_guard<std::mutex> lock(_mutex);
    if (auto it = _slots.find(slotId); it != _slots.end())
    {
      _slots.erase(it);
      return true;
    }
    
    return false;
  }

  // Emit signal and run all slot directly
  void emit(Args... args)
  {
    std::map<int64_t, Slot<Args...> *> tempSlots;
    {
      std::lock_guard<std::mutex> lock(_mutex);
      tempSlots = _slots;
    }

    // Slot will be called and it will executed directly or in the task queue depends on the connection type
    for (const auto &[_, slot] : tempSlots)
    {
      slot->call(args...);
    }
  }

private:
  std::map<int64_t, Slot<Args...> *> _slots;

  std::mutex _mutex;
};

#endif