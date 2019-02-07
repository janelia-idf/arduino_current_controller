#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif

namespace constants {
  enum {
    CHANNEL_COUNT=4,
    MODE_COUNT=2,
  };
  enum modes {
    AC,
    DC,
  };

  // Communications parameters
  extern const int baudrate;

  // Device parameters
  extern const int modelNumber;
  extern const int serialNumberMin;
  extern const int serialNumberMax;
  extern const int firmwareNumber;

  // Pin assignment
  extern const int enablePinArray[CHANNEL_COUNT];
  extern const int pwmPinArray[CHANNEL_COUNT];
  extern const int switchInterruptPin;
  extern const int bncAPin;
  extern const int bncBPin;
  extern const int encoderAPin;
  extern const int encoderBPin;
  extern const int encoderBtnInterruptPin;

  // Current parameters
  extern const int percentCurrentMin;
  extern const int percentCurrentMax;
  extern const int percentCurrentDefault;
  extern const int percentCurrentInc;
  extern const int analogOutMin;
  extern const int analogOutMax;

  // Pwm parameters
  extern const int durationMin;
  extern const int durationMax;
  extern const int durationDefault;
  extern const int durationInc;
  extern const int countMin;
  extern const int countMax;
  extern const int countDefault;
  extern const int countInc;
  extern const int frequencyMin;
  extern const int frequencyMax;
  extern const int dutyCycleMin;
  extern const int dutyCycleMax;

  // Display parameters
  extern const int displayPeriod;
  extern const int displayFieldLength;

  // External Interrupts
  extern const int switchInterruptNumber;
  extern const int bncInterruptNumber;
  extern const int encoderBtnInterruptNumber;

}
#endif
