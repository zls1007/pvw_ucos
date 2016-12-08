#ifndef __BATTERY_H_
#define __BATTERY_H_
#include "stm32f4xx.h"
#include "iic.h"



#define BatteryTemperCmd 			0x08    //��ȡ��صĲ����¶�
#define BatteryVoltageCmd			0x09		//��ȡ��ص��ܵ�ѹֵ
#define BatteryCurrentCmd			0x0a		//��ȡ��صĵ���ֵ
#define BatteryAverageCurCmd	0x0b		//��ȡ��ص�ƽ������ֵ    //��ʱû��
#define BatteryRelativeCapCmd 0x0d    //��ȡ��ص���������ٷֱ�
#define BatteryAbsoluteCapCmd 0x0e    //��ȡ��صľ��������ٷֱ�

s16 Battery_Read_Word(u8 command);
float GetBatteryTemper(void);
float GetBatteryVoltage(void);
float GetBatteryCurrent(void);
float GetBatteryAverageCur(void);
s8 GetBatteryRelativeCap(void);
s8 GetBatteryAbsoluteCap(void);

void ReadBatteryInfo(float *temper, float *vol, float *cur, s8 *relcap, s8 *abscap);

#endif
