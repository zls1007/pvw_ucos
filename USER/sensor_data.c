#include "sensor_data.h"
#include "string.h"

static SensorData_TypeDef  mSensorData;    //ϵͳȫ�ִ��������ݽṹ��

/****************************************************************************************
											��������
*****************************************************************************************/

/*  ��ѯ/���µ������ */
void SetBatteryData(float *vol, float *cur)
{
	mSensorData.state_volatage = *vol;
	mSensorData.state_currage  = *cur;
}
void GetBatteryData(float *vol, float *cur)
{
	*vol = mSensorData.state_volatage;
	*cur = mSensorData.state_currage;
}

/*  ��ѯ/����ˮ������  */
void SetDeepData(float *deep)
{
	mSensorData.state_deep = *deep;
}
void GetDeepData(float *deep)
{
	*deep = mSensorData.state_deep;
}

/*  ��ѯ/������ѹ�¶����� */
void SetPTData(float *press, float *temp)
{
	mSensorData.state_atmosphsric = *press;
	mSensorData.state_temperature = *temp;
}
void GetPTData(float *press, float *temp)
{
	*press = mSensorData.state_atmosphsric;
	*temp  = mSensorData.state_temperature;
}

/*  ��ѯ/������ѹ�¶����� */
void SetAttitudeData(float *roll, float *pitch, float *yaw)
{
	mSensorData.state_roll   = *roll;
	mSensorData.state_pitch  = *pitch;
	mSensorData.state_yaw 	 = *yaw;
}
void GetAttitudeData(float *roll, float *pitch, float *yaw)
{
	*roll  = mSensorData.state_roll;
	*pitch = mSensorData.state_pitch;
	*yaw   = mSensorData.state_yaw;
}

/*  ��ѯ/����������ԭʼ����*/
void SetGyroData(float *gx, float *gy, float *gz)
{
	mSensorData.state_gyro_x = *gx;
	mSensorData.state_gyro_y = *gy;
	mSensorData.state_gyro_z = *gz;
}
void GetGyroData(float *gx, float *gy, float *gz)
{
	*gx = mSensorData.state_gyro_x;
	*gy = mSensorData.state_gyro_y;
	*gz = mSensorData.state_gyro_z;
}

/*  ��ѯ/���¼��ٶ�ԭʼ����*/
void SetAccData(float *ax, float *ay, float *az)
{
	mSensorData.state_acc_x = *ax;
	mSensorData.state_acc_y = *ay;
	mSensorData.state_acc_z = *az;
}
void GetAccData(float *ax, float *ay, float *az)
{
	*ax = mSensorData.state_acc_x;
	*ay = mSensorData.state_acc_y;
	*az = mSensorData.state_acc_z;
}

/*  ��ѯ/���´�ǿ��ԭʼ����*/
void SetMagData(float *mx, float *my, float *mz)
{
	mSensorData.state_mag_x = *mx;
	mSensorData.state_mag_y = *my;
	mSensorData.state_mag_z = *mz;
}
void GetMagData(float *mx, float *my, float *mz)
{
	*mx = mSensorData.state_mag_x;
	*my = mSensorData.state_mag_y;
	*mz = mSensorData.state_mag_z;
}

/*  ��ѯ/�������д���������*/
void SetSensorData(SensorData_TypeDef *data)
{
	memcpy(&mSensorData, data, sizeof(SensorData_TypeDef));
}
void GetSensorData(SensorData_TypeDef *data)
{
	memcpy(data, &mSensorData, sizeof(SensorData_TypeDef));
}
