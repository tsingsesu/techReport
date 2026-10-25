#include "zf_common_headfile.h"
#include "System.h"
       /****************************    舵机控制变量    ****************************/
uint8    Point_Mid = MT9V03X_W/2;
uint8    Point_Left = 43;//43
uint8    Point_Right = 37;//37
int32    SD5_Duty =  SteerMiddle;
uint8    showpoint[2] = {0};
uint16   Speed_max = 150,Speed_min = 20;
uint16   Pwm_Max = 915,Pwm_Min = 785;

uint8     Line_Point;    // 目标点 纵坐标
uint8     Point;                         //目标点横坐标
char      Point_Error;
float     Mid_Ka = 0, Mid_Kb = 0;   // 拟合的赛道中线的斜率
uint8     Fictitious_Midline[61] ;  // 虚拟赛道中线


int32 AD_Error;
/***************************  电机控制变量 *******************************/
int32   RSpeedPWM,LSpeedPWM;
uint8   Distance_ON = 0;         //默认关闭距离记录 用于行车过程的记录
double  Distance;                //距离保存变量，勿动
uint8   Distance_Test_ON = 0;  
double  Distance_Test;                //距离保存变量，勿动

uint8   Stop = 0;                 //停车
uint8   StartStopLine = 0;
int16   speedshow[4]={0};

/***************************** 赛道识别 ************************************/
char   Track_Line_Num=0,Track_Line_Time=0;
int16  Fictitious_Beeline_Num=0;
char   Last_Error[5];
int16  Max_Speed=0,Min_Speed=0;
uint8  Foresight;
uint8  Set_Speed;        //速度挡位  超过8为全程匀速


// 舵机控制
void S3010_Control(void)
{
    Point_Error = ABS(Point-Point_Mid);   //用于赛道判断
    if(Flag_turn==1)//左转
    {
        SD5_Duty=908;//904
    }
    else if(Flag_turn==2)//右转
    {
        SD5_Duty=794;//799
    }
    else if(Flag_turn==3)//中间
    {
        SD5_Duty=854;
    }
    else if(Flag_turn==4)//da左转
    {
        SD5_Duty=920;
    }
    else if(Flag_turn==5)//微偏右转
    {
        SD5_Duty=856;
    }
    else if(Flag_turn==6)//微偏右转
        {
            SD5_Duty=787;
        }
    else if(Flag_turn==8)//微偏右转
        {
            SD5_Duty=908;
        }
    else
    {
        SD5_Duty = PlacePID_Control(&Point_PID,Point_Mid,Point);   // 舵机PID 得到当前的舵机占空比

//        if(SD5_Duty<=852)
//        {
//            SD5_Duty= 2*SD5_Duty-852;
//        }
        SD5_Duty = range_protect(SD5_Duty, Pwm_Min, Pwm_Max);  //舵机占空比限幅保护

    }
    SteerSet(SD5_Duty);     //修改舵机占空比
}

void Motor_Control(void)
{
//    编码器;
    Rcount = -encoder_get_count(ENCODER1_TIM); //这里需要注意第二个参数务必填写A相引脚
    Lcount = encoder_get_count(ENCODER2_TIM);

    encoder_clear_count(ENCODER2_TIM);
    encoder_clear_count(ENCODER1_TIM);

//     Distance_ON=1;
    if(Distance_ON)                    //开启距离累加
    {
        Distance += (DISTANCE( +(Rcount+Lcount)/2) );
    }
   // else  Distance = 0;

    
    if(Distance_Test_ON)                    //开启距离累加
    {
        Distance_Test += (DISTANCE(Rcount+Lcount)/2 );
    }
    //else  Distance_Test = 0;
    
    Rrealspeed  = Rcount;
    Lrealspeed  =  Lcount;

    printf("%d,%d",Rrealspeed,RMotorSpeed);


    RSpeedPWM = RMotorPid_Col(RMotorSpeed);
    LSpeedPWM = LMotorPid_Col(LMotorSpeed);
//    LSpeedPWM = -2000;
//    RSpeedPWM=-2000;
    if(LSpeedPWM >= 0)
    {
        gpio_set_level(MOTOR2_A,1);
        pwm_set_duty(MOTOR2_B, LSpeedPWM);
    }
    else
    {
        gpio_set_level(MOTOR2_A,0);
        pwm_set_duty(MOTOR2_B, - LSpeedPWM);
    }

    if(RSpeedPWM >= 0)                         //新方案双电机控制
    {
        gpio_set_level(MOTOR1_A,1);
        pwm_set_duty(MOTOR1_B, RSpeedPWM);
    }
    else
    {
        gpio_set_level(MOTOR1_A,0);
        pwm_set_duty(MOTOR1_B, - RSpeedPWM);
    }
}

