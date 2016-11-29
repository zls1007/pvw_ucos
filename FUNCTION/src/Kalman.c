
#include "Kalman.h"
#include "math.h"
#include "usart.h"
#include "param.h"
#define fRad2Deg 57.295779513f

/*�������˲�����*/

//�˲����Ĳ�������
float dt = 0.1; 
float C_0 = 1;

//Э����
float P_R[2][2] = {{1,0},{0,1}};   //��ת��ʼЭ����
float P_P[2][2] = {{1,0},{0,1}};   //������ʼЭ����
float P_Y[2][2] = {{1,0},{0,1}};   //ƫ����ʼЭ����

//���Ŷ�
float Q_roll = 0.001, Q_roll_dot = 0.005;   	//��ת�Ƕ��������Ŷȣ����ٶ��������Ŷ�
float Q_pitch = 0.001, Q_pitch_dot = 0.005;   //�����Ƕ��������Ŷȣ����ٶ��������Ŷ�
float Q_yaw = 0.001, Q_yaw_dot = 0.005;   		//ƫ���Ƕ��������Ŷȣ����ٶ��������Ŷ�

float R_roll = 0.5;  //��ת�������Ŷ�
float R_pitch = 0.5; //�����������Ŷ�
float R_yaw = 0.3;	 //ƫ���������Ŷ�

//�������
float roll, roll_dot, roll_bias;  	//��ת�Ƕ�����ٶ� ���ٶ�ƫ��ֵ
float pitch, pitch_dot, pitch_bias; //�����Ƕ�����ٶ�
float yaw, yaw_dot, yaw_bias;				//ƫ���Ƕ�����ٶ�

//�м����
float Pdot[4];
float angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;

//�������˲�
void Kalman_Filter_Roll(float roll_m, float roll_gyro_m)  //angleAx, gyroGy
{
	roll += (roll_gyro_m - roll_bias)*dt;   //Ԥ��Ƕ�ֵ
	angle_err = roll_m -roll; 		 //����ֵ��Ԥ��ֵ��ƫ��
	Pdot[0] = Q_roll - P_R[0][1] - P_R[1][0];
	Pdot[1] = -P_R[1][1];
	Pdot[2] = -P_R[1][1];
	Pdot[3] = Q_roll_dot;
	
	P_R[0][0] += Pdot[0]*dt;		//Ԥ��Э����
	P_R[0][1] += Pdot[1]*dt;
	P_R[1][0] += Pdot[2]*dt;
	P_R[1][1] += Pdot[3]*dt;
	
	PCt_0 = C_0 * P_R[0][0];
	PCt_1 = C_0 * P_R[1][0];
	E = R_roll + C_0 * PCt_0;
	K_0 = PCt_0 / E;					//����������
	K_1 = PCt_1 / E;
	t_0 = PCt_0;
	t_1 = C_0 * P_R[0][1];
	
	P_R[0][0] -= K_0 * t_0;			//����Э����
	P_R[0][1] -= K_0 * t_1;
	P_R[1][0] -= K_1 * t_0;
	P_R[1][1] -= K_1 * t_1;	
	
	roll += K_0 * angle_err;  		//�����Ƕ�ֵ
	roll_bias += K_1 * angle_err; 		//���½��ٶ�ƫ��ֵ
	roll_dot = roll_gyro_m - roll_bias; 	//�������ٶ�
}

//�������˲�
void Kalman_Filter_Pitch(float pitch_m, float pitch_gyro_m)  //angleAx, gyroGy
{
	pitch += (pitch_gyro_m - pitch_bias)*dt;   //Ԥ��Ƕ�ֵ
	angle_err = pitch_m -pitch; 		 //����ֵ��Ԥ��ֵ��ƫ��
	Pdot[0] = Q_pitch - P_P[0][1] - P_P[1][0];
	Pdot[1] = -P_P[1][1];
	Pdot[2] = -P_P[1][1];
	Pdot[3] = Q_pitch_dot;
	
	P_P[0][0] += Pdot[0]*dt;		//Ԥ��Э����
	P_P[0][1] += Pdot[1]*dt;
	P_P[1][0] += Pdot[2]*dt;
	P_P[1][1] += Pdot[3]*dt;
	
	PCt_0 = C_0 * P_P[0][0];
	PCt_1 = C_0 * P_P[1][0];
	E = R_pitch + C_0 * PCt_0;
	K_0 = PCt_0 / E;					//����������
	K_1 = PCt_1 / E;
	t_0 = PCt_0;
	t_1 = C_0 * P_P[0][1];
	
	P_P[0][0] -= K_0 * t_0;			//����Э����
	P_P[0][1] -= K_0 * t_1;
	P_P[1][0] -= K_1 * t_0;
	P_P[1][1] -= K_1 * t_1;	
	
	pitch += K_0 * angle_err;  		//�����Ƕ�ֵ
	pitch_bias += K_1 * angle_err; 		//���½��ٶ�ƫ��ֵ
	pitch_dot = pitch_gyro_m - pitch_bias; 	//�������ٶ�
}

