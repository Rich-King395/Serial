/**
  ******************************************************************************
  * @file                task.c
  * @author              BruceOu
  * @lib version         HAL
  * @version             V1.0
  * @date                2021-02-27
  * @blog                https://blog.bruceou.cn/
  * @Official Accounts   嵌入式实验楼
  * @brief               RTT任务
  ******************************************************************************
  */
/*Includes**********************************************************************/
#include "task.h"

#define THREAD_PRIORITY         2
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;

#define SAMPLE_UART_NAME       "uart1"    /* 串口设备名称 */
#define PWM_PIN GET_PIN(B,1)
static rt_device_t serial;                /* 串口设备句柄 */
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */
void servo_open(void);//机械爪打开
void servo_close(void);//机械爪关闭
//char receive_info[]="Successfully receive!\r\n";

/* 线程 的入口函数 */
static void thread_entry(void *parameter)
{
    rt_uint32_t count = 1;
    char flag='0';
    char useful_detect[]="Successfully receive";
    //char str[] = "hello RT-Thread!\r\n";
    int send=1;
        /* step1：查找串口设备 */
        serial = rt_device_find(SAMPLE_UART_NAME);
        /* step2：修改串口配置参数 */
        config.baud_rate = BAUD_RATE_9600;        //修改波特率为 9600
        config.data_bits = DATA_BITS_8;           //数据位 8
        config.stop_bits = STOP_BITS_1;           //停止位 1
        config.bufsz     = 128;                   //修改缓冲区 buff size 为 128
        config.parity    = PARITY_NONE;           //无奇偶校验位
        rt_pin_mode(PWM_PIN, PIN_MODE_OUTPUT);
        /* step3：控制串口设备。通过控制接口传入命令控制字，与控制参数 */
        rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

        /* step4：打开串口设备。以中断接收及轮询发送模式打开串口设备 */
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
            /* 发送字符串 */
            rt_device_write(serial, 0, &send, (sizeof(send)));
            //rt_thread_mdelay(200);
            rt_device_read(serial, 0, &flag, (sizeof(flag)));
            if(flag=='1')
            {
                rt_kprintf("%s \r\n",useful_detect);python
                //rt_device_write(serial, 0, receive_info, (sizeof(str) - 1));
                servo_open();//机械爪打开
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
}//控制舵机打开

void servo_close(void){
    for(int i=0;i<10;i++){
        rt_thread_mdelay(18);
        rt_pin_write(PWM_PIN, PIN_HIGH);
        rt_thread_mdelay(2);
        rt_pin_write(PWM_PIN, PIN_LOW);
    }
}//控制舵机关闭

/* 线程 */
int thread_serial(void)
{
    /* 创建线程 ，名称是 thread，入口是 thread_entry*/
    tid1 = rt_thread_create("thread",
                            thread_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(thread_serial, thread serial);
INIT_COMPONENT_EXPORT(thread_serial);
