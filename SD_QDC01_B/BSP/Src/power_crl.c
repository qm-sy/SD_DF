#include "power_crl.h"

SLAVE_06 slave_06;
CAPACITY capacity;
AC_CTRL ac_ctrl;

/**
 * @brief	移相触发调用结构体初始化
 *
 * @param   
 *
 * @return  void
**/
void Power_Init( void )
{
    ac_ctrl.zero_flag       = 0;
    ac_ctrl.connect_flag    = 0;
    ac_ctrl.temp_alarm_flag = 0;
    ac_ctrl.channel1_enable = 0;
    ac_ctrl.channel2_enable = 0;
    ac_ctrl.channel3_enable = 0;
    ac_ctrl.time_delay      = 56500;
}

/**
 * @brief	功率计时初始化
 *
 * @param   
 *
 * @return  void
**/
void Capacity_Init( void )
{
    capacity.capacity_s = 0;
    capacity.record_flag = 0;
}

/**
 * @brief	外部中断0中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void INT0_ISR( void ) interrupt 0
{
    /* 1, 检测到外部中断后，等待THL\TLI后触发TIM1中断       */
    TL1 = ac_ctrl.time_delay;				
	TH1 = ac_ctrl.time_delay >> 8;				

    ac_ctrl.zero_flag = 1;
    ac_ctrl.connect_flag = 1;
    /* 2, 定时器1开始计时，打开TIM1中断                     */
    TR1 = 1;				     
    ET1 = 1; 
}

/**
 * @brief	Timer1中断处理函数
 *
 * @param   
 *
 * @return  void
**/
void Tim1_ISR( void ) interrupt 3   //10ms
{
    /* 1, 中断触发后，power_ch电平 由高电平变为低电平           */
    if( ac_ctrl.zero_flag == 1 )
    {
        ac_ctrl.zero_flag = 0;

         /* 2, 温度允许，使能为1时可开启输出          */
        if(( ac_ctrl.channel1_enable == 1 ) && (ac_ctrl.temp_alarm_flag == 0))
        {
            AC_Out1 = 0;
        }
        if(( ac_ctrl.channel2_enable == 1 ) && (ac_ctrl.temp_alarm_flag == 0))
        {
            AC_Out2 = 0;
        }
        if(( ac_ctrl.channel3_enable == 1 ) && (ac_ctrl.temp_alarm_flag == 0))
        {
            AC_Out3 = 0;
        }

         /* 3, 设置下一次Timer1中断触发所需时间，即脉冲时间       */
        TL1 = 0xF7;				
        TH1 = 0xFF;				
    }else
    {
        /* 2, 下一次进入Timer1中断，power_ch电平 由低电平变为高电平，完成一次10us脉冲，即斩波*/
        AC_Out1 = AC_Out2 = AC_Out3 = 1;

        /* 3, 定时器1停止计时，关闭TIM1中断，等待下一次外部中断     */
        TR1 = 0;				    
        ET1 = 0; 
    }
}

/**
 * @brief	220V输出功率控制函数 
 *
 * @param   power_level：输出功率百分比  0%~100%
 *
 * @return  void
**/
void AC_level_ctrl( uint8_t power_level )
{
    ac_ctrl.time_delay = 56500 + 90*power_level;
}

/**
 * @brief	220V输出功率控制函数 
 *
 * @param   power_level：输出功率百分比  0%~100%
 *
 * @return  void
**/
void AC_channel_ctrl( uint8_t channel_num )
{
    switch (channel_num)
    {
    case 0:
        ac_ctrl.channel1_enable = 0;
        ac_ctrl.channel2_enable = 0;
        ac_ctrl.channel3_enable = 0;

        break;

    case 1:
        ac_ctrl.channel1_enable = 1;
        ac_ctrl.channel2_enable = 0;
        ac_ctrl.channel3_enable = 0;

        break;

    case 2:
        ac_ctrl.channel1_enable = 0;
        ac_ctrl.channel2_enable = 1;
        ac_ctrl.channel3_enable = 0;
        
        break;

    case 3:
        ac_ctrl.channel1_enable = 0;
        ac_ctrl.channel2_enable = 0;
        ac_ctrl.channel3_enable = 1;
        
        break;

    case 4:
        ac_ctrl.channel1_enable = 1;
        ac_ctrl.channel2_enable = 1;
        ac_ctrl.channel3_enable = 0;
        
        break;

    case 5:
        ac_ctrl.channel1_enable = 1;
        ac_ctrl.channel2_enable = 1;
        ac_ctrl.channel3_enable = 1;
        
        break;

    default:
        break;
    }
}

