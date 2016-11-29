#include "task.h"
#include "mpu9250.h"
#include "Kalman.h"
//������ƿ�
OS_TCB	ImuDataTaskTCB;
//�����ջ
CPU_STK	IMU_DATA_TASK_STK[IMU_DATA_STK_SIZE];

//��̬���������յ�������
int16_t imu[10] = {0};

OS_MUTEX MY_MUTEX;   	//����һ���������ź��������ڷ��ʹ�����Դ

void imuData_task_create(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//�����ٽ���
	
	//��������
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
	OS_CRITICAL_EXIT();		//�˳��ٽ���	
}

//��������Ƶļ���ֵ
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
	//���������Ư��
	int offset_gyro[3] = {0, 0, 0};
	int i;
	CPU_SR_ALLOC();
	MPU9250_Config();   //mpu9250 ��ʼ��
	//���������ǵ����Ư��
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
		//��ȡ��ǿ�Ƽ�ֵ
		//MagTop(imu[7], imu[8], imu[9]);
		
		//�������˲����
		Kalman_Filter(imu, offset_gyro);
		OSMutexPend(&MY_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err); 	//���󻥳��ź���
		//printf("imu:%d, %d, %d, %d, %d, %d\r\n",imu[1], imu[2], imu[3], imu[4], imu[5], imu[6]);
		OSMutexPost (&MY_MUTEX,OS_OPT_POST_NONE,&err);					//���ͻ����ź���
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
	}
}



