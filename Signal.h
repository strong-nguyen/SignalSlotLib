#pragma once


#include <functional>  // In the first version, we use std::function for faster impl => optimize later
#include <list>

/*
* A Signal class can accept arbitrary number of arguments and type, then it is a good candidate to apply template
*/
template<typename... Args>
class Signal
{
public:
  using Callback = std::function<void(Args...)>;

  void connect(Callback slot)
  {
    _slots.push_back(slot);
  }

  // Another overload accept a class member method
  template<typename T, typename... Args>
  void connect(T* instance, void (T::* method)(Args...))
  {
    _slots.push_back(std::bind_front(method, instance));
  }

  void emit(Args... args)
  {
    // Currently, slot run sequently one by one in the same thread as signal
    for (const auto& slot : _slots)
    {
      slot(std::forward<Args>(args)...);
    }
  }

private:
  std::list<Callback> _slots;
};