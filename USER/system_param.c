#include "system_param.h"

static SysState_Type mSysState;   	//ϵͳ����״̬
static MoveMode_Type mMoveMode;	 		//ϵͳ�˶�ģʽ
static UserCmd_TypeDef mUserCmd;		//�û���������

/****************************************************************************************
											��������
*****************************************************************************************/
/*  ϵͳ������ʼ��  */
void system_param_init(void)
{
	mSysState = MODE_MOVE;   //ϵͳ���������˶�ģʽ
	
	mMoveMode = MODE_MANUAL;  //�ֶ��˶�ģʽ
	
	mUserCmd.usercmd_move_x = 0;  //��ʼ״̬����ֹ
	mUserCmd.usercmd_move_r = 0;
	mUserCmd.usercmd_move_z = 0;
	
	mUserCmd.usercmd_led_1 = 20;		//��ʼ״̬��΢��
	mUserCmd.usercmd_led_2 = 20;
}



/*  ��ѯ/����ϵͳ����״̬  */
void SetSysState(SysState_Type *state)
{
	mSysState = *state;
}
void GetSysState(SysState_Type *state)
{
	*state = mSysState;
}

/*  ��ѯ/�����˶�ģʽ  */
void SetMoveMode(MoveMode_Type *mode)
{
	mMoveMode = *mode;
}
void GetMoveMode(MoveMode_Type *mode)
{
	*mode = mMoveMode;
}

/*  ��ѯ/�����û��˶��ٶ� */
void SetMoveSpeed(s16 *x, s16 *r, s16 *z)
{
	mUserCmd.usercmd_move_x = *x;
	mUserCmd.usercmd_move_r = *r;
	mUserCmd.usercmd_move_z = *z;
}
void GetMoveSpeed(s16 *x, s16 *r, s16 *z)
{
	*x = mUserCmd.usercmd_move_x;
	*r = mUserCmd.usercmd_move_r;
	*z = mUserCmd.usercmd_move_z;
}

/*  ��ѯ/�����û��ƹ����� */
void SetLedLight(s16 *d1, s16 *d2)
{
	mUserCmd.usercmd_led_1 = *d1;
	mUserCmd.usercmd_led_2 = *d2;
}
void GetLedLight(s16 *d1, s16 *d2)
{
	*d1 = mUserCmd.usercmd_led_1;
	*d2 = mUserCmd.usercmd_led_2;
}
