#include "zf_common_headfile.h"


void System()
{

    gpio_init(MOTOR1_A, GPO, GPIO_HIGH, GPO_PUSH_PULL);// 新方案双电机初始化
    pwm_init(MOTOR1_B, 17000, 0);
    gpio_init(MOTOR2_A, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    pwm_init(MOTOR2_B, 17000, 0);

    pwm_init(ATOM1_CH1_P33_9,50,SteerMiddle); // 舵机初始化
    //编码器初始化
    encoder_quad_init(ENCODER1_TIM,ENCOEDER1_CH1,ENCOEDER1_CH2);
    encoder_quad_init(ENCODER2_TIM,ENCOEDER2_CH1,ENCOEDER2_CH2);

    dl1b_init();
    dl1b2_init();
    ips200_init(IPS200_TYPE_PARALLEL8);
    mt9v03x_init();

    key_init(10);
    gpio_init(P33_13, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(P33_10, GPO, GPIO_LOW, GPO_PUSH_PULL);
//    gpio_init(P20_9, GPO, GPIO_HIGH, GPO_PUSH_PULL);
//    gpio_init(P20_8, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    if(gpio_get_level(P33_12))    Parameter_Read();//读取存储的参数

    /*灯*/
    gpio_init(P32_4, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(P20_10, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(P11_3, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(P11_2, GPO, GPIO_LOW, GPO_PUSH_PULL);//原本20.10
    gpio_init(P15_7, GPO, GPIO_LOW, GPO_PUSH_PULL);//原本为20.7
    gpio_init(P23_1, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(P11_6, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(P20_7, GPO, GPIO_LOW, GPO_PUSH_PULL);

    Flag_stop=1;
    Flag_turn=3;


    Light_Flag=15;
    system_delay_ms(100);
    ///////////////开局亮灯///////////////////////
//    gpio_set_level(P32_4,1);//左前转向
//    gpio_set_level(P20_10,1);//左后转向
//    gpio_set_level(P11_3,1);//右前转向
//    gpio_set_level(P11_2,1);//右后转向
//    gpio_set_level(P15_7,1);//左前大灯
//    gpio_set_level(P23_1,1);//右前大灯
//    gpio_set_level(P11_6,1);//左后刹灯
//    gpio_set_level(P20_7,1);//右后刹灯

    ////////////感应发车/////////////////////////

    pit_ms_init(CCU60_CH0, 5);
    pit_ms_init(CCU60_CH1, 5);
    pit_ms_init(CCU61_CH0, 10);
    pit_ms_init(CCU61_CH1, 10);
}



