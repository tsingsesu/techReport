#include "zf_common_headfile.h"
#include "System.h"
       /****************************    舵机控制变量    ****************************/
uint8    Point_Mid = MT9V03X_W/2;
uint8    Point_Left = 43;//43
uint8    Point_Right = 37;//37
int32    SD5_Duty =  SteerMiddle;
uint8    showpoint[2] = {0};
uint16   Speed_max = 150,Speed_min = 20;
uint16   Pwm_Max = 925,Pwm_Min = 785;

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
/* ============ [2026-10-25] 项目一：超车与会车 ============
   规则：
     1) 超车：对同向龟行车实施超车，并在中线为虚线时回线
        转向灯未闪 扣5，超过 2 米未回线 扣10，压线/刮蹭 扣10
     2) 会车：距离对向车 2 米内前大灯闪 2~3 次；会车后转向左道

   实现：摄像头统计两条边线之间【左半/右半车道】的白点占比（见 Car_Scan），
         自己那一半占比高 = 前方有车；对向那一半占比高 = 对向来车。

   ★★ OVERTAKE_ENABLE 默认 0 ★★
      保持 0 时本模块完全不动舵机、不动灯，原有行为不变。
      调好 Car_Scan 的阈值（看屏幕 car_pct_l / car_pct_r）之后再改 1。
*/
#define OVERTAKE_ENABLE     (0)     /* ★总开关 */

#define OVT_OWN_RIGHT       (1)     /* 1 = 自己走右道（左半是对向车道） */
#define OVT_LANE_PX         (25)    /* 借道时目标点偏移 px（正数=往左借道） */
#define OVT_ENTER_TICKS     (20)    /* 前车持续 20×5ms=100ms 才动作 */
#define OVT_PASS_TICKS      (400)   /* 借道保持 2.0 秒 */
#define OVT_BACK_TICKS      (300)   /* 回线保持 1.5 秒 */
#define OVT_COOL_TICKS      (300)   /* 冷却 1.5 秒 */
#define CAR_NEAR_ROW        (40)    /* 车到这一行以后算“近了” */
#define CAR_PCT_HI          (45)    /* 白点占比超过它认为有车（%） */

#define OVT_IDLE            (0)
#define OVT_GO              (1)
#define OVT_BACK            (2)

uint8  ovt_state      = OVT_IDLE;
int16  ovt_offset     = 0;
uint8  ovt_light      = 0;          /* 0=灭 1=左 2=右 */
volatile uint8 head_flash_req = 0;  /* 置 n = 前大灯闪 n 次 */

/* 前方车辆识别（在 Img_analyse.c 的 Car_Scan 里计算） */
extern volatile uint8 car_pct_l, car_pct_r, car_row_l, car_row_r;

void Overtake_Control(void);


