// SavedVariables.h
#ifndef _SAVEDVARIABLES_H_
#define _SAVEDVARIABLES_H_

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif
#include <EEPROM.h>
#include "constants.h"


class SavedVariables {

 public:

  SavedVariables();
  int getSerialNumber();
  bool setSerialNumber(int serial_number);

 private:

  enum {eepromAddressInitialized,
        eepromAddressSerialNumber,
  };

  enum {initializedValue = 14};

  int _serialNumber;

  void initialize();

};

extern SavedVariables savedVariables;

#endif
