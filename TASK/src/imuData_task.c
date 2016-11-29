#include "task.h"
#include "mpu9250.h"
#include "Kalman.h"
//任务控制块
OS_TCB	ImuDataTaskTCB;
//任务堆栈
CPU_STK	IMU_DATA_TASK_STK[IMU_DATA_STK_SIZE];

//姿态传感器接收到的数据
int16_t imu[10] = {0};

OS_MUTEX MY_MUTEX;   	//定义一个互斥型信号量，用于访问共享资源

void imuData_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//进入临界区
	
	//创建任务
	OSTaskCreate((OS_TCB 	* )&ImuDataTaskTCB,		
				 (CPU_CHAR	* )"imu data task", 		
                 (OS_TASK_PTR )imuData_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )IMU_DATA_TASK_PRIO,     	
                 (CPU_STK  *)&IMU_DATA_TASK_STK[0],	
                 (CPU_STK_SIZE)IMU_DATA_STK_SIZE/10,	
                 (CPU_STK_SIZE)IMU_DATA_STK_SIZE,		
                 (OS_MSG_QTY)0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);
	OS_CRITICAL_EXIT();		//退出临界区	
}

//计算磁力计的极限值
void MagTop(int16_t mx, int16_t my, int16_t mz)
{
	static int16_t mx_min, mx_max;
	static int16_t my_min, my_max;
	static int16_t mz_min, mz_max;
	
	if(mx > mx_max) mx_max = mx;
	if(mx < mx_min) mx_min = mx;
	if(my > my_max) my_max = my;
	if(my < my_min) my_min = my;
	if(mz > mz_max) mz_max = mz;
	if(mz < mz_min) mz_min = mz;
	printf("x_min:%d\tx_max:%d\ty_min:%d\ty_max:%d\tz_min:%d\tz_max:%d\r\n", mx_min, mx_max, my_min, my_max, mz_min, mz_max);
	
}

void imuData_task(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;  
	//陀螺仪零点漂移
	int offset_gyro[3] = {0, 0, 0};
	int i;
	CPU_SR_ALLOC();
	MPU9250_Config();   //mpu9250 初始化
	//计算陀螺仪的零点漂移
	for(i=0; i<100; i++)
	{
		MPU9250_getData(imu);
		offset_gyro[0] += imu[4];
		offset_gyro[1] += imu[5];
		offset_gyro[2] += imu[6];
		delay_ms(2);
	}
	offset_gyro[0] = offset_gyro[0]/100;
	offset_gyro[1] = offset_gyro[1]/100;
	offset_gyro[2] = offset_gyro[2]/100;

	while(1)
	{
		//printf("time=%d\r\n", OSTimeGet(&err) - ts);
		//ts = OSTimeGet(&err);
		MPU9250_getData(imu);
		//获取磁强计极值
		//MagTop(imu[7], imu[8], imu[9]);
		
		//卡尔曼滤波输出
		Kalman_Filter(imu, offset_gyro);
		OSMutexPend(&MY_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err); 	//请求互斥信号量
		//printf("imu:%d, %d, %d, %d, %d, %d\r\n",imu[1], imu[2], imu[3], imu[4], imu[5], imu[6]);
		OSMutexPost (&MY_MUTEX,OS_OPT_POST_NONE,&err);					//发送互斥信号量
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
	}
}



