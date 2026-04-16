#include <memory>


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

  void call(Args... args) override
  {
    _callable(args...);
  }

private:
  Callable _callable;
};
