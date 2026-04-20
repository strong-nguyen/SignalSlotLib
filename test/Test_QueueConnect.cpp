#include <gtest/gtest.h>
#include <TaskQueue.h>
#include "SignalContainer.h"

#include <thread>



class QueueConnectTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    _taskQueue.run();
  }

  void TearDown() override
  {
    _taskQueue.stop(true);
  }

  TaskQueue _taskQueue;
};

TEST_F(QueueConnectTest, QueueConnect)
{
  std::thread::id mainId = std::this_thread::get_id();
  SignalContainer signalContainer;
  signalContainer.ProgressSignal.queueConnect(
    [mainId](int progress)
    {
      std::cout << "Progress: " << progress << std::endl;
      EXPECT_NE(mainId, std::this_thread::get_id());
    },
    &_taskQueue);

  signalContainer.run();
  _taskQueue.stop(true);
}


TEST_F(QueueConnectTest, QueueConnectFailed)
{
  SignalContainer signalContainer;
  int64_t connId = signalContainer.TextSignal.queueConnect((ReceiverObject*)nullptr, &ReceiverObject::onReceiveStringUpdate, &_taskQueue);
  EXPECT_EQ(connId, -1);
}

TEST_F(QueueConnectTest, QueueConnectSuccess)
{
  ReceiverObject o;
  SignalContainer signalContainer;
  int64_t connId = signalContainer.TextSignal.queueConnect(&o, &ReceiverObject::onReceiveStringUpdate, &_taskQueue);
  EXPECT_NE(connId, -1);
  signalContainer.run();
  _taskQueue.stop(true);
}