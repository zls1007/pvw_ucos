#include "moter_control.h"
#include "param.h"
#include "pid.h"

//�������ƺ���
void manualControlFuc(MoveMode_Type myLastState)
{
	s16 x, r, z;  			//�û������˶�ָ��  -100 ~ +100
	s16 p1, p2, p3; 		//����ٶ�ֵ  -500 ~ 500
	
	//��ȡ�û�����
	GetMoveSpeed(&x, &r, &z);
	
//	//ȥ������
//	if(x > 10) x -= 10;
//	else if(x < -10) x += 10;
//	else x = 0;
//	if(r > 15) r -= 15;
//	else if(r < -15) r += 15;
//	else r = 0;
//	if(z > 10) z -= 10;
//	else if(z < -10) z += 10;
//	else z = 0;
	
	//�ٶ�ӳ��
		p1 = x*5 + r*3;
		p2 = -x*5 + r*3;
		p3 = z*3;
		
	//�������
	MoterPWMoutput(p1, p2, p3);
}


//������̬����
void pitchControlFuc(MoveMode_Type myLastState)
{
	s16 x, r, z;  						//�û������˶�ָ��  -100 ~ +100
	s16 p1, p2, p3; 					//����ٶ�ֵ  -500 ~ 500
	float roll, pitch, yaw; 		//��������̬  *100
	static int p3_inc;								//��̬���´�ֱ���ת��
	static PIDParam_TypeDef myparam; //PID����
	
	//��ȡ�û�����
	GetMoveSpeed(&x, &r, &z);
	//��ȡ��������̬
	GetAttitudeData(&roll, &pitch, &yaw);
	//��ȡPID����
	GetPitchPID(&myparam.Kp, &myparam.Ki, &myparam.Kd);
	
	//����ƫ��
	myparam.err = 0 - pitch;
	//PID�㷨
	p3_inc += PID_Fuc(&myparam);
	
	//ȷ�����������
	p1 = x*5 + r*3;
	p2 = -x*5 + r*3;
	
	if(z == 0)  //����
	{
		p3 = p3_inc;
	}else
	{
		p3 = z*3;
	}
	//printf("err=%d,kp=%d,ki=%d,kd=%d,inc=%d\r\n",myparam.err, myparam.Kp, myparam.Ki, myparam.Kd, p3_inc);
	//�������
	MoterPWMoutput(p1, p2, p3);
		
}

