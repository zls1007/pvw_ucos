#ifndef __CMD_DEAL_H
#define __CMD_DEAL_H
#include "includes.h"
#include "param.h"

//���û�ͨ�����Ͷ���
#define ID_TYPE_GET_STATE				1				//״̬��ѯ
#define ID_TYPE_MOVE_VOL  			2				//�ٶȿ���
#define ID_TYPE_CMD_LED					3				//�ƹ����
#define ID_TYPE_MOVE_MODE				4				//�˶�ģʽѡ��
#define ID_TYPE_EMERGENCY_STOP	5				//����ֹͣ

#define ID_TYPE_MODE_PARAM			11			//�������ģʽ
#define ID_TYPE_MODE_MOVE				12			//�����˶�ģʽ
#define ID_TYPE_MOVE_CHANNEL		13			//��ͨ������

#define ID_TYPE_READ_PID				15			//��ȡPID����ֵ
#define ID_TYPE_READ_IMU				16			//��ȡIMU����
#define ID_TYPE_READ_MSG				17			//��ȡ��ǿ������
#define ID_TYPE_READ_DEEP				18			//��ȡˮѹ������

#define ID_TYPE_SET_PID					21			//����PID����ֵ
#define ID_TYPE_SET_GYRO_OFFSET 22			//���������Ǿ�̬ƫ��
#define ID_TYPE_SET_MSG_EXTREME 23			//���ô�ǿ�Ƽ�ֵ
#define ID_TYPE_SET_DEEP_ZERO		24			//����ˮѹ�����
#define ID_TYPE_SET_MOTER_ZERO	25			//���õ������
#define ID_TYPE_RESUME_BACKUP		31			//�ָ���������
#define ID_TYPE_UPDATE_APP			32			//���³���

//��ǰled����״̬
typedef struct led_cmd_t
{
	s16 id;  					//��ǰ������������Ӧ��id  -1��ʾû���û�����led����ָ��
	s16 cnt;					//1s���յ���id����Ĵ���
	s16 light;				//��ʱ������ֵ
} LedCmd_TypeDef;

//��ǰ�˶�����״̬
typedef struct move_cmd_t
{
	s16 id;						//��ǰ��Ӧ���˶��û�id  -1��ʾû���û������˶�����
	u8 ismove;				//��ǰ�������Ƿ����˶�״̬ 0-��ֹ  1-�˶�
	s16 cnt;					//1s���յ����Ը�id����Ĵ���
	s16 x, r, z;			//��ʱ�˶���ָ��
} MoveCmd_TypeDef;

//��ʱ������1s��
void tmr1_callback(void);


//���ַ����������ṹ��
void CmdAnalysis(Cmd_TypeDef *cmd, CPU_INT08U* p, CPU_INT08U size);

//���������·��
void CmdRouter(Cmd_TypeDef *cmd);

//��Ϣ������
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

//��Ϣ����
void CmdFeedbackNoData(Cmd_TypeDef *cmd, ComErr_Type err);
void CmdFeedbackRunState(void);
void CmdFeedbackPIDParam(void);
void CmdFeedbackIMUParam(void);
void CmdFeedbackMSGParam(void);
void CmdFeedbackDEEPParam(void);



#endif
