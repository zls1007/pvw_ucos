/**
  ******************************************************************************
  * @file    spi.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-08-15
  * @brief   ���ļ���spi1�������ã��������mpu9250�������ݲɼ�
	*			Ӳ����Դ��
  *          	SPI1  ����Ƶ��1Mhz			���жϷ�ʽ
  *
	*			������
	*						SPI1_Init(void);     spi1���ú���
	*						SPI_RW(u8 byte);		 spi1��д��������ȡĳ�Ĵ�����ַ��ֵ
*/
#include "spi.h"
/****************************************************************************
*			SPI1 ���ú���
*			PA5 ---- SCK	
*			PA6 ---- MISO   
*			PA7 ---- MOSI	
*			PC2 ---- CS 
*			PD15 ---- DRDY 
*			256Mhz/256
******************************************************************************/
void SPI1_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

		//����ʱ��
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD ,ENABLE);
	
		//����GPIO�����ã�
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	  //����GPIO�����������
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
  	GPIO_SetBits(GPIOC, GPIO_Pin_2);//��ѡ��
		
		//SPI1����
  	SPI_I2S_DeInit(SPI1);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ����ģʽ
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ģʽ��SCKΪ1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//���ݲ����ӵ�2��ʱ����ؿ�ʼ
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�������
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//������
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ģʽ
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC����ʽ
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����ģʽ
  	SPI_Init(SPI1, &SPI_InitStructure);
  	SPI_Cmd(SPI1, ENABLE);
}

/****************************************************************************
*			SPI1 ���Ĵ���ֵ����
*			���룺
*					byte -- �Ĵ�����ַ
*			���أ�
*					�Ĵ�����ֵ
******************************************************************************/
u8 SPI_RW(u8 byte)
{
 	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET);
 	SPI1->DR = byte;
 	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET);
 	return(SPI1->DR);
}
