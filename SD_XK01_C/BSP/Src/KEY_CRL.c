#include "KEY_CRL.h"

KEY key;

/**
 * @brief	跳转按键初始状态
 *
 * @param   void
 *
 * @return  void
 */
void jump_to_init( void )
{
    /*       无图标跳动     */
    gui_flicker.enable_flag = false;

    /*       可继续查询从机状态     */
    modbus.modbus_04_scan_allow = 1;

    /*       复位图标跳动起始点     */
    gui_flicker.selection = 0;
}

/**
 * @brief	按键扫描 50ms/次
 *
 * @param   void
 *
 * @return  void
 */
void key_scan( void )
{
    uint8_t key_value;

    /*       50ms检测一次     */
    if(key.key_scan_flag == 1)
	{
		key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
	}
    switch (key_value)
    {
        case KEY1:      KEY1_press();       break;

        case KEY2:      KEY2_press();       break;

        case KEY3:      KEY3_press();       break;

        case KEY4:      KEY4_press();       break;

        case KEY5:      KEY5_press();       break;

        default:                            break;
    }
}

/**
 * @brief	KEY1功能：长按跳转报警温度设置，短按调节AC输出通道
 *
 * @param   void
 *
 * @return  void
 */
void KEY1_press( void )
{
    uint8_t key_value = KEY1;
    uint8_t key1_delay_cnt = 100;
    if( gui_info.power_switch == true )
    {
        buzzer_open();
        delay_ms(10);   
        /*               KEY1长按                 */
        while((key_value == KEY1)&&(key1_delay_cnt!=0))        
        {
            key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
            key1_delay_cnt--;
            delay_ms(10); 
        }
        if( key1_delay_cnt == 0 )                   //长按1s后跳转报警温度调节界面
        {
            gui_flicker.enable_flag = true;
            gui_flicker.selection = TEMP_ALARM;              
            
            buzzer_close();
            delay_ms(500);
        }else/*         KEY1短按                */
        {
            gui_info.channel_num++;                 //调节通道
            if( gui_info.channel_num > 5 )
            {
                gui_info.channel_num = 0;
            }
            channel_dis();
            write_slave_06(POWER_CHANNEL_ADDR,gui_info.power_level,gui_info.channel_num);
        }
        buzzer_close();
        key.key_scan_flag = 0;
    }
}

/**
 * @brief	KEY2功能：无图标跳动时调节功率（↓） 有图标跳动时调节风扇、模式、LED开关、报警温度（↓）
 *
 * @param   void
 *
 * @return  void
 */
void KEY2_press( void )
{
    if( gui_info.power_switch == true )
    {
        buzzer_open();
        delay_ms(10);
        if( gui_flicker.enable_flag == 0 )
        {
            if( gui_info.power_level > 20 )
            {
                gui_info.power_level -= 5;
                write_slave_06(POWER_CHANNEL_ADDR,gui_info.power_level,gui_info.channel_num);
            }
            power_dis();
        }else
        {
            switch(gui_flicker.selection)
            {
                case FAN_ICON:
                    if( gui_info.fan_level > 0)
                    {
                        gui_info.fan_level -= 1;
                        write_slave_06(FAN_ADDR,0X00,gui_info.fan_level);
                    }
                    fan_dis();
                    break;

                case LED_ICON:
                    gui_info.led_switch = LED_OFF;
                    write_slave_06(LED_ADDR,0X00,gui_info.led_switch);
                    led_dis();    

                    break;

                case MODE_ICON:
                    gui_info.mode_num -= 1;
                    if( gui_info.mode_num <= 1 )
                    {
                        gui_info.mode_num = 1;
                    }
                    mode_select();
                    write_slave_16();
                    mode_dis();
                    break;

                case TEMP_ALARM:
                    if( gui_info.temp_alarm_value > 20 )
                    {
                        gui_info.temp_alarm_value -= 1;
                        write_slave_06(TEMP_ALARM_ADDR,0X00,gui_info.temp_alarm_value);
                    }
                    temp_alarm_dis();

                    break;

                default:
                    break;
            }
        }
        buzzer_close();
        key.key_scan_flag = 0;
    }
 }

 /**
 * @brief	KEY3功能：长按：开机  短按：1. 进入风扇、LED、模式设置 再次点击后按顺序跳转 2.关机时开机
 *
 * @param   void
 *
 * @return  void
 */
