#pragma once
#ifndef _MOTOR_H_
#define _MOTOR_H_

struct {
    int PIN_IN_1;
    int PIN_IN_2;
    int PIN_ENABLE;
} typedef Motor;

void InitializeMotor(Motor* m);
void EnableMotor(Motor* m);
void DisableMotor(Motor* m);
void SpinMotorForward(Motor* m);
void SpinMotorBackward(Motor* m);

#endif // _MOTOR_H_
