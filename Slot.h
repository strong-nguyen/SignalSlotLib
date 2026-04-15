


template <typename... Args>
class Slot
{
public:
  virtual void call() = 0;

  virtual ~Slot() = default;
};