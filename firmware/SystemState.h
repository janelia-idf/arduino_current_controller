#ifndef _SYSTEM_STATE_H_
#define _SYSTEM_STATE_H_
#include "constants.h"
#include "TimeTriggeredScheduler.h"
#include "NewhavenDisplay.h"
#include "EncoderVariables.h"


enum {SYS_ERR_BUF_SZ=50};

class SystemState {

 public:
  SystemState();

  void setErrMsg(char *);
  char errMsg[SYS_ERR_BUF_SZ];

  void initialize();

  void enable();
  void enable(int channel);
  void disable();
  void disable(int channel);
  void setPercentCurrent(int percent_current);
  void setPercentCurrent(int percent_current, int channel);

  void turnOn();
  void turnOn(int percent_current);
  void turnOn(int percent_current, int channel);
  void turnOff();
  void turnOff(int channel);
  void startPwm();
  void startPwm(int percent_current, int duration_on, int duration_off, int count);
  void startPwm(int percent_current, int duration_on, int duration_off, int count, int channel);
  void stopPwm();
  void stopPwm(int channel);

  void initializeDisplay();
  void labelDisplay();
  void updateDisplay();
  void enableDisplay();
  void disableDisplay();

  void update();

  void handleSetPwmOn(int channel);
  void handleSetPwmOff(int channel);
  void handleTriggerInterrupt();
  void handleEncoderBtnInterrupt();

 private:
  bool enabled[constants::CHANNEL_COUNT];
  struct pwmTask_t {
    int onTaskId;
    int offTaskId;
  };
  pwmTask_t pwmTasks[constants::CHANNEL_COUNT];

  NewhavenDisplay display;
  bool displayEnabled;
  unsigned long time_last_display_update;

  bool turned_on;
  constants::modes mode;
  constants::modes mode_prev;

  volatile int pulseCount;
  volatile int pulseLimit;
};

extern SystemState systemState;

inline void inlineSetPwmOn(int channel) {systemState.handleSetPwmOn(channel);}
inline void inlineSetPwmOff(int channel) {systemState.handleSetPwmOff(channel);}
inline void switchISR() {systemState.handleTriggerInterrupt();}
inline void bncISR() {systemState.handleTriggerInterrupt();}
inline void encoderBtnISR() {systemState.handleEncoderBtnInterrupt();}

#endif
