#include "config_param.h"
#include "string.h"

static ConfigParam_TypeDef mConfigParam;    //ϵͳ���ò���

/****************************************************************************************
											��������
*****************************************************************************************/
/*  ϵͳ���ò�����ʼ��*/
void config_param_init(void)
{
	//�����ջ�PID����
	mConfigParam.param_pitch_p = 20.0;
	mConfigParam.param_pitch_i = 0.0;
	mConfigParam.param_pitch_d = 0.0;
	
	//��ȱջ�PID����
	mConfigParam.param_deep_p = 20.0;
	mConfigParam.param_deep_i = 0.0;
	mConfigParam.param_deep_d = 0.0;
	
	//ƫ���Ǳջ�PID����
	mConfigParam.param_deep_p = 10.0;
	mConfigParam.param_deep_i = 0.0;
	mConfigParam.param_deep_d = 0.0;
}



/*  ��ѯ/���¸���PID���� */
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

/*  ��ѯ/�������PID���� */
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

/*  ��ѯ/����ƫ��PID���� */
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

/*  ��ѯ/�������ƫ����� */
void SetDeepOffset(float *deep_offset)
{
	mConfigParam.param_deep_offset = *deep_offset;
}
void GetDeepOffset(float *deep_offset)
{
	*deep_offset = mConfigParam.param_deep_offset;
}

/*  ��ѯ/�����������ò��� */
void SetConfigParam(ConfigParam_TypeDef *param)
{
	memcpy(&mConfigParam, param, sizeof(ConfigParam_TypeDef));
}
void GetConfigParam(ConfigParam_TypeDef *param)
{
	memcpy(param, &mConfigParam, sizeof(ConfigParam_TypeDef));
}

/*  ��ϵͳ���ò���д��flash */
void ConfigParamToFlash(ConfigParam_TypeDef *param)
{

}

/*  ��flash�ж�ȡϵͳ���ò��� */
void FlashToConfigParam(ConfigParam_TypeDef *param)
{

}
