#include <gtest/gtest.h>
#include "SignalContainer.h"

#include <iostream>

void onProgressUpdate(int progress)
{
  std::cout << "Progress: " << progress << std::endl;
}


TEST(Disconnect, DisconnectWithWrongId)
{
  SignalContainer container;
  int connId = container.ProgressSignal.connect(onProgressUpdate);
  ASSERT_NE(connId, -1);

  // Disconnect
  bool ret = container.ProgressSignal.disconnect(connId + 1);
  EXPECT_EQ(ret, false);
}

TEST(Disconnect, DisconnectSuccess)
{
  SignalContainer container;
  int connId = container.ProgressSignal.connect(onProgressUpdate);
  ASSERT_NE(connId, -1);

  // Disconnect
  bool ret = container.ProgressSignal.disconnect(connId);
  EXPECT_EQ(ret, true);
}

TEST(Disconnect, DisconnectMultiThread)
{
  SignalContainer container;
  std::vector<int64_t> slotIds;
  for (int i = 0; i < 10; ++i)
  {
    int64_t connId = container.ProgressSignal.connect(onProgressUpdate);
    slotIds.push_back(connId);
  }

  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i)
  {
    auto t = std::thread([&container, &slotIds, i]()
      {
        bool ret = container.ProgressSignal.disconnect(slotIds[i]);
        EXPECT_EQ(ret, true);
      });

    threads.push_back(std::move(t));
  }

  for (auto& t : threads)
  {
    t.join();
  }
}