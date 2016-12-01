#include "cmd_deal.h"
#include "io_deal.h"
#include "usart_cmd.h"
#include "param.h"



//�洢���������
static u8 feedback_buf[100];

//�洢��ǰLED�����״̬
static LedCmd_TypeDef mLedCmd;

//�洢��ǰ�˶������״̬
MoveCmd_TypeDef mMoveCmd;

//���ַ�������������
void CmdAnalysis(Cmd_TypeDef *cmd, CPU_INT08U* p, CPU_INT08U size)
{
	u8 type = 0;
	u8 prio = 0;
	u8 lng = 0;
	s16 data[10] = {0};
	
	u8 tag = 0;  //0-��  1-type  2-prio  3-lng  4-data
	u8 index = 0;    //������ָ��
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
	//���ṹ�帳ֵ
	cmd -> type = type;
	cmd -> prio = prio;
	cmd -> lng  = lng;
	for(i=0; i<10; i++)
	{
		cmd -> data[i] = data[i];
	}	
}


//��ʱ������1s���ڣ�
void tmr1_callback(void)
{
	 //�Եƹ���Ƶļ���
	if(mLedCmd.cnt == 0)   //�ÿ����û����ٷ��Ϳ�������
	{
		mLedCmd.id = -1;  //���»ص��޽���״̬���ƹⱣ����һʱ�̵�����
	}
	else
	{
		mLedCmd.cnt = 0;  //��ʱ����
	}
	//���˶����Ƶļ���
	if(mMoveCmd.cnt == 0)  //���û����ڷ��Ϳ�������
	{
		mMoveCmd.id = -1;
		mMoveCmd.ismove = 0;
		mMoveCmd.x = 0;
		mMoveCmd.r = 0;
		mMoveCmd.z = 0;
		//����ϵͳ����
		SetMoveSpeed(&mMoveCmd.x,&mMoveCmd.r,&mMoveCmd.z);
	}
	else
	{
		mMoveCmd.cnt = 0;
	}
	
}

//

//�Կ���������д���
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

//��ȡ����״̬
void GetStateFuc(Cmd_TypeDef *cmd)
{
	printf("get data\r\n");
	CmdFeedbackRunState();
}