/*
*   计算终点位置
*
*/
void Point_Control(void)
{
    float res;
    int32 Sum = 0;
//    if(Deal_Flag > 20 && !Dis_Flag)
//    {
//        Line_Point = 45;
//    }
//    else
        Line_Point = 20;//识别远近

    Least_Sqaure_Method(&Mid_Ka,&Mid_Kb,58, Line_Count+2); // 拟合中线斜率
    //Least_Sqaure_Method(&Mid_Ka,&Mid_Kb,58, Line_Count+2); // 拟合中线斜率
    Fictitious_Beeline_Num = Fictitious_Beeline(Mid_Ka,Mid_Kb);  // 拟合中线  返回相似度
    res = Line_Point * Mid_Ka + Mid_Kb;
    Point = range_protect((int32)res, 2, 118);

}


void Control_Speed(void)
{
    LMotorSpeed = RMotorSpeed= 210;//360
    if(Flag_stop==1)//停车
    {
        LMotorSpeed = RMotorSpeed = 0;
    }
    else if(Flag_stop==2)//左转
    {
        LMotorSpeed = 135;
        RMotorSpeed = 135;
    }
    else if(Flag_stop==3)//右转
    {
         RMotorSpeed =135;
         LMotorSpeed =135;
    }
    else if(Flag_stop==4)//加速
    {
        LMotorSpeed = RMotorSpeed =135;
    }
    else if(Flag_stop==5)//低速
    {
        LMotorSpeed = RMotorSpeed =135;
    }
    else if(Flag_stop==6)//（中速）
    {
        LMotorSpeed = RMotorSpeed =175;
    }
    else if(Flag_stop==7)//（转弯前减速）
        {
            LMotorSpeed = RMotorSpeed =135;
        }
    else if(Flag_stop==8)//（转弯前减速）
            {
                LMotorSpeed = RMotorSpeed =85;
            }

}



//最小二乘法求散点拟合函数y=a+bx的参数a和b
void Least_Sqaure_Method(float *Ka, float *Kb,uint8 start, uint8 end)
{
    uint8 i;
    double Sum1=0,Sum2=0;
    double Mean_i=0,Mean_y=0;
    //计算平均值
    for(i=start;i>end;i--)
    {
        Sum1 += i;
        Sum2 += Mid_Line[i];
    }
    Mean_i = 1.0*Sum1/(start-end);
    Mean_y = 1.0*Sum2/(start-end);
    //计算Ka
    Sum1=Sum2=0;
    for(i=start;i>end;i--)
    {
        Sum1+=1.0*(i-Mean_i)*(Mid_Line[i]-Mean_y);
        Sum2+=1.0*(i-Mean_i)*(i-Mean_i);
    }
    *Ka=Sum1/Sum2;
    //计算Kb
    *Kb=Mean_y - (*Ka)*Mean_i;
}

/*
*   虚拟直线
*
*   说明：构建虚拟直线，赛道中线与之比较，返回相似度
*/
int16 Fictitious_Beeline(float Ka, float Kb)
{
    uint8 i;
    int16 Sum=0;
    int16 Num;
    float res;
    for(i=59;i>5;i-=1)
    {
        res = i * Ka + Kb;
        Fictitious_Midline[i] = range_protect((int32)res, 1, COL-1);
        if (Mid_Line[i])
        {
            Num = Mid_Line[i]- Fictitious_Midline[i];
            Num = ABS(Num);
            Sum+=Num;
        }
    }
    return Sum;
}

