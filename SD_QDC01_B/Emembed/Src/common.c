#include "common.h"

/**
 * @brief	一些定时执行的事件   Timer3：10ms
 *
 * @param   
 *
 * @return  void
**/
void Tim3_ISR( void ) interrupt 19
{
    static uint16_t temp_scan_cnt = 0;
    static uint16_t AC_connect_cnt = 0;
    static uint8_t channel1_cnt,channel2_cnt,channel3_cnt = 0;
    static uint16_t capacity_time = 0;

    /* 1, Signal_IN状态查询           */
    if( Signal_IN == 1 )
    {
        ac_ctrl.signal_flag = 1;
    }else
    {
        ac_ctrl.signal_flag = 0;
    }

    /* 2, temp 上报 1s/次                             */
    if( temp.scan_flag == 0 )
    {
        temp_scan_cnt++;
        if( temp_scan_cnt == 100 )
        {
            temp.scan_flag = 1;
            temp_scan_cnt  = 0;
        }
    }
    
    /*3, 检测220V是否接入 5s关一次耗电计时，检测到220v输入再次开启计时*/
    if( ac_ctrl.connect_flag == 1 ) 
    {
        AC_connect_cnt++;
        if( AC_connect_cnt == 500 )
        {
            AC_connect_cnt = 0;
            ac_ctrl.connect_flag = 0;
        }
    }

    /*4, 耗电计时*/
    if( ac_ctrl.connect_flag == 1) 
    {
        if(( ac_ctrl.channel1_enable == 1 ) && (ac_ctrl.temp_alarm_flag == 0))
        {
            channel1_cnt++;
        }
        if(( ac_ctrl.channel2_enable == 1 ) && (ac_ctrl.temp_alarm_flag == 0))
        {
            channel2_cnt++;
        }
        if(( ac_ctrl.channel3_enable == 1 ) && (ac_ctrl.temp_alarm_flag == 0))
        {
            channel3_cnt++;
        }
    }
    
    capacity_time = (channel1_cnt + channel2_cnt + channel3_cnt) * slave_06.power_level;
    if( capacity_time >= 30000 )                    //1s
    {
        capacity.capacity_s ++;

        capacity_time = 0;
        channel1_cnt = channel2_cnt = channel3_cnt = 0;

        if( capacity.capacity_s == 60 )             //1min
        {
            capacity.capacity_min++;

            capacity.capacity_s = 0;

            capacity.record_flag = 1;   //满功率1分钟记录一次

            if( capacity.capacity_min == 60 )       //1h
            {
                capacity.capacity_h++;

                capacity.capacity_min = 0;
            }
        }
    }
}
