#ifndef __BATTERY_H_
#define __BATTERY_H_
#include "stm32f4xx.h"
#include "iic.h"



#define BatteryTemperCmd 			0x08    //读取电池的测量温度
#define BatteryVoltageCmd			0x09		//读取电池的总电压值
#define BatteryCurrentCmd			0x0a		//读取电池的电流值
#define BatteryAverageCurCmd	0x0b		//读取电池的平均电流值    //暂时没用
#define BatteryRelativeCapCmd 0x0d    //读取电池的相对容量百分比
#define BatteryAbsoluteCapCmd 0x0e    //读取电池的绝对容量百分比

s16 Battery_Read_Word(u8 command);
float GetBatteryTemper(void);
float GetBatteryVoltage(void);
float GetBatteryCurrent(void);
float GetBatteryAverageCur(void);
s8 GetBatteryRelativeCap(void);
s8 GetBatteryAbsoluteCap(void);

void ReadBatteryInfo(float *temper, float *vol, float *cur, s8 *relcap, s8 *abscap);

#endif
