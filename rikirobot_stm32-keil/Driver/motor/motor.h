#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "config.h"
#include "timer.h"
#define constrain(amt,low,high) \
	((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


class Motor {
	public:
		int rpm;
		Motor(Motor_TypeDef _motor);
		static int counts_per_rev_;
		void updateSpeed(long encoder_ticks);
		void spin(int pwm);

	private:
				Motor_TypeDef motor;
	
		long prev_encoder_ticks_; 
	 	unsigned long prev_update_time_; 

		void motor_pwm_init(); 
};

#endif //_MOTOR_H_
