#include "config_param.h"
#include "string.h"

static ConfigParam_TypeDef mConfigParam;    //系统配置参数

/****************************************************************************************
											操作函数
*****************************************************************************************/

/*  查询/更新俯仰PID参数 */
void SetPitchPID(float *kp, float *ki, float *kd)
{
	mConfigParam.param_pitch_p = *kp;
	mConfigParam.param_pitch_i = *ki;
	mConfigParam.param_pitch_d = *kd;
}
void GetPitchPID(float *kp, float *ki, float *kd)
{
	*kp = mConfigParam.param_pitch_p;
	*ki = mConfigParam.param_pitch_i;
	*kd = mConfigParam.param_pitch_d;
}

/*  查询/更新深度PID参数 */
void SetDeepPID(float *kp, float *ki, float *kd)
{
	mConfigParam.param_deep_p = *kp;
	mConfigParam.param_deep_i = *ki;
	mConfigParam.param_deep_d = *kd;
}
void GetDeepPID(float *kp, float *ki, float *kd)
{
	*kp = mConfigParam.param_deep_p;
	*ki = mConfigParam.param_deep_i;
	*kd = mConfigParam.param_deep_d;
}

/*  查询/更新偏航PID参数 */
void SetDirPID(float *kp, float *ki, float *kd)
{
	mConfigParam.param_dir_p = *kp;
	mConfigParam.param_dir_i = *ki;
	mConfigParam.param_dir_d = *kd;
}
void GetDirPID(float *kp, float *ki, float *kd)
{
	*kp = mConfigParam.param_dir_p;
	*ki = mConfigParam.param_dir_i;
	*kd = mConfigParam.param_dir_d;
}

/*  查询/更新深度偏差参数 */
void SetDeepOffset(float *deep_offset)
{
	mConfigParam.param_deep_offset = *deep_offset;
}
void GetDeepOffset(float *deep_offset)
{
	*deep_offset = mConfigParam.param_deep_offset;
}

/*  查询/更新所有配置参数 */
void SetConfigParam(ConfigParam_TypeDef *param)
{
	memcpy(&mConfigParam, param, sizeof(ConfigParam_TypeDef));
}
void GetConfigParam(ConfigParam_TypeDef *param)
{
	memcpy(param, &mConfigParam, sizeof(ConfigParam_TypeDef));
}

/*  把系统配置参数写入flash */
void ConfigParamToFlash(ConfigParam_TypeDef *param)
{

}

/*  从flash中读取系统配置参数 */
void FlashToConfigParam(ConfigParam_TypeDef *param)
{

}
