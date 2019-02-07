#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "string.h"
#include "SystemState.h"

SystemState::SystemState() {
  setErrMsg("");
}

void SystemState::setErrMsg(char *msg) {
  strncpy(errMsg,msg,SYS_ERR_BUF_SZ);
}

void SystemState::initialize() {
  tts.init();
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    pinMode(constants::enablePinArray[channel], OUTPUT);
    pwmTasks[channel].onTaskId = -1;
    pwmTasks[channel].offTaskId = -1;
    turnOff(channel);
  }

  turned_on = false;
  mode_prev = encoderVariables.getMode();

  // Display
  display = NewhavenDisplay();
  initializeDisplay();

  // Interrupts
  pinMode(constants::switchInterruptPin,INPUT);
  digitalWrite(constants::switchInterruptPin,HIGH);
  attachInterrupt(constants::switchInterruptNumber,switchISR,FALLING);

  pinMode(constants::bncAPin,INPUT);
  digitalWrite(constants::bncAPin,HIGH);
  attachInterrupt(constants::bncInterruptNumber,bncISR,FALLING);

  pinMode(constants::encoderBtnInterruptPin,INPUT);
  digitalWrite(constants::encoderBtnInterruptPin,HIGH);
  attachInterrupt(constants::encoderBtnInterruptNumber,encoderBtnISR,FALLING);

  // Outputs
  pinMode(constants::bncBPin,OUTPUT);
  digitalWrite(constants::bncBPin,LOW);

}

void SystemState::enable() {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    enable(channel);
  }
}

void SystemState::enable(int channel) {
  enabled[channel] = true;
  digitalWrite(constants::enablePinArray[channel],HIGH);
}

void SystemState::disable() {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    disable(channel);
  }
}

void SystemState::disable(int channel) {
  enabled[channel] = false;
  digitalWrite(constants::enablePinArray[channel],LOW);
}

void SystemState::setPercentCurrent(int percent_current) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    setPercentCurrent(percent_current,channel);
  }
}

void SystemState::setPercentCurrent(int percent_current, int channel) {
  int analog_out = map(percent_current,
                       constants::percentCurrentMin,
                       constants::percentCurrentMax,
                       constants::analogOutMin,
                       constants::analogOutMax);
  analogWrite(constants::pwmPinArray[channel],analog_out);
}

void SystemState::turnOn() {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    turnOn(encoderVariables.getPercentCurrent(channel));
  }
}

void SystemState::turnOn(int percent_current) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    turnOn(percent_current, channel);
  }
}

void SystemState::turnOn(int percent_current, int channel) {
  turnOff(channel);
  setPercentCurrent(percent_current, channel);
  enable(channel);
  turned_on = true;
  digitalWrite(constants::bncBPin,HIGH);
}

void SystemState::turnOff() {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    turnOff(channel);
  }
}

void SystemState::turnOff(int channel) {
  stopPwm(channel);
}

void SystemState::startPwm() {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    startPwm(encoderVariables.getPercentCurrent(channel),
             encoderVariables.getDurationOn(channel),
             encoderVariables.getDurationOff(channel),
             encoderVariables.getCount(channel),
             channel);
  }
}

void SystemState::startPwm(int percent_current, int duration_on, int duration_off, int count) {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    startPwm(percent_current, duration_on, duration_off, count, channel);
  }
}

void SystemState::startPwm(int percent_current, int duration_on, int duration_off, int count, int channel) {
  turnOff(channel);
  setPercentCurrent(percent_current, channel);
  long period = duration_on + duration_off;
  if (channel == 0) {
    pulseCount = 0;
    pulseLimit = count;
  }
  pwmTasks[channel].onTaskId = (int)tts.addTaskUsingDelay(100,inlineSetPwmOn,channel,period,(long)count,false);
  pwmTasks[channel].offTaskId = (int)tts.addTaskUsingOffset((uint8_t)pwmTasks[channel].onTaskId,(uint32_t)duration_on,inlineSetPwmOff,channel,period,(long)count,false);
}

void SystemState::stopPwm() {
  for (int channel=0;channel<constants::CHANNEL_COUNT;channel++) {
    stopPwm(channel);
  }
}

void SystemState::stopPwm(int channel) {
  int onTaskId = pwmTasks[channel].onTaskId;
  int offTaskId = pwmTasks[channel].offTaskId;
  if (0 <= onTaskId) {
    tts.removeTask((uint8_t)onTaskId);
    pwmTasks[channel].onTaskId = -1;
  }
  if (0 <= offTaskId) {
    tts.removeTask((uint8_t)offTaskId);
    pwmTasks[channel].offTaskId = -1;
  }
  // setPercentCurrent(constants::percentCurrentMin, channel);
  disable(channel);
  turned_on = false;
  if (channel == 0) {
    digitalWrite(constants::bncBPin,LOW);
    pulseCount = 0;
    pulseLimit = 0;
  }
}

void SystemState::initializeDisplay() {
  display.init();
  delay(100);
  labelDisplay();
  displayEnabled = true;
  display.blinkingCursorOn();
}

