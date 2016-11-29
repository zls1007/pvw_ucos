#ifndef __CONFIG_PARAM_H
#define __CONFIG_PARAM_H
#include "stm32f4xx.h"


/****************************************************************************************
											系统参数
*****************************************************************************************/
typedef struct config_param
{
	
	float param_pitch_p;				//俯仰闭环PID比例参数p 
	float param_pitch_i;				//俯仰闭环PID比例参数i  
	float param_pitch_d;				//俯仰闭环PID比例参数d  
	
	float param_deep_p;					//深度闭环PID比例参数p 
	float param_deep_i;					//深度闭环PID比例参数i  
	float param_deep_d;					//深度闭环PID比例参数d  
	
	float param_dir_p;					//方向闭环PID比例参数p  
	float param_dir_i;					//方向闭环PID比例参数i 
	float param_dir_d;					//方向闭环PID比例参数d  
	
	float param_deep_offset;		//深度计零点偏差  单位：m
	
	u16 param_offset_gx;			//陀螺仪x轴零点偏移
	u16 param_offset_gy;			//陀螺仪y轴零点偏移
	u16 param_offset_gz;			//陀螺仪z轴零点偏移
	
	u16 param_max_msgx;				//磁强计x轴极大值
	u16 param_max_msgy;				//磁强计y轴极大值
	u16 param_max_msgz;				//磁强计z轴极大值
	
	u16 param_min_msgx;				//磁强计x轴极小值
	u16 param_min_msgy;				//磁强计y轴极小值
	u16 param_min_msgz;				//磁强计z轴极小值
	
	u16 param_moter1_zone_p;	//无刷电机#1死区+
	u16 param_moter1_zone_n;	//无刷电机#1死区-
	u16 param_moter2_zone_p;	//无刷电机#2死区+
	u16 param_moter2_zone_n;	//无刷电机#2死区-
	u16 param_moter3_zone_p;	//无刷电机#3死区+
	u16 param_moter3_zone_n;	//无刷电机#3死区-
	
} ConfigParam_TypeDef;


/****************************************************************************************
											操作函数
*****************************************************************************************/

/*  查询/更新俯仰PID参数 */
void SetPitchPID(float *kp, float *ki, float *kd);
void GetPitchPID(float *kp, float *ki, float *kd);

/*  查询/更新深度PID参数 */
void SetDeepPID(float *kp, float *ki, float *kd);
void GetDeepPID(float *kp, float *ki, float *kd);

/*  查询/更新偏航PID参数 */
void SetDirPID(float *kp, float *ki, float *kd);
void GetDirPID(float *kp, float *ki, float *kd);

/*  查询/更新深度偏差参数 */
void SetDeepOffset(float *deep_offset);
void GetDeepOffset(float *deep_offset);

/*  查询/更新所有配置参数 */
void SetConfigParam(ConfigParam_TypeDef *param);
void GetConfigParam(ConfigParam_TypeDef *param);

/*  把系统配置参数写入flash */
void ConfigParamToFlash(ConfigParam_TypeDef *param);

/*  从flash中读取系统配置参数 */
void FlashToConfigParam(ConfigParam_TypeDef *param);

#endif
