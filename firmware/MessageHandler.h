#ifndef _MESSAGE_HANDER_H_
#define _MESSAGE_HANDER_H_
#include <util/atomic.h>
#include <SerialReceiver.h>
#include "DictPrinter.h"
#include "constants.h"
#include "SavedVariables.h"
#include "Streaming.h"
#include "SystemState.h"
#include "EncoderVariables.h"


class MessageHandler : public SerialReceiver {

 public:
  void processMsg();

 private:
  DictPrinter dprint;
  void msgSwitchYard();
  void handleGetDevInfo();
  void handleGetCmds();

  bool checkNumberOfArgs(int num);
  bool checkSerialNumberArg(int serial_number);
  bool checkPercentCurrentArg(int percent_current);
  bool checkDurationArg(int duration);
  bool checkCountArg(int count);
  void systemCmdRsp(bool flag);

  void handleGetRspCodes();
  void handleSetSerialNumber();

  void handleTurnOnAll();
  void handleTurnOffAll();
  void handleStartPwmAll();
  void handleStopPwmAll();

  // Development
  void handleDebug();
};

extern MessageHandler messageHandler;
#endif
