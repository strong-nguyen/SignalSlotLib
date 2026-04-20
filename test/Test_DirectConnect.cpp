#include <gtest/gtest.h>
#include "SignalContainer.h"

#include <thread>
#include <vector>
#include <atomic>


TEST(DirectConnect, TestConnectSuccess)
{
  SignalContainer signalContainer;
  int64_t connId1 = signalContainer.ProgressSignal.connect([](int progress) {
    });
  ASSERT_NE(connId1, -1);

  int64_t connId2 = signalContainer.ProgressSignal.connect([](int progress) {
    });
  ASSERT_NE(connId1, -1);
}

TEST(DirectConnect, TestConnectWithReceiverObj)
{
  ReceiverObject obj;
  SignalContainer signalContainer;
  int64_t connId = signalContainer.ProgressSignal.connect((ReceiverObject*)nullptr, &ReceiverObject::onReceivedProgressUpdate);
  ASSERT_EQ(connId, -1);

  connId = signalContainer.ProgressSignal.connect(&obj, &ReceiverObject::onReceivedProgressUpdate);
  ASSERT_NE(connId, -1);
}

TEST(DirectConnect, TestExecuteSlot)
{
  SignalContainer signalContainer;
  signalContainer.ProgressSignal.connect([](int progress) {
    static int i = 0;
    if (i == 0)
    {
      EXPECT_EQ(progress, 0);
    }
    else if (i == 1)
    {
      EXPECT_EQ(progress, 50);
    }
    else if (i == 2)
    {
      EXPECT_EQ(progress, 100);
    }
    ++i;
    });
  signalContainer.run();
}

TEST(DirectConnect, TestConnectMultiThread)
{
  SignalContainer signalContainer;

  std::vector< std::unique_ptr<std::thread>> threads;
  for (int i = 0; i < 10; ++i)
  {
    threads.push_back(std::make_unique<std::thread>([&signalContainer]() {
      signalContainer.ProgressSignal.connect([](int progress)
        {
          static std::atomic<int> i = 0;
          if (i / 10 == 0)
          {
            EXPECT_EQ(progress, 0);
          }
          else if (i / 10 == 1)
          {
            EXPECT_EQ(progress, 50);
          }
          else if (i / 10 == 2)
          {
            EXPECT_EQ(progress, 100);
          }
          ++i;
        });
      }));
  }

  for (const auto& t : threads)
  {
    t->join();
  }

  signalContainer.run();
}

