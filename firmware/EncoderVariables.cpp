#include "EncoderVariables.h"

EncoderVariables::EncoderVariables() :
  encoder(constants::encoderBPin,constants::encoderAPin)
{
  variableActive = MODE;
  setMode(constants::AC);
  setPercentCurrent(constants::percentCurrentDefault);
  setDurationOn(constants::durationDefault);
  setDurationOff(constants::durationDefault);
  setCount(constants::countDefault);

  // Encoder
  encoder.write(0);
}

void EncoderVariables::update() {
  encoder_inc = encoder.read();
  if (encoder_inc != 0) {
    encoder.write(0);
    switch (variableActive) {
    case MODE: {
      incMode(encoder_inc);
    } break;
    case PERCENT_CURRENT: {
      incPercentCurrent(encoder_inc*constants::percentCurrentInc);
    } break;
    case DURATION_ON: {
      incDurationOn(encoder_inc*constants::durationInc);
    } break;
    case DURATION_OFF: {
      incDurationOff(encoder_inc*constants::durationInc);
    } break;
    case COUNT: {
      incCount(encoder_inc*constants::countInc);
    } break;
    case FREQUENCY: {
      int duration_on_old = getDurationOn();
      int duration_off_old = getDurationOff();
      int duration_on_new = duration_on_old;
      int duration_off_new = duration_off_old;
      int frequency_inc = 1;
      while ((duration_on_old == duration_on_new) &&
             (duration_off_old == duration_off_new)) {
        incFrequency(encoder_inc*frequency_inc);
        duration_on_new = getDurationOn();
        duration_off_new = getDurationOff();
        frequency_inc++;
      }
    } break;
    case DUTY_CYCLE: {
      int duration_on_old = getDurationOn();
      int duration_off_old = getDurationOff();
      int duration_on_new = duration_on_old;
      int duration_off_new = duration_off_old;
      int duty_cycle_inc = 1;
      while ((duration_on_old == duration_on_new) &&
             (duration_off_old == duration_off_new)) {
        incDutyCycle(encoder_inc*duty_cycle_inc);
        duration_on_new = getDurationOn();
        duration_off_new = getDurationOff();
        duty_cycle_inc++;
      }
    } break;
    }
  }
}

void EncoderVariables::incMode(int inc) {
  int old_value = getMode();
  int new_value = coerce((old_value + inc),0,constants::MODE_COUNT-1);
  mode = (constants::modes)new_value;
}

void EncoderVariables::setMode(constants::modes mode) {
  this->mode = mode;
}

constants::modes EncoderVariables::getMode() {
  return mode;
}

void EncoderVariables::incPercentCurrent(int percent_current) {
  int old_value = getPercentCurrent();
  int new_value = coerce((old_value + percent_current),constants::percentCurrentMin,constants::percentCurrentMax);
  setPercentCurrent(new_value);
}

void EncoderVariables::setPercentCurrent(int percent_current) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    setPercentCurrent(percent_current,channel);
  }
}

void EncoderVariables::setPercentCurrent(int percent_current, int channel) {
  percent_current_array[channel] = percent_current;
}

int EncoderVariables::getPercentCurrent() {
  return percent_current_array[0];
}

int EncoderVariables::getPercentCurrent(int channel) {
  return percent_current_array[channel];
}

void EncoderVariables::incDurationOn(int duration_on) {
  int old_value = getDurationOn();
  int new_value = coerce((old_value + duration_on),constants::durationMin,constants::durationMax);
  setDurationOn(new_value);
}

void EncoderVariables::setDurationOn(int duration_on) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    setDurationOn(duration_on,channel);
  }
}

void EncoderVariables::setDurationOn(int duration_on, int channel) {
  duration_on_array[channel] = duration_on;
}

int EncoderVariables::getDurationOn() {
  return duration_on_array[0];
}

int EncoderVariables::getDurationOn(int channel) {
  return duration_on_array[channel];
}

void EncoderVariables::incDurationOff(int duration_off) {
  int old_value = getDurationOff();
  int new_value = coerce((old_value + duration_off),constants::durationMin,constants::durationMax);
  setDurationOff(new_value);
}

