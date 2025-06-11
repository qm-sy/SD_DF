#ifndef __POWER_CRL_H_
#define __POWER_CRL_H_

#include "sys.h"
#include "gpio.h"
#include "ntc.h"
#include "eeprom_crl.h"
#include "dht11.h"

#define FAN_ON    1
#define FAN_OFF   0

typedef struct 
{
    uint8_t  fan1_level;             //220V输入检测
    uint8_t  fan2_level;             //220V输入检测
    uint8_t  power_level;
    uint8_t  channel_num;
    uint8_t  sync_switch;             //同步标志位
    uint8_t  mode_num;             //模式选择
    uint8_t  temp_alarm_value;
    uint8_t  power_switch;
}SLAVE_06;

typedef struct 
{
    uint8_t  capacity_s;              //功率累加1s
    uint8_t  capacity_min;            //功率累加1min
    uint16_t capacity_h;              //功率累加1h
    uint8_t  capacity_h_H;
    uint8_t  capacity_h_L;
    uint8_t  record_flag;    //功率累加记录标志位

}CAPACITY;

typedef struct 
{
    uint8_t  channel1_enable;     //通道1输出使能
    uint8_t  channel2_enable;     //通道2输出使能
    uint8_t  channel3_enable;     //通道3输出使能
    uint8_t  zero_flag; 
    uint16_t time_delay;            //移相延时
    uint8_t  signal_flag;         //24V输入检测
    uint8_t  temp_alarm_flag;
    uint8_t  connect_flag;
    uint16_t capacity_time;
}AC_CTRL;

extern SLAVE_06 slave_06;
extern CAPACITY capacity;
extern AC_CTRL ac_ctrl;

void Power_Init( void );
void AC_level_ctrl( uint8_t power_level );
void sync_ctrl( void );
void temp_scan( void );
void fan1_ctrl( uint8_t level );
void fan2_ctrl( uint8_t level );
void mode_ctrl( uint8_t mode_num );
void Capacity_Init( void );
void capacity_scan( void );
void AC_channel_ctrl( uint8_t channel_num );
void power_switch_ctrl( uint8_t power_switch );

#endif