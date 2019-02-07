// EncoderVariables.h
#ifndef _ENCODERVARIABLES_H_
#define _ENCODERVARIABLES_H_

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <wiring.h>
#endif
#include "Encoder.h"
#include "constants.h"


class EncoderVariables {

 public:

  enum {VARIABLE_AC_COUNT=7,
        VARIABLE_DC_COUNT=2};
  enum variables {
    MODE,
    PERCENT_CURRENT,
    DURATION_ON,
    DURATION_OFF,
    COUNT,
    FREQUENCY,
    DUTY_CYCLE,
  };
  EncoderVariables();
  void update();
  void incMode(int inc);
  void setMode(constants::modes mode);
  constants::modes getMode();
  void incPercentCurrent(int percent_current);
  void setPercentCurrent(int percent_current);
  void setPercentCurrent(int percent_current, int channel);
  int getPercentCurrent();
  int getPercentCurrent(int channel);
  void incDurationOn(int duration_on);
  void setDurationOn(int duration_on);
  void setDurationOn(int duration_on, int channel);
  int getDurationOn();
  int getDurationOn(int channel);
  void incDurationOff(int duration_off);
  void setDurationOff(int duration_off);
  void setDurationOff(int duration_off, int channel);
  int getDurationOff();
  int getDurationOff(int channel);
  void incCount(int count);
  void setCount(int count);
  void setCount(int count, int channel);
  int getCount();
  int getCount(int channel);
  void incFrequency(int frequency);
  void setFrequency(int frequency);
  void setFrequency(int frequency, int channel);
  float getFrequency();
  float getFrequency(int channel);
  void incDutyCycle(int duty_cycle);
  void setDutyCycle(int duty_cycle);
  void setDutyCycle(int duty_cycle, int channel);
  float getDutyCycle();
  float getDutyCycle(int channel);
  void incVariableActive();
  void setVariableActive(variables variable);
  variables getVariableActive();

 private:
  Encoder encoder;
  variables variableActive;
  constants::modes mode;
  int encoder_inc;
  int percent_current_array[constants::CHANNEL_COUNT];
  int duration_on_array[constants::CHANNEL_COUNT];
  int duration_off_array[constants::CHANNEL_COUNT];
  int count_array[constants::CHANNEL_COUNT];
  int coerce(int value, int min, int max);
  int frequency_inc;
  int duty_cycle_inc;

};

extern EncoderVariables encoderVariables;

#endif
