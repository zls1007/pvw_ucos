#include "cmd_deal.h"
#include "io_deal.h"
#include "usart_cmd.h"
#include "param.h"



//存储命令返回数组
u8 feedback_buf[100];

//把字符串解析到数组
void CmdAnalysis(Cmd_TypeDef *cmd, CPU_INT08U* p, CPU_INT08U size)
{
	u8 type = 0;
	u8 prio = 0;
	u8 lng = 0;
	s16 data[10] = {0};
	
	u8 tag = 0;  //0-无  1-type  2-prio  3-lng  4-data
	u8 index = 0;    //数据区指针
	u8 isStart = 0;
	s8 isPlus = 1;
	u8 c;
	
	u8 i;
	for(i=0; i<size; i++)
	{
		c = p[i];
		if(c == 'T') {tag = 1;}
		else if(c == 'P') {tag = 2;}
		else if(c == 'L') {tag = 3;}
		else if(c == 'D') {tag = 4;}
		else if(c == ':' || c == '[') {isStart = 1;}
		else if(c == '-') {isPlus = -1;}
		else if(c == ',' || c == ']')
		{
			switch(tag)
			{
				case 1: type *= isPlus;  isPlus = 1; isStart = 0; break;
				case 2: prio *= isPlus; isPlus = 1; isStart = 0; break;
				case 3: lng *= isPlus; isPlus = 1; isStart = 0; break;
				case 4: data[index] *= isPlus; isPlus = 1; index++; break;
			}
		}	
		else if(isStart == 1) 
		{
			switch(tag)
			{
				case 1: type = type*10 + c - '0'; break;
				case 2: prio = prio*10 + c - '0'; break;
				case 3: lng = lng*10 + c - '0'; break;
				case 4: data[index] = data[index]*10 + c - '0'; break;
			}
		}
	
	}
	//给结构体赋值
	cmd -> type = type;
	cmd -> prio = prio;
	cmd -> lng  = lng;
	for(i=0; i<10; i++)
	{
		cmd -> data[i] = data[i];
	}	
}

//对控制命令进行处理
void CmdRouter(Cmd_TypeDef *cmd)
{
	switch(cmd -> type)
	{
		case ID_TYPE_GET_STATE: 
			GetStateFuc(cmd);
			break;
		case ID_TYPE_MOVE_VOL: 
			MoterVolFuc(cmd);  
			break;
		case ID_TYPE_CMD_LED:
			CmdLedFuc(cmd);
			break;
		case ID_TYPE_MOVE_MODE:
			MoveModeSetFuc(cmd);
			break;
		case ID_TYPE_EMERGENCY_STOP:
			EmergencyStopFuc(cmd);
			break;
		case ID_TYPE_MODE_PARAM:
			ModeParamFuc(cmd);
			break;
		case ID_TYPE_MODE_MOVE:
			ModeMoveFuc(cmd);
			break;
		case ID_TYPE_MOVE_CHANNEL:
			MoveChannelFuc(cmd);
			break;
		case ID_TYPE_READ_PID:
			ReadPIDFuc(cmd);
			break;
		case ID_TYPE_READ_IMU:
			ReadImuFuc(cmd);
			break;
		case ID_TYPE_READ_MSG:
			ReadMsgFuc(cmd);
			break;
		case ID_TYPE_READ_DEEP:
			ReadDeepFuc(cmd);
			break;
		case ID_TYPE_SET_PID:
			SetPIDFuc(cmd);
			break;
		case ID_TYPE_SET_GYRO_OFFSET:
			SetGyroOffsetFuc(cmd);
			break;
		case ID_TYPE_SET_MSG_EXTREME:
			SetMsgExtremeFuc(cmd);
			break;
		case ID_TYPE_SET_DEEP_ZERO:
			SetDeepZeroFuc(cmd);
			break;
		case ID_TYPE_SET_MOTER_ZERO:
			SetMoterZeroFuc(cmd);
			break;
		case ID_TYPE_RESUME_BACKUP:
			ResumeBuckupFuc(cmd);
			break;
		case ID_TYPE_UPDATE_APP:
			UpdateAppFuc(cmd);
			break;
		default:
			break;
		
	}
}

//获取运行状态
void GetStateFuc(Cmd_TypeDef *cmd)
{
	printf("get data\r\n");
	CmdFeedbackRunState();
}