void Light(void)
{
    switch(Light_Flag)
    {
        case 0://刹车灯熄
        {
            gpio_set_level(P32_4,0);//左前转向
            gpio_set_level(P20_10,0);//左后转向
            gpio_set_level(P11_3,0);//右前转向
            gpio_set_level(P11_2,0);//右后转向
            gpio_set_level(P15_7,0);//左前大灯
            gpio_set_level(P23_1,0);//右前大灯
            gpio_set_level(P11_6,0);//左后刹灯
            gpio_set_level(P20_7,0);//右后刹灯
            break;
        }
        case 1://刹车灯亮
        {
            gpio_set_level(P11_6,1);//左后刹灯
                gpio_set_level(P20_7,1);//右后刹灯
            break;
        }
        case 2://左转向灯
        {
            if(Distance<40)
            {
                gpio_set_level(P32_4,1);//左前转向
                   gpio_set_level(P20_10,1);//左后转向
            }
            if(Distance>39&&Distance<80)
            {
                gpio_set_level(P32_4,0);//左前转向
                   gpio_set_level(P20_10,0);//左后转向
            }
            if(Distance>79&&Distance<120)
            {
                gpio_set_level(P32_4,1);//左前转向
                   gpio_set_level(P20_10,1);//左后转向
            }
            if(Distance>120)
            {
                gpio_set_level(P32_4,0);//左前转向
                   gpio_set_level(P20_10,0);//左后转向
            }
            break;
         }
        case 3://右转向灯
        {
            if(Distance<40)
            {
                gpio_set_level(P11_3,1);//右前转向
                    gpio_set_level(P11_2,1);//右后转向
            }
            if(Distance>39&&Distance<80)
            {
                gpio_set_level(P11_3,0);//右前转向
                    gpio_set_level(P11_2,0);//右后转向
            }
            if(Distance>79&&Distance<120)
            {
                gpio_set_level(P11_3,1);//右前转向
                    gpio_set_level(P11_2,1);//右后转向
            }
            if(Distance>119)
            {
                gpio_set_level(P11_3,0);//右前转向
                    gpio_set_level(P11_2,0);//右后转向
            }
            break;
        }
        case 4://会车灯
        {
            if(Distance_Test >50 && Distance_Test < 100)
            {
                gpio_set_level(P15_7,1);//左前大灯
                    gpio_set_level(P23_1,1);//右前大灯
            }
            else if(Distance_Test >100 && Distance_Test < 120)
            {
                gpio_set_level(P15_7,0);//左前大灯
                    gpio_set_level(P23_1,0);//右前大灯
            }
            if(Distance_Test >120 && Distance_Test < 170)
            {
                gpio_set_level(P15_7,1);//左前大灯
                    gpio_set_level(P23_1,1);//右前大灯
            }
            else if(Distance_Test >170 && Distance_Test < 200)
            {
                gpio_set_level(P15_7,0);//左前大灯
                    gpio_set_level(P23_1,0);//右前大灯
            }
            break;
        }

        case 10://刹车灯熄
        {
            gpio_set_level(P11_6,0);//左后刹灯
            gpio_set_level(P20_7,0);//右后刹灯
            break;
        }
        default:break;
    }
//    if(Light_Flag==3)//刹车灯亮
//    {
//        gpio_set_level(P20_7,1);
//        gpio_set_level(P11_6,1);
//    }
//    if(Light_Flag==4)//刹车灯灭
//    {
//        gpio_set_level(P20_7,0);
//        gpio_set_level(P11_6,0);
//    }
//    if(Flag_stop==2 || Flag_stop==6)
//    {
//        if(Light_Flag==1)//左转
//        {
//            if(Distance<10)
//            {
//                gpio_set_level(P21_3,1);
////                gpio_set_level(P11_2,1);
//            }
//            if(Distance>11&&Distance<17)
//            {
//                gpio_set_level(P21_3,0);
//             //   gpio_set_level(P11_2,0);
//            }
//            if(Distance>18&&Distance<25)
//            {
//                gpio_set_level(P21_3,1);
//               // gpio_set_level(P11_2,1);
//            }
//            if(Distance>26)
//            {
//                gpio_set_level(P21_3,0);
//               // gpio_set_level(P11_2,0);
//            }
//        }
//    }
//    if(Light_Flag==5)
//    {
//        gpio_set_level(P20_10,0);
//        gpio_set_level(P11_2,0);
//    }
//    if(Flag_stop==3)
//    {
//        if(Light_Flag==2)//右转
//        {
//            if(Distance<10)
//            {
//                gpio_set_level(P21_5,1);
//               // gpio_set_level(P21_5,1);
//            }
//            if(Distance>11&&Distance<18)
//            {
//                gpio_set_level(P21_5,0);
//               // gpio_set_level(P11_3,0);
//            }
//            if(Distance>20&&Distance<27)
//            {
//                gpio_set_level(P21_5,1);
//               // gpio_set_level(P32_4,1);
//            }
//            if(Distance>28)
//            {
//                gpio_set_level(P21_5,0);
//                //gpio_set_level(P11_3,0);
//            }
//        }
//    }
//    if(Light_Flag==6)
//    {
//        gpio_set_level(P32_4,0);
//        gpio_set_level(P11_3,0);
//    }
//    if(Light_Flag == 7)
//    {
//        if(Distance_Test >50 && Distance_Test < 100)
//        {
//            gpio_set_level(P23_1, 1);           //前右白色
//            gpio_set_level(P15_7, 1);           //前左白色
//        }
//        else if(Distance_Test >100 && Distance_Test < 120)
//        {
//            gpio_set_level(P23_1, 0);           //前右白色
//            gpio_set_level(P15_7, 0);           //前左白色
//        }
//        if(Distance_Test >120 && Distance_Test < 170)
//        {
//            gpio_set_level(P23_1, 1);           //前右白色
//            gpio_set_level(P15_7, 1);           //前左白色
//        }
//        else if(Distance_Test >170 && Distance_Test < 200)
//        {
//            gpio_set_level(P23_1, 0);           //前右白色
//            gpio_set_level(P15_7, 0);           //前左白色
//        }
//    }
}