/**
 * @brief	24V PWM风扇1档位控制函数
 *
 * @param   level :风扇档位 0~6档
 *
 * @return  void
**/
void fan1_ctrl( uint8_t level )
{
    PWMB_CCR7= level * 184;
}

/**
 * @brief	24V PWM风扇2档位控制函数
 *
 * @param   level :风扇档位 0~6档
 *
 * @return  void
**/
void fan2_ctrl( uint8_t level )
{
    PWMB_CCR8= level * 184;
}

/**
 * @brief	同步控制函数：无外部信号时 PWM风扇和220V输出关闭
 *
 * @param   
 *
 * @return  void
**/
void sync_ctrl( void )
{
    if( slave_06.sync_switch == 1 )
    {
        if( ac_ctrl.signal_flag == 1 )
        {
            PWMB_BKR = 0x80;    //PWM控制
            EX0 = 1;            //外部中断控制
        }else
        {
            PWMB_BKR = 0x00; 
            EX0 = 0;
        }
    }else
    {
        PWMB_BKR = 0x80; 
        EX0 = 1;
    }
}

/**
 * @brief 温度扫描，DHT11温湿度扫描 1s/次 控制220V输出使能
 *
 * @param[in] 
 * 
 * @return  
 * 
**/
void temp_scan( void )
{
    if( temp.scan_flag == 1)
    {
        temp.NTC1_value =  get_temp(NTC1);

        Read_DHT11();

        if( temp.NTC1_value >= slave_06.temp_alarm_value )  
        {
            ac_ctrl.temp_alarm_flag = 1;     
            Buzzer = 0;
        }else
        {
            ac_ctrl.temp_alarm_flag = 0;    
            Buzzer = 1; 
        }

        temp.scan_flag = 0;
    }
}

/**
 * @brief 用电量累计扫描 三路全开满功率1min记录一次
 *
 * @param[in] 
 * 
 * @return  
 * 
**/
void capacity_scan( void )
{
    if( capacity.record_flag == 1 )
    {
        capacity.capacity_h_H = capacity.capacity_h >> 8;
        capacity.capacity_h_L = capacity.capacity_h;

        eeprom_data_record(); 

        capacity.record_flag = 0;
    }
}

/**
 * @brief	模式控制函数 
 *
 * @param   
 *
 * @return  void
**/
void mode_ctrl( uint8_t mode_num )
{
    switch (mode_num)
    {
        case 1:         //节能模式
            AC_level_ctrl(30);
            fan1_ctrl(3);

            slave_06.power_level = 30;
            slave_06.fan1_level = 3;
            eeprom_data_record();

            break;

        case 2:         //普通模式
            AC_level_ctrl(50);
            fan1_ctrl(4);

            slave_06.power_level = 50;
            slave_06.fan1_level = 4;
            eeprom_data_record();

            break;

        case 3:         //强劲模式
            AC_level_ctrl(80);
            fan1_ctrl(6);

            slave_06.power_level = 80;
            slave_06.fan1_level = 6;
            eeprom_data_record();

            break;

        default:
            break;
    }
}

/**
 * @brief	电源开关（受屏幕控制） 
 *
 * @param   
 *
 * @return  void
**/
void power_switch_ctrl( uint8_t power_switch )
{
    if( power_switch== 0 )
    {
        PWMB_BKR = 0x00; 
        EX0 = 0;
    }else
    {
        PWMB_BKR = 0x80;    //PWM控制
        EX0 = 1;            //外部中断控制
    }
}