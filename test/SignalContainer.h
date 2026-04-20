#ifndef SIGNAL_CONTAINER_H
#define SIGNAL_CONTAINER_H

#include "Signal.h"

#include <string>


class SignalContainer
{
public:
  Signal<int> ProgressSignal;

  Signal<const std::string&> TextSignal;

  void run();

private:
  void updateProgress(int progress);

  void updateStatus(const std::string& status);
};


class ReceiverObject
{
public:
  void onReceivedProgressUpdate(int progress);

  void onReceiveStringUpdate(const std::string& str);
};



#endif