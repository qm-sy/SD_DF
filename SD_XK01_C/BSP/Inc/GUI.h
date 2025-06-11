#ifndef __GUI_H_
#define __GUI_H_

#include "sys.h"
#include "ST7789V2.h"
#include "modbus_rtu.h"
#include "KEY_CRL.h"

#define LED_ON 			1
#define LED_OFF 		0

#define MOED_1          1
#define MOED_2          2
#define MOED_3          3

#define LEVEL_0         0
#define LEVEL_1         1
#define LEVEL_2         2
#define LEVEL_3         3
#define LEVEL_4         4
#define LEVEL_5         5
#define LEVEL_6         6

#define CHANNLE_1       1
#define CHANNLE_2       2
#define CHANNLE_3       3
#define CHANNLE_12      4
#define CHANNLE_123     5

#define SYNC_ON         1
#define SYNC_OFF        0

#define CONNECT         1
#define UNCONNECT       0

/*      需要跳动显示的内容   */
#define FAN1_ICON        1
#define FAN2_ICON        2
#define MODE_ICON       3
#define TEMP_ALARM      4

typedef struct  
{       
    uint8_t  fan1_level;
    uint8_t  fan2_level;
    uint8_t  mode_num;
    uint8_t  mode_write_flag;
    uint8_t  power_level;       
    uint8_t  channel_num;
    uint8_t  sync_switch;
    uint8_t  temp_alarm_value;
    uint8_t  temp_value;
    uint8_t  envir_temp;
    uint8_t  envir_humidity;

    uint8_t  capacity_min;
    uint16_t capacity_h;
    float    capacity;

    bool connect_flag;
    bool power_switch;
}GUI_INFO;

typedef struct 
{
    bool  start_flag;
    bool  clear_flag;
    bool  enable_flag; 
    uint8_t  selection;
}GUI_FLICKER;

extern GUI_INFO gui_info;
extern GUI_FLICKER gui_flicker;

void GUI_Info_Init( void );
void GUI_Icon_Init( void );

void Icon_Flicker( void );

void connect_dis( void );
void sync_dis( void );
void alarm_icon_dis( void );
void dht11_dis( void );
void channel_dis( void );
void temp_dis( void );
void temp_alarm_dis( void );
void power_dis( void );
void capacity_dis( void );
void fan1_dis( void );
void fan2_dis( void );
void mode_dis( void );

void gui_info_update( void );
void gui_info_refresh( void );

#endif

