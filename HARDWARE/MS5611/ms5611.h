#ifndef __MS5611_H
#define __MS5611_H
#include "stm32f4xx.h"

#define ATMOSPHERE_ENABLE 	          GPIO_ResetBits(GPIOD, GPIO_Pin_7)    //片选有效
#define ATMOSPHERE_DISENABLE          GPIO_SetBits(GPIOD, GPIO_Pin_7)      //片选无效


/*MS5611模块传感器地址定义*/
#define MS5611_ADC       			0x00
#define CMD_MS5611_RESET 			0x1E
#define CMD_MS5611_PROM_Setup 0xA0
#define CMD_MS5611_PROM_C1 		0xA2
#define CMD_MS5611_PROM_C2 		0xA4
#define CMD_MS5611_PROM_C3 		0xA6
#define CMD_MS5611_PROM_C4 		0xA8
#define CMD_MS5611_PROM_C5 		0xAA
#define CMD_MS5611_PROM_C6 		0xAC
#define CMD_MS5611_PROM_CRC 	0xAE
#define CMD_CONVERT_D1_OSR4096 0x48   // Maximun resolution
#define CMD_CONVERT_D2_OSR4096 0x58   // Maximun resolution

/*相关函数声明*/
void SPI2_Init(void);
void SPI2_SetSpeed(u8 SpeedSet); 
u8 SPI2_ReadWriteByte(u8 TxData);
u8 ATMOSPHERE_Write_Reg(u8 REG_Address,u8 REG_data);
u8 ATMOSPHERE_Read_Reg(u8 REG_Address);
void MS5611_RESET(void);
u16  SPI1_ReadWrite_16bits(unsigned char addr);
void MS5611_PROM_READ(void);
long MS5611_SPI_read_ADC(void);
float MS5611_getPressure(unsigned char OSR_Pres);
float MS5611_getTemperature(unsigned char OSR_Temp);
float get_altitude(void);
void MS5611_Config(void);
#endif