void EncoderVariables::setDurationOff(int duration_off) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    setDurationOff(duration_off,channel);
  }
}

void EncoderVariables::setDurationOff(int duration_off, int channel) {
  duration_off_array[channel] = duration_off;
}

int EncoderVariables::getDurationOff() {
  return duration_off_array[0];
}

int EncoderVariables::getDurationOff(int channel) {
  return duration_off_array[channel];
}

void EncoderVariables::incCount(int count) {
  int old_value = getCount();
  int new_value = coerce((old_value + count),constants::countMin,constants::countMax);
  setCount(new_value);
}

void EncoderVariables::setCount(int count) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    setCount(count,channel);
  }
}

void EncoderVariables::setCount(int count, int channel) {
  count_array[channel] = count;
}

int EncoderVariables::getCount() {
  return count_array[0];
}

int EncoderVariables::getCount(int channel) {
  return count_array[channel];
}

void EncoderVariables::incFrequency(int frequency) {
  int old_value = getFrequency();
  int new_value = coerce((old_value + frequency),constants::frequencyMin,constants::frequencyMax);
  setFrequency(new_value);
}

void EncoderVariables::setFrequency(int frequency) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    setFrequency(frequency,channel);
  }
}

void EncoderVariables::setFrequency(int frequency, int channel) {
  float duty_cycle = getDutyCycle();
  float period = 1000/frequency;
  int duration_on = (int)((duty_cycle*period)/100 + 0.5);
  int duration_off = (int)((period - duration_on) + 0.5);
  setDurationOn(duration_on,channel);
  setDurationOff(duration_off,channel);
}

float EncoderVariables::getFrequency() {
  int duration_on = getDurationOn();
  int duration_off = getDurationOff();
  float frequency = 1000/(duration_on + duration_off);
  return frequency;
}

float EncoderVariables::getFrequency(int channel) {
  int duration_on = getDurationOn(channel);
  int duration_off = getDurationOff(channel);
  float frequency = 1000/(duration_on + duration_off);
  return frequency;
}

void EncoderVariables::incDutyCycle(int duty_cycle) {
  int old_value = getDutyCycle();
  int new_value = coerce((old_value + duty_cycle),constants::dutyCycleMin,constants::dutyCycleMax);
  setDutyCycle(new_value);
}

void EncoderVariables::setDutyCycle(int duty_cycle) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    setDutyCycle(duty_cycle,channel);
  }
}

void EncoderVariables::setDutyCycle(int duty_cycle, int channel) {
  float frequency = getFrequency();
  float period = 1000/frequency;
  int duration_on = (int)((duty_cycle*period)/100 + 0.5);
  int duration_off = (int)((period - duration_on) + 0.5);
  setDurationOn(duration_on,channel);
  setDurationOff(duration_off,channel);
}

float EncoderVariables::getDutyCycle() {
  int duration_on = getDurationOn();
  int duration_off = getDurationOff();
  float duty_cycle = (100*duration_on)/(duration_on + duration_off);
  return duty_cycle;
}

float EncoderVariables::getDutyCycle(int channel) {
  int duration_on = getDurationOn(channel);
  int duration_off = getDurationOff(channel);
  float duty_cycle = (100*duration_on)/(duration_on + duration_off);
  return duty_cycle;
}

void EncoderVariables::incVariableActive() {
  int new_value;
  switch (mode) {
  case constants::AC: {
    new_value = (variableActive+1)%VARIABLE_AC_COUNT;
  } break;
  case constants::DC: {
    new_value = (variableActive+1)%VARIABLE_DC_COUNT;
  } break;
  }
  variableActive = (variables)new_value;
}

void EncoderVariables::setVariableActive(variables variable) {
  if ((variable>0) && (variable<VARIABLE_AC_COUNT)) {
    if (variable >= VARIABLE_DC_COUNT) {
      setMode(constants::DC);
    }
    variableActive = variable;
  }
}

EncoderVariables::variables EncoderVariables::getVariableActive() {
  return variableActive;
}

int EncoderVariables::coerce(int value, int min, int max) {
  while (value < min) {
    value = max + 1 + value - min;
  }
  while (value > max) {
    value = max + 1 - value + min;
  }
  return value;
}

EncoderVariables encoderVariables;
