
/*����������MS5611����ѹǿģ�����ݲɼ����� V1.0*/
/********************************************************************************    
		
  * MS5611  SPI2
  * PB14 -- MISO
	* PB15 -- MOSI
	*	PB10 -- SCK
	* PD7  -- CS
*********************************************************************************/

#include "ms5611.h"
#include "delay.h"
#include "math.h"

u16 setup,C1,C2,C3,C4,C5,C6,CRC_1;  //���ڴ��PROM�е�8������
unsigned long D1_Pres,D2_Temp;      //���ڴ���¶Ⱥ�ѹ�� 
double OFF,SENS;                    //ʵ���¶ȵ�����ʵ���¶�������
float dT,Temperature,Pressure;      //ʵ�ʺͲο��¶�֮��Ĳ��졢�¶�ֵ��ѹ��ֵ
float TEMP2,Aux,OFF2,SENS2;         //�¶ȼ���ֵ

/*******************************************************************************
	����ѹǿģ���õ���SPI2�ӿڳ�ʼ��
	�������ƣ�SPI2_Init
	���������void
	���������void
	* PB14 -- MISO  �������
	* PB15 -- MOSI
	*	PB10 -- SCK
	* PD7  -- CS		�������
	* PE10 -- ����������ʹ��
	128MHz/256
*******************************************************************************/
void SPI2_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIODʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIODʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //ʹ��SPI2ʱ��
	
	//PD7 �������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;    					
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);     					 //��ʼ��
	
		//����������ʹ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//ѡ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//����Ϊ��ͨ���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//���Ƶ��Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//����Ϊ����   GPIO_PuPd_NOPULL(������)	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
		//������������
	GPIO_SetBits(GPIOE,GPIO_Pin_10);
 
	//PB10 PB14 PB15���ù������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;									 	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                        //���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                  //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);                              //��ʼ��   
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_SPI2);  //PB10����Ϊ SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);  //PB14����Ϊ SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);  //PB15����Ϊ SPI2
 
	//����ֻ���SPI�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);  //��λSPI2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE); //ֹͣ��λSPI2

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//128M/256=500k.
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE);   //ʹ��SPI����

	SPI2_ReadWriteByte(0xff);//��������		 	 
}   
/*******************************************************************************
  SPI2�ٶ����ú���
	�������ƣ�SPI2_SetSpeed
	���������SPI_BaudRatePrescaler
	���������void
  SPI�ٶ�=APB1/��Ƶϵ��
  @ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
  APB1ʱ��һ��Ϊ128Mhz
*******************************************************************************/
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI2->CR1&=0XFFC7;                                             //λ3-5���㣬�������ò�����
	SPI2->CR1|=SPI_BaudRatePrescaler;	                             //����SPI2�ٶ� 
	SPI_Cmd(SPI2,ENABLE);                                          //ʹ��SPI2
} 

/*******************************************************************************
	SPI2 ��дһ���ֽ�
	�������ƣ�SPI1_ReadWriteByte
	���������TxData
	�����������ȡ�����ֽ�
*******************************************************************************/
u8 SPI2_ReadWriteByte(u8 TxData)
{		 			 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ��byte  ����	
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����	    
}

/*******************************************************************************
	SPI2д�Ĵ���	
	�������ƣ�ATMOSPHERE_Write_Reg
	���������REG_Address��ָ���ļĴ�����ַ����REG_data(�Ĵ�����ֵ)
	�������������״ֵ̬
*******************************************************************************/
u8 ATMOSPHERE_Write_Reg(u8 REG_Address,u8 REG_data)
{
	u8 status;
	
	ATMOSPHERE_ENABLE;                        //ʹ�ܴ���ѹǿģ��
	status=SPI2_ReadWriteByte(REG_Address);   //����д����+�Ĵ�����
	SPI2_ReadWriteByte(REG_data);             //д��Ĵ���ֵ
	ATMOSPHERE_DISENABLE;                     //��ֹ����ѹǿģ��
	return(status);                           //����״ֵ̬
}

/*******************************************************************************
	MS5611ģ�鸴λ����
	�������ƣ�MS5611_RESET
	���������void
	���������void
*******************************************************************************/
void MS5611_RESET(void)
{
	 ATMOSPHERE_ENABLE;                         //ʹ�ܴ���ѹǿģ��
   SPI2_ReadWriteByte(CMD_MS5611_RESET);
	 ATMOSPHERE_DISENABLE;                      //��ֹ����ѹǿģ��
}

/*******************************************************************************
	SPI2 ��д�����ֽ�
	�������ƣ�SPI2_ReadWrite_16bits
	���������addr
	�����������ȡ����16λ����
*******************************************************************************/
u16  SPI2_ReadWrite_16bits(unsigned char addr)
{
  unsigned char byteH,byteL;
  unsigned int return_value;
	
  ATMOSPHERE_ENABLE;//ʹ�ܴ���ѹǿģ��  
  SPI2_ReadWriteByte(addr);
  byteH = SPI2_ReadWriteByte(0);
  byteL = SPI2_ReadWriteByte(0);
  ATMOSPHERE_DISENABLE;//��ֹ����ѹǿģ��
  return_value = ((unsigned int)byteH<<8) | (byteL);
  return(return_value);    
}

