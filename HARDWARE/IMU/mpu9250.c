
#include "mpu9250.h"
#include "spi.h"
#include "delay.h"
#include "usart.h"

#define _USE_MAG_AK8963
#define MAG_READ_DELAY 256

#define SPIx_CS_H()  GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define SPIx_CS_L()  GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define Byte16(Type, ByteH, ByteL)  ((Type)((((uint16_t)(ByteH))<<8) | ((uint16_t)(ByteL))))

#ifdef _USE_MAG_AK8963
int16_t AK8963_ASA[3] = {0};
#endif
/****************************************************************************
*			MPU9250 给寄存器写数据函数（单个寄存器）
*
*			writeAddr ---- 寄存器地址
*			writeData ---- 数据  
******************************************************************************/
void MPU9250_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  SPIx_CS_L();
  SPI_RW(writeAddr);
  SPI_RW(writeData);
  SPIx_CS_H();
}
/****************************************************************************
*			MPU9250 给寄存器写数据函数（多个寄存器）
*
*			writeAddr ---- 寄存器地址
*			writeData ---- 数据数组
*			lens ---- 数据长度
******************************************************************************/
void MPU9250_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  uint8_t i;
	SPIx_CS_L();
  SPI_RW(writeAddr);
  for(i = 0; i < lens; i++)
    SPI_RW(writeData[i]);
  SPIx_CS_H();
}

/****************************************************************************
*			MPU9250 读寄存器数据函数（单个寄存器）
*
*			readAddr ---- 寄存器地址
*			
*			返回：寄存器的值
******************************************************************************/
uint8_t MPU9250_ReadReg( uint8_t readAddr )
{
  uint8_t readData = 0;

  SPIx_CS_L();
  SPI_RW(0x80 | readAddr);
  readData = SPI_RW(0x00);
  SPIx_CS_H();

  return readData;
}

/****************************************************************************
*			MPU9250 读寄存器数据函数（多个寄存器）
*
*			readAddr ---- 寄存器地址
*			readData ---- 寄存器的值
*			lens     ---- 数据长度
******************************************************************************/
void MPU9250_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  uint8_t i;
	SPIx_CS_L();
  SPI_RW(0x80 | readAddr);
  for(i = 0; i < lens; i++)
    readData[i] = SPI_RW(0x00);
  SPIx_CS_H();
}

/****************************************************************************
*			AK8963 给寄存器写数据函数（单个寄存器）
*
*			writeAddr ---- 寄存器地址
*			writeData ---- 数据  
******************************************************************************/
#ifdef _USE_MAG_AK8963
void MPU9250_Mag_WriteReg( uint8_t writeAddr, uint8_t writeData )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_DO, writeData);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  delay_ms(1);

  do {
    status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    delay_ms(1);
  } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
}

#endif
/****************************************************************************
*			AK8963 给寄存器写数据函数（多个寄存器）
*
*			writeAddr ---- 寄存器地址
*			writeData ---- 数据数组
*			lens ---- 数据长度
******************************************************************************/
#ifdef _USE_MAG_AK8963
void MPU9250_Mag_WriteRegs( uint8_t writeAddr, uint8_t *writeData, uint8_t lens )
{
  uint8_t  status = 0;
  uint32_t timeout = MAG_READ_DELAY;
	uint8_t i;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR);
  delay_ms(1);
  for(i = 0; i < lens; i++) {
    MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, writeAddr + i);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_DO, writeData[i]);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    delay_ms(1);

    do {
      status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));
  }
}
#endif
/****************************************************************************
*			AK8963 读寄存器数据函数（单个寄存器）
*
*			readAddr ---- 寄存器地址
*			
*			返回：寄存器的值
******************************************************************************/
#ifdef _USE_MAG_AK8963
uint8_t MPU9250_Mag_ReadReg( uint8_t readAddr )
{
  uint8_t status = 0;
  uint8_t readData = 0;
  uint32_t timeout = MAG_READ_DELAY;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, readAddr);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
  delay_ms(1);

  do {
    status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    delay_ms(1);
  } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

  readData = MPU9250_ReadReg(MPU6500_I2C_SLV4_DI);

  return readData;
}
#endif
/****************************************************************************
*			AK8963 读寄存器数据函数（多个寄存器）
*
*			readAddr ---- 寄存器地址
*			readData ---- 寄存器的值
*			lens     ---- 数据长度
******************************************************************************/
#ifdef _USE_MAG_AK8963
void MPU9250_Mag_ReadRegs( uint8_t readAddr, uint8_t *readData, uint8_t lens )
{
  uint8_t status = 0;
  uint32_t timeout = MAG_READ_DELAY;
	uint8_t i;

  MPU9250_WriteReg(MPU6500_I2C_SLV4_ADDR, AK8963_I2C_ADDR | 0x80);
  delay_ms(1);
  for(i = 0; i< lens; i++) {
    MPU9250_WriteReg(MPU6500_I2C_SLV4_REG, readAddr + i);
    delay_ms(1);
    MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, MPU6500_I2C_SLVx_EN);
    delay_ms(1);

    do {
      status = MPU9250_ReadReg(MPU6500_I2C_MST_STATUS);
    } while(((status & MPU6500_I2C_SLV4_DONE) == 0) && (timeout--));

    readData[i] = MPU9250_ReadReg(MPU6500_I2C_SLV4_DI);
    delay_ms(1);
  }
}
#endif

