#include "GUI.h"
#include "pic.h"

GUI_INFO gui_info;
GUI_FLICKER gui_flicker;

/**
 * @brief	界面变量初始化
 *
 * @param   void
 *
 * @return  void
 */
void GUI_Info_Init( void )
{
    gui_info.connect_flag           = UNCONNECT;

    gui_info.sync_switch            = SYNC_OFF;
    gui_info.channel_num            = CHANNLE_123;
    gui_info.power_level            = 50;
    gui_info.fan1_level             = LEVEL_3; 
    gui_info.fan2_level             = LEVEL_3;
    gui_info.mode_write_flag        = false;
    gui_info.mode_num               = MOED_1;
    gui_info.temp_value             = 25;
    gui_info.temp_alarm_value       = 50;
    gui_info.capacity_h             = 0;
    gui_info.capacity_min           = 0;
    gui_info.capacity               = 0.0f;
    gui_info.power_switch           = true;

    gui_flicker.start_flag          = false;
    gui_flicker.clear_flag          = false;
    gui_flicker.enable_flag         = false; 
    gui_flicker.selection           = 0;
}

/**
 * @brief	开机后界面初始化
 *
 * @param   void
 *
 * @return  void
 */
void GUI_Icon_Init( void )
{
    /*              connect icon                */
    connect_dis();

    /*              sync icon                */
    sync_dis();

    /*              alarm icon                */
    alarm_icon_dis();

    /*              temp && shidu icon                */
    dht11_dis();
    PutChinese_12(13,245,"度",BACK_COLOR,POINT_COLOR);
    LCD_Show_Image_Internal_Flash(285,13,13,12,gImage_baifenbi,312);

    /*              channel icon                */
    channel_dis();
    temp_dis();
    PutChinese_12(57,290,"度",BACK_COLOR,POINT_COLOR);

    /*              capacity icon                */
    power_dis();

    LCD_Show_Image_Internal_Flash(182,113,35,35,gImage_baifenbi_big,2450);

    PutChinese_strings(150,162,"实时耗电",BACK_COLOR,POINT_COLOR);
    LCD_ShowChar(215,161,':',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowString(280,161,32,16,16,"kw/h",POINT_COLOR,BACK_COLOR);

    /*              Fan icon                */
    LCD_Show_Image_Internal_Flash(42,193,32,32,gImage_fan1,2048);

    fan1_dis();
    LCD_ShowNum(72,215,1,1,12,BROWN,BACK_COLOR);
    /*              LED icon                */
    LCD_Show_Image_Internal_Flash(137,193,32,32,gImage_fan1,2048);

    fan2_dis();
    LCD_ShowNum(168,215,2,1,12,BROWN,BACK_COLOR);
    /*              Mode icon                */
    LCD_Show_Image_Internal_Flash(230,193,42,31,gImage_mode,2604);
    mode_dis();

    /*              draw line                */
    LCD_DrawLine(1,40,320,40,GRAY);

    LCD_DrawLine(20,180,300,180,GRAY);
}

/**
 * @brief	界面信息-报警温度 跳动
 *
 * @param   void
 *
 * @return  void
 */
static void temp_alarm_flicker( void )
{
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(260,54,284,70,BACK_COLOR);
    }else
    {
        temp_alarm_dis();
    }
    
}

/**
 * @brief	界面信息-风扇1档位 跳动
 *
 * @param   void
 *
 * @return  void
 */
static void fan1_flicker( void )
{   
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(80,202,92,226,BACK_COLOR);
    }else
    {
        fan1_dis();
    }
}

/**
 * @brief	界面信息-风扇2档位 跳动
 *
 * @param   void
 *
 * @return  void
 */
static void fan2_flicker( void )
{   
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(177,202,189,226,BACK_COLOR);
    }else
    {
        fan2_dis();
    }
}

/**
 * @brief	界面信息-模式选择 跳动
 *
 * @param   void
 *
 * @return  void
 */
static void mode_flicker( void )
{   
    if( gui_flicker.clear_flag == true )
    {
        LCD_Fill(278,203,310,219,BACK_COLOR);
    }else
    {
        mode_dis();
    }
}

/**
 * @brief	界面信息-连接状态 显示
 *
 * @param   void
 *
 * @return  void
 */
void connect_dis( void )
{
    if( gui_info.connect_flag == true )
    {
        LCD_Show_Image_Internal_Flash(13,3,29,30,gImage_connect_on,1740);
    }else
    {
        LCD_Show_Image_Internal_Flash(13,3,30,30,gImage_connect_off,1800);
    }
}

/**
 * @brief	界面信息-同步状态 显示
 *
 * @param   void
 *
 * @return  void
 */
void sync_dis( void )
{
    if( gui_info.sync_switch == SYNC_ON )
    {
        LCD_Show_Image_Internal_Flash(55,3,30,30,gImage_sync_on,1800);
    }else
    {
        LCD_Fill(55,3,85,33,BACK_COLOR);
    }
}

/**
 * @brief	界面信息-温度报警开关状态 显示
 *
 * @param   void
 *
 * @return  void
 */
void alarm_icon_dis( void )
{
    if( gui_info.temp_value >= gui_info.temp_alarm_value )
    {
        LCD_Show_Image_Internal_Flash(97,3,29,29,gImage_temp_alarm_red,1682);
    }else
    {
        LCD_Fill(97,3,126,32,BACK_COLOR);
    }
}