void SystemState::labelDisplay() {
  display.clearScreen();
  display.setCursor(0,0);
  display.printPadRight("Mode",constants::displayFieldLength);
  display.setCursor(0,10);
  display.printPadRight("%I",constants::displayFieldLength);
  display.setCursor(1,0);
  display.printPadRight("Trig",constants::displayFieldLength);
  switch (encoderVariables.getMode()) {
  case constants::AC: {
    display.setCursor(1,10);
    display.printPadRight("Cnt",constants::displayFieldLength);
    display.setCursor(2,0);
    display.printPadRight("On",constants::displayFieldLength);
    display.setCursor(2,10);
    display.printPadRight("Off",constants::displayFieldLength);
    display.setCursor(3,0);
    display.printPadRight("Freq",constants::displayFieldLength);
    display.setCursor(3,10);
    display.printPadRight("Duty",constants::displayFieldLength);
  } break;
  }
  time_last_display_update = millis();
}

void SystemState::updateDisplay() {
  if (displayEnabled) {
    unsigned long time_now = millis();
    if ((time_now - time_last_display_update) > constants::displayPeriod) {
      time_last_display_update = time_now;
      mode = encoderVariables.getMode();

      display.setCursor(0,15);
      display.printPadRight(String(encoderVariables.getPercentCurrent()),constants::displayFieldLength);
      switch (mode) {
      case constants::AC: {
        int duration_on = encoderVariables.getDurationOn();
        int duration_off = encoderVariables.getDurationOff();
        display.setCursor(0,5);
        display.printPadRight("AC",constants::displayFieldLength);
        display.setCursor(1,5);
        if (tts.activeTasks()) {
          display.printPadRight("Yes",constants::displayFieldLength);
        } else {
          display.printPadRight("No",constants::displayFieldLength);
        }
        display.setCursor(1,15);
        display.printPadRight(String(encoderVariables.getCount()),constants::displayFieldLength);
        display.setCursor(2,5);
        display.printPadRight(String(duration_on),constants::displayFieldLength);
        display.setCursor(2,15);
        display.printPadRight(String(duration_off),constants::displayFieldLength);
        display.setCursor(3,5);
        int period = duration_on + duration_off;
        if (period > 1000) {
          display.printPadRight("<1",constants::displayFieldLength);
        } else {
          int freq = (int)(encoderVariables.getFrequency() + 0.5);
          display.printPadRight(String(freq),constants::displayFieldLength);
        }
        display.setCursor(3,15);
        int duty_cycle = (int)(encoderVariables.getDutyCycle() + 0.5);
        display.printPadRight(String(duty_cycle),constants::displayFieldLength);
      } break;
      case constants::DC: {
        display.setCursor(0,5);
        display.printPadRight("DC",constants::displayFieldLength);
        display.setCursor(1,5);
        if (turned_on) {
          display.printPadRight("Yes",constants::displayFieldLength);
        } else {
          display.printPadRight("No",constants::displayFieldLength);
        }
      } break;
      }
    }
    switch (encoderVariables.getVariableActive()) {
    case EncoderVariables::MODE:
      display.setCursor(0,5);
      break;
    case EncoderVariables::PERCENT_CURRENT:
      display.setCursor(0,15);
      break;
    case EncoderVariables::DURATION_ON:
      display.setCursor(2,5);
      break;
    case EncoderVariables::DURATION_OFF:
      display.setCursor(2,15);
      break;
    case EncoderVariables::COUNT:
      display.setCursor(1,15);
      break;
    case EncoderVariables::FREQUENCY:
      display.setCursor(3,5);
      break;
    case EncoderVariables::DUTY_CYCLE:
      display.setCursor(3,15);
      break;
    }
  }
}

void SystemState::update() {
  encoderVariables.update();
  mode = encoderVariables.getMode();
  if (mode != mode_prev) {
    mode_prev = mode;
    turnOff();
    labelDisplay();
  }
  updateDisplay();
  if (encoderVariables.getMode() == constants::DC) {
    if (turned_on) {
      setPercentCurrent(encoderVariables.getPercentCurrent());
    }
  }
}

void SystemState::handleSetPwmOn(int channel) {
  if (channel == 0) {
    if (pulseCount == 0) {
      digitalWrite(constants::bncBPin,HIGH);
    }
    pulseCount++;
  }
  enable(channel);
}

void SystemState::handleSetPwmOff(int channel) {
  if ((channel == 0) && (pulseCount == pulseLimit)) {
    digitalWrite(constants::bncBPin,LOW);
    pulseCount = 0;
    pulseLimit = 0;
  }
  disable(channel);
}

void SystemState::handleTriggerInterrupt() {
  switch (encoderVariables.getMode()) {
  case constants::AC: {
    if (!tts.activeTasks()) {
      startPwm();
    } else {
      stopPwm();
    }
  } break;
  case constants::DC: {
    if (!turned_on) {
      turnOn();
    } else {
      turnOff();
    }
  } break;
  }
}

void SystemState::handleEncoderBtnInterrupt() {
  encoderVariables.incVariableActive();
}


SystemState systemState;
