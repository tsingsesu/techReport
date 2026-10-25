/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu0_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

int core0_main(void)
{
   system_delay_ms(1000);
   clock_init();                   // 获取时钟频率<务必保留>
   debug_init();                   // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等
    System();
    System();
    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while(TRUE)
    {
        LuUI_Key();
        LuUI_Show();
        //////感应发车/////////////

        if(Stop_1==0)
        {


            if(dl1b_distance_mm<201)
            {
                time_dl1b_on=1;
            }
            if(dl1b_distance_mm<400&&dl1b_distance_mm>200)
            {

                Differ_light-=3;
                while(dl1b_distance_mm<=400);
            }
            if(dl1b2_distance_mm<600&&dl1b2_distance_mm>400)
            {

                Differ_light+=2;
                while(dl1b2_distance_mm<=600);
            }
            if(dl1b2_distance_mm<151)
            {
                System();
            }
            if(dl1b2_distance_mm>150&&dl1b2_distance_mm<400)
            {
                tiaoguang=1;
                time_dl1b_on=1;
            }
            if(tiaoguang==1&&time_dl1b_s>300)
            {
                time_dl1b_on=0;
                time_dl1b_s=0;
                time_dl1b=0;
                tiaoguang=0;
                Differ_light+=8;
            }
            gpio_set_level(P32_4,1);//左前转向
            gpio_set_level(P20_10,1);//左后转向
            gpio_set_level(P11_3,1);//右前转向
            gpio_set_level(P11_2,1);//右后转向
            gpio_set_level(P15_7,1);//左前大灯
            gpio_set_level(P23_1,1);//右前大灯
            gpio_set_level(P11_6,1);//左后刹灯
            gpio_set_level(P20_7,1);//右后刹灯
            if(dl1b_distance_mm<400)
            {
                time_dl1b_on=1;

            }

            if(time_dl1b_s>500)
            {
                if(time_dl1b>200)
                {

                    gpio_set_level(P32_4,0);//左前转向
                    gpio_set_level(P20_10,0);//左后转向
                    gpio_set_level(P11_3,0);//右前转向
                    gpio_set_level(P11_2,0);//右后转向
                    gpio_set_level(P15_7,0);//左前大灯
                    gpio_set_level(P23_1,0);//右前大灯
                    gpio_set_level(P11_6,0);//左后刹灯
                    gpio_set_level(P20_7,0);//右后刹灯
                    Flag_turn=3;
                    Flag_stop=0;
                    Deal_Flag=0;
                    mode_2024=2;

                }
                else
                {

                    gpio_set_level(P32_4,0);//左前转向
                    gpio_set_level(P20_10,0);//左后转向
                    gpio_set_level(P11_3,0);//右前转向
                    gpio_set_level(P11_2,0);//右后转向
                    gpio_set_level(P15_7,0);//左前大灯
                    gpio_set_level(P23_1,0);//右前大灯
                    gpio_set_level(P11_6,0);//左后刹灯
                    gpio_set_level(P20_7,0);//右后刹灯
                    Flag_turn=3;
                    Flag_stop=0;
                    Deal_Flag=0;
                    mode_2024=1;
                }

                time_dl1b_s=0;
                time_dl1b=0;
                time_dl1b_on=0;

                Stop_1=1;
            }
        }


    }
}

#pragma section all restore
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：LED 不闪烁
//      如果使用主板测试，主板必须要用电池供电
//      查看程序是否正常烧录，是否下载报错，确认正常按下复位按键
//      万用表测量对应 LED 引脚电压是否变化，如果不变化证明程序未运行，如果变化证明 LED 灯珠损坏
// 问题2：SWITCH1 / SWITCH2 更改组合流水灯频率无变化
//      如果使用主板测试，主板必须要用电池供电
//      查看程序是否正常烧录，是否下载报错，确认正常按下复位按键
//      万用表测量对应 LED 引脚电压是否变化，如果不变化证明程序未运行，如果变化证明 LED 灯珠损坏
//      万用表检查对应 SWITCH1 / SWITCH2 引脚电压是否正常变化，是否跟接入信号不符，引脚是否接错
// 问题2：KEY1 / KEY2 / KEY3 / KEY4 接GND或者按键按下无变化
//      如果使用主板测试，主板必须要用电池供电
//      查看程序是否正常烧录，是否下载报错，确认正常按下复位按键
//      万用表测量对应 LED 引脚电压是否变化，如果不变化证明程序未运行，如果变化证明 LED 灯珠损坏
//      万用表检查对应 KEY1 / KEY2 / KEY3 / KEY4 引脚电压是否正常变化，是否跟接入信号不符，引脚是否接错

