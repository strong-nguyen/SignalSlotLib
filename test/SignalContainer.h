#ifndef SIGNAL_CONTAINER_H
#define SIGNAL_CONTAINER_H

#include "Signal.h"


class SignalContainer
{
public:
  Signal<int> ProgressSignal;

  void updateProgress(int progress);

  void run();
};


class ReceiverObject
{
public:
  void onReceivedProgressUpdate(int progress);
};



#endif