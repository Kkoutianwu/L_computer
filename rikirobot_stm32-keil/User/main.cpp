
#include <stdio.h>
#include "hardwareserial.h"

#include "gy85.h"
#include "encoder.h"
#include "battery.h"
#include "led.h"
#include "PID.h"
#include <geometry_msgs/Twist.h> //键盘消息

#include <ros.h>
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "sstream"
#include <geometry_msgs/Vector3.h>
#include <ros/time.h>

#include <riki_msgs/Velocities.h>
#include <riki_msgs/PID.h>
#include <riki_msgs/Imu.h>
#include <riki_msgs/Battery.h>

#include "oled.h"
#include "US_100.h"
#include "timer.h"
#include "key.h"
#include "gy85.h"
#include "motor.h"
#include "Kinematics.h"

#define DEBUG_RATE 100

int Motor::counts_per_rev_ = COUNTS_PER_REV;

double required_angular_vel = 0;							//角速度
double required_linear_vel = 0;								//线速度
uint32_t previous_command_time = 0;

int mode_value = 1;
char battery_buf[50];
char speed_buf[50];
char speed_vaule_buf[4];
int  speed_value = 10;
bool is_first = true;
bool is_mode = false;
bool is_mode_speed = true;
bool is_speed = false;
bool is_key = true;
bool is_set_ok = false; 
bool accel, gyro, mag;//判断读取GY-85的三个数据标志位

PID motor1_pid(-255, 255, K_P, K_I, K_D);
PID motor2_pid(-255, 255, K_P, K_I, K_D);
PID motor3_pid(-255, 255, K_P, K_I, K_D);
PID motor4_pid(-255, 255, K_P, K_I, K_D);

Led led;	
Battery bat(25, 10.6, 12.6);
US_100 us_100;
Gy85  imu;

void pid_callback( const riki_msgs::PID& pid);
void command_callback( const geometry_msgs::Twist& cmd_msg);

ros::NodeHandle  nh;

riki_msgs::Velocities raw_vel_msg;
riki_msgs::Battery raw_battery_msg;                                           //定义电压消息类型
riki_msgs::Imu raw_imu_msg;                                                   //定义IMU消息类型
std_msgs::Float64 Ultrasonic_Vaule;                                           //定义超声波消息类型

ros::Publisher raw_imu_pub("robot_imu", &raw_imu_msg);                        //创建IMU节点并绑定发布内容
ros::Publisher raw_battery_pub("robot_battery", &raw_battery_msg);            //创建电压节点并绑定发布内容
ros::Publisher Ultrasonic("robot_ultrasonic",&Ultrasonic_Vaule);              //创建超声波节点并绑定发布内容
ros::Publisher raw_vel_pub("raw_vel", &raw_vel_msg);                          //创建当前速度节点并绑定发布内容

ros::Subscriber<geometry_msgs::Twist> cmd_sub("FUCK_vel", command_callback);   //创建键盘指令并订阅
ros::Subscriber<geometry_msgs::Twist> FUCK_sub("FUCK", command_callback);   //创建键盘指令并订阅
ros::Subscriber<riki_msgs::PID> pid_sub("robot_pid", pid_callback);           //创建PID并订阅
void Mode_type(int mode_value)
{
			if(mode_value == 1)
			{  
				 OLED_ShowStr(50,2,"        ", 2);
			   OLED_ShowStr(50,2,"0-100", 2);
				 
			}
			if(mode_value == 2)
			{  
				 OLED_ShowStr(50,2,"         ", 2);
			   OLED_ShowStr(50,2,"100-200", 2);
			}
		  if(mode_value == 3)
			{  
				 OLED_ShowStr(50,2,"         ", 2);
			   OLED_ShowStr(50,2,"200-300", 2);
			}
			if(mode_value == 4)
			{   
				 OLED_ShowStr(50,2,"         ", 2);
			   OLED_ShowStr(50,2,"300-400", 2);
			}
}
void robot_Set()
{ 
	
	switch(KEY_Scan(0))
	{
		case 1: if(is_mode_speed)
						{ 
						  is_mode = true;
							is_speed = false;
							
						}
						else
						{
						  is_speed = true;
							is_mode = false;
						}
						speed_vaule_buf[mode_value] = speed_value;
						is_mode_speed =!is_mode_speed;  
		break;
	
		case 2:  
			      if(is_set_ok)
							is_key = false;
						if(is_set_ok == false)
							is_speed = true;
						is_set_ok =!is_set_ok;
		break;
		
		case 3: 
						if(is_mode)
						{
						   mode_value += 1;
							 if(mode_value>4)
								mode_value = 1;
							 Mode_type(mode_value);
						}
						if(is_speed)
						{   
						   speed_value += 1;
							 if(speed_value>20)speed_value = 10;
							 sprintf(speed_buf,"%dm/s",speed_value);
							 OLED_ShowStr(60,4,"     ", 2);
							 OLED_ShowStr(60,4,speed_buf, 2);
						
						}   
		break;	
		
		case 4:
			      if(is_mode)
						{
						   mode_value -= 1;
							if(mode_value<1)
								mode_value = 4;
							 Mode_type(mode_value);
						}
						if(is_speed)
						{
						   speed_value -= 1;
							 if(speed_value<10)speed_value = 20;
							 sprintf(speed_buf,"%dm/s",speed_value);
							 OLED_ShowStr(60,4,"     ", 2);
							 OLED_ShowStr(60,4,speed_buf, 2);
						}
		break;
			
	}
	
}
void pid_callback( const riki_msgs::PID& pid)
{
    motor1_pid.updateConstants(pid.p, pid.i, pid.d);
    motor2_pid.updateConstants(pid.p, pid.i, pid.d);
    motor3_pid.updateConstants(pid.p, pid.i, pid.d);
    motor4_pid.updateConstants(pid.p, pid.i, pid.d);
}
void command_callback( const geometry_msgs::Twist& cmd_msg)
{     
		
  required_linear_vel = cmd_msg.linear.x;
	required_angular_vel = cmd_msg.angular.z;
  previous_command_time = millis();
}
void oled_callback(const std_msgs::Float64&value)															//OLED数据处理函数
{     
   
}
void move_base()
{   
	if(required_linear_vel>0)
	{
	    
			 ESC1_PWM( 1600+required_linear_vel*300);
			 ESC2_PWM( 1600+required_linear_vel*300);
			 ESC3_PWM( 1600+required_linear_vel*300);
		   ESC4_PWM( 1600+required_linear_vel*300);
	}
	if(required_linear_vel == 0)
	{
			 ESC1_PWM( 1526);
			 ESC2_PWM( 1526);
			 ESC3_PWM( 1526);
		   ESC4_PWM( 1526);
	
	}

	if(required_linear_vel<0)
	{
	    
			 ESC1_PWM( 1400+(required_linear_vel*300));
			 ESC2_PWM( 1400+(required_linear_vel*300));
			 ESC3_PWM( 1400+(required_linear_vel*300));
		   ESC4_PWM( 1400+(required_linear_vel*300));
	}
}
void stop_base()
{
    required_linear_vel = 0;
    required_angular_vel = 0;
}

