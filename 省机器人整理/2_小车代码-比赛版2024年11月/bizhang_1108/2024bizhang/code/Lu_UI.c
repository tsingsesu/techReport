/*
 * Lu_UI.c
 *
 *  Created on: 2023年3月9日
 *      Author: 86185
 */
/*
 * 2023.6.28第 1 版调参UI——lujunjie
 */
#include "Lu_UI.h"
#include "zf_common_headfile.h"
int8 Page_Choose_flag = 0;
int8 Parameter_Choose_flag = 0;
uint8 Page_Clear_flag = 0;
uint8 Key_Protect[KEY_NUMBER] = {0};
uint16 Show_Y[]={0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300};
uint16 Show_X[]={0,5,150};
int16 canshu_test[8] = {0};
int8 Differ;

/*************************************************************************
 *
 *  功能说明：屏幕显示页
 *
 *************************************************************************/
void LuUI_Show_page1(void)
{
    if(mt9v03x_finish_flag)       //总钻风图像
        {
            ips200_show_gray_image(0, 0,*imagebinary, MT9V03X_W, MT9V03X_H, 240, 140, zyuzhi);     // 显示二值化图像
            DisplayLine(Left_Add_Line, RGB565_RED);
            DisplayLine(Right_Add_Line, RGB565_BLUE);
            for(uint8 i = 59;i>5;i--)
            {
               ips200_drawpoint1(2*Fictitious_Midline[i], 2*i,RGB565_YELLOW);
            }
            for(uint8 i=2;i<238;i++)
            {
                ips200_draw_point(i,5*2,RGB565_PURPLE);
            }
            for(uint8 i=2;i<238;i++)
            {
                ips200_draw_point(i,30*2,RGB565_PURPLE);
            }
            for(uint8 j = 2; j < 138; j++)
            {
                ips200_draw_point(56*2,j,RGB565_PURPLE);
            }
            for(uint8 j = 2; j < 138; j++)
            {
                ips200_draw_point(64*2,j,RGB565_PURPLE);
            }
            mt9v03x_finish_flag=0;
        }

    ips200_show_int(Show_X[1],Show_Y[7],dl1b_distance_mm,4);
    ips200_show_int(Show_X[2],Show_Y[7],dl1b2_distance_mm,4);

    ips200_show_float(Show_X[1],Show_Y[8],Flag_turn,4,4);
    ips200_show_float(Show_X[2],Show_Y[8],Flag_stop,4,4);

    ips200_show_int(Show_X[1],Show_Y[9],Rhombus_Flag,4);
    ips200_show_int(Show_X[2],Show_Y[9],Deal_Flag,4);

    ips200_show_int(Show_X[1],Show_Y[10],Distance_Test,4);
    ips200_show_int(Show_X[2],Show_Y[10],Distance,4);

    ips200_show_int(Show_X[1],Show_Y[11],Rhombus_Out_Flag,4);
    ips200_show_int(Show_X[2],Show_Y[11],Rhombus_Out,4);

    ips200_show_int(Show_X[1],Show_Y[12],time_dl1b_s,4);
    ips200_show_int(Show_X[2],Show_Y[12],time_dl1b_s,4);

    ips200_show_int(Show_X[1],Show_Y[13],Distance,4);
    ips200_show_int(Show_X[2],Show_Y[13],SD5_Duty,4);
//
//    ips200_show_float(Show_X[1],Show_Y[14],KR,4,4);
//    ips200_show_float(Show_X[2],Show_Y[14],KL,4,4);
//
//
 //   ips200_show_int(Show_X[1],Show_Y[15],Stop_Flag,4);
//    ips200_show_int(Show_X[2],Show_Y[15],Turnoff_Flag,4);
}
void LuUI_Show_page2(void)
{
        ips200_show_int(Show_X[1],Show_Y[7],Lcount,4);
        ips200_show_int(Show_X[2],Show_Y[7],Rcount,4);

        ips200_show_int(Show_X[1],Show_Y[9],LMotorSpeed,4);
        ips200_show_int(Show_X[2],Show_Y[9],RMotorSpeed,4);
//
        ips200_show_int(Show_X[1],Show_Y[11],Lrealspeed,4);
        ips200_show_int(Show_X[2],Show_Y[11],Rrealspeed,4);
//
        ips200_show_int(Show_X[1],Show_Y[14],LSpeedPWM,4);
        ips200_show_int(Show_X[2],Show_Y[14],RSpeedPWM,4);
//
//    ips200_show_string_color(Show_X[1],Show_Y[4],"InductorValue[3]",RGB565_BLACK,RGB565_YELLOW);
//    ips200_show_int(Show_X[2],Show_Y[4],InductorValue_Read[3],4);
//
//    ips200_show_string_color(Show_X[1],Show_Y[5],"Inductor_Error_1_4",RGB565_BLACK,RGB565_YELLOW);
//    ips200_show_int(Show_X[2],Show_Y[5],Inductor_Error_1_4,4);
//
//    ips200_show_int(Show_X[1],Show_Y[6],ad_max[3],4);
//    ips200_show_int(Show_X[2],Show_Y[6],ad_max[0],4);
//
//    ips200_show_string_color(Show_X[1],Show_Y[7],"Use_Inductor_Flag",RGB565_BLACK,RGB565_YELLOW);
//    ips200_show_int(Show_X[2],Show_Y[7],Use_Inductor_Flag,4);
//    //
//    //    ips200_show_string_color(Show_X[1],Show_Y[7],"Current_Speed_L",RGB565_BLACK,RGB565_YELLOW);
//    //    ips200_show_int(Show_X[2],Show_Y[7],Current_Speed_L,4);
//    ips200_show_string_color(Show_X[1],Show_Y[11],"Roadblock_Flag",RGB565_BLACK,RGB565_YELLOW);
//    ips200_show_int(Show_X[2],Show_Y[11],Roadblock_Flag,4);
//
//    ips200_show_string_color(Show_X[1],Show_Y[12],"Roadblock_Count",RGB565_BLACK,RGB565_YELLOW);
//    ips200_show_int(Show_X[2],Show_Y[12],Roadblock_Count,4);
//
//    ips200_show_string_color(Show_X[1],Show_Y[13],"Ramp_Flag",RGB565_BLACK,RGB565_YELLOW);
//    ips200_show_int(Show_X[2],Show_Y[13],Ramp_Flag,4);
//
//    ips200_show_string_color(Show_X[1],Show_Y[14],"Flag1",RGB565_BLACK,RGB565_YELLOW);
//    ips200_show_int(Show_X[2],Show_Y[14],Flag1,4);
//
//    ips200_show_string_color(Show_X[1],Show_Y[15],"Ring_Count",RGB565_BLACK,RGB565_YELLOW);
//    ips200_show_int(Show_X[2],Show_Y[15],Ring_Count,4);
}
void LuUI_Show_page3(void)
{
    ips200_show_string_color(Show_X[1],Show_Y[1],"ZYuzhi",RGB565_BLACK,RGB565_GREEN);
    ips200_show_float(Show_X[2],Show_Y[1],zyuzhi,3,3);

    ips200_show_string_color(Show_X[1],Show_Y[2],"LMotorSpeed",RGB565_BLACK,RGB565_GREEN);
    ips200_show_float(Show_X[2],Show_Y[2],LMotorSpeed,3,3);

    ips200_show_string_color(Show_X[1],Show_Y[3],"Parameter_Choose_flag",RGB565_BLACK,RGB565_GREEN);
    ips200_show_float(Show_X[2],Show_Y[3],Parameter_Choose_flag,3,3);

    ips200_show_string_color(Show_X[1],Show_Y[4],"Differ",RGB565_BLACK,RGB565_GREEN);
    ips200_show_float(Show_X[2],Show_Y[4],Differ,3,3);
//
//    ips200_show_string_color(Show_X[1],Show_Y[4],"SD5_Duty",RGB565_BLACK,RGB565_GREEN);
//    ips200_show_float(Show_X[2],Show_Y[4],SD5_Duty,3,3);
//
//    ips200_show_string_color(Show_X[1],Show_Y[5],"Inductor_speed",RGB565_BLACK,RGB565_GREEN);
//    ips200_show_float(Show_X[2],Show_Y[5],Inductor_speed,3,3);
//
//    ips200_show_string_color(Show_X[1],Show_Y[6],"L_In_Speed",RGB565_BLACK,RGB565_GREEN);
//    ips200_show_float(Show_X[2],Show_Y[6],L_In_Speed,3,3);
//
//    ips200_show_string_color(Show_X[1],Show_Y[7],"R_In_Speed",RGB565_BLACK,RGB565_GREEN);
//    ips200_show_float(Show_X[2],Show_Y[7],R_In_Speed,3,3);


    ips200_show_char(Show_X[0],Show_Y[Parameter_Choose_flag],'*');
}
void LuUI_Show(void)
{
    if(Page_Clear_flag){ ips200_clear(); Page_Clear_flag = 0; }
    switch(Page_Choose_flag)
        {
            case 0:{ LuUI_Show_page1(); break; }
            case 1:{ LuUI_Show_page2(); break; }
            case 2:{ LuUI_Show_page3(); break; }
            default:break;
        };
}
void Computer_Show(void)
{


}
void LuUI_Key(void)
{
    key_scanner();
    if(key_get_state(KEY_MID)==KEY_SHORT_PRESS && Key_Protect[KEY_MID]==0)
    {
        Page_Choose_flag ++;
        Page_Choose_flag = (Page_Choose_flag < PAGES) ? Page_Choose_flag : 0;
        Page_Clear_flag = 1;
        Key_Protect[KEY_MID] = 1;
    }
    else if(key_get_state(KEY_MID)==KEY_RELEASE)
    {
        Key_Protect[KEY_MID] = 0;
    }
    if(key_get_state(KEY_DOWN)==KEY_SHORT_PRESS && Key_Protect[KEY_DOWN]==0)
    {
        if(Page_Choose_flag == 2)
        {
            Parameter_Choose_flag --;
            Parameter_Choose_flag = (Parameter_Choose_flag > 0) ? Parameter_Choose_flag : PARAMETERS;
            Page_Clear_flag = 1;
            Key_Protect[KEY_DOWN] = 1;
        }
    }
    else if(key_get_state(KEY_DOWN)== KEY_RELEASE)
    {
        Key_Protect[KEY_DOWN] = 0;
    }
    if(key_get_state(KEY_UP)==KEY_SHORT_PRESS && Key_Protect[KEY_UP]==0)
    {
        if(Page_Choose_flag == 2)
        {
            Parameter_Choose_flag ++;
            Parameter_Choose_flag = (Parameter_Choose_flag < PARAMETERS+1) ? Parameter_Choose_flag : 0;
            Page_Clear_flag = 1;
            Key_Protect[KEY_UP] = 1;
        }
    }
    else if(key_get_state(KEY_UP)== KEY_RELEASE)
    {
        Key_Protect[KEY_UP] = 0;
    }
    if(key_get_state(KEY_LEFT)==KEY_SHORT_PRESS && Key_Protect[KEY_LEFT]==0)
    {
        if(Page_Choose_flag == 2)
        {
            switch(Parameter_Choose_flag)
            {
                case 1:{zyuzhi +=2 ; break;}
                case 4:{Differ+=1; break;}
//                case 3:{ +=2 ; break;}
//                case 4:{ +=2; break;}
//                case 5:{ +=2 ; break;}
//                case 6:{ +=2; break;}
//                case 7:{ +=2; break;}
                default:break;
            }
            Key_Protect[KEY_LEFT] = 1;
            Parameter_Save();
        }
    }
    else if(key_get_state(KEY_LEFT)== KEY_RELEASE)
    {
        Key_Protect[KEY_LEFT] = 0;
    }
    if(key_get_state(KEY_RIGHT)==KEY_SHORT_PRESS && Key_Protect[KEY_RIGHT]==0)
    {
        if(Page_Choose_flag == 2)
        {

            switch(Parameter_Choose_flag)
            {
                case 1:{ zyuzhi-=2 ; break;}
                case 4:{ Differ-=1; break;}
//                case 3:{ -=2 ; break;}
//                case 4:{ -=2; break;}
//                case 5:{ -=2 ; break;}
//                case 6:{ -=2; break;}
//                case 7:{ -=2; break;}
                default:break;
            }
            Key_Protect[KEY_RIGHT] = 1;
            Parameter_Save();
        }
    }
    else if(key_get_state(KEY_RIGHT)== KEY_RELEASE)
    {
        Key_Protect[KEY_RIGHT] = 0;
    }
}
void Parameter_Save(void)
{
    if(flash_check(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX))
        flash_erase_page(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
    flash_buffer_clear();
        flash_union_buffer[0].float_type = zyuzhi;
        flash_union_buffer[1].float_type = LMotorSpeed;
        flash_union_buffer[2].float_type = Differ;
    //    flash_union_buffer[2].float_type = ;
    //    flash_union_buffer[3].float_type = ;
    //    flash_union_buffer[4].float_type = ;
    //    flash_union_buffer[5].float_type = ;
    //    flash_union_buffer[6].float_type = ;
    //    flash_union_buffer[7].float_type = ;
    //    flash_union_buffer[8].float_type = ;
    //    flash_union_buffer[9].float_type = ;
    //    flash_union_buffer[10].float_type = ;
    //    flash_union_buffer[11].float_type = ;
    //    flash_union_buffer[12].float_type = ;
    //    flash_union_buffer[13].float_type = ;
    //    flash_union_buffer[14].float_type = ;
    flash_write_page_from_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
}
void Parameter_Read(void)
{
    flash_buffer_clear();
    flash_read_page_to_buffer(FLASH_SECTION_INDEX, FLASH_PAGE_INDEX);
    zyuzhi = flash_union_buffer[0].float_type;
    LMotorSpeed = flash_union_buffer[1].float_type;
    Differ = flash_union_buffer[2].float_type;
    //    =flash_union_buffer[2].float_type;
    //    =flash_union_buffer[3].float_type;
    //    =flash_union_buffer[4].float_type;
    //    =flash_union_buffer[5].float_type;
    //    =flash_union_buffer[6].float_type;
    //    =flash_union_buffer[7].float_type;
    //    =flash_union_buffer[8].float_type;
    //    =flash_union_buffer[9].float_type;
    //    =flash_union_buffer[10].float_type;
    //    =flash_union_buffer[11].float_type;
    //    =flash_union_buffer[12].float_type;
    //    =flash_union_buffer[13].float_type;
    //    =flash_union_buffer[14].float_type;
}


/*
 *  @brief      显示线的函数
 */
void DisplayLine(uint16 *p, uint16 color)
{
    uint8 i;
    uint16 x;
        for( i = 60; i >= Line_Count; i-=1)                         //显示线
        {
             x = p[i];
             ips200_drawpoint1(2*x, 2*i,color);
        }
}
