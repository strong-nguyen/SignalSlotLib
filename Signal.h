#pragma once

#include "Slot.h"


#include <functional>  // In the first version, we use std::function for faster impl => optimize later
#include <list>
#include <memory>

/*
* A Signal class can accept arbitrary number of arguments and type, then it is a good candidate to apply template
*/
template<typename... Args>
class Signal
{
public:

  template <typename Callable>
  void connect(Callable&& slot)
  {
    // Use std::decay_t to remove Callable to the basic type one
    _slots.push_back(std::make_unique<
      SlotImpl<std::decay_t<Callable>, Args...>>(
        std::forward<Callable>(slot)));
  }

  // Another overload accept a class member method
  template<typename T>
  void connect(T * instance, void (T:: * method)(Args...))
  {
    connect(std::bind_front(method, instance));
  }

  void emit(Args... args)
  {
    // Currently, slot run sequently one by one in the same thread as signal
    for (const auto& slot : _slots)
    {
      slot->call(args...);
    }
  }

private:
  std::list<std::unique_ptr<Slot<Args...>>> _slots;
};