#include <Streaming.h>
#include <EEPROM.h>

#include "SerialReceiver.h"
#include "DictPrinter.h"
#include "TimeTriggeredScheduler.h"
#include "NewhavenDisplay.h"
#include "Encoder.h"

#include "constants.h"
#include "SavedVariables.h"
#include "SystemState.h"
#include "MessageHandler.h"

void setup() {
  Serial.begin(constants::baudrate);
  systemState.initialize();
}

void loop() {
  messageHandler.processMsg();
  systemState.update();
}
