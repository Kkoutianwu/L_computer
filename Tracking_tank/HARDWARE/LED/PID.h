#ifndef PID_H
#define PID_H

void PID_ParameterReckonL(void);
void PID_ParameterReckonR(void);
float PID_ControlL(float e);
float PID_ControlR(float e);
#endif