//控制电机运动
void MoterVolFuc(Cmd_TypeDef *cmd)
{
		s16 x, r, z;
	
		//获取数据
		x = cmd -> data[0];
		r = cmd -> data[1];
		z = cmd -> data[2];
	
		//更新系统参数
		SetMoveSpeed(&x,&r,&z);
	
		//输出调试信息
		printf("moter move!\r\n");
		
		//返回结果
		CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//控制LED亮度
void CmdLedFuc(Cmd_TypeDef *cmd)
{
//	//给综合板发指令
//	IOMsgSend(cmd -> data[0], cmd -> data[1]);
	s16 d1, d2;
	d1 = cmd -> data[0];
	d2 = cmd -> data[1];
	SetLedLight(&d1, &d2);
	
	//调试信息
	printf("led:d1=%d, d2=%d\r\n", d1, d2);
	
	//返回结果
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}

//设置系统运动模式
void MoveModeSetFuc(Cmd_TypeDef *cmd)
{
	MoveMode_Type mode = MODE_MANUAL;
	//更新运动模式参数
	switch(cmd -> data[0])
	{
		case 0: mode = MODE_MANUAL; break;
		case 1: mode = MODE_PITCH;  break;
		case 2: mode = MODE_DEEP;   break;
		case 3: mode = MODE_DIR;    break;
		case 4: mode = MODE_BOTH;   break;
		default: break;
	}
	SetMoveMode(&mode);
	//调试信息
	printf("move mode:%d\r\n", cmd -> data[0]);
	
	//返回结果
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}

//紧急停止
void EmergencyStopFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//进入参数设定模式
void ModeParamFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//进入运动模式
void ModeMoveFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//单通道运动控制
void MoveChannelFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//读取PID参数
void ReadPIDFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackPIDParam();
}
//读取IMU参数
void ReadImuFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackIMUParam();
}
//读取磁强计参数
void ReadMsgFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackMSGParam();
}
//读取深度参数
void ReadDeepFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackDEEPParam();
}
//设置系统PID参数
void SetPIDFuc(Cmd_TypeDef *cmd)
{
	float Kp, Ki, Kd;
	Kp = (float)cmd->data[1]*0.01;
	Ki = (float)cmd->data[2]*0.01;
	Kd = (float)cmd->data[3]*0.01; 	
	switch(cmd->data[0])
	{
		case 0: SetPitchPID(&Kp, &Ki, &Kd); break;  	//更新俯仰姿态PID
		case 1: SetDeepPID(&Kp, &Ki, &Kd); break;  	//更新俯仰姿态PID
		case 2: SetDirPID(&Kp, &Ki, &Kd); break;  		//更新俯仰姿态PID
		default: break;
	}
	
	//调试信息
	printf("updata pid\r\n");
	
	//返回结果
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//设置陀螺仪零点偏移
void SetGyroOffsetFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//设置磁强计极值参数
void SetMsgExtremeFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//设置水压计零点
void SetDeepZeroFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//设置电机死区范围
void SetMoterZeroFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//恢复出厂设置
void ResumeBuckupFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//更新应用程序
void UpdateAppFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}

/*   消息返回函数   */
//命令执行结果返回
void CmdFeedbackNoData(Cmd_TypeDef *cmd, ComErr_Type err)
{
	sprintf((char *)feedback_buf, "{\"T\":%d,\"P\":0,\"L\":1,\"D\":[%d]}\r\n", cmd->type+50, err);
	os_print_str(USART2, (char *)feedback_buf);
}
void CmdFeedbackRunState(void)
{
	float vol, cur, deep;
	float r, p, y;
	float t, press;
	GetBatteryData(&vol, &cur);
	GetDeepData(&deep);
	GetPTData(&press, &t);

	sprintf((char *)feedback_buf, "{\"T\":51,\"P\":0,\"L\":7,\"D\":[%d,%d,%d,%d,%d,%d,%d]}\r\n", (int)(vol*100),(int)(cur*100),(int)(deep*100),(int)(r*100),(int)(p*100),(int)(y*100),(int)(press));
	//sprintf((char *)feedback_buf, "{\"T\":51,\"P\":0,\"L\":7,\"D\":[%d,%d,%d,%d]}\r\n",r,p,y,press);
	os_print_str(USART2, (char *)feedback_buf);
}
void CmdFeedbackPIDParam(void)
{

}
void CmdFeedbackIMUParam(void)
{

}
void CmdFeedbackMSGParam(void)
{

}
void CmdFeedbackDEEPParam(void)
{

}
