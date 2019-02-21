#include "motor.h"
#include "oled.h"

Motor::Motor(Motor_TypeDef _motor)
{
	motor = _motor;
}


void Motor::spin(int pwm)
{  

	if(this->motor == MOTOR1)
	{
		ESC1_PWM(pwm);
	}
	if(this->motor == MOTOR2){
		ESC2_PWM(pwm);
	}
	if(this->motor == MOTOR3){
		ESC3_PWM(pwm);
	}
	if(this->motor == MOTOR4){
		ESC4_PWM(pwm);
	}

}
void Motor::updateSpeed(long encoder_ticks)
{
	//this function calculates the motor's RPM based on encoder ticks and delta time
	unsigned long current_time = millis();
	unsigned long dt = current_time - prev_update_time_;

	//convert the time from milliseconds to minutes
	double dtm = (double)dt / 60000;
	double delta_ticks = encoder_ticks - prev_encoder_ticks_;

	//calculate wheel's speed (RPM)
	rpm = (delta_ticks / counts_per_rev_) / dtm;

	prev_update_time_ = current_time;
	prev_encoder_ticks_ = encoder_ticks;
}

