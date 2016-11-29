#ifndef __CONFIG_PARAM_H
#define __CONFIG_PARAM_H
#include "stm32f4xx.h"


/****************************************************************************************
											ϵͳ����
*****************************************************************************************/
typedef struct config_param
{
	
	float param_pitch_p;				//�����ջ�PID��������p 
	float param_pitch_i;				//�����ջ�PID��������i  
	float param_pitch_d;				//�����ջ�PID��������d  
	
	float param_deep_p;					//��ȱջ�PID��������p 
	float param_deep_i;					//��ȱջ�PID��������i  
	float param_deep_d;					//��ȱջ�PID��������d  
	
	float param_dir_p;					//����ջ�PID��������p  
	float param_dir_i;					//����ջ�PID��������i 
	float param_dir_d;					//����ջ�PID��������d  
	
	float param_deep_offset;		//��ȼ����ƫ��  ��λ��m
	
	u16 param_offset_gx;			//������x�����ƫ��
	u16 param_offset_gy;			//������y�����ƫ��
	u16 param_offset_gz;			//������z�����ƫ��
	
	u16 param_max_msgx;				//��ǿ��x�Ἣ��ֵ
	u16 param_max_msgy;				//��ǿ��y�Ἣ��ֵ
	u16 param_max_msgz;				//��ǿ��z�Ἣ��ֵ
	
	u16 param_min_msgx;				//��ǿ��x�ἫСֵ
	u16 param_min_msgy;				//��ǿ��y�ἫСֵ
	u16 param_min_msgz;				//��ǿ��z�ἫСֵ
	
	u16 param_moter1_zone_p;	//��ˢ���#1����+
	u16 param_moter1_zone_n;	//��ˢ���#1����-
	u16 param_moter2_zone_p;	//��ˢ���#2����+
	u16 param_moter2_zone_n;	//��ˢ���#2����-
	u16 param_moter3_zone_p;	//��ˢ���#3����+
	u16 param_moter3_zone_n;	//��ˢ���#3����-
	
} ConfigParam_TypeDef;


/****************************************************************************************
											��������
*****************************************************************************************/

/*  ��ѯ/���¸���PID���� */
void SetPitchPID(float *kp, float *ki, float *kd);
void GetPitchPID(float *kp, float *ki, float *kd);

/*  ��ѯ/�������PID���� */
void SetDeepPID(float *kp, float *ki, float *kd);
void GetDeepPID(float *kp, float *ki, float *kd);

/*  ��ѯ/����ƫ��PID���� */
void SetDirPID(float *kp, float *ki, float *kd);
void GetDirPID(float *kp, float *ki, float *kd);

/*  ��ѯ/�������ƫ����� */
void SetDeepOffset(float *deep_offset);
void GetDeepOffset(float *deep_offset);

/*  ��ѯ/�����������ò��� */
void SetConfigParam(ConfigParam_TypeDef *param);
void GetConfigParam(ConfigParam_TypeDef *param);

/*  ��ϵͳ���ò���д��flash */
void ConfigParamToFlash(ConfigParam_TypeDef *param);

/*  ��flash�ж�ȡϵͳ���ò��� */
void FlashToConfigParam(ConfigParam_TypeDef *param);

#endif
