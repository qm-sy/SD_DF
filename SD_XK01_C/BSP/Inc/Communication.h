#ifndef __COMMUNICATION_H_
#define __COMMUNICATION_H_

#include "sys.h"
#include "main.h"
#include "usart.h"
#include "modbus_rtu.h"

typedef struct  
{
	//作为从机时使用
    uint8_t  timout;       //modbus数据持续时间
    uint8_t  recount;      //modbus端口接收到的数据个数
    uint8_t  timrun;       //modbus定时器是否计时标志
    volatile uint8_t  reflag;       //modbus一帧数据接受完成标志位
    uint8_t  rcvbuf[100];   //modbus接收缓冲区
    uint8_t  sendbuf[100]; //modbus接发送缓冲区
}RS485;

extern RS485 rs485;

void RS485_Init( void );

#endif




