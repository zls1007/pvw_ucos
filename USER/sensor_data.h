#ifndef __SENSOR_DATA_H
#define __SENSOR_DATA_H
#include "stm32f4xx.h"


/****************************************************************************************
											传感器数据
*****************************************************************************************/
typedef struct sensor_data
{
	/* 机器人电压电流参数 */
	float state_volatage;					//本体电池电压  	单位：V
	float state_currage;					//本体电池电流   	单位：A
	float state_deep;							//机器人下潜深度	单位：m
	float state_temperature;			//机器人板温度		单位：℃
	float state_atmosphsric;			//机器人腔体气压	单位：mbr
	
	/* 机器人姿态参数 */
	float state_roll;							//机器人滚转角度	单位：1°
	float state_pitch;						//机器人俯仰角度 	单位：1°
	float state_yaw;							//机器人偏航角度	单位：1°
	
	/* 机器人姿态传感器原始数据 */
	u16 state_acc_x;						//机器人加速度x采样值   16位AD  量程+-4g
	u16 state_acc_y;						//机器人加速度y采样值   16位AD  量程+-4g
	u16 state_acc_z;						//机器人加速度z采样值   16位AD  量程+-4g
	u16 state_gyro_x;						//机器人陀螺仪x采样值   16位AD  量程+-2000dps	
	u16 state_gyro_y;						//机器人陀螺仪y采样值   16位AD  量程+-2000dps		
	u16 state_gyro_z;						//机器人陀螺仪z采样值   16位AD  量程+-2000dps	
	u16 state_mag_x;						//机器人磁强计x采样值   16位AD  量程+-4800uT		
	u16 state_mag_y;						//机器人磁强计y采样值   16位AD  量程+-4800uT			
	u16 state_mag_z;						//机器人磁强计z采样值   16位AD  量程+-4800uT			
		
} SensorData_TypeDef;



/****************************************************************************************
											操作函数
*****************************************************************************************/

/*  查询/更新电池数据 */
void SetBatteryData(float *vol, float *cur);
void GetBatteryData(float *vol, float *cur);

/*  查询/更新水深数据  */
void SetDeepData(float *deep);
void GetDeepData(float *deep);

/*  查询/更新气压温度数据 */
void SetPTData(float *press, float *temp);
void GetPTData(float *press, float *temp);

/*  查询/更新气压温度数据 */
void SetAttitudeData(float *roll, float *pitch, float *yaw);
void GetAttitudeData(float *roll, float *pitch, float *yaw);

/*  查询/更新陀螺仪原始数据*/
void SetGyroData(float *gx, float *gy, float *gz);
void GetGyroData(float *gx, float *gy, float *gz);

/*  查询/更新加速度原始数据*/
void SetAccData(float *ax, float *ay, float *az);
void GetAccData(float *ax, float *ay, float *az);

/*  查询/更新磁强计原始数据*/
void SetMagData(float *mx, float *my, float *mz);
void GetMagData(float *mx, float *my, float *mz);

/*  查询/更新所有传感器数据*/
void SetSensorData(SensorData_TypeDef *data);
void GetSensorData(SensorData_TypeDef *data);



#endif
