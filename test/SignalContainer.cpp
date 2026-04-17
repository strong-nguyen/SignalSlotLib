#include "SignalContainer.h"

#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

void SignalContainer::updateProgress(int progress)
{
  ProgressSignal.emit(progress);
}

void SignalContainer::run()
{
  std::cout << "Start\n";
  updateProgress(0);
  std::this_thread::sleep_for(2s);

  updateProgress(50);

  std::this_thread::sleep_for(2s);
  updateProgress(100);
  std::cout << "Completed\n";
}

void ReceiverObject::onReceivedProgressUpdate(int progress)
{
  std::cout << "Object " << this << " received " << progress << std::endl;
}