void publish_linear_velocity()
{
	
}
void print_debug()
{
    char buffer[50]; 
    
		sprintf (buffer, "Encoder Right FUCK: %d, Left Front: %d", en_pos3, en_pos4);
    nh.loginfo(buffer);
	  sprintf (buffer, "Encoder Left Rear: %d, Right Rear: %d", en_pos1, en_pos2);
		nh.loginfo(buffer);
}
void check_imu()                                                              //IMU初始化校验函数
{
    gyro = imu.check_gyroscope();
    accel = imu.check_accelerometer();
    mag = imu.check_magnetometer();

    if (!accel){
        nh.logerror("Accelerometer ARE NOT FOUND!");
    }   

    if (!gyro){
        nh.logerror("Gyroscope ARE NOT FOUND!");
    }   

    if (!mag){
        nh.logerror("Magnetometer NOT FOUND!");
    }
    is_first = false;
}
void publish_imu()                                                            //IMU数据处理函数
{
    //geometry_msgs::Vector3 acceler, gyro, mag;
    //this function publishes raw IMU reading
    //measure accelerometer
    if (accel){
        imu.measure_acceleration();
        raw_imu_msg.linear_acceleration = imu.raw_acceleration;
    }

    //measure gyroscope
    if (gyro){
        imu.measure_gyroscope();
        raw_imu_msg.angular_velocity = imu.raw_rotation;
    }

    //measure magnetometer
    if (mag){
        imu.measure_magnetometer();
        raw_imu_msg.magnetic_field = imu.raw_magnetic_field;
    }

    //publish raw_imu_msg object to ROS
    raw_imu_pub.publish(&raw_imu_msg);

}
void publish_Ultrasonic()                                                     //超声波数据处理函数
{
//   Ultrasonic_Vaule.data = us_100.US100_GetVal();
//	 Ultrasonic.publish(&Ultrasonic_Vaule);
}
void publishBAT()
{
	raw_battery_msg.battery = bat.get_volt();
	raw_battery_pub.publish(&raw_battery_msg);
}
int main(void) 
{
	bool OnOff = true;
	uint32_t previous_battery_debug_time = 0;
	uint32_t previous_debug_time = 0;
	uint32_t previous_imu_time = 0;
	uint32_t previous_control_time = 0;
	uint32_t publish_vel_time = 0;
	char battery_buffer[]= "The voltage is lower than 11.3V,Please charge! ";
	
	



	
	SystemInit();							//系统时钟初始化
	initialise();							//延时函数初始化*/
	led.init();  							//led初始化
  

 
 
	encoder_init(ENCODER1);
	encoder_init(ENCODER2);
	encoder_init(ENCODER3);
	encoder_init(ENCODER4);
	
	bat.init();  							//电压检测初始化
	imu.init();
	KEY_Init();

  TIM3_PWM_Init(19999,71);	//电调PWM初始化
	ESC_Init(19280);						//电调初始化
	
	
	AIN1=1;
	AIN2=0;
	BIN1=0;
	BIN2=1;	
	
	IIC_Configuation();
	OLED_Init();             	//OLED初始化
	OLED_CLS();

//	OLED_ShowStr(0,2,"Mode:", 2);
//	OLED_ShowStr(50,2,"0-100", 2);
//	OLED_ShowStr(0,4,"Speed:", 2);
//	OLED_ShowStr(60,4,"10m/s", 2);
//	while(is_key)
//	{ 
//		robot_Set();
//	}
	nh.initNode(); //初始化
		
  while (!nh.connected())                                                       //等待连接
	{
		nh.spinOnce();

	}  
  led.on_off(1);
	nh.loginfo("robot_Star Connected!");                                          //打印连接成功                                          
	//led.on_off(OnOff);
	delay_us(10);
	nh.advertise(Ultrasonic);																											//进行发布超声波主题
	nh.advertise(raw_battery_pub);											 													//进行发布电压主题
	nh.advertise(raw_imu_pub);																										//进行发布IMU主题
	nh.advertise(raw_vel_pub);                                                    //进行发布当前速度主题
	
	nh.subscribe(pid_sub);                                                        //订阅PID
	nh.subscribe(cmd_sub);                                                        //订阅键盘指令
	while(1)
	{ 

//		if ((millis() - previous_control_time) >= (1000 / COMMAND_RATE))
//		{     
//					
//					move_base();
//					char _buff[50];
//					sprintf(_buff,"li:%f",required_linear_vel);
//					OLED_ShowStr(0,4,_buff, 2);
//					sprintf(_buff,"an:%f",required_angular_vel);
//					OLED_ShowStr(0,6,_buff, 2);
//					previous_control_time = millis();
//		}		
//		if ((millis() - previous_command_time) >= 400)
//		{
//					stop_base();
//		}
//		if ((millis() - publish_vel_time) >= (1000 / VEL_PUBLISH_RATE))
//		{
//					publish_linear_velocity();
//					publish_vel_time = millis();
//		}
//    if ((millis() - previous_imu_time) >= (1000 / IMU_PUBLISH_RATE))
//		{
//					if(is_first)
//					{
//									//sanity check if the IMU exits
//									check_imu();
//					} 
//					else
//					{
//									//publish the IMU data
//									publish_imu();
//					}
//					previous_imu_time = millis();
//     }
//     if( (millis() - previous_battery_debug_time) >= (1000 / BAT_PUBLISH_RATE))
//		 {
//            if(bat.get_volt() < 11.300000)
//						{
//                OnOff = !OnOff;
//                led.on_off(OnOff);
//                nh.logwarn(battery_buffer);			
//            }
//            publishBAT();
//            previous_battery_debug_time = millis();		
//      }

     if(DEBUG)
			{
            if ((millis() - previous_debug_time) >= (100000 / DEBUG_RATE))
						{
                print_debug();
						  	publishBAT();
                previous_debug_time = millis();
            }
      }
		nh.spinOnce();
	}
	return 0;

}




