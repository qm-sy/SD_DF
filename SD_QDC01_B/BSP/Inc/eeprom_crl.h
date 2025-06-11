#ifndef __EEPROM_CRL_H_
#define __EEPROM_CRL_H_

#include "eeprom.h"
#include "sys.h"
#include "power_crl.h"
#include "ntc.h"
#include "stdio.h"

#define FAN_ADDR            0x0000
#define LED_ADDR            0x0001
#define POWER_ADDR          0x0002
#define CHANNEL_ADDR        0x0003
#define SYNC_ADDR           0X0004 
#define MODE_ADDR           0X0005
#define TEMP_ALARM          0X0006
#define CAPACITY_min        0X0007
#define CAPACITY_h_H        0X0008
#define CAPACITY_h_L        0X0009

#define EEPROM_STATU_JUDGE  0X0010         

void eeprom_statu_judge( void );
void eeprom_data_record( void );
void eeprom_data_init( void );

#endif