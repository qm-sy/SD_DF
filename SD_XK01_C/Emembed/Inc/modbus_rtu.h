#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "usart.h"
#include "main.h"
#include "PWM_CRL.h"
#include "communication.h"
#include "delay.h"
#include "GUI.h"

#define SLAVE_ADDR          0x1B

#define FUN_03      0X03
#define FUN_04      0X04
#define FUN_06      0X06
#define FUN_16      0X10

#define START_REG_H_03      0
#define START_REG_L_03      0
#define REG_NUM_H_03        0
#define REG_NUM_L_03        6

#define START_REG_H_04      0
#define START_REG_L_04      0
#define REG_NUM_H_04        0
#define REG_NUM_L_04        4

#define FAN_ADDR            0X00
#define LED_ADDR            0X01
#define POWER_CHANNEL_ADDR  0X02
#define SYNC_ADDR           0X03
#define MODE_ADDR           0X04
#define TEMP_ALARM_ADDR     0X05
#define POWER_SWITCH_ADDR   0x06

typedef struct  
{
    uint8_t send_buf[48];
    uint8_t rcv_buf[48];

    bool signal_in_flag;

    bool modbus_04_scan_flag;
    bool modbus_04_scan_allow;
}MODBUS_INFO;

extern MODBUS_INFO modbus;

void Modbus_Fun3( void );

void Modbus_Fun4( void );

// void Modbus_Fun6( void );

// void Modbus_Fun16( void );

void Modbus_Event( void );

void modbus_send_data( uint8_t *buf , uint8_t len );

uint8_t modbus_wait_receive( void );

uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);

void slave_statu_query_modify( uint8_t fun, uint16_t reg_addr,uint16_t reg_num,uint16_t reg_val );
void test_hanshu(void);
void Modbus_fun03_Master( uint16_t reg_addr,uint16_t reg_num );
void Modbus_fun04_Master( uint16_t reg_addr,uint16_t reg_num );
void Modbus_fun06_Master( uint16_t reg_addr,uint16_t reg_num, uint16_t reg_val );
void write_slave_16( void );
void send_param( void );
void read_slave_03( void );
void read_slave_04( void );
void write_slave_06( uint16_t reg_addr, uint8_t reg_val_H, uint8_t reg_val_L);

#endif
