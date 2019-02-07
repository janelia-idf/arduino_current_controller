#include "MessageHandler.h"

enum {
  cmdGetDevInfo,                    // Done
  cmdGetCmds,                       // Done
  cmdGetRspCodes,                   // Done
  cmdSetSerialNumber,               //

  cmdTurnOnAll,
  cmdTurnOffAll,
  cmdStartPwmAll,
  cmdStopPwmAll,

  // DEVELOPMENT
  cmdDebug,
};


const int rspSuccess = 1;
const int rspError = 0;

void MessageHandler::processMsg() {
  while (Serial.available() > 0) {
    process(Serial.read());
    if (messageReady()) {
      msgSwitchYard();
      reset();
    }
  }
  return;
}

void MessageHandler::msgSwitchYard() {
  int cmd = readInt(0);
  dprint.start();
  dprint.addIntItem("cmd_id", cmd);

  switch (cmd) {

  case cmdGetDevInfo:
    handleGetDevInfo();
    break;

  case cmdGetCmds:
    handleGetCmds();
    break;

  case cmdGetRspCodes:
    handleGetRspCodes();
    break;

  case cmdSetSerialNumber:
    handleSetSerialNumber();
    break;

  case cmdTurnOnAll:
    handleTurnOnAll();
    break;

  case cmdTurnOffAll:
    handleTurnOffAll();
    break;

  case cmdStartPwmAll:
    handleStartPwmAll();
    break;

  case cmdStopPwmAll:
    handleStopPwmAll();
    break;

  // DEVELOPMENT
  case cmdDebug:
    handleDebug();
    break;

  default:
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "unknown command");
    break;
  }
  dprint.stop();
}

void MessageHandler::handleGetCmds() {
  dprint.addIntItem("status", rspSuccess);
  dprint.addIntItem("getDevInfo", cmdGetDevInfo);
  dprint.addIntItem("getCmds", cmdGetCmds);
  dprint.addIntItem("getRspCodes", cmdGetRspCodes);
  dprint.addIntItem("setArduinoSerialNumber", cmdSetSerialNumber);

  dprint.addIntItem("turnOnAll", cmdTurnOnAll);
  dprint.addIntItem("turnOffAll", cmdTurnOffAll);
  dprint.addIntItem("startPwmAll", cmdStartPwmAll);
  dprint.addIntItem("stopPwmAll", cmdStopPwmAll);

  // DEVELOPMENT
  dprint.addIntItem("cmdDebug", cmdDebug);
}

void MessageHandler::handleGetDevInfo() {
  dprint.addIntItem("status", rspSuccess);
  dprint.addIntItem("model_number",  constants::modelNumber);
  dprint.addIntItem("serial_number", savedVariables.getSerialNumber());
  dprint.addIntItem("firmware_number", constants::firmwareNumber);
  dprint.addIntItem("channel_count", constants::CHANNEL_COUNT);
}

bool MessageHandler::checkNumberOfArgs(int num) {
  bool flag = true;
  if (numberOfItems() != num) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "incorrect number of arguments");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkSerialNumberArg(int serial_number) {
  bool flag = true;
  if ((serial_number<constants::serialNumberMin) || (constants::serialNumberMax<serial_number)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "serial number argument out of range");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkPercentCurrentArg(int percent_current) {
  bool flag = true;
  if ((percent_current<constants::percentCurrentMin) || (constants::percentCurrentMax<percent_current)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "percent current argument out of range");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkDurationArg(int duration) {
  bool flag = true;
  if ((duration<constants::durationMin) || (constants::durationMax<duration)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "duration argument out of range");
    flag = false;
  }
  return flag;
}

bool MessageHandler::checkCountArg(int count) {
  bool flag = true;
  if ((count<constants::countMin) || (constants::countMax<count)) {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", "count argument out of range");
    flag = false;
  }
  return flag;
}

void MessageHandler::systemCmdRsp(bool flag) {
  if (flag) {
    dprint.addIntItem("status", rspSuccess);
  }
  else {
    dprint.addIntItem("status", rspError);
    dprint.addStrItem("err_msg", systemState.errMsg);
  }
}

void MessageHandler::handleGetRspCodes() {
  dprint.addIntItem("status", rspSuccess);
  dprint.addIntItem("rsp_success",rspSuccess);
  dprint.addIntItem("rsp_error", rspError);
}

void MessageHandler::handleSetSerialNumber() {
  if (!checkNumberOfArgs(2)) {return;}
  int serialNumber = readInt(1);
  if (checkSerialNumberArg(serialNumber)) {
    systemCmdRsp(savedVariables.setSerialNumber(serialNumber));
  }
}

void MessageHandler::handleTurnOnAll() {
  if (checkNumberOfArgs(1)) {
    systemState.turnOn();
  } else if (!checkNumberOfArgs(2)) {
    return;
  }
  int percent_current = readInt(1);
  if (checkPercentCurrentArg(percent_current)) {
    systemState.turnOn(percent_current);
    systemCmdRsp(true);
  }
}

void MessageHandler::handleTurnOffAll() {
  systemState.turnOff();
  systemCmdRsp(true);
}

void MessageHandler::handleStartPwmAll() {
  if (checkNumberOfArgs(1)) {
    systemState.startPwm();
  } else if (!checkNumberOfArgs(5)) {
    return;
  }
  int percent_current = readInt(1);
  int duration_on = readInt(2);
  int duration_off = readInt(3);
  int count = readInt(4);
  if (checkPercentCurrentArg(percent_current)
      && checkDurationArg(duration_on)
      && checkDurationArg(duration_off)
      && checkCountArg(count)) {
    systemState.startPwm(percent_current,duration_on,duration_off,count);
    systemCmdRsp(true);
  }
}

void MessageHandler::handleStopPwmAll() {
  systemState.stopPwm();
  systemCmdRsp(true);
}

// -------------------------------------------------


void MessageHandler::handleDebug() {
  char name[20];
  dprint.addIntItem("status", rspSuccess);
}


MessageHandler messageHandler;
