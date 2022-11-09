/* 包含头文件 */
#include "ioCC2530.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/* 四个LED灯的端口定义 */
#define D3 P1_0
#define D4 P1_1
#define D5 P1_3
#define D6 P1_4
/* 按键的端口定义 */
#define SW1 P1_2
#define SW2 P0_1

/* 变量定义 */
int sys_state=0;//撤防模式：sys_state=0    布防模式：sys_state=1

unsigned int counter=0;      //10ms计数值
unsigned int alarmDelay=0;   //报警延时值

/**********LED端口初始化************************/
void InitLED(void)
{
    P1SEL &=~0x1B;      //设置D3 D4 D5 D6为普通IO口 
    P1DIR |= 0x1B;      //设置D3 D4 D5 D6为输出方向
    D3=0;
    D4=0;
    D5=0;
    D6=0;  
}

void InitKey(void)
{
  
   
    P1SEL &= ~0X04;       //设置SW1为普通IO口
    P1DIR &= ~0X04;       //设置SW1为输入引脚    
    
    /*.......答题区3 开始.......按钮SW1(P1_2)配置成采用中断的方式进行控制，
    上拉输入模式,下降沿触发中断...................*/
    
     
    
    
    
 /*.......答题区3 结束.......................................*/
    
    
    
    
    //按钮SW2(P0_1)配置成输入引脚
    // 1、初始化IO口工作在普通IO、上下拉模式
    P0SEL&=~0X02;       //设置KEY2为普通IO口
    P0DIR&=~0X02;       //设置KEY2为输入引脚
    P0INP&=~0X02;       //设置KEY2为上下拉模式

   
}

/***********定时器初始化************************/
void InittTimer1(void)
{
    T1IF=0;                  //清除timer1中断标志
    T1STAT &= ~0x01;         //清除通道0中断标志
    
    T1CTL = 0x0A;            //配置32分频，模模式
    
    T1CCTL0 |= 0x04;         //设定timer1通道0比较模式，定时10ms
    T1CC0L = 10000&0xFF;     //把10000的低8位写入T1CC0L
    T1CC0H = (10000>>8)&0xFF;//把10000的高8位写入T1CC0H
    
    T1IE = 1;                //使能定时器1的中断，或者写为IEN1 |= 0x02;
}

/**********串口通信初始化************************/
void InitUART0(void)
{
    PERCFG = 0x00; //usart0 使用备用位置1 TX-P0.3 RX-P0.2  
    P2DIR &= ~0xC0;//P0优先作为UART方式  
    P0SEL |= 0x0c; //P0.2 P0.3 用于外设功能
    
    U0CSR |= 0x80; //uart模式 允许发送，禁止接收
    
    U0BAUD = 59;//波特率：9600
    U0GCR = 8; 
    
    U0UCR |= 0x80; //流控无 无奇偶校验 8位数据位  1位停止位
}

/*************** 往串口发送指定长度的数据  ***************/
void uart_tx_string(char *data_tx)  
{   
  while(*data_tx!='\0')
  {   
    U0DBUF = *data_tx; // 将要发送的1字节数据写入U0DBUF
    while(UTX0IF == 0);  // 等待TX中断标志，即U0DBUF就绪
    UTX0IF = 0;          // 清零TX中断标志
    data_tx++;
  }
}

void uart_tx_byte(char data_tx)  
{   
    U0DBUF = data_tx;   // 将要发送的1字节数据写入U0DBUF
    while(UTX0IF == 0); // 等待TX中断标志，即U0DBUF就绪
    UTX0IF = 0;         // 清零TX中断标志
} 


/************定时器T1中断服务子程序**************/
#pragma vector = T1_VECTOR //中断服务子程序
__interrupt void T1_ISR(void)
{   
    counter++;
    if(alarmDelay>0)
      alarmDelay--;
}
/************按键SW1中断服务子程序**************/
#pragma vector=P1INT_VECTOR //第P1组中断
__interrupt void EXTI_ISR()
{
    
    if(P1IFG &0X04) //按键SW1按下时
    {
       
        /*由考生设置按下按键后:
        1)切换系统的工作状态：设防与撤防。
        2)工作状态切换后，通过串口输出状态信息
          设防输出：SYSTEM RUN<回车换行>
          撤防输出：SYSTEM STOP<回车换行>
        */
         /*.......答题区4 开始.........*/
      
        
      
      
      
        /*.......答题区4 结束.........*/
    }
  
    //注意产生中断时会把以下值变为1，执行完中断后务必记得下面的操作：要清0
    //中断标志位清0
    P1IFG&=~(0X1<<2);//清SW1中断标志
    IRCON2&=~(0x1<<3);  //清P1端口组中断标志 第3位为0代表端口1组中断标志位被清除  P1IF=0;
}



/************撤防模式**************/
void StopMode(void)
{
    //由考生编写 ，撤防模式时，D5灯每500ms闪烁一次，其他灯熄灭
    /*.......答题区2 开始：....................*/
   
  
  
    
   /*.......答题区2 结束.......................*/
    alarmDelay=0;//撤防时，用alarmDelay控制D3 D4 D6全灭
}

/***************布防模式*****************/
void RunMode(void)
{
   
   
   
    if(SW2==0)
    {
        alarmDelay=1000; //按下SW2后 设置10秒延时
    }
     /*
    在布防模式时，D5常亮
    
    在SW2按下后，触发报警，报警时 D3 D4 D6轮流点亮，为跑马灯效果，每个灯约500ms亮,
    跑马灯效果保持约10秒后D3 D4 D6全部熄灭
    */  
    
    /*.......答题区5 开始....................*/
    
    
    
    
    
   /*.......答题区5 结束.......................................*/
}

/************main函数入口**************************/
void main(void)
{
    //时钟初始化,速度设置为32MHz
    CLKCONCMD &= 0X80;
    while(CLKCONSTA&0X40);
    
    InitLED();
    InitKey();
    InittTimer1();      //初始化Timer1
    InitUART0();        //UART0初始化

    EA = 1;             //使能全局中断
    
    while(1)
    {
      /*.......答题区1开始：按sys_state的不同值进行撤防与布防工作模式调用...*/
 
       
      
      
      
      
        /*.......答题区1结束...***********************************/
 
    }
}
