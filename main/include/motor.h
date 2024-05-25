#pragma once
#ifndef _MOTOR_H_
#define _MOTOR_H_

struct {
  int PIN_IN_1;
  int PIN_IN_2;
  int PIN_ENABLE;
} typedef Motor;

void MotorInitialize(Motor *m);
void MotorCalibrate(Motor* m);
void MotorEnable(Motor *m);
void MotorDisable(Motor *m);
void MotorSpinForward(Motor *m);
void MotorSpinBackward(Motor *m);


#endif // _MOTOR_H_
