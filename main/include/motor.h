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

// This will be dependent on a switch that if set will not allow the motor to
// spin in software
// 1 - True
// 0 - False
int IsMovable(Motor* m);

#endif // _MOTOR_H_