//�������˲�
void Kalman_Filter_Yaw(float yaw_m, float yaw_gyro_m)  //angleAx, gyroGy
{
	if(yaw_m - yaw > 300) yaw_m -= 360;
	else if(yaw - yaw_m > 300) yaw_m += 360;
	
	yaw += (yaw_gyro_m - yaw_bias)*dt;   //Ԥ��Ƕ�ֵ
	angle_err = yaw_m -yaw; 		 //����ֵ��Ԥ��ֵ��ƫ��
	Pdot[0] = Q_yaw - P_Y[0][1] - P_Y[1][0];
	Pdot[1] = -P_Y[1][1];
	Pdot[2] = -P_Y[1][1];
	Pdot[3] = Q_yaw_dot;
	
	P_Y[0][0] += Pdot[0]*dt;		//Ԥ��Э����
	P_Y[0][1] += Pdot[1]*dt;
	P_Y[1][0] += Pdot[2]*dt;
	P_Y[1][1] += Pdot[3]*dt;
	
	PCt_0 = C_0 * P_Y[0][0];
	PCt_1 = C_0 * P_Y[1][0];
	E = R_yaw + C_0 * PCt_0;
	K_0 = PCt_0 / E;					//����������
	K_1 = PCt_1 / E;
	t_0 = PCt_0;
	t_1 = C_0 * P_Y[0][1];
	
	P_Y[0][0] -= K_0 * t_0;			//����Э����
	P_Y[0][1] -= K_0 * t_1;
	P_Y[1][0] -= K_1 * t_0;
	P_Y[1][1] -= K_1 * t_1;	
	
	yaw += K_0 * angle_err;  					//�����Ƕ�ֵ
	yaw_bias += K_1 * angle_err; 			//���½��ٶ�ƫ��ֵ
	yaw_dot = yaw_gyro_m - yaw_bias; 	//�������ٶ�
	
	if(yaw >= 180) yaw -= 360;
	else if(yaw < -180) yaw += 360;
}

//��������ƵĲ��������
float MagPose(int16_t mx, int16_t my, int16_t mz, float p, float r)
{
	static int16_t Xoffset = 9; 			//-360	379	
	static int16_t Yoffset = 251;			//-129	632
	static int16_t Zoffset = -417;		//-834	0
	float Xh, Yh, Zh;
	float Xe, Ye;
	float th;
	Xh = mx - Xoffset;
	Yh = my - Yoffset;
	Zh = mz - Zoffset;

	Ye = Xh*sin(p)*sin(r)+Yh*cos(p);
	Xe = Xh*cos(r);

	th = atan(Ye/Xe);
	th *= fRad2Deg;	
	//printf("xh=%.2f\tyh=%.2f\tzh=%.2f\r\n",Xh,Yh,Zh);
	//printf("pitch=%.2f\troll=%.2f\tthk=%.2f\r\n", p*fRad2Deg, r*fRad2Deg, th);
	//ת����0~360��
	if(Xe < 0 && Ye > 0) th += 180;
	else if(Xe < 0 && Ye < 0) th -= 180;
	//printf("th=%.2f\r\n", th);

	return th;
}


void Kalman_Filter(int16_t imu[], int offset_gyro[])
{
	float pitch_m, roll_m, yaw_m;  //���ٶȼƺʹ����ƵĲ���ֵ
	float gx, gy, gz;  //�����ǵı仯ֵ	

	//��ʱ����
	float sum;
	
	//������ٶȼƵĲ�����
	sum = sqrt(imu[2]*imu[2] + imu[3]*imu[3]);
	sum = imu[1]/sum;
	pitch_m = atan(sum);
//	sum = sqrt(imu[1]*imu[1] + imu[3]*imu[3]);
//	sum = imu[2]/sum;
//	ay = atan(sum);
	roll_m = atan(imu[2]*1.0/imu[3]);
	//��������ƵĲ�����
	yaw_m = MagPose(imu[7], imu[8], imu[9], pitch_m, roll_m);

	//�ѻ���ת��Ϊ�Ƕ�ֵ
	pitch_m *= fRad2Deg;
	roll_m *= fRad2Deg;
	
	//���������Ư������ ת��Ϊ�Ƕ�ֵ
	gx = (imu[4] - offset_gyro[0])/16.384;
	gy = (imu[5] - offset_gyro[1])/16.384;
	gz = (imu[6] - offset_gyro[2])/16.384;
	
	//�������˲�
	Kalman_Filter_Roll(roll_m, gx);
	Kalman_Filter_Pitch(pitch_m, gy);
	Kalman_Filter_Yaw(yaw_m, gz);
	//printf("pitch=%.2f\troll=%.2f\tyaw=%.2f\r\n", pitch, roll, yaw);
	//printf("roll=%.2f\tpitch=%.2f\tyaw=%.2f\r\n", roll, pitch, yaw);
	
	//����ϵͳ��̬״̬
	//SetAttitudeState((s16)(roll*100), (s16)(pitch*100), (s16)(yaw*100));
	SetAttitudeData(&roll, &pitch, &yaw);
}
