#include "system_param.h"

static SysState_Type mSysState;   	//系统运行状态
static MoveMode_Type mMoveMode;	 		//系统运动模式
static UserCmd_TypeDef mUserCmd;		//用户命令输入

/****************************************************************************************
											操作函数
*****************************************************************************************/
/*  系统参数初始化  */
void system_param_init(void)
{
	mSysState = MODE_MOVE;   //系统处于正常运动模式
	
	mMoveMode = MODE_MANUAL;  //手动运动模式
	
	mUserCmd.usercmd_move_x = 0;  //初始状态：静止
	mUserCmd.usercmd_move_r = 0;
	mUserCmd.usercmd_move_z = 0;
	
	mUserCmd.usercmd_led_1 = 20;		//初始状态：微亮
	mUserCmd.usercmd_led_2 = 20;
}



/*  查询/更新系统运行状态  */
void SetSysState(SysState_Type *state)
{
	mSysState = *state;
}
void GetSysState(SysState_Type *state)
{
	*state = mSysState;
}

/*  查询/更新运动模式  */
void SetMoveMode(MoveMode_Type *mode)
{
	mMoveMode = *mode;
}
void GetMoveMode(MoveMode_Type *mode)
{
	*mode = mMoveMode;
}

/*  查询/更新用户运动速度 */
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

/*  查询/更新用户灯光亮度 */
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