/*******************************************************************************
	��PROM��ȡ����У׼����
	�������ƣ�MS5611_PROM_READ
	���������void
	���������void
*******************************************************************************/
void MS5611_PROM_READ(void)
{
  ATMOSPHERE_ENABLE;//ʹ�ܴ���ѹǿģ�� 
  C1 = SPI2_ReadWrite_16bits(CMD_MS5611_PROM_C1);
  C2 = SPI2_ReadWrite_16bits(CMD_MS5611_PROM_C2);
  C3 = SPI2_ReadWrite_16bits(CMD_MS5611_PROM_C3);
  C4 = SPI2_ReadWrite_16bits(CMD_MS5611_PROM_C4);
  C5 = SPI2_ReadWrite_16bits(CMD_MS5611_PROM_C5);
  C6 = SPI2_ReadWrite_16bits(CMD_MS5611_PROM_C6);
  setup = SPI2_ReadWrite_16bits(CMD_MS5611_RESET);
  CRC_1 = SPI2_ReadWrite_16bits(CMD_MS5611_PROM_CRC); 
  ATMOSPHERE_DISENABLE;//��ֹ����ѹǿģ��
}

/*******************************************************************************
	��ȡADC���
	�������ƣ�MS5611_SPI_read_ADC
	���������void
	�������������24λ����ѹ/�¶�ֵ
*******************************************************************************/
long MS5611_SPI_read_ADC(void)
{
  unsigned char byteH,byteM,byteL;
  long return_value;
  
  ATMOSPHERE_ENABLE;//ʹ�ܴ���ѹǿģ��  
  SPI2_ReadWriteByte(MS5611_ADC);
  byteH = SPI2_ReadWriteByte(0);
  byteM = SPI2_ReadWriteByte(0);
  byteL = SPI2_ReadWriteByte(0);
  ATMOSPHERE_DISENABLE;//��ֹ����ѹǿģ��  
  return_value = (((long)byteH)<<16) | (((long)byteM)<<8) | (byteL);
  return(return_value);
}
/*******************************************************************************
	ͨ��MS5611ģ��õ���ѹֵ	10mbar ~ 1200mbar
	�������ƣ�MS5611_getPressure
	�����������ѹ�ֱ���ϵ��
	���������ʵ��ѹ��ֵ
*******************************************************************************/
float MS5611_getPressure(unsigned char OSR_Pres)
{
	u8 temp;
	
	ATMOSPHERE_ENABLE;                                 //ʹ�ܴ���ѹǿģ��  
  temp = SPI2_ReadWriteByte(CMD_CONVERT_D1_OSR4096);
	ATMOSPHERE_DISENABLE;                              //��ֹ����ѹǿģ��
	delay_ms(10);                                      //�����
  D1_Pres=MS5611_SPI_read_ADC();

  OFF=(unsigned long)C2*65536+((unsigned long)C4*dT)/128;
  SENS=(unsigned long)C1*32768+((unsigned long)C3*dT)/256;
  
  if(Temperature<2000)
  {
    // second order temperature compensation when under 20 degrees C
    TEMP2 = (dT*dT) / 0x80000000;
    //Aux = Temperature*Temperature;
		Aux = (Temperature-2000)*(Temperature-2000);
    OFF2 = 2.5*Aux;
    SENS2 = 1.25*Aux;
    
		Temperature = Temperature - TEMP2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2; 
  }
  Pressure=(D1_Pres*SENS/2097152-OFF)/32768;   //ʵ��ѹ��ֵ*0.01
	return(Pressure/100);
}

/*******************************************************************************
	ͨ��MS5611ģ��õ��¶�ֵ -40 ~ 85
	�������ƣ�MS5611_getTemperature
	����������¶ȷֱ���ϵ��
	���������void
*******************************************************************************/
float MS5611_getTemperature(unsigned char OSR_Temp)    
{
	u8 temp = 0;
		
	ATMOSPHERE_ENABLE;                                  //ʹ�ܴ���ѹǿģ��  
  temp = SPI2_ReadWriteByte(CMD_CONVERT_D2_OSR4096);
	ATMOSPHERE_DISENABLE;                               //��ֹ����ѹǿģ��
  delay_ms(10);                                       //�˴���ʱ�����
  D2_Temp=MS5611_SPI_read_ADC();	
  dT=D2_Temp - (((unsigned long)C5)<<8);
  Temperature=2000+dT*((unsigned long)C6)/8388608;		//ʵ���¶�*0.01
	return (Temperature/100);
}

/*******************************************************************************
	���ݴ���ѹ������θ߶� 
	�������ƣ�get_altitude
	���������void
	������������θ߶�
*******************************************************************************/
float get_altitude(void)                             
{
  float tmp_float,Altitude;    
	
  tmp_float = (Pressure / 101325.0);
  tmp_float = pow(tmp_float, 0.190295);               
  Altitude = 44330 * (1.0 - tmp_float);
  return (Altitude);
}

/*******************************************************************************
	����ѹ��ʼ��
	�������ƣ�MS5611_Config();
	���������void
	���������void
*******************************************************************************/
void MS5611_Config(void)
{
	 SPI2_Init();                                    //SPI1��ʼ��
   SPI2_SetSpeed(SPI_BaudRatePrescaler_256);	   //����SPI����  128M/256=500KHZ
   MS5611_RESET();                                 //MS5611��ʼ��
   MS5611_PROM_READ();                             //��ȡ�洢��(128-bit PROM)
	 delay_ms(100);
}

/*end*/


