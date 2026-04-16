#pragma once

#include "Slot.h"


#include <functional>  // In the first version, we use std::function for faster impl => optimize later
#include <list>
#include <memory>
#include <mutex>

/*
* A Signal class can accept arbitrary number of arguments and type, then it is a good candidate to apply template
*/
template<typename... Args>
class Signal
{
public:

  ~Signal()
  {
    std::lock_guard<std::mutex> lock(_mutex);

    for (const auto& slot : _slots)
    {
      delete slot;
    }
    _slots.clear();
  }

  template <typename Callable>
  void connect(Callable&& slot)
  {
    std::lock_guard<std::mutex> lock(_mutex);

    // Use std::decay_t to remove Callable to the basic type one
    _slots.push_back(new SlotImpl<std::decay_t<Callable>, Args...>(
        std::forward<Callable>(slot)));
  }

  // Another overload accept a class member method
  template<typename T>
  void connect(T * instance, void (T:: * method)(Args...))
  {
    if (!instance)
    {
      return;
    }
    connect(std::bind_front(method, instance));
  }

  void emit(Args... args)
  {
    std::list<Slot<Args...>*> tempSlots;
    {
      std::lock_guard<std::mutex> lock(_mutex);
      tempSlots =_slots;
    }

    // Currently, slot run sequently one by one in the same thread as signal
    for (const auto& slot : tempSlots)
    {
      slot->call(args...);
    }
  }

private:
  std::list<Slot<Args...>*> _slots;

  std::mutex _mutex;
};