/**
  ******************************************************************************
  * @file                task.c
  * @author              BruceOu
  * @lib version         HAL
  * @version             V1.0
  * @date                2021-02-27
  * @blog                https://blog.bruceou.cn/
  * @Official Accounts   Ƕ��ʽʵ��¥
  * @brief               RTT����
  ******************************************************************************
  */
/*Includes**********************************************************************/
#include "task.h"

#define THREAD_PRIORITY         2
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;

#define SAMPLE_UART_NAME       "uart1"    /* �����豸���� */
#define PWM_PIN GET_PIN(B,1)
static rt_device_t serial;                /* �����豸��� */
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* ��ʼ�����ò��� */
void servo_open(void);//��еצ��
void servo_close(void);//��еצ�ر�
//char receive_info[]="Successfully receive!\r\n";

/* �߳� ����ں��� */
static void thread_entry(void *parameter)
{
    rt_uint32_t count = 1;
    char flag='0';
    char useful_detect[]="Successfully receive";
    //char str[] = "hello RT-Thread!\r\n";
    int send=1;
        /* step1�����Ҵ����豸 */
        serial = rt_device_find(SAMPLE_UART_NAME);
        /* step2���޸Ĵ������ò��� */
        config.baud_rate = BAUD_RATE_9600;        //�޸Ĳ�����Ϊ 9600
        config.data_bits = DATA_BITS_8;           //����λ 8
        config.stop_bits = STOP_BITS_1;           //ֹͣλ 1
        config.bufsz     = 128;                   //�޸Ļ����� buff size Ϊ 128
        config.parity    = PARITY_NONE;           //����żУ��λ
        rt_pin_mode(PWM_PIN, PIN_MODE_OUTPUT);
        /* step3�����ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
        rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

        /* step4���򿪴����豸�����жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
        rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
        while (count++)
        {
            flag='0';
            if(count == 5000)
            {
                count = 1;
            }
            rt_thread_mdelay(200);
            //str=count;
            /* �����ַ��� */
            rt_device_write(serial, 0, &send, (sizeof(send)));
            //rt_thread_mdelay(200);
            rt_device_read(serial, 0, &flag, (sizeof(flag)));
            if(flag=='1')
            {
                rt_kprintf("%s \r\n",useful_detect);python
                //rt_device_write(serial, 0, receive_info, (sizeof(str) - 1));
                servo_open();//��еצ��
                rt_thread_mdelay(1000);
                servo_close();
            }

        }

}
void servo_open(void){
    for(int i=0;i<10;i++){
        rt_thread_mdelay(18.1);
        rt_pin_write(PWM_PIN, PIN_HIGH);
        rt_thread_mdelay(1.9);
        rt_pin_write(PWM_PIN, PIN_LOW);
    }
}//���ƶ����

void servo_close(void){
    for(int i=0;i<10;i++){
        rt_thread_mdelay(18);
        rt_pin_write(PWM_PIN, PIN_HIGH);
        rt_thread_mdelay(2);
        rt_pin_write(PWM_PIN, PIN_LOW);
    }
}//���ƶ���ر�

/* �߳� */
int thread_serial(void)
{
    /* �����߳� �������� thread������� thread_entry*/
    tid1 = rt_thread_create("thread",
                            thread_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* �������߳̿��ƿ飬��������߳� */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return 0;
}

/* ������ msh �����б��� */
MSH_CMD_EXPORT(thread_serial, thread serial);
INIT_COMPONENT_EXPORT(thread_serial);