/****************************************************************************
*			MPU9250 配置函数
*
*			MPUx     ---- 量程配置参数
*
*			返回     ---- 成功与否
******************************************************************************/
#define MPU6500_InitRegNum 11
uint8_t MPU9250_Init(MPU_InitTypeDef *MPUx)
{
  uint8_t status = ERROR;
	
	#ifdef _USE_MAG_AK8963
  uint8_t tmpRead[3] = {0};
	#endif
	
	uint8_t i;

  uint8_t MPU6500_InitData[MPU6500_InitRegNum][2] = {
    {0x80, MPU6500_PWR_MGMT_1},     // [0]  Reset Device
    {0x04, MPU6500_PWR_MGMT_1},     // [1]  Clock Source
    {0x10, MPU6500_INT_PIN_CFG},    // [2]  Set INT_ANYRD_2CLEAR
    {0x01, MPU6500_INT_ENABLE},     // [3]  Set RAW_RDY_EN
    {0x00, MPU6500_PWR_MGMT_2},     // [4]  Enable Acc & Gyro
    {0x00, MPU6500_SMPLRT_DIV},     // [5]  Sample Rate Divider
    {0x18, MPU6500_GYRO_CONFIG},    // [6]  default : +-2000dps
    {0x08, MPU6500_ACCEL_CONFIG},   // [7]  default : +-4G
    {0x07, MPU6500_CONFIG},         // [8]  default : LPS_41Hz
    {0x03, MPU6500_ACCEL_CONFIG_2}, // [9]  default : LPS_41Hz
    {0x30, MPU6500_USER_CTRL},      // [10] Set I2C_MST_EN, I2C_IF_DIS
  };

  MPU6500_InitData[6][0] = MPUx->MPU_Gyr_FullScale;       // MPU6500_GYRO_CONFIG
  MPU6500_InitData[8][0] = MPUx->MPU_Gyr_LowPassFilter;   // MPU6500_CONFIG
  MPU6500_InitData[7][0] = MPUx->MPU_Acc_FullScale;       // MPU6500_ACCEL_CONFIG
  MPU6500_InitData[9][0] = MPUx->MPU_Acc_LowPassFilter;   // MPU6500_ACCEL_CONFIG_2

  for(i = 0; i < MPU6500_InitRegNum; i++) {
    MPU9250_WriteReg(MPU6500_InitData[i][1], MPU6500_InitData[i][0]);
    delay_ms(1);
  }
	
	delay_ms(100);
	 status = MPU9250_Check();
	//printf("status=%d", status);
  if(status != SUCCESS)
    return ERROR;
  delay_ms(10); 
	
	#ifdef _USE_MAG_AK8963
  MPU9250_Mag_WriteReg(AK8963_CNTL2, 0x01);       // Reset Device
  delay_ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);       // Power-down mode
  delay_ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x1F);       // Fuse ROM access mode
  delay_ms(1);
  MPU9250_Mag_ReadRegs(AK8963_ASAX, tmpRead, 3);  // Read sensitivity adjustment values
  delay_ms(1);
  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x10);       // Power-down mode
  delay_ms(1);

  AK8963_ASA[0] = (int16_t)(tmpRead[0]) + 128;
  AK8963_ASA[1] = (int16_t)(tmpRead[1]) + 128;
  AK8963_ASA[2] = (int16_t)(tmpRead[2]) + 128;

  MPU9250_WriteReg(MPU6500_I2C_MST_CTRL, 0x5D);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_ADDR, AK8963_I2C_ADDR | 0x80);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_REG, AK8963_ST1);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_SLV0_CTRL, MPU6500_I2C_SLVx_EN | 8);
  delay_ms(1);

  MPU9250_Mag_WriteReg(AK8963_CNTL1, 0x16);       // Continuous measurement mode 2
  delay_ms(1);

  MPU9250_WriteReg(MPU6500_I2C_SLV4_CTRL, 0x09);
  delay_ms(1);
  MPU9250_WriteReg(MPU6500_I2C_MST_DELAY_CTRL, 0x81);
  delay_ms(100);
#endif


	//MPU9250_SetSpeed(32);
  delay_ms(10);

  return SUCCESS;
}

/****************************************************************************
*			MPU9250 检测设备id
*
*			输入     ----  无
*
*			返回     ---- 成功与否
******************************************************************************/
uint8_t MPU9250_Check( void )
{
  uint8_t deviceID = ERROR;

  deviceID = MPU9250_ReadReg(MPU6500_WHO_AM_I);
	printf("mpuId=%d\r\n", deviceID);
  if(deviceID != MPU6500_Device_ID)
    return ERROR;
	
	#ifdef _USE_MAG_AK8963
  deviceID = MPU9250_Mag_ReadReg(AK8963_WIA);
	printf("magId=%d\r\n", deviceID);
  if(deviceID != AK8963_Device_ID)
    return ERROR;
	#endif

  return SUCCESS;
}

/****************************************************************************
*			MPU9250 读取数据
*
*			dataIMU  ----  返回的数据值
*
******************************************************************************/
void MPU9250_getData( int16_t *dataIMU )
{
  uint8_t tmpRead[22] = {0};

	//读取数据
	#ifdef _USE_MAG_AK8963
		MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 22);
	#else
		MPU9250_ReadRegs(MPU6500_ACCEL_XOUT_H, tmpRead, 14);
	#endif

	//数据处理
  dataIMU[0] = (Byte16(int16_t, tmpRead[6],  tmpRead[7]));    // Temp
  dataIMU[1] = (Byte16(int16_t, tmpRead[0],  tmpRead[1]));    // Acc.X
  dataIMU[2] = (Byte16(int16_t, tmpRead[2],  tmpRead[3]));    // Acc.Y
  dataIMU[3] = (Byte16(int16_t, tmpRead[4],  tmpRead[5]));    // Acc.Z
  dataIMU[4] = (Byte16(int16_t, tmpRead[8],  tmpRead[9]));    // Gyr.X
  dataIMU[5] = (Byte16(int16_t, tmpRead[10], tmpRead[11]));   // Gyr.Y
  dataIMU[6] = (Byte16(int16_t, tmpRead[12], tmpRead[13]));   // Gyr.Z 
	
	#ifdef _USE_MAG_AK8963
  if(!(tmpRead[14] & AK8963_STATUS_DRDY) || (tmpRead[14] & AK8963_STATUS_DOR) || (tmpRead[21] & AK8963_STATUS_HOFL))
    return;

  dataIMU[7] = (Byte16(int16_t, tmpRead[16], tmpRead[15]));   // Mag.X
  dataIMU[8] = (Byte16(int16_t, tmpRead[18], tmpRead[17]));   // Mag.Y
  dataIMU[9] = (Byte16(int16_t, tmpRead[20], tmpRead[19]));   // Mag.Z
	#endif  
}

/****************************************************************************
*			MPU9250 spi通讯速率设置
*
******************************************************************************/
void MPU9250_SetSpeed(uint8_t speedSel)
{
	SPI_InitTypeDef  SPI_InitStructure;
		
	//SPI1配置
	SPI_Cmd(SPI1, DISABLE);
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//空闲模式下SCK为1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据采样从第2个时间边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//波特率
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//大端模式
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC多项式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

/****************************************************************************
*			MPU9250 设备初始化
*
******************************************************************************/
void MPU9250_Config(void)
{
  MPU_InitTypeDef MPU_InitStruct;
	
	SPI1_Init();   //spi初始化
	
  delay_ms(10);
	
  MPU_InitStruct.MPU_Gyr_FullScale     = MPU_GyrFS_2000dps;
  MPU_InitStruct.MPU_Gyr_LowPassFilter = MPU_GyrLPS_41Hz;
  MPU_InitStruct.MPU_Acc_FullScale     = MPU_AccFS_4g;
  MPU_InitStruct.MPU_Acc_LowPassFilter = MPU_AccLPS_41Hz;
  if(MPU9250_Init(&MPU_InitStruct) != SUCCESS) 
	{
    printf("MPU9250 ERROR\r\n");
    while(1) 
		{
      if(MPU9250_Init(&MPU_InitStruct) != SUCCESS) 
				delay_ms(200);
			else 
				break;
    }
  }
  else 
	{
    printf("MPU9250 SUCCESS\r\n");
  }

  delay_ms(100);
}
