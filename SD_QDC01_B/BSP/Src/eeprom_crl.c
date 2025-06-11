#include "eeprom_crl.h"

/**
 * @brief	eeprom状态判定，是否写入过
 *
 * @param   
 *
 * @return  void
**/
void eeprom_statu_judge( void )
{
    uint8_t eeprom_statu_flag;

    eeprom_statu_flag = ISP_Read(EEPROM_STATU_JUDGE);

    if( eeprom_statu_flag != 0x58 )
    {
        slave_06.fan1_level        = 3;
        slave_06.fan2_level        = 3;
        slave_06.power_level       = 50;
        slave_06.channel_num       = 5;
        slave_06.sync_switch       = 0;
        slave_06.mode_num          = 2;
        slave_06.temp_alarm_value  = 80;
        capacity.capacity_min      = 0;
        capacity.capacity_h_H      = 0;
        capacity.capacity_h_L      = 0;
        eeprom_data_record(); 
    }
    eeprom_data_init();    
}

/**
 * @brief	主机更改参数后写入
 *
 * @param   
 *
 * @return  void
**/
void eeprom_data_record( void )
{
    ISP_Earse(0x0000);

    ISP_Write(FAN1_ADDR,slave_06.fan1_level);
    ISP_Write(FAN2_ADDR,slave_06.fan2_level);
    ISP_Write(POWER_ADDR,slave_06.power_level);
    ISP_Write(CHANNEL_ADDR,slave_06.channel_num);
    ISP_Write(SYNC_ADDR,slave_06.sync_switch);
    ISP_Write(MODE_ADDR,slave_06.mode_num);
    ISP_Write(TEMP_ALARM,slave_06.temp_alarm_value);
    ISP_Write(CAPACITY_min,capacity.capacity_min);
    ISP_Write(CAPACITY_h_H,capacity.capacity_h_H);
    ISP_Write(CAPACITY_h_L,capacity.capacity_h_L);

    ISP_Write(EEPROM_STATU_JUDGE,0x58);
    delay_ms(10);
}


/**
 * @brief	eeprom 数据初始化
 *
 * @param   
 *
 * @return  void
**/
void eeprom_data_init( void )
{
    /*    PWM风速初始化    */
    slave_06.fan1_level = ISP_Read(FAN1_ADDR);

    fan1_ctrl(slave_06.fan1_level);

    /*    LED开关状态初始化    */
    slave_06.fan2_level = ISP_Read(FAN2_ADDR);

    fan2_ctrl(slave_06.fan2_level);

    /*    三路220V输出使能及功率初始化    */
    slave_06.channel_num = ISP_Read(CHANNEL_ADDR);
    AC_channel_ctrl(slave_06.channel_num);

    /*    三路220V输出功率初始化    */
    slave_06.power_level = ISP_Read(POWER_ADDR);

    AC_level_ctrl(slave_06.power_level);

    /*    同步状态初始化    */
    slave_06.sync_switch = ISP_Read(SYNC_ADDR);

    sync_ctrl();

    /*    工作模式初始化    */
    slave_06.mode_num = ISP_Read(MODE_ADDR);

    //mode_ctrl(slave_06.mode_num);

    /*    报警温度初始化    */
    slave_06.temp_alarm_value = ISP_Read(TEMP_ALARM);

    /*    报警温度初始化    */
    capacity.capacity_min = ISP_Read(CAPACITY_min);
    capacity.capacity_h_H = ISP_Read(CAPACITY_h_H);
    capacity.capacity_h_L = ISP_Read(CAPACITY_h_L);   
    capacity.capacity_h   = (capacity.capacity_h_H << 8) | capacity.capacity_h_L;

    slave_06.power_switch = 1;
}
