#include "io_deal.h"
#include "usart_io.h"

//对接收到的消息进行处理
void IOMsgAnalysis(CPU_INT08U buf[], u8 size)
{
	Float_Type deep, cur, vol;		//水深、电流、电压值
	u8 IsWater;		//是否漏水
	
	
	deep.bit[0] = buf[3];
	deep.bit[1] = buf[4];
	deep.bit[2] = buf[5];
	deep.bit[3] = buf[6];
	cur.bit[0] = buf[7];
	cur.bit[1] = buf[8];
	cur.bit[2] = buf[9];
	cur.bit[3] = buf[10];
	vol.bit[0] = buf[11];
	vol.bit[1] = buf[12];
	vol.bit[2] = buf[13];
	vol.bit[3] = buf[14];
	IsWater = buf[15];
	
	deep.f = deep.f *1000 / 165;    //转换成电流mA
	deep.f = (deep.f - 4)*100/16;		//转换成深度m
	
	cur.f = (cur.f - 2.66) * 15;		//转换为电流A
  vol.f = vol.f * 4;							//转换为电压V
	
	cur.f += 0.3;
	deep.f += 0.5;
	
	//printf("deep=%.1fm cur=%.1fA vol=%.1fV\r\n", deep, cur, vol);

	//更新系统IO状态
	SetBatteryData(&vol.f, &cur.f);
	SetDeepData(&deep.f);

}

//发送消息，控制灯光亮度
void IOMsgSend(u8 d1, u8 d2)
{
	u8 i;
	u8 buf[8];
	buf[0] = 0xa5;
	buf[1] = 0x5a;
	buf[2] = 0xd1;
	buf[3] = d1;
	buf[4] = d2;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0xff;
	
	//发送
	io_print_hex(USART3, buf, 8);
}