//���Ƶ���˶�
void MoterVolFuc(Cmd_TypeDef *cmd)
{
		s16 x, r, z;
		u8 ismove;  //�Ƿ��˶�
	
		//��ȡ����
		x = cmd -> data[0];
		r = cmd -> data[1];
		z = cmd -> data[2];
	  if(x == 0 && r == 0 && z == 0)
			ismove = 0;
		else
			ismove = 1;
		
		//����˶���ͻ
		if(ismove) //���û���������ʹ�������˶�
		{
			if(mMoveCmd.id == -1) //��ʱû���κ�ָ��
			{
				//�����κ���Ӧ
			}
			else if(mMoveCmd.ismove == 0)  //���ھ�ֹ
			{
				mMoveCmd.id = cmd ->prio;   //�л������û�
				mMoveCmd.ismove = ismove;
				mMoveCmd.cnt = 0;
				mMoveCmd.x = x;
				mMoveCmd.r = r;
				mMoveCmd.z = z;
			}
			else if(mMoveCmd.id == cmd->prio)  //������������ͬһ�û�
			{
				mMoveCmd.ismove = ismove;		//������û����˶�������ı�
				mMoveCmd.cnt ++;						//������1
				mMoveCmd.x = x;
				mMoveCmd.r = r;
				mMoveCmd.z = z;	
			}
			else if(mMoveCmd.id > cmd->prio)   //�����ȼ��û������˶�����
			{
				mMoveCmd.id = cmd ->prio;   //�л������û�
				mMoveCmd.ismove = ismove;
				mMoveCmd.cnt = 0;
				mMoveCmd.x = x;
				mMoveCmd.r = r;
				mMoveCmd.z = z;
			}
		}
		else   //�û���������ʹ�����˾�ֹ
		{
			if(mMoveCmd.id == -1) //��ʱû���κ�ָ��
			{
				mMoveCmd.id = cmd ->prio;   //ֱ����Ӧ���û�
				mMoveCmd.cnt = 0;
				mMoveCmd.ismove = ismove;
				mMoveCmd.x = x;
				mMoveCmd.r = r;
				mMoveCmd.z = z;
			}
			else if(mMoveCmd.id == cmd->prio)  //����ͬһ�û�
			{
				mMoveCmd.cnt ++;
				mMoveCmd.ismove = ismove;
				mMoveCmd.x = x;
				mMoveCmd.r = r;
				mMoveCmd.z = z;
			}

		}
	
		//����ϵͳ����
		SetMoveSpeed(&mMoveCmd.x,&mMoveCmd.r,&mMoveCmd.z);
	
		//���������Ϣ
		printf("moter move!\r\n");
		
		//���ؽ��
		CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//����LED����
void CmdLedFuc(Cmd_TypeDef *cmd)
{
  
	s16 d1, d2;
	d1 = cmd -> data[0];
	d2 = cmd -> data[1];
	
	//���led��ͻ
	if(mLedCmd.id == -1) //֮ǰû�н��յ�ledָ��
	{
		mLedCmd.id = cmd -> prio;
		mLedCmd.cnt = 0;
		mLedCmd.light = d1;
	}
	else if(mLedCmd.id == cmd -> prio)  //��ָ���û��뵱ǰled״̬����ͬһ�û�
	{
		mLedCmd.cnt ++; //�����ۼ�
		mLedCmd.light = d1;  //������û��ı仯
	}
	else if(mLedCmd.id != cmd ->prio)   //�����û�Ҳ����led����ָ��
	{
		if(d1 > mLedCmd.light)  //���û����͵ı���һ���û��Ĵ�
		{
			mLedCmd.id = cmd -> prio;   //��ʼ��Ӧ���û�������
			mLedCmd.cnt = 0;
			mLedCmd.light = d1;
		}
	}
	
	SetLedLight(&mLedCmd.light, &mLedCmd.light);
	
	//������Ϣ
	printf("led:d1=%d, d2=%d\r\n", d1, d2);
	
	//���ؽ��
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}

//����ϵͳ�˶�ģʽ
void MoveModeSetFuc(Cmd_TypeDef *cmd)
{
	MoveMode_Type mode = MODE_MANUAL;
	//�����˶�ģʽ����
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
	//������Ϣ
	printf("move mode:%d\r\n", cmd -> data[0]);
	
	//���ؽ��
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}

//����ֹͣ
void EmergencyStopFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//��������趨ģʽ
void ModeParamFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//�����˶�ģʽ
void ModeMoveFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//��ͨ���˶�����
void MoveChannelFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//��ȡPID����
void ReadPIDFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackPIDParam();
}
//��ȡIMU����
void ReadImuFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackIMUParam();
}
//��ȡ��ǿ�Ʋ���
void ReadMsgFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackMSGParam();
}
//��ȡ��Ȳ���
void ReadDeepFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackDEEPParam();
}
//����ϵͳPID����
void SetPIDFuc(Cmd_TypeDef *cmd)
{
	float Kp, Ki, Kd;
	Kp = (float)cmd->data[1]*0.01;
	Ki = (float)cmd->data[2]*0.01;
	Kd = (float)cmd->data[3]*0.01; 	
	switch(cmd->data[0])
	{
		case 0: SetPitchPID(&Kp, &Ki, &Kd); break;  	//���¸�����̬PID
		case 1: SetDeepPID(&Kp, &Ki, &Kd); break;  	//���¸�����̬PID
		case 2: SetDirPID(&Kp, &Ki, &Kd); break;  		//���¸�����̬PID
		default: break;
	}
	
	//������Ϣ
	printf("updata pid\r\n");
	
	//���ؽ��
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//�������������ƫ��
void SetGyroOffsetFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//���ô�ǿ�Ƽ�ֵ����
void SetMsgExtremeFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//����ˮѹ�����
void SetDeepZeroFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//���õ��������Χ
void SetMoterZeroFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//�ָ���������
void ResumeBuckupFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}
//����Ӧ�ó���
void UpdateAppFuc(Cmd_TypeDef *cmd)
{
	CmdFeedbackNoData(cmd, ERR_SUCCESS);
}

/*   ��Ϣ���غ���   */
//����ִ�н������
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
