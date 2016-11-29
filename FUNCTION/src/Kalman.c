
#include "Kalman.h"
#include "math.h"
#include "usart.h"
#include "param.h"
#define fRad2Deg 57.295779513f

/*卡尔曼滤波参数*/

//滤波器的采样周期
float dt = 0.1; 
float C_0 = 1;

//协方差
float P_R[2][2] = {{1,0},{0,1}};   //滚转初始协方差
float P_P[2][2] = {{1,0},{0,1}};   //俯仰初始协方差
float P_Y[2][2] = {{1,0},{0,1}};   //偏航初始协方差

//置信度
float Q_roll = 0.001, Q_roll_dot = 0.005;   	//滚转角度数据置信度，角速度数据置信度
float Q_pitch = 0.001, Q_pitch_dot = 0.005;   //俯仰角度数据置信度，角速度数据置信度
float Q_yaw = 0.001, Q_yaw_dot = 0.005;   		//偏航角度数据置信度，角速度数据置信度

float R_roll = 0.5;  //滚转测量置信度
float R_pitch = 0.5; //俯仰测量置信度
float R_yaw = 0.3;	 //偏航测量置信度

//测量结果
float roll, roll_dot, roll_bias;  	//滚转角度与角速度 角速度偏差值
float pitch, pitch_dot, pitch_bias; //俯仰角度与角速度
float yaw, yaw_dot, yaw_bias;				//偏航角度与角速度

//中间变量
float Pdot[4];
float angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;

//卡尔曼滤波
void Kalman_Filter_Roll(float roll_m, float roll_gyro_m)  //angleAx, gyroGy
{
	roll += (roll_gyro_m - roll_bias)*dt;   //预测角度值
	angle_err = roll_m -roll; 		 //测量值与预测值的偏差
	Pdot[0] = Q_roll - P_R[0][1] - P_R[1][0];
	Pdot[1] = -P_R[1][1];
	Pdot[2] = -P_R[1][1];
	Pdot[3] = Q_roll_dot;
	
	P_R[0][0] += Pdot[0]*dt;		//预测协方差
	P_R[0][1] += Pdot[1]*dt;
	P_R[1][0] += Pdot[2]*dt;
	P_R[1][1] += Pdot[3]*dt;
	
	PCt_0 = C_0 * P_R[0][0];
	PCt_1 = C_0 * P_R[1][0];
	E = R_roll + C_0 * PCt_0;
	K_0 = PCt_0 / E;					//卡尔曼增益
	K_1 = PCt_1 / E;
	t_0 = PCt_0;
	t_1 = C_0 * P_R[0][1];
	
	P_R[0][0] -= K_0 * t_0;			//修正协方差
	P_R[0][1] -= K_0 * t_1;
	P_R[1][0] -= K_1 * t_0;
	P_R[1][1] -= K_1 * t_1;	
	
	roll += K_0 * angle_err;  		//修正角度值
	roll_bias += K_1 * angle_err; 		//更新角速度偏差值
	roll_dot = roll_gyro_m - roll_bias; 	//修正角速度
}

//卡尔曼滤波
void Kalman_Filter_Pitch(float pitch_m, float pitch_gyro_m)  //angleAx, gyroGy
{
	pitch += (pitch_gyro_m - pitch_bias)*dt;   //预测角度值
	angle_err = pitch_m -pitch; 		 //测量值与预测值的偏差
	Pdot[0] = Q_pitch - P_P[0][1] - P_P[1][0];
	Pdot[1] = -P_P[1][1];
	Pdot[2] = -P_P[1][1];
	Pdot[3] = Q_pitch_dot;
	
	P_P[0][0] += Pdot[0]*dt;		//预测协方差
	P_P[0][1] += Pdot[1]*dt;
	P_P[1][0] += Pdot[2]*dt;
	P_P[1][1] += Pdot[3]*dt;
	
	PCt_0 = C_0 * P_P[0][0];
	PCt_1 = C_0 * P_P[1][0];
	E = R_pitch + C_0 * PCt_0;
	K_0 = PCt_0 / E;					//卡尔曼增益
	K_1 = PCt_1 / E;
	t_0 = PCt_0;
	t_1 = C_0 * P_P[0][1];
	
	P_P[0][0] -= K_0 * t_0;			//修正协方差
	P_P[0][1] -= K_0 * t_1;
	P_P[1][0] -= K_1 * t_0;
	P_P[1][1] -= K_1 * t_1;	
	
	pitch += K_0 * angle_err;  		//修正角度值
	pitch_bias += K_1 * angle_err; 		//更新角速度偏差值
	pitch_dot = pitch_gyro_m - pitch_bias; 	//修正角速度
}

