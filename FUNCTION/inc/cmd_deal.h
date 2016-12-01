#ifndef __CMD_DEAL_H
#define __CMD_DEAL_H
#include "includes.h"
#include "param.h"

//与用户通信类型定义
#define ID_TYPE_GET_STATE				1				//状态查询
#define ID_TYPE_MOVE_VOL  			2				//速度控制
#define ID_TYPE_CMD_LED					3				//灯光控制
#define ID_TYPE_MOVE_MODE				4				//运动模式选择
#define ID_TYPE_EMERGENCY_STOP	5				//紧急停止

#define ID_TYPE_MODE_PARAM			11			//进入调参模式
#define ID_TYPE_MODE_MOVE				12			//进入运动模式
#define ID_TYPE_MOVE_CHANNEL		13			//单通道控制

#define ID_TYPE_READ_PID				15			//读取PID参数值
#define ID_TYPE_READ_IMU				16			//读取IMU数据
#define ID_TYPE_READ_MSG				17			//读取磁强计数据
#define ID_TYPE_READ_DEEP				18			//读取水压计数据

#define ID_TYPE_SET_PID					21			//设置PID参数值
#define ID_TYPE_SET_GYRO_OFFSET 22			//设置陀螺仪静态偏差
#define ID_TYPE_SET_MSG_EXTREME 23			//设置磁强计极值
#define ID_TYPE_SET_DEEP_ZERO		24			//设置水压计零点
#define ID_TYPE_SET_MOTER_ZERO	25			//设置电机死区
#define ID_TYPE_RESUME_BACKUP		31			//恢复出厂设置
#define ID_TYPE_UPDATE_APP			32			//更新程序

//当前led命令状态
typedef struct led_cmd_t
{
	s16 id;  					//当前照明亮度所对应的id  -1表示没有用户发送led控制指令
	s16 cnt;					//1s接收到该id命令的次数
	s16 light;				//此时的亮度值
} LedCmd_TypeDef;

//当前运动命令状态
typedef struct move_cmd_t
{
	s16 id;						//当前响应的运动用户id  -1表示没有用户发送运动命令
	u8 ismove;				//当前机器人是否处于运动状态 0-静止  1-运动
	s16 cnt;					//1s接收到来自该id命令的次数
	s16 x, r, z;			//此时运动的指令
} MoveCmd_TypeDef;

//定时器任务（1s）
void tmr1_callback(void);


//把字符串解析到结构体
void CmdAnalysis(Cmd_TypeDef *cmd, CPU_INT08U* p, CPU_INT08U size);

//对命令进行路由
void CmdRouter(Cmd_TypeDef *cmd);

//消息处理函数
void GetStateFuc(Cmd_TypeDef *cmd);
void MoterVolFuc(Cmd_TypeDef *cmd); 
void CmdLedFuc(Cmd_TypeDef *cmd);
void MoveModeSetFuc(Cmd_TypeDef *cmd);
void EmergencyStopFuc(Cmd_TypeDef *cmd);

void ModeParamFuc(Cmd_TypeDef *cmd);
void ModeMoveFuc(Cmd_TypeDef *cmd);
void MoveChannelFuc(Cmd_TypeDef *cmd);
void ReadPIDFuc(Cmd_TypeDef *cmd);
void ReadImuFuc(Cmd_TypeDef *cmd);
void ReadMsgFuc(Cmd_TypeDef *cmd);
void ReadDeepFuc(Cmd_TypeDef *cmd);
void SetPIDFuc(Cmd_TypeDef *cmd);
void SetGyroOffsetFuc(Cmd_TypeDef *cmd);
void SetMsgExtremeFuc(Cmd_TypeDef *cmd);
void SetDeepZeroFuc(Cmd_TypeDef *cmd);
void SetMoterZeroFuc(Cmd_TypeDef *cmd);
void ResumeBuckupFuc(Cmd_TypeDef *cmd);
void UpdateAppFuc(Cmd_TypeDef *cmd);

//信息反馈
void CmdFeedbackNoData(Cmd_TypeDef *cmd, ComErr_Type err);
void CmdFeedbackRunState(void);
void CmdFeedbackPIDParam(void);
void CmdFeedbackIMUParam(void);
void CmdFeedbackMSGParam(void);
void CmdFeedbackDEEPParam(void);



#endif
