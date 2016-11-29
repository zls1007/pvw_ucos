
#define   BSP_MODULE
#include  <bsp.h>


#define  BSP_REG_DEM_CR                           (*(CPU_REG32 *)0xE000EDFC)   	//DEMCR�Ĵ���
#define  BSP_REG_DWT_CR                           (*(CPU_REG32 *)0xE0001000)		//DWT���ƼĴ���
#define  BSP_REG_DWT_CYCCNT                       (*(CPU_REG32 *)0xE0001004)		//DWTʱ�Ӽ����Ĵ���
#define  BSP_REG_DBGMCU_CR                        (*(CPU_REG32 *)0xE0042004)

//DEMCR�Ĵ����ĵ�24λ�����Ҫʹ��DWT ETM ITM�� TPIU�Ļ�
//DEMCR�Ĵ����ĵ�24λ��1
#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24

//DWTCR�Ĵ����ĵ�0λ����Ϊ1��ʱ��ʹ��CYCCNT������
//ʹ��CYCCNT֮ǰӦ����ʼ��
#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00

//��ȡϵͳʱ��
//����ֵ��ϵͳʱ��HCLK
CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  hclk_freq;

    hclk_freq = 168000000;
    return (hclk_freq);
}

//�˺������������ͳ�ʼ��CPU��ʱ�����ʱ������ʵ����ʹ��DWT��CYCCNT
//���ҳ�ʼ��CYCCNTΪ0
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  fclk_freq;

    fclk_freq = BSP_CPU_ClkFreq();

    BSP_REG_DEM_CR     |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA;    /* Enable Cortex-M4's DWT CYCCNT reg.                   */
    BSP_REG_DWT_CYCCNT  = (CPU_INT32U)0u;
    BSP_REG_DWT_CR     |= (CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif

//�˺�����ʵ���ǻ�ȡCYCCNT�е�ֵ
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;


    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif

//����������������CPU_TS32_to_uSec()��CPU_TS64_to_uSec()����������ȡ����
//ʱ��������ת��Ϊus
#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;
	
    fclk_freq = BSP_CPU_ClkFreq();
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);
    return (ts_us);
}
#endif

#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;

    fclk_freq = BSP_CPU_ClkFreq();
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif
