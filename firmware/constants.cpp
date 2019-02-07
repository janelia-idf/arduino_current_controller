#include "constants.h"
namespace constants {
  // Communications parameters
  const int baudrate = 9600;

  // Device parameters
  const int modelNumber = 2346;
  const int serialNumberMin = 0;
  const int serialNumberMax = 255;
  const int firmwareNumber = 0;

  // Pin assignment
  const int enablePinArray[CHANNEL_COUNT] = {5,6,11,12};
  const int pwmPinArray[CHANNEL_COUNT] = {7,8,9,10};
  const int switchInterruptPin = 18;
  const int bncAPin = 21;
  const int bncBPin = 20;
  const int encoderAPin = 2;
  const int encoderBPin = 3;
  const int encoderBtnInterruptPin = 19;

  // Current parameters
  const int percentCurrentMin = 0;
  const int percentCurrentMax = 100;
  const int percentCurrentDefault = 20;
  const int percentCurrentInc = 1;
  const int analogOutMin = 255;
  const int analogOutMax = 0;

  // Pwm parameters
  const int durationMin = 1;
  const int durationMax = 100;
  const int durationDefault = 50;
  const int durationInc = 1;
  const int countMin = 1;
  const int countMax = 50;
  const int countDefault = 5;
  const int countInc = 1;
  const int frequencyMin = 1000/(2*durationMax);
  const int frequencyMax = 1000/(2*durationMin);
  const int dutyCycleMin = 1;
  const int dutyCycleMax = 99;

  // Display parameters
  const int displayPeriod = 300;
  const int displayFieldLength = 4;

  // External Interrupts
  const int switchInterruptNumber = 5;
  const int bncInterruptNumber = 2;
  const int encoderBtnInterruptNumber = 4;

}