void S3010_Control(void)
{

        Point_Error = ABS(Point-Point_Mid);   //用于赛道判断
        if(Flag_turn==1)//左转
        {
            SD5_Duty=908;//904
        }
        else if(Flag_turn==2)//右转
        {
            SD5_Duty=799;//802
        }
        else if(Flag_turn==3)//中间
        {
            SD5_Duty=857;
        }
        else if(Flag_turn==4)//da左转
        {
            SD5_Duty=916;
        }
        else if(Flag_turn==5)//微偏右转
        {
            SD5_Duty=852;
        }
        else if(Flag_turn==6)//微偏右转
            {
                SD5_Duty=787;
            }
        else if(Flag_turn==8)//微偏右转
            {
                SD5_Duty=857;
            }
    else
    {
        int32 now = (int32)Point_Mid;

        #if OVERTAKE_ENABLE
        /* [2026-10-25] 借道/回线：把参考点整体平移。
           ovt_offset > 0 -> iError 变负 -> duty 变大 -> 往【左】借道 */
        if(ovt_state != OVT_IDLE) now += (int32)ovt_offset;
        #endif
        SD5_Duty = PlacePID_Control(&Point_PID, now, Point);

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
    Rcount =  encoder_get_count(ENCODER1_TIM); //这里需要注意第二个参数务必填写A相引脚
    Lcount = -encoder_get_count(ENCODER2_TIM);

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



    RSpeedPWM = RMotorPid_Col(RMotorSpeed);
    LSpeedPWM = LMotorPid_Col(LMotorSpeed);
//    LSpeedPWM = -2000;
//    RSpeedPWM=-2000;
    if(LSpeedPWM >= 0)
    {
        gpio_set_level(MOTOR2_A,0);
        pwm_set_duty(MOTOR2_B, LSpeedPWM);
    }
    else
    {
        gpio_set_level(MOTOR2_A,1);
        pwm_set_duty(MOTOR2_B, - LSpeedPWM);
    }

    if(RSpeedPWM >= 0)                         //新方案双电机控制
    {
        gpio_set_level(MOTOR1_A,0);
        pwm_set_duty(MOTOR1_B, RSpeedPWM);
    }
    else
    {
        gpio_set_level(MOTOR1_A,1);
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
    LMotorSpeed = RMotorSpeed=205;//360
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
        LMotorSpeed = RMotorSpeed =150;
    }
    else if(Flag_stop==7)//（转弯前减速）
        {
            LMotorSpeed = RMotorSpeed =135;
        }


    // ★统一按 SPEED_PERCENT 缩放目标速度
    LMotorSpeed = LMotorSpeed * SPEED_PERCENT / 100;
    RMotorSpeed = RMotorSpeed * SPEED_PERCENT / 100;

    // ★软启动：加速限速（每 MOTOR_RAMP_TICKS 个 5ms 才 +1，40 x 5ms = 200ms 一格）
    //   减速 / 停车立即生效，不做缓降
    {
        static uint16 ramp_cnt = 0;
        static int32  out_L = 0, out_R = 0;

        if(++ ramp_cnt >= MOTOR_RAMP_TICKS)
        {
            ramp_cnt = 0;
            if(out_L < LMotorSpeed) out_L ++;
            if(out_R < RMotorSpeed) out_R ++;
        }
        if(out_L > LMotorSpeed) out_L = LMotorSpeed;
        if(out_R > RMotorSpeed) out_R = RMotorSpeed;

        LMotorSpeed = out_L;
        RMotorSpeed = out_R;
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

/* ============ [2026-10-25] 车灯重写 ============
   接线（实测确认，不是按位置顺序排的）：
       P02_4 = 左前转向   P02_5 = 右前转向
       P02_6 = 左后转向   P02_7 = 右后转向
       P20_11 = 左后刹灯  P00_8 = 右后刹灯
       P20_13 = 左前大灯  P20_14 = 右前大灯

   规则要求「同侧前后转向灯同频率闪亮」，所以按【左右】成对控制：
       左 = P02_4 + P02_6     右 = P02_5 + P02_7

   旧版靠 Light_Flag 驱动，但 Light_Flag 只在 System.c 里赋过一次初值，
   实际从来没被改过 -> 灯一直是灭的。这里改成直接看全局状态。
*/
#define LIGHT_BLINK_TICKS   (13)   /* 10ms × 13 = 130ms 半周期，约 3.8Hz */
#define LIGHT_FLASH_TICKS   (10)   /* 大灯闪烁半周期 */

void Light(void)
{
    static uint8 blink = 0;
    static uint8 fl_tick = 0, fl_times = 0, fl_state = 0;
    uint8 on, brake, left, right, head;

    /* 1) 没发车：全亮（规则要求开机后所有车灯亮起超过 3 秒） */
    if(!Stop_1)
    {
        gpio_set_level(P02_4,  1);
        gpio_set_level(P02_5,  1);
        gpio_set_level(P02_6,  1);
        gpio_set_level(P02_7,  1);
        gpio_set_level(P20_11, 1);
        gpio_set_level(P00_8,  1);
        gpio_set_level(P20_13, 1);
        gpio_set_level(P20_14, 1);
        blink = 0;
        fl_times = 0;
        fl_tick  = 0;
        fl_state = 0;
        return;
    }

    /* 2) 转向灯节拍 */
    if(++ blink >= (uint8)(LIGHT_BLINK_TICKS * 2)) blink = 0;
    on = (uint8)(blink < LIGHT_BLINK_TICKS);

    /* 3) 大灯闪烁（会车用）：闪 n 次 */
    if(fl_times)
    {
        if(++ fl_tick >= LIGHT_FLASH_TICKS)
        {
            fl_tick  = 0;
            fl_state = (uint8)(!fl_state);
            if(!fl_state) fl_times --;          /* 灭一次算闪完一次 */
        }
    }
    else if(head_flash_req)
    {
        fl_times       = head_flash_req;
        head_flash_req = 0;
        fl_tick        = 0;
        fl_state       = 1;
    }
    head = fl_times ? fl_state : 1;

    /* 4) 刹车灯：停车时亮 */
    brake = (uint8)(Flag_stop == 1);

    /* 5) 转向灯：超车状态机优先，其次看 Flag_turn */
    if(ovt_light == 1)      { left = 1; right = 0; }
    else if(ovt_light == 2) { left = 0; right = 1; }
    else
    {
        left  = (uint8)((Flag_turn == 1) || (Flag_turn == 4));
        right = (uint8)((Flag_turn == 2) || (Flag_turn == 6));
    }

    /* 6) 输出 */
    gpio_set_level(P02_4,  (left  && on) ? 1 : 0);   /* 左前转向 */
    gpio_set_level(P02_6,  (left  && on) ? 1 : 0);   /* 左后转向 */
    gpio_set_level(P02_5,  (right && on) ? 1 : 0);   /* 右前转向 */
    gpio_set_level(P02_7,  (right && on) ? 1 : 0);   /* 右后转向 */

    gpio_set_level(P20_11, brake ? 1 : 0);           /* 左后刹灯 */
    gpio_set_level(P00_8,  brake ? 1 : 0);           /* 右后刹灯 */

    gpio_set_level(P20_13, head);                    /* 左前大灯 */
    gpio_set_level(P20_14, head);                    /* 右前大灯 */
}

/* ============ [2026-10-25] 超车与会车状态机（5ms 调用一次）============
   状态流转：
     IDLE  -> 自己那半车道白点占比高 且 车已靠近    -> GO
     GO    -> 左转向灯 + 参考点左移 25px（借道），保持 2 秒 -> BACK
     BACK  -> 右转向灯 + 参考点回中（回线），保持 1.5 秒     -> IDLE
   另外：任何时刻对向车道出现车 -> 请求大灯闪 3 次（4 秒内不重复）
*/
void Overtake_Control(void)
{
#if OVERTAKE_ENABLE
    static uint16 hold = 0, cool = 0, det = 0, flash_cool = 0;
    uint8 own_pct, own_row, opp_pct, car_here;

    own_pct = OVT_OWN_RIGHT ? car_pct_r : car_pct_l;
    own_row = OVT_OWN_RIGHT ? car_row_r : car_row_l;
    opp_pct = OVT_OWN_RIGHT ? car_pct_l : car_pct_r;

    car_here = (uint8)((own_pct >= CAR_PCT_HI) && (own_row >= CAR_NEAR_ROW));

    if(cool)       cool --;
    if(flash_cool) flash_cool --;

    /* 会车：对向车道有车 -> 大灯闪 3 次 */
    if(opp_pct >= CAR_PCT_HI && flash_cool == 0)
    {
        head_flash_req = 3;
        flash_cool     = 800;          /* 5ms × 800 = 4 秒 */
    }

    switch(ovt_state)
    {
        case OVT_IDLE:
            if(car_here) { if(det < 60000) det ++; }
            else         { det = 0; }

            if(det >= OVT_ENTER_TICKS && cool == 0)
            {
                ovt_state  = OVT_GO;
                ovt_offset = OVT_LANE_PX;      /* 正 = 往左借道 */
                ovt_light  = 1;                /* 左转向灯 */
                hold       = OVT_PASS_TICKS;
                det        = 0;
            }
            break;

        case OVT_GO:
            if(hold) hold --;
            else
            {
                ovt_state  = OVT_BACK;
                ovt_offset = 0;                /* 回本道中心 */
                ovt_light  = 2;                /* 右转向灯 */
                hold       = OVT_BACK_TICKS;
            }
            break;

        case OVT_BACK:
            if(hold) hold --;
            else
            {
                ovt_state  = OVT_IDLE;
                ovt_offset = 0;
                ovt_light  = 0;                /* 回正熄灭 */
                cool       = OVT_COOL_TICKS;
            }
            break;

        default:
            ovt_state = OVT_IDLE;
            break;
    }
#else
    ovt_state  = OVT_IDLE;
    ovt_offset = 0;
    ovt_light  = 0;
#endif
}