//卡尔曼滤波
void Kalman_Filter_Yaw(float yaw_m, float yaw_gyro_m)  //angleAx, gyroGy
{
	if(yaw_m - yaw > 300) yaw_m -= 360;
	else if(yaw - yaw_m > 300) yaw_m += 360;
	
	yaw += (yaw_gyro_m - yaw_bias)*dt;   //预测角度值
	angle_err = yaw_m -yaw; 		 //测量值与预测值的偏差
	Pdot[0] = Q_yaw - P_Y[0][1] - P_Y[1][0];
	Pdot[1] = -P_Y[1][1];
	Pdot[2] = -P_Y[1][1];
	Pdot[3] = Q_yaw_dot;
	
	P_Y[0][0] += Pdot[0]*dt;		//预测协方差
	P_Y[0][1] += Pdot[1]*dt;
	P_Y[1][0] += Pdot[2]*dt;
	P_Y[1][1] += Pdot[3]*dt;
	
	PCt_0 = C_0 * P_Y[0][0];
	PCt_1 = C_0 * P_Y[1][0];
	E = R_yaw + C_0 * PCt_0;
	K_0 = PCt_0 / E;					//卡尔曼增益
	K_1 = PCt_1 / E;
	t_0 = PCt_0;
	t_1 = C_0 * P_Y[0][1];
	
	P_Y[0][0] -= K_0 * t_0;			//修正协方差
	P_Y[0][1] -= K_0 * t_1;
	P_Y[1][0] -= K_1 * t_0;
	P_Y[1][1] -= K_1 * t_1;	
	
	yaw += K_0 * angle_err;  					//修正角度值
	yaw_bias += K_1 * angle_err; 			//更新角速度偏差值
	yaw_dot = yaw_gyro_m - yaw_bias; 	//修正角速度
	
	if(yaw >= 180) yaw -= 360;
	else if(yaw < -180) yaw += 360;
}

//计算磁力计的测量航向角
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
	//转换到0~360度
	if(Xe < 0 && Ye > 0) th += 180;
	else if(Xe < 0 && Ye < 0) th -= 180;
	//printf("th=%.2f\r\n", th);

	return th;
}


void Kalman_Filter(int16_t imu[], int offset_gyro[])
{
	float pitch_m, roll_m, yaw_m;  //加速度计和磁力计的测量值
	float gx, gy, gz;  //陀螺仪的变化值	

	//临时变量
	float sum;
	
	//计算加速度计的测量角
	sum = sqrt(imu[2]*imu[2] + imu[3]*imu[3]);
	sum = imu[1]/sum;
	pitch_m = atan(sum);
//	sum = sqrt(imu[1]*imu[1] + imu[3]*imu[3]);
//	sum = imu[2]/sum;
//	ay = atan(sum);
	roll_m = atan(imu[2]*1.0/imu[3]);
	//计算磁力计的测量角
	yaw_m = MagPose(imu[7], imu[8], imu[9], pitch_m, roll_m);

	//把弧度转换为角度值
	pitch_m *= fRad2Deg;
	roll_m *= fRad2Deg;
	
	//陀螺仪零点漂移修正 转换为角度值
	gx = (imu[4] - offset_gyro[0])/16.384;
	gy = (imu[5] - offset_gyro[1])/16.384;
	gz = (imu[6] - offset_gyro[2])/16.384;
	
	//卡尔曼滤波
	Kalman_Filter_Roll(roll_m, gx);
	Kalman_Filter_Pitch(pitch_m, gy);
	Kalman_Filter_Yaw(yaw_m, gz);
	//printf("pitch=%.2f\troll=%.2f\tyaw=%.2f\r\n", pitch, roll, yaw);
	//printf("roll=%.2f\tpitch=%.2f\tyaw=%.2f\r\n", roll, pitch, yaw);
	
	//更新系统姿态状态
	//SetAttitudeState((s16)(roll*100), (s16)(pitch*100), (s16)(yaw*100));
	SetAttitudeData(&roll, &pitch, &yaw);
}