void KEY3_press( void )
{
    uint8_t key3_delay_cnt = 100;
    uint8_t key_value = KEY3;

    buzzer_open();
    delay_ms(10);
     /*               KEY1长按                 */
    if( gui_info.power_switch == true )
    {
        while((key_value == KEY3)&&(key3_delay_cnt!=0))        
        {
            key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
            key3_delay_cnt--;
            delay_ms(10); 
        }
        if( key3_delay_cnt == 0 )                   //长按1s后跳转报警温度调节界面
        {
            gui_info.power_switch = false;
            LCD_Clear(BLACK);        
            write_slave_06(POWER_SWITCH_ADDR,0x00,0x00);
            buzzer_close();
            delay_ms(500);
        }else
        {
            switch(gui_flicker.selection)
            {
                case FAN_ICON:
                    fan_dis();
                    gui_flicker.selection = LED_ICON;

                    break;

                case LED_ICON:
                    led_dis();
                    gui_flicker.selection = MODE_ICON;

                    break;

                case MODE_ICON:
                    mode_dis();
                    jump_to_init();
                    break;

                default:
                    gui_flicker.selection++;
                    gui_flicker.enable_flag = true;

                    break;
                
            }
        }
    }else
    {
        gui_info.power_switch = true;
        write_slave_06(POWER_SWITCH_ADDR,0x00,0x01);
        GUI_Icon_Init();
    }
    
    buzzer_close();
    key.key_scan_flag = 0;
}

/**
 * @brief	KEY4功能：无图标跳动时调节功率（↓） 有图标跳动时调节风扇、模式、LED开关、报警温度（↑）
 *
 * @param   void
 *
 * @return  void
 */
void KEY4_press( void )
{
    if( gui_info.power_switch == true )
    {
        buzzer_open();
        delay_ms(10);
        if( gui_flicker.enable_flag == 0 )
        {
            if( gui_info.power_level < 100 )
            {
                gui_info.power_level += 5;
                write_slave_06(POWER_CHANNEL_ADDR,gui_info.power_level,gui_info.channel_num);
            }
            power_dis();
        }else
        {
            switch(gui_flicker.selection)
            {
                case FAN_ICON:
                    if( gui_info.fan_level < 6 )
                    {
                        gui_info.fan_level += 1;
                        write_slave_06(FAN_ADDR,0X00,gui_info.fan_level);
                    }
                    fan_dis();
                    break;

                case LED_ICON:
                    gui_info.led_switch = LED_ON;
                    write_slave_06(LED_ADDR,0X00,gui_info.led_switch);
                    led_dis();    

                    break;

                case MODE_ICON:
                    gui_info.mode_num += 1;
                    if( gui_info.mode_num >= 3 )
                    {
                        gui_info.mode_num = 3;
                    }
                    mode_select();
                    write_slave_16();
                    mode_dis();

                    break;

                case TEMP_ALARM:
                    if( gui_info.temp_alarm_value < 120 )
                    {
                        gui_info.temp_alarm_value += 1;
                        write_slave_06(TEMP_ALARM_ADDR,0X00,gui_info.temp_alarm_value);
                    }
                    temp_alarm_dis();

                    break;

                default:
                    break;
            }
        }
        
        buzzer_close();
        key.key_scan_flag = 0;
    }
}

/**
 * @brief	KEY5功能：长按选择同步模式开关，短按跳转按键初始化状态
 *
 * @param   void
 *
 * @return  void
 */
void KEY5_press( void )
{
    uint8_t key_value = KEY5;
    uint8_t key5_delay_cnt = 100;
    if( gui_info.power_switch == true )
    {
        buzzer_open();
        delay_ms(10);
        while((key_value == KEY5)&&(key5_delay_cnt!=0)) 
        {
            key_value = (B1_VAL) | (B2_VAL<<1) | (B3_VAL<<2) | (B4_VAL<<3) | (B5_VAL<<4);
            key5_delay_cnt--;
            delay_ms(10); 
        }
        if( key5_delay_cnt == 0 )
        {
            gui_info.sync_switch = 1 - gui_info.sync_switch;
            sync_dis();
            write_slave_06(SYNC_ADDR,0X00,gui_info.sync_switch);

            buzzer_close();
            delay_ms(500);
        }else
        {
            GUI_Icon_Init();
            jump_to_init();
        }

        buzzer_close();
        key.key_scan_flag = 0;
    }
}

/**
 * @brief	蜂鸣器开
 *
 * @param   void
 *
 * @return  void
 */
void buzzer_open( void )
{
    HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_SET);
}

/**
 * @brief	蜂鸣器关
 *
 * @param   void
 *
 * @return  void
 */
void buzzer_close( void )
{
    delay_ms(150);
    HAL_GPIO_WritePin(Buzzer_GPIO_Port,Buzzer_Pin,GPIO_PIN_RESET);
}

/**
 * @brief	模式选择
 *
 * @param   void
 *
 * @return  void
 */
void mode_select( void )
{
    gui_info.mode_write_flag = 1;
    
    switch (gui_info.mode_num)
    {
        case 1:
            gui_info.power_level = 30;
            gui_info.fan_level   = 3;

            break;

        case 2:
            gui_info.power_level = 50;
            gui_info.fan_level   = 4;

            break;

        case 3:
            gui_info.power_level = 80;
            gui_info.fan_level   = 6;

            break;

        default:
            break;
    }
}
