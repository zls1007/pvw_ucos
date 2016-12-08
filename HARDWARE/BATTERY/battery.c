
#include "battery.h"
#include "iic.h"
#include "includes.h"

//==========================================
//读取字节
//==========================================
s16 Battery_Read_Word(u8 command)
{
	u8 i = 0;
	u8 read_buf[3];  	//暂存读取的数据
	u8 crc = 0x00;   	//crc校验值
	u16 word;					//读到的数据
	
	//开始读取
	I2C_Start();                				//起始信号
	I2C_Write(SlaveAddress);    	//发送设备地址+写信号（+0）
	I2C_Write(command);    				 	//发送存储单元地址
	I2C_Start();                				//起始信号
	I2C_Write(SlaveAddress+1); 	//发送设备地址+读信号（+1）	
	
	//I2C_Delay();
	for(i = 0;i < 3;i++)
	{
		read_buf[i] = I2C_Read();
		if(i == 2)
			I2C_SendACK(1);			
		else
			I2C_SendACK(0);
	}
	I2C_Stop();			//读取完成

	//word = ((u16)read_buf[1]<<8) + read_buf[0];
	word = read_buf[1]*0xff + read_buf[0];
	//printf("value:%d, %d, %d\r\n", read_buf[0], read_buf[1], read_buf[2]);
	
	//crc 校验
	crc = GetCrc8(crc, SlaveAddress);
	crc = GetCrc8(crc, command);
	crc = GetCrc8(crc, SlaveAddress+1);
	crc = GetCrc8(crc, read_buf[0]);
	crc = GetCrc8(crc, read_buf[1]);
	if(crc != read_buf[2])
		return -1;
	else
	{
		//printf("crc ok\r\n");
		return word;
	}		
	
}

//==========================================
//读取字节
//==========================================
s16 Battery_Read_Halfword(u8 command)
{
	u8 i = 0;
	u8 read_buf[3];  	//暂存读取的数据
	u8 crc = 0x00;   	//crc校验值
	u16 word;					//读到的数据
	
	//开始读取
	I2C_Start();                				//起始信号
	I2C_Write(SlaveAddress);    	//发送设备地址+写信号（+0）
	I2C_Write(command);    				 	//发送存储单元地址
	I2C_Start();                				//起始信号
	I2C_Write(SlaveAddress+1); 	//发送设备地址+读信号（+1）	
//	for(i=0; i<15; i++)
//	{
//		I2C_Delay();  //延时
//	}
	delay_ms(1);
	

	for(i = 0;i < 3;i++)
	{
		read_buf[i] = I2C_Read();
		if(i == 2)
			I2C_SendACK(1);			
		else
			I2C_SendACK(0);
	}
	I2C_Stop();			//读取完成
	
	//word = ((u16)read_buf[1]<<8) + read_buf[0];
	word = read_buf[1]*0xff + read_buf[0];
	//printf("value:%d, %d, %d\r\n", read_buf[0], read_buf[1], read_buf[2]);
	
	//crc 校验
	crc = GetCrc8(crc, SlaveAddress);
	crc = GetCrc8(crc, command);
	crc = GetCrc8(crc, SlaveAddress+1);
	crc = GetCrc8(crc, read_buf[0]);
	crc = GetCrc8(crc, read_buf[1]);
	if(crc != read_buf[2])
		return -1;
	else
	{
		//printf("crc ok\r\n");
		return word;
	}		
	
}


//==========================================
//读取电池温度  单位：摄氏度
//若错误，返回-1
//==========================================
float GetBatteryTemper(void)
{
	s16 wold;
	float temper;
	wold = Battery_Read_Word(BatteryTemperCmd);
	
	if(wold == -1)  return -1;
	
	temper = (wold - 2731) * 0.1;
	return temper;
}

//==========================================
//读取电池电压 单位：v
//若错误，返回-1
//==========================================
float GetBatteryVoltage(void)
{
	s16 wold;
	float voltage;
	wold = Battery_Read_Word(BatteryVoltageCmd);
	
	if(wold == -1)  return -1;
	
	voltage = wold * 0.001;
	//printf("vol=%.2f\r\n", voltage);
	return voltage;
}

//==========================================
//读取电池电流  单位：a
//若错误，返回-1
//==========================================
float GetBatteryCurrent(void)
{
	s16 wold;
	float current;
	wold = Battery_Read_Word(BatteryCurrentCmd);
	
	if(wold == -1)  return -1;
	
	current = (s16)wold * 0.001;
	return current;
}

//==========================================
//读取电池平均电流  单位：a
//若错误，返回-1
//==========================================
float GetBatteryAverageCur(void)
{
	s16 wold;
	float current;
	wold = Battery_Read_Word(BatteryAverageCurCmd);
	
	if(wold == -1)  return -1;
	
	current = wold * 0.001;
	return current;
}

//==========================================
//读取电池相对容量百分比  单位：%
//若错误，返回-1
//==========================================
s8 GetBatteryRelativeCap(void)
{
	s16 halfwold;
	s8 capacity;
	halfwold = Battery_Read_Halfword(BatteryRelativeCapCmd);
	
	if(halfwold == -1)  return -1;
	
	capacity = halfwold;
	return capacity;
}

//==========================================
//读取电池绝对容量百分比  单位：%
//若错误，返回-1
//==========================================
s8 GetBatteryAbsoluteCap(void)
{
	s16 halfwold;
	s16 capacity;
	halfwold = Battery_Read_Halfword(BatteryAbsoluteCapCmd);
	
	if(halfwold == -1)  return -1;
	
	capacity = halfwold;
	return capacity;
}

//==========================================
//输出电池参数
//==========================================
void ReadBatteryInfo(float *temper, float *vol, float *cur, s8 *relcap, s8 *abscap)
{
	
	*temper = GetBatteryTemper();
	*vol = GetBatteryVoltage();
	*cur = GetBatteryCurrent();

	*relcap = GetBatteryRelativeCap();
	*abscap = GetBatteryAbsoluteCap();

}
	

