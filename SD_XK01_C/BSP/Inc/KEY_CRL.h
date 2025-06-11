#ifndef __KEY_CRL_H_
#define __KEY_CRL_H_

#include "main.h"
#include "delay.h"
#include "GUI.h"
#include "POWER_CRL.h"

#define KEY1        15
#define KEY2        23  
#define KEY3        27
#define KEY4        29
#define KEY5        30

#define B1_VAL HAL_GPIO_ReadPin(button1_GPIO_Port,button1_Pin)
#define B2_VAL HAL_GPIO_ReadPin(button2_GPIO_Port,button2_Pin)
#define B3_VAL HAL_GPIO_ReadPin(button3_GPIO_Port,button3_Pin)
#define B4_VAL HAL_GPIO_ReadPin(button4_GPIO_Port,button4_Pin)
#define B5_VAL HAL_GPIO_ReadPin(button5_GPIO_Port,button5_Pin)

typedef struct 
{       
    uint8_t key_scan_flag;
}KEY;

extern KEY key;

void key_scan( void );

void key_init( void );

void KEY1_press( void );

void KEY2_press( void );

void KEY3_press( void );

void KEY4_press( void );

void KEY5_press( void );

void buzzer_open( void );

void buzzer_close( void );

void jump_to_init( void );

void mode_select( void );

#endif