/**
 * @brief	界面信息-温湿度 显示
 *
 * @param   void
 *
 * @return  void
 */
void dht11_dis( void )
{
    LCD_ShowNum(215,10,gui_info.envir_temp,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(257,10,gui_info.envir_humidity,3,16,POINT_COLOR,BACK_COLOR);
}

/**
 * @brief	界面信息-通道选择 显示
 *
 * @param   void
 *
 * @return  void
 */
void channel_dis( void )
{
    switch (gui_info.channel_num)
    {
        case CHANNLE_1: 
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_off,2024);

            break;

        case CHANNLE_2: 
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_off,2024);

            break;

        case CHANNLE_3:
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_on,2024);

            break;

        case CHANNLE_12: 
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_off,2024);

            break;
            
        case CHANNLE_123: 
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_on,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_on,2024);

            break;

    default:
            LCD_Show_Image_Internal_Flash(29,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(112,51,46,22,gImage_channel_off,2024);
            LCD_Show_Image_Internal_Flash(194,51,46,22,gImage_channel_off,2024);

            break;
    }
}

/**
 * @brief	界面信息-报警温度 显示
 *
 * @param   void
 *
 * @return  void
 */
void temp_alarm_dis( void )
{
    LCD_ShowNum(260,54,gui_info.temp_alarm_value,3,16,POINT_COLOR,BACK_COLOR);
}

/**
 * @brief	界面信息-温度 显示
 *
 * @param   void
 *
 * @return  void
 */
void temp_dis( void )
{
    LCD_ShowNum(260,54,gui_info.temp_value,3,16,POINT_COLOR,BACK_COLOR);
}


/**
 * @brief	界面信息-功率档位 显示
 *
 * @param   void
 *
 * @return  void
 */
void power_dis( void )
{
    LCD_ShowNum(110,101,gui_info.power_level,3,32,GREEN,BACK_COLOR);
}

/**
 * @brief	界面信息-功率 显示
 *
 * @param   void
 *
 * @return  void
 */
void capacity_dis( void )
{
    gui_info.capacity = ((gui_info.capacity_h * 4.0f) + ( gui_info.capacity_min *4.0f / 60.0f ));
    delay_ms(1);
    LCD_ShowxFloat(230,161,gui_info.capacity,1,16,BACK_COLOR,POINT_COLOR);
}

/**
 * @brief	界面信息-风扇1档位 显示
 *
 * @param   void
 *
 * @return  void
 */
void fan1_dis( void )
{
    LCD_ShowNum(80,202,gui_info.fan1_level,1,24,GREEN,BACK_COLOR);
}

/**
 * @brief	界面信息-风扇1档位 显示
 *
 * @param   void
 *
 * @return  void
 */
void fan2_dis( void )
{
    LCD_ShowNum(177,202,gui_info.fan2_level,1,24,GREEN,BACK_COLOR);
}

/**
 * @brief	界面信息-模式选择 显示
 *
 * @param   void
 *
 * @return  void
 */
void mode_dis( void )
{
    switch (gui_info.mode_num)
    {
        case MOED_1:
            PutChinese_16(203,278,"节",BACK_COLOR,GREEN);
            PutChinese_16(203,295,"能",BACK_COLOR,GREEN);

            break;

        case MOED_2:
            PutChinese_16(203,278,"普",BACK_COLOR,YELLOW);
            PutChinese_16(203,295,"通",BACK_COLOR,YELLOW);

            break;

        case MOED_3:
            PutChinese_16(203,278,"强",BACK_COLOR,BRRED);
            PutChinese_16(203,295,"劲",BACK_COLOR,BRRED);

            break;

        default:
            PutChinese_16(203,278,"普",BACK_COLOR,YELLOW);
            PutChinese_16(203,295,"通",BACK_COLOR,YELLOW);

            break;
    }
}

/**
 * @brief	界面信息-图标闪烁控制 
 *
 * @param   void
 *
 * @return  void
 */
void Icon_Flicker( void )
{
    if( gui_flicker.start_flag == 1 )
    {
        gui_flicker.clear_flag = !gui_flicker.clear_flag;

        switch (gui_flicker.selection)
        {
            case FAN1_ICON:     fan1_flicker();          break;

            case FAN2_ICON:     fan2_flicker();          break;

            case MODE_ICON:     mode_flicker();          break;

            case TEMP_ALARM:    temp_alarm_flicker();    break;

            default:                                     break;
        }
    }

    gui_flicker.start_flag = 0; 
}

/**
 * @brief	界面信息-更新（04寄存器）
 *
 * @param   void
 *
 * @return  void
 */
void gui_info_update( void )
{
    if( gui_flicker.enable_flag == false )
    {
        if(( modbus.modbus_04_scan_flag == true ) && ( modbus.modbus_04_scan_allow == true ))
        {
            /*      1.读取从机04寄存器信息       */
            read_slave_04();

            /*      2.刷新屏幕       */
            gui_info_refresh();    
            modbus.modbus_04_scan_flag = 0;
        }
    }  
}

/**
 * @brief	读取从机04寄存器后刷新屏幕显示
 *
 * @param   void
 *
 * @return  void
 */
void gui_info_refresh( void )
{
    temp_dis();
    alarm_icon_dis();
    dht11_dis();
    capacity_dis();    
}




