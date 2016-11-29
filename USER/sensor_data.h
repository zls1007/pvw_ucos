#ifndef __SENSOR_DATA_H
#define __SENSOR_DATA_H
#include "stm32f4xx.h"


/****************************************************************************************
											����������
*****************************************************************************************/
typedef struct sensor_data
{
	/* �����˵�ѹ�������� */
	float state_volatage;					//�����ص�ѹ  	��λ��V
	float state_currage;					//�����ص���   	��λ��A
	float state_deep;							//��������Ǳ���	��λ��m
	float state_temperature;			//�����˰��¶�		��λ����
	float state_atmosphsric;			//������ǻ����ѹ	��λ��mbr
	
	/* ��������̬���� */
	float state_roll;							//�����˹�ת�Ƕ�	��λ��1��
	float state_pitch;						//�����˸����Ƕ� 	��λ��1��
	float state_yaw;							//������ƫ���Ƕ�	��λ��1��
	
	/* ��������̬������ԭʼ���� */
	u16 state_acc_x;						//�����˼��ٶ�x����ֵ   16λAD  ����+-4g
	u16 state_acc_y;						//�����˼��ٶ�y����ֵ   16λAD  ����+-4g
	u16 state_acc_z;						//�����˼��ٶ�z����ֵ   16λAD  ����+-4g
	u16 state_gyro_x;						//������������x����ֵ   16λAD  ����+-2000dps	
	u16 state_gyro_y;						//������������y����ֵ   16λAD  ����+-2000dps		
	u16 state_gyro_z;						//������������z����ֵ   16λAD  ����+-2000dps	
	u16 state_mag_x;						//�����˴�ǿ��x����ֵ   16λAD  ����+-4800uT		
	u16 state_mag_y;						//�����˴�ǿ��y����ֵ   16λAD  ����+-4800uT			
	u16 state_mag_z;						//�����˴�ǿ��z����ֵ   16λAD  ����+-4800uT			
		
} SensorData_TypeDef;



/****************************************************************************************
											��������
*****************************************************************************************/

/*  ��ѯ/���µ������ */
void SetBatteryData(float *vol, float *cur);
void GetBatteryData(float *vol, float *cur);

/*  ��ѯ/����ˮ������  */
void SetDeepData(float *deep);
void GetDeepData(float *deep);

/*  ��ѯ/������ѹ�¶����� */
void SetPTData(float *press, float *temp);
void GetPTData(float *press, float *temp);

/*  ��ѯ/������ѹ�¶����� */
void SetAttitudeData(float *roll, float *pitch, float *yaw);
void GetAttitudeData(float *roll, float *pitch, float *yaw);

/*  ��ѯ/����������ԭʼ����*/
void SetGyroData(float *gx, float *gy, float *gz);
void GetGyroData(float *gx, float *gy, float *gz);

/*  ��ѯ/���¼��ٶ�ԭʼ����*/
void SetAccData(float *ax, float *ay, float *az);
void GetAccData(float *ax, float *ay, float *az);

/*  ��ѯ/���´�ǿ��ԭʼ����*/
void SetMagData(float *mx, float *my, float *mz);
void GetMagData(float *mx, float *my, float *mz);

/*  ��ѯ/�������д���������*/
void SetSensorData(SensorData_TypeDef *data);
void GetSensorData(SensorData_TypeDef *data);



#endif
