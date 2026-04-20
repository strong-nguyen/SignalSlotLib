#include "SignalContainer.h"

#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

void SignalContainer::updateProgress(int progress)
{
  ProgressSignal.emit(progress);
}

void SignalContainer::updateStatus(const std::string& status)
{
  TextSignal.emit(status);
}

void SignalContainer::run()
{
  updateStatus("Start");
  updateProgress(0);
  std::this_thread::sleep_for(2s);

  updateProgress(50);

  std::this_thread::sleep_for(2s);
  updateProgress(100);
  updateStatus("Completed");
}

void ReceiverObject::onReceivedProgressUpdate(int progress)
{
  std::cout << "Object " << this << " received " << progress << std::endl;
}

void ReceiverObject::onReceiveStringUpdate(const std::string& str)
{
  std::cout << "Object " << this << " received " << str << std::endl;
}