//�����˶�ģʽ
void deepControlFuc(MoveMode_Type myLastState)
{
	s16 x, r, z;  						//�û������˶�ָ��  -100 ~ +100
	s16 p1, p2, p3; 					//����ٶ�ֵ  -500 ~ 500
	float roll, pitch, yaw; 		//��������̬  
	float deep;									//���������  
	
	static float deep_origin;   		//Ŀ�����	
	static float pitch_out;					//��ȵ�����̬�仯
	static float p3_out;						//��̬���´�ֱ���ת��
	static float last_deep;
		
	static PIDParam_TypeDef pitch_param; //PID����
	static PIDParam_TypeDef deep_param; //PID����
	
	static u16 cnt = 0;
	
	
	//��ȡ�û�����
	GetMoveSpeed(&x, &r, &z);
	//��ȡ��������̬
	GetAttitudeData(&roll, &pitch, &yaw);
	//��ȡ���������
	GetDeepData(&deep);
	
	//�ж��Ƿ�Ϊ��һ�ν����ģʽ
	if(myLastState != MODE_DEEP)
	{
		deep_origin = deep;    //�Ե�ǰ���ΪĿ��ֵ
	}
//	if(((deep-last_deep) < 0.06)  && ((last_deep-deep) < 0.06) )  deep = last_deep;
//	else last_deep = deep;
	//printf("deep=%.2f,last=%.2f\r\n", deep, last_deep);
	
	//��ȡPID����
	GetPitchPID(&pitch_param.Kp, &pitch_param.Ki, &pitch_param.Kd);
	GetDeepPID(&deep_param.Kp, &deep_param.Ki, &deep_param.Kd);
	
	//�������ƫ��   -+0.06 Ϊ����̬����ȣ�
	deep_param.err = deep - deep_origin;
//	if(deep_param.err  > 0.06) deep_param.err  -= 0.06;
//	else if(deep_param.err  < -0.06) deep_param.err  += 0.06;
//	else deep_param.err  = 0.0;
	
	//��Ƶ 100Hz/5 = 20Hz
	if(cnt < 5) cnt ++;
	else
	{
		cnt = 0;
			//���PID�㷨
		pitch_out += PID_Fuc(&deep_param);
	}

	
	
	//������̬ƫ��   -+1.0  Ϊ����̬��������
	pitch_param.err  = pitch_out - pitch;
//	if(pitch_param.err  > 1.0) pitch_param.err  -= 1.0;
//	else if(pitch_param.err  < -1.0) pitch_param.err  += 1.0;
//	else pitch_param.err  = 0.0;

	
	//��̬PID�㷨
	p3_out += PID_Fuc(&pitch_param);

	
	//ȷ�����������
	p1 = x*5 + r*3;
	p2 = -x*5 + r*3;
	
	if(z == 0)  //����
	{
		p3 = (s16)(p3_out);
	}else			
	{
		p3 = z*3;
		pitch_out = 0;
		deep_origin = deep;  //����Ŀ�����
	}
	
	//printf("d=%.2f,de=%.2f,po=%.2f,p=%.2f,pe=%.2f,inc=%.2f\r\n",deep, deep_param.err, pitch_out, pitch,pitch_param.err, p3_out);
	//printf("err=%d,kp=%d,ki=%d,kd=%d,inc=%d\r\n",myparam.err, myparam.Kp, myparam.Ki, myparam.Kd, p3_inc);
	//�������
	MoterPWMoutput(p1, p2, p3);
		
}


//�����˶�ģʽ
void dirControlFuc(MoveMode_Type myLastState)
{

}

//ȫ�ջ�����ģʽ
void allControlFuc(MoveMode_Type myLastState)
{

}

//���pwm���
void MoterPWMoutput(s16 p1, s16 p2, s16 p3)
{
		s16 c1, c2, c3;  //��ˢ���ռ�ձ� 1000 ~ 2000
		static float cur_c3;
	
		//��������
		//if(abs(p1) < 60) p1 = 0;
		//if(abs(p2) < 30) p2 = 0;
	  //if(abs(p3) < 50) p3 = 0;
	  cur_c3 = 0.05*p3 + 0.95*cur_c3;
		p3 = (s16)cur_c3;
		if(abs(p3) < 40) p3 = 0;
		
		//ռ�ձ�ӳ��
		if(p1 > 0) c1 = 1542 + p1;
		else if(p1 < 0) c1 = 1498 + p1;
		else c1 = 1522;
		if(p2 > 0) c2 = 1542 + p2;
		else if(p2 < 0) c2 = 1498 + p2;
		else c2 = 1522;
		if(p3 > 0) c3 = 1540 + p3;
		else if(p3 < 0) c3 = 1496 + p3;
		else c3 = 1522;
//	c3 = 1522 + p3;
		
		//��Χ�޶�
		if(c1 > 2000) c1 = 2000; 
		else if(c1 < 1000) c1 = 1000;
		if(c2 > 2000) c2 = 2000; 
		else if(c2 < 1000) c2 = 1000;
		if(c3 > 2000) c3 = 2000; 
		else if(c3 < 1000) c3 = 1000;
		
		//printf("move:c1=%d, c2=%d, c3=%d\r\n", c1, c2, c3);
		
		//���
		TIM_SetCompare4(TIM1, c1);
		TIM_SetCompare3(TIM1, c2);
		TIM_SetCompare2(TIM1, c3);
}
