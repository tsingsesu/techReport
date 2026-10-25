#include "zf_common_headfile.h"

/*
*   结果变量 用于在屏幕显示
*/
uint8 result_screen;
uint8 Rhombus_Out_Flag;
uint8 aaa=0,Rhombus_Out;

uint32 time_dl1b;
uint32 time_dl1b_s;
uint8 time_dl1b_on;
uint8 mode_2024;

/*************************图像赛道数据****************************/
uint16 Left_Line[61], Right_Line[61], Mid_Line[61]; // 原始左右边界数据
uint16 Left_Add_Line[61], Right_Add_Line[61];       // 左右边界补线数据
uint8 Left_Add_Flag[61], Right_Add_Flag[61];        // 左右边界补线标记位
uint16 Width_Real[61];                                  // 实际赛道宽度
int16 Width_Add[61];                                    // 补线赛道宽度
int16 Width_Min;                                        // 最小赛道宽度

uint8 Left_Add_Start, Right_Add_Start;  // 左右补线起始行坐标
uint8 Left_Add_Stop, Right_Add_Stop;    // 左右补线结束行坐标

uint8 Left_Line_Start, Right_Line_Start;  // 左右 边界线 实线起始点
uint8 Left_Line_Break, Right_Line_Break;   // 左右 边界线 断点

int8 Out_Side = 0;  // 离线控制
int8 Line_Count;        // 记录成功识别到的赛道行数

int Count=0;
uint8 Last_Point;
uint8 turnleft;
/***********************赛道斜率************************/
float Left_Ka = 0, Right_Ka = 0;
float Left_Kb = 1, Right_Kb = (COL - 1);    // 最小二次函数参数
float Mid_qulv,Right_qulv,Left_qulv;

/************圆环标记*****************/
uint8 LDrop_Line_Num , RDrop_Line_Num;  // 左右 连续 掉线 数量 2020.5.26
uint8 Left_Linear_Flag, Right_Linear_Flag;     // 左右 边界线 是否 是 直线 的标记 1 为 直线

uint8 Left_Ring_Flag,Right_Ring_Flag;    // 进行圆环识别的标记
uint8 Ring_Flag= 0;
uint8 Flag1=0;          //0直道  1 圆环入口  2 圆环第二个入口  3 圆环中间
uint8 Dirc;           //1左  2右
uint8 Ring_Enter = 0;
uint8 Ring_Enter1 = 0;
uint8 Cycle_Flag;
/************停车标记*****************/
uint8 Flag_stop;
uint8 stop,Stop_1;
uint8 Start_Line_Flag,Stop_Flag;
uint8 white_count,n,k,j,a,b,c,i,a1,b1;
uint8 Zuo_Flag,Zhuan_Flag,Left_Flag1,Right_Flag1,Stop1,Right_Flag2;
uint8 Flag_turn;
uint8 Deal_Flag,Time_banmaxian,Time_banmaxian_Flag,Dis_Flag,Measurement;
uint16 Distances = 120;
uint8 Guaidian;
uint8 Rhombus_Flag;
uint8 White_Flag;
uint8 Right_Max,Left_Min,Right_Flag,Left_Flag,Right_Flag1;
uint8 zuobianjie_Flag,youbianjie_Flag;
uint8 Cone_bucket,Lin_Flag,rhombus_Flag,rhombus_Flag1;
uint32 distance_0,distance_1,distance_2;                //转换出来的距离  单位为毫米
uint8 Distance_Left_Flag,Distance_Right_Flag;
uint8 Stadiometry_Flag1,Stadiometry_Flag2,Stdiometry_Flag3;
uint8 Light_Flag,Light_Flag1;
uint8 Num_Turn_L,Num_Turn_R;
int32 Time_Cal;
/*
 *
 *
*   图像算法参数初始化
*
*   说明：仅影响第一行特殊处理
*/
void Image_Para_Init(void)
{
    Mid_Line[60] = 60;
    Left_Line[60] = 2;
    Right_Line[60] = (COL - 2);
    Left_Add_Line[60] = 2;
    Right_Add_Line[60] = (COL - 2);
    Width_Real[60] = (COL - 2);
    Width_Add[60] = (COL - 2);
}
/*
*   图像处理算法
*
*   说明：处理普通图像，包括十字、停车
*/
void Image_Handle(uint8 *data)
{       //变量设置
    uint8 i;    // 控制行
    uint8 j;
    uint8 res;
    Line_Count = 0; // 赛道行数复位
    Width_Min = COL-1;
    //        Count++;     // 不知用途  我用来 时间计数
    //=====================================================
    Stop1=1;
    Left_Add_Start = 0;  // 左复位补线起始行坐标
    Right_Add_Start = 0; // 右复位补线起始行坐标
    Left_Add_Stop = 0;   // 左补线结束行坐标
    Right_Add_Stop = 0;  // 右补线结束行坐标

    Left_Line_Start = 0;   //左右 边界线 实线起始点
    Right_Line_Start = 0;
    Left_Line_Break = 0;   // 左右 边界线 断点
    Right_Line_Break = 0;

    LDrop_Line_Num = 0;    // 左右 连续 掉线 数量
    RDrop_Line_Num = 0;
    //Out_Side = 0;         // 离线控制
    Line_Count = 59;      // 记录成功识别到的赛道行数
    /***************************** 第一行特殊处理 *****************************/
    if(!Right_Flag)
    {
        res = First_Line_Handle(data);
    }
    if (res == 0)
    {
        //            if (g_fDirectionSum[0] + g_fDirectionSum[1] < 50 )
        {
            //                        Out_Side = 1;
        }  // 离线 停车#

        return ;
    }
    /*************************** 第一行特殊处理结束 ***************************/
    for (i = 59;i >= 5;)
    {
        i -= 1;
        if (data[(i-2)*COL + Mid_Line[i+1]] && data[(i-1)*COL + Mid_Line[i+1]]) // 前一行中点在本行为黑点，可能是赛道结束，也可能是圆环
        {
            if(i <= 50)     // 防止找的线行数太少
                break;
        }
        else  // 使用前2行中点向两边查找边界
        {
            Traversal_Mid_Line(i, data, Mid_Line[i+1], 1, (COL-3), Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
        }
        /**************************** 补线检测开始 ****************************/
        if (Width_Real[i] > Width_Min+10)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))   // 赛道宽度变宽，可能是十字
        {
            if (Left_Add_Line[i] <  Left_Add_Line[i+1])
            {
                if (!Left_Add_Flag[i])
                {
                    Left_Add_Flag[i] = 1;       // 开始设置为需要补线
                }
            }
            if (Right_Add_Line[i] > Right_Add_Line[i+1])
            {
                if (!Right_Add_Flag[i])
                {
                    Right_Add_Flag[i] = 1;       // 开始设置为需要补线
                }
            }
        }

        /******************** 第二阶段 补线检测开始 ******************************/
        if (Left_Add_Line[i] > Left_Add_Line[i+1]+10 && !Left_Add_Flag[i+1])  // 如果这次的点 远离上一个点位置太远 且上一个点不需要补线
        {
            if (!Left_Add_Flag[i])
            {
                Left_Add_Flag[i] = 1;       // 开始设置为需要补线
            }
        }
        if (Right_Add_Line[i] < Right_Add_Line[i+1]-10 && !Right_Add_Flag[i+1])  // 如果这次的点 远离上一个点位置太远 且上一个点不需要补线
        {
            if (!Right_Add_Flag[i])
            {
                Right_Add_Flag[i] = 1;       // 开始设置为需要补线
            }
        }

        /*************************** 第一阶段补线开始 ***************************/
        if (Left_Add_Flag[i])   // 左侧需要补线
        {
            if (i > 10 && i < 53)
            {
                LDrop_Line_Num++;
                // 左掉线数量
            }
            if (i >= 55)    // 前5行补线不计
            {
                if (!Left_Add_Start)
                {
                    Left_Ka = 0;
                    Left_Kb = Left_Add_Line[i];
                }
                Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);
            }
            else
            {
                if (!Left_Add_Start)    // 此前没有补线
                {
                    Left_Add_Start = i+1; // 记录左侧补线开始行
                }
                if (Left_Line_Break && (Left_Line_Start -Left_Line_Break) >= 5)    // 如果 左断点存在 用起始点斜率补线
                {
                    Curve2_Fitting(&Left_Ka, &Left_Kb,Left_Line_Start,Left_Line_Break,Left_Add_Line,1);
                }
                else
                    Curve_Fitting(&Left_Ka, &Left_Kb, &i, Left_Add_Line, Left_Add_Flag, 1);    // 补线
                Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);  // 补线完成
            }
        }
        else   // 不需要补线  记下补线结束行  用来最后进行两点补线
        {
            if (!Left_Line_Start  && i > 50 && i < 56)    // 记下 左实际边界线的起点
            {
                Left_Line_Start = i;
            }
            if (Left_Add_Start) // 已经开始补线
            {
                if (!Left_Add_Stop && !Left_Add_Flag[i+1]) // && !Left_Add_Flag[i+2])
                {
                    if ((Left_Add_Line[i] > Left_Add_Line[i+1])) // && (Left_Add_Line[i+1] > Left_Add_Line[i+2]))
                    {
                        Left_Add_Stop = i+1;        // 记录左侧补线结束行
                    }
                }
            }
        }

        if (Right_Add_Flag[i])  // 右侧需要补线
        {
            if (i > 10 && i<53)
            {
                RDrop_Line_Num++;
            }
            if(Right_Flag) break;
            if (i >= 55)    // 前三行补线不计
            {
                if (!Right_Add_Start)
                {
                    Right_Ka = 0;
                    Right_Kb = Right_Add_Line[i];
                }

                Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);   // 使用前一行图像右边界斜率补线
            }
            else
            {
                if (!Right_Add_Start)       // 此前没有补线
                {
                    Right_Add_Start = i+1;  // 记录右侧补线开始行
                }
                if (Right_Line_Break && (Right_Line_Start - Right_Line_Break) >= 5)    // 如果 左断点存在 用起始点斜率补线
                {
                    Curve2_Fitting(&Right_Ka, &Right_Kb,Right_Line_Start,Right_Line_Break,Right_Add_Line,2);
                }
                else
                    Curve_Fitting(&Right_Ka, &Right_Kb, &i, Right_Add_Line, Right_Add_Flag, 2);   // 使用两点法拟合直线

                Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);// 补线完成
            }
        }
        else     // 不需要补线  记下补线结束行  用来最后进行两点补线
        {
            if (!Right_Line_Start)    // 记下 左实际边界线的起点
            {
                Right_Line_Start = i;
            }
            if (Right_Add_Start)    // 已经开始补线
            {
                if (!Right_Add_Stop && !Right_Add_Flag[i+1]) // && !Right_Add_Flag[i+2])
                {
                    if ((Right_Add_Line[i] < Right_Add_Line[i+1]))// && (Right_Add_Line[i+1]<Right_Add_Line[i+2]))
                    {
                        Right_Add_Stop = i+1; // 记录右侧补线结束行
                    }
                }
            }
        }
        /*************************** 第一阶段补线结束 ***************************/
        Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];        // 重新计算赛道宽度
        Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;   // 计算中线

        if (Width_Add[i] < Width_Min) //&& !Left_Add_Flag[i] && !Right_Add_Flag[i])
        {
            Width_Min = Width_Add[i];                               // 更新最小赛道宽度
        }
        Line_Count = i;                                            //     记录成功识别到的赛道行数
        if(Left_Add_Stop && Right_Add_Stop)                      //补线结束后又需要补线这不补
        {
            if(Left_Add_Flag[i]||Right_Add_Flag[i])
            {
                Line_Count = i+2;
                break;
            }
        }
        //             if(Right_Add_Line[i] < 5)
        //                 Right_Add_Line[i] = Right_Add_Line[i+1];
    }//for循环结束



    /*************************** 第二阶段补线修复开始 ***************************/
    if (Left_Add_Stop)      // 左边界需要补线
    {
        Line_Repair(Left_Add_Start, Left_Add_Stop, data, Left_Line, Left_Add_Line, Left_Add_Flag, 1);
    }
    if (Right_Add_Stop) // 右边界需要补线
    {
        Line_Repair(Right_Add_Start, Right_Add_Stop, data, Right_Line, Right_Add_Line, Right_Add_Flag, 2);
    }
    /*************************** 第二阶段补线修复结束 ***************************/
    for (i = 59; i >= Line_Count;)   // 重新计算 中点
    {
        i -= 1;
        Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2+Differ ;

    }


    /************************************************  运动处理函数2024   ******************************************************/
    /************************************************  运动处理函数2024   ******************************************************/
    /************************************************  运动处理函数2024   ******************************************************/
    /************************************************  运动处理函数2024   ******************************************************/

        if(mode_2024==2)
        {
            if(Deal_Flag==0)
            {
                Flag_turn=0;
                Flag_stop=0;
                Deal_Flag=1;
            }
            if(Deal_Flag==1&&Rhombus_Flag==0)//识别到减速物体
            {
                Find_Decelerate(data);
            }
            if(Deal_Flag==2&&Rhombus_Flag==0)//识别到减速物体
            {
                Find_Decelerate(data);
            }
            if(Deal_Flag==3&&Rhombus_Flag==0)//识别到减速物体
            {
                Find_Rhombus(data);
            }
            if(Deal_Flag==4&&Rhombus_Flag==0)//识别到减速物体
            {
                Find_rhombus(data);
            }
        }
        if(mode_2024==1)
        {
            if(Deal_Flag==0)
            {
                Flag_turn=0;
                Flag_stop=0;
                Deal_Flag=1;

            }
            dl1b_get_distance();
            if(Deal_Flag==1&& dl1b_distance_mm<1200&&Rhombus_Flag==0)//识别到菱形物体
            {
                Rhombus_Flag=1;
            }
            if(Deal_Flag==2&&Rhombus_Flag==0)
            {

                Rhombus_Flag=1;
            }
            if(Deal_Flag>=30)
            {
                dl1b2_get_distance();
                Find_Decelerate(data);
                if(dl1b2_distance_mm<450)
                {
                   Deal_Flag=4;
                   Rhombus_Flag=1;
                }
            }
             if(Rhombus_Flag==0&&Deal_Flag==5)
             {
                 Find_Rhombus(data);
             }
             if(Rhombus_Flag==0&&Deal_Flag==6)
             {
                 Find_rhombus(data);
             }
         }


    //////////////////////////////////////////////////////执行阶段/////////////////////////////////////////////////


        if(Rhombus_Flag==1&&mode_2024==2)
        {
                switch(Deal_Flag)
                {
                case 1:
                    {
                        Decelerate(data);//6
                        break;
                    }
                case 2:
                    {
                        Decelerate(data);//0
                        break;
                    }
                case 3:
                    {
                        Zebra_Crossing(data);//6（斑马线处理）
                        break;
                    }
                case 4:
                    {

                        Flag_stop=0;
                        Flag_turn=5;
                        //Rhombus_Flag=0;
                        Distance_ON=1;
                        Deal_Flag=5;
                        break;
                    }
                case 5:
                    {
                        if(Distance>120 )
                        {
                        gpio_set_level(P33_10,1);
                        Flag_stop=1;
                        Light_Flag=1;
                        }
                        break;
                    }
                default:break;
                }
        }
        if(Rhombus_Flag==1&&mode_2024==1)
        {
                    switch(Deal_Flag)
                    {
                    case 1:
                        {
                            Turn_Left();//6（左转处理）
                            break;
                        }
                    case 2:
                        {
                            Turn_Right();//0（右转处理）
                            break;
                        }
                    case 30:
                        {
                            Decelerate(data);//6（减速处理）
                            break;
                        }
                    case 31:
                        {
                            Decelerate(data);//6（减速处理）
                            break;
                        }
                    case 4:
                        {
                            Turn_Left_1();//6（左转1处理）
                            break;
                        }
                    case 5:
                        {
                            Zebra_Crossing(data);//6（斑马线处理）
                            break;
                        }
                    case 6:
                        {
                            Flag_stop=0;
                            Flag_turn=5;
            //                Rhombus_Flag=0;
                            Distance_ON=1;
                            Deal_Flag=7;
                            break;
                        }
                    case 7:
                        {
                            if(Distance>120)
                            {
                            gpio_set_level(P33_10,1);
                            Flag_stop=1;
                            Light_Flag=1;
                            }
                            break;
                        }
                    default:break;
                    }
         }

//8.6






  // 7.31
//    if(Deal_Flag==1&&Rhombus_Flag==1)
//    {
//        Turn_Left();//左转处理
//    }
//    if(Deal_Flag==2&&Rhombus_Flag==1)
//    {
//        Turn_Right();//右转处理
//    }
//    if(Deal_Flag==3&&Rhombus_Flag==1)
//    {
//        Decelerate(data);//减速处理
//    }
//    if(Deal_Flag==4&&Rhombus_Flag==1)
//    {
//        Decelerate(data);//减速处理
//    }
//    if(Deal_Flag==6&&Rhombus_Flag==1)
//    {
//        Flag_stop=0;
//        Flag_turn=3;
//        Rhombus_Flag=0;
//
//      //  Distance_Test_ON=0;
//        Distance_ON=1;
//        Deal_Flag++;
//    }
//    if(Deal_Flag==7&&Distance>50)
//    {
//        Flag_stop=1;//停车
//    }
//    if(Flag_turn == 1 && Distance > 124 )
//    {
//        gpio_set_level(P33_10,0);//舵机关闭
//        Flag_turn = 2;//右转完成
//        Distance_ON = 0;
//        Distance = 0;
//        Flag_stop = 5;//右转完成速度
//        Distance_Test_ON = 1;
//
//    }
//    if(Flag_turn == 2 && Distance_Test > 62 )
//    {
//        Flag_turn = 0;
//        Distance_Test = 0;
//        Distance_Test_ON = 0;
//        Flag_stop = 0;
//        gpio_set_level(P21_3, 0);
//    }
//    if(1)
//    {
//        Distance_Test_ON = 1;
//        Light_Flag = 7;
//        if(Distance_Test > 100)
//        {
//            gpio_set_level(P33_10,1);
//            Distance_ON = 1;
//            Flag_turn = 2;//右转
//            Flag_stop = 3;
//            Distance_Test_ON = 0;
//            Distance_Test = 0;
//            Light_Flag=2;
//        }
//    }


//    if(Flag_turn == 2 && Distance > 200  )
//    {
//        gpio_set_level(P33_10,0);
//        Light_Flag = 6;
//        Flag_turn = 0;
//        Distance_ON = 0;
//        Distance = 0;
//        Flag_stop = 0;
//        Distance_Test_ON = 0;//原来为1
//        gpio_set_level(P21_5, 0);
//    }
//


    //如果右侧的OF距离变化，就不执行它的动作，直接采集，直接识别菱形线，停车
//    if(Turn_Flag == 2 && Distance_Test > Distances)//寻找菱形
//    {
//        Measurement = 1;
//        Find_rhombus(data);
//    }
//    if(Turn_Flag == 2 && Rhombus_Flag == 1)//找到第一个菱形
//    {
//        Flag_turn = 3;
//        Distance_Test_ON = 0;
//        Distance_Test = 0;
//        gpio_set_level(P33_10,1);
//        Flag_stop = 0;
//        Turn_Flag = 3;//第一个菱形
//    }
//    if(Turn_Flag == 3 && !Dis_Flag)
//    {
//        Distance_ON=1;
//        Flag_turn = 3;
//        if(Distance>45)
//        {
//            gpio_set_level(P33_10,0);
//            Flag_turn = 0;
//            Rhombus_Flag = 0;
//            Flag_stop = 0;
//            Turn_Flag = 4;//第一个菱形结束
//            Distance_ON = 0;
//            Distance = 0;
//        }
//    }
//    if(Turn_Flag == 4 && !Dis_Flag)//寻找菱形
//    {
//        Flag_stop = 0;
//        Distance_Test_ON = 1;
//        if(Distance_Test > 20)
//        {
//            Find_rhombus(data);
//            Distance_Test_ON = 0;
//            Distance_Test = 0;
//        }
//    }
//    if(Turn_Flag == 4 && Rhombus_Flag == 1 && !Dis_Flag)
//    {
//        gpio_set_level(P33_10,1);
//        Flag_stop = 0;
//        Flag_turn = 3;
//        Turn_Flag = 5;//第二个菱形
//    }
//    if(Turn_Flag == 5 && !Dis_Flag)
//    {
//        Flag_turn = 3;
//        Distance_ON=1;
//        if(Distance>45)
//        {
//            gpio_set_level(P33_10,0);
//            Flag_turn = 0;
//            Rhombus_Flag = 0;
//            Flag_stop = 0;
//            Turn_Flag = 6;//第二个菱形结束
//            Distance_ON = 0;
//            Distance = 0;
//            Distance_Test_ON = 1;
//        }
//    }
//    if(Measurement == 1 && !Dis_Flag)
//    {
//        gpio_toggle_level(P20_8);
//        if(dl1a_distance_mm > 100 && dl1a_distance_mm < 700 && dl1b_distance_mm > 8000)
//        {
//            if(Turn_Flag > 2)//菱形之后
//            {
//                gpio_toggle_level(P20_9);
//                Dis_Flag = 1;
//                Distance_ON = 0;
//                Distance = 0;
//                Distance_Test_ON = 0;
//                Distance_Test = 0;
//            }
//            else
//            {
//                if(Point_Error < 10)
//                {
//                    gpio_toggle_level(P20_9);
//                    Dis_Flag = 1;
//                    Distance_ON = 0;
//                    Distance = 0;
//                    Distance_Test_ON = 0;
//                    Distance_Test = 0;
//                }
//            }
//
//        }
//    }
//    if(Dis_Flag == 1)//识别到有车
//    {
//        Dis_Flag = 2;
//    }
//    if(Dis_Flag == 2)
//    {
//        gpio_set_level(P33_10,1);
//        Distance_ON = 1;
//        Flag_turn = 1;//左转
//        Flag_stop = 6;
//    }
//    if(Flag_turn == 1 && Distance > 62 && Dis_Flag == 2)
//    {
//        Dis_Flag = 3;
//        gpio_set_level(P33_10,0);
//        Flag_turn = 2;
//        Distance_ON = 0;
//        Distance = 0;
//        Flag_stop = 6;
//        Distance_Test_ON = 1;
//    }
//    if(Flag_turn == 2 && Distance_Test > 5 && Dis_Flag == 3)
//    {
//        Light_Flag = 5;
//        Flag_turn = 0;
//        Distance_Test = 0;
//        Distance_Test_ON = 0;
//        Flag_stop = 4;
//        Dis_Flag = 4;
//    }



  //后续处理
//    if(Dis_Flag == 4 && !Start_Line_Flag)
//    {
//        Flag_stop = 4;//加速
//        Flag_turn = 0;
//        Start_Line_Judg(data);//寻找起始线
//    }
//    if(Dis_Flag == 4 && Start_Line_Flag == 1)
//    {
//        Flag_stop = 4;
//        gpio_set_level(P33_10,1);
//        Dis_Flag = 5;//找到起始线
//    }
//    if(Dis_Flag == 5)
//    {
//        Flag_stop = 4;
//        Flag_turn = 3;
//        gpio_set_level(P33_10,0);
//        Distance_ON = 1;
//        if(Distance > 25)//原本200
//        {
//            Flag_stop = 1;//原本为0（停车）
//            Dis_Flag = 6;
//            Distance_ON = 0;
//            Distance = 0;
//            Time_banmaxian=1;
//            Turn_Flag=10;
//        }
//    }


    //后续处理
//    if(Time_banmaxian_Flag == 1)
//     {
//        Flag_stop=5;
//        gpio_set_level(P33_10,1);
//        if(dl1b_distance_mm<150)
//        {
//            gpio_set_level(P33_10,0);
//            Flag_stop=1;
//            Turn_Flag=0;
//        }
//
//     }



//    if(Dis_Flag == 6)
//    {
//        Flag_stop = 4;
//        gpio_set_level(P33_10,1);
//        Flag_turn = 0;
//        Dis_Flag = 7;//要结束        Time_banmaxian_Flag
//        Light_Flag = 3;
//    }
//    if(Dis_Flag == 7)
//    {
//        Flag_turn = 0;
//        Flag_stop = 4;
//        Stop_stop(data);
//        if(Stop_Flag == 1)
//        {
//            Flag_turn = 3;
//            Distance_Test_ON = 1;
//            if(Distance_Test > 20)//原本60
//            {
//                Flag_turn = 3;
//                Dis_Flag = 10;
//                Flag_stop = 1;//停车
//            }
//        }
//    }
//    if(Dis_Flag == 10)
//    {
//        Light_Flag = 4;
//        Flag_stop = 1;
//        Flag_turn = 3;
//    }
//    Num_Turn_L = Find_J_Black(data,5,58,20);
//    Num_Turn_R = Find_J_Black(data,115,58,20);
}     //此处为Image函数结束

            //以下为子函数
uint8 Start_Line_Judg(uint8 *data)   // 起始线判断
{
    uint8 j;
    uint8 i,white_count,k,a;
    for(i = 25;i>15;i--)
    {
        k= 0,a=0 ;
        for(j = 20;j<100;j++)
        {
            if(data[i*COL + j])//白色像素（计数）
            {
                white_count=1;//修正：补充白色像素计数注释，原乱码部分为计数说明
            }
                        else
                        {
                            white_count=0;
                        }

                        if(white_count!=n)
                        {
                            n = white_count;
                            k++;
                        }
                        if(k>11)
                        {
                            a++;
                            if(a>3) break;
                        }
                    }
                    if(a>3) break;
                }
                if(a>3)
                {
                    Start_Line_Flag=1;
                }
            }
            void Stop_stop(uint8 *data)
            {
                uint8 j;
                uint8 i,a;
                // 从第45行到第41行（倒序），在列55-64范围内检测白色像素，判断是否为停车区域
                for(i = 45;i>40;i--)
                {
                    for(j = 55;j<65;j++)
                    {
                        if(data[i*COL+j]) // 检测到白色像素（停车线特征）
                        {
                            Stop_Flag=1; // 置位停车标记
                        }
                    }
                }

            }
            // 判断菱形（识别菱形标志物）
            void Find_rhombus(uint8 *data)
            {
                uint8 i,j,a=0;
                // 在第30-55行、列58-61范围内统计白色像素数量（菱形特征区域）
                for(i=30;i<=55;i++)
                {
                    for(j=58;j<62;j++)
                    {
                        if(data[i*COL+j]) // 检测到白色像素
                        {
                            a++; // 计数白色像素
                        }
                    }
                    if(a>2) // 白色像素数量超过2，判定为检测到菱形
                    {
                        Rhombus_Flag = 1; // 置位菱形识别标记
                        break;
                    }
                }
            }
            // 判断菱形（另一种菱形检测逻辑，检测范围更广）
            void Find_Rhombus(uint8 *data)
            {
                uint8 i,j,a=0;
                // 在第25-50行（倒序）、列52-67范围内统计白色像素数量（更大的菱形特征区域）
                for(i=50;i>=25;i--)
                {
                    for(j=52;j<68;j++)
                    {
                        if(data[i*COL+j]) // 检测到白色像素
                        {
                            a++; // 计数白色像素
                        }
                    }
                    if(a>3) // 白色像素数量超过3，判定为检测到菱形
                    {
                        Rhombus_Flag = 1; // 置位菱形识别标记
                        break;
                    }
                }
            }
            // 检测减速区域（识别减速标志物）
            void Find_Decelerate(uint8 *data)
            {
                uint8 i,j,a=0;
                // 在第25-55行（倒序）、列58-61范围内统计白色像素数量（减速区域特征）
                for(i=55;i>24;i--)
                {
                    for(j=58;j<62;j++)
                    {
                        if(data[i*COL+j]) // 检测到白色像素
                        {
                            a++; // 计数白色像素
                        }
                    }
                    if(a>2) // 白色像素数量超过2，判定为检测到减速区域
                    {
                        Rhombus_Flag = 1; // 置位菱形标记（此处复用菱形标记表示检测到目标）
                        break;
                    }
                }
            }
            // 检测菱形出环（判断是否离开菱形区域）
            void Find_rhombus_Out(uint8 *data)
            {
                uint8 i,j,a=0;
                // 在第21-65行（倒序）、列58-61范围内统计白色像素数量
                for(i=65;i>20;i--)
                {
                    for(j=58;j<62;j++)
                    {
                        if(data[i*COL+j]) // 检测到白色像素
                        {
                            a++; // 计数白色像素
                        }
                    }
                }
                if(a<3) // 白色像素数量少于3，判定为已离开菱形区域
                {
                    Rhombus_Out_Flag =1; // 置位菱形出环标记
                }
            }
            /*********自定义边界（手动设定赛道边界）*************************/
            void New_boundary(uint8 *data, float *Ka, float *Kb)
            {
                float ka,kb;
                uint8 res;
                Zhuan_Flag=1; // 置位转向标记
                if(Zhuan_Flag) // 若转向标记有效
                {
                // 在第15-17行、列30-34范围内查找边界特征点
                for(i=15;i<18;i++)
                {
                    for(j=35;j>30;j--)
                    {
                        // 检测当前点为白色，且上下点为黑色（边界跳变特征）
                        if(data[i*COL+j]&&!data[(i-1)*COL+j]&&!data[(i+1)*COL+j])
                        {
                            a=i; // 记录边界点行坐标
                            b=j; // 记录边界点列坐标
                        }
                    }
                }
                a1=60; // 设定另一个边界点行坐标（固定为第60行）
                b1=98; // 设定另一个边界点列坐标（固定为第98列）
                    // 计算两点之间的斜率（y=ka*x+kb）
                    ka = 1.0*(b1-b) / (a1-a);
                    kb = 1.0*b - (ka*a);
                    res = a * ka + kb;
                    // 对计算结果进行限幅（确保在1~COL-2范围内）
                    res = range_protect((int32)res, 1, (COL-2));
                    *Ka = ka; // 输出斜率参数
                    *Kb = kb; // 输出截距参数
                Right_Ka=*Ka; // 更新右边界斜率
                Right_Kb=*Kb; // 更新右边界截距
                // 从第60行到第6行（倒序），根据计算的边界参数补全右边界，左边界固定为4
                for(i=60;i>5;i--)
                {
                    Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);
                    Left_Add_Line[i]=4;
                    Mid_Line[i] = (Right_Add_Line[i]+Left_Add_Line[i]) / 2; // 重新计算中线
                }
                }
            }
            /*********辅助识别锥桶（识别锥桶标志物）*************************/
            uint8 Find_Cone_bucket(uint8 *data)
            {
                uint8 i,k;
                // 在第0-9列、第60-63行范围内统计白色像素数量（锥桶特征区域）
                for(j=0;j<10;j++)
                {
                    for(i=60;i<64;i++)
                    {
                        if(data[i*COL+j]) // 检测到白色像素
                        {
                            k++; // 计数白色像素
                            if(k>10) // 白色像素数量超过10，判定为检测到锥桶
                            {
                            Cone_bucket=1; // 置位锥桶识别标记
                            zyuzhi = 38; // 设置阈值（可能为转向或速度阈值）
                            }
                        }
                    }
                }
            }
            // 从指定列、指定行范围（从下往上）查找跳变点，返回跳变点行号
            // 功能：检测灰度跳变（用于边界或标志物识别）
            uint8 Find_J_Black(uint8 *data, uint8 j,uint8 start,uint8 end)
            {
                uint8 i;
                uint8 result = 0;

                  // 从start行到end行（倒序），检测当前行与Gray_Num行上方的像素是否跳变
                  for(i=start;i>=end;)
                  {
                    i -= 1;
                    if(data[(i+Gray_Num)*COL + j]!=data[i*COL + j]) // 检测到灰度跳变
                    {
                         result = i; // 记录跳变点行号
                         return result;
                    }
                  }
                 return i; // 未检测到跳变，返回end行号
            }


            /*
            *   两点法求直线斜率
            *
            *   说明：拟合直线 y = Ka * x + Kb  的参数
            */
            void Curve_Fitting(float *Ka, float *Kb, uint8 *Start, uint16 *Line, uint8 *Add_Flag, uint8 Mode)
            {
                uint8 start=0,start2,start3;
                uint8 stop=0;
                float ka,kb;
                float res;
                // 从需要补线的起始点向下找第一个不需要补线的点，作为斜率计算起点
                for (uint8 i = *Start+2; i < 57; i++)
                {
                    if (!Add_Flag[i])
                    {
                        start = i;
                        break;
                    }
                }
                if (start)  // 找到斜率计算起点
                {
                    // 从斜率计算起点向下找10行内第一个不需要补线的点，作为斜率计算终点
                    for (uint8 i = start+2; i < start+10; i++)
                    {
                        if (!Add_Flag[i])
                        {
                            stop = i+1;
                            break;
                        }
                        if (i > 58) // 超出最大行（58行），停止查找
                        {
                            break;
                        }
                    }
                if (stop)        // 找到斜率计算终点
                    {
                        // 计算两点之间的斜率和截距
                        ka = 1.0*(Line[stop] - Line[start]) / (stop - start);
                        kb = 1.0*Line[start] - (ka*start);
                    }
                }
                else // 未找到斜率计算起点，默认斜率为0，截距为上一点列坐标
                {
                    ka = 0;
                    kb = Line[*Start+1];
                }
                // 计算当前行的补线列坐标
                res = (*Start) * ka + kb;
                // 对结果进行限幅（确保在1~COL-2范围内）
                res = range_protect((int32)res, 1, (COL-2));
                if (Mode ==1 ) // Mode=1：左边界补线
                {
                    // 若计算结果与上一点偏差超过-3，使用上一点坐标（防止补线偏差过大）
                    if ((res - Line[*Start+1]) < -3)
                    {
                        Line[*Start] =  Line[*Start+1];
                        *Ka = 0;
                        *Kb = Line[*Start+1];
                    }
                    else // 偏差正常，使用计算的斜率和截距
                    {
                        *Ka = ka;
                        *Kb = kb;
                        Line[*Start] = res;
                    }
                }
                else if (Mode == 2) // Mode=2：右边界补线
                {
                    // 若计算结果与上一点偏差超过3，使用上一点坐标（防止补线偏差过大）
                    if ((res - Line[*Start+1]) > 3)
                    {
                        Line[*Start] =  Line[*Start+1];
                        *Ka = 0;
                        *Kb = Line[*Start+1];
                    }
                    else // 偏差正常，使用计算的斜率和截距
                    {
                        *Ka = ka;
                        *Kb = kb;
                        Line[*Start] = res;
                    }
                }
            }
            /*
            *   两点法求直线
            *
            *   说明：拟合直线 y = Ka * x + Kb   Mode == 1表示左边界，Mode == 2表示右边界
            */
            void Curve2_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, uint16 *Line, uint8 Mode)
            {
                    if (Mode==1) // Mode=1：左边界拟合
                    {
                         // 计算Start行与End行之间的斜率和截距
                         *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
                         *Kb = 1.0*Line[End] - (*Ka * End);
                    }
                    else // Mode=2：右边界拟合
                    {
                          // 计算Start行与End行之间的斜率和截距
                          *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
                          *Kb = 1.0*Line[End] - (*Ka * End);
                    }

            }

            /*
            *   计算补线坐标
            *
            *   说明：使用两点法计算拟合出的补线坐标
            */
            uint8 Calculate_Add(uint8 i, float Ka, float Kb)
            {
                float res;
                uint8 Result;

                // 根据直线方程计算当前行（i）的补线列坐标
                res = i * Ka + Kb;
                // 对结果进行限幅（确保在1~COL-2范围内）
                Result = range_protect((int32)res, 1, (COL-2));

                return Result;
            }

            // 第一行处理（对第59行进行特殊处理，初始化边界）
            uint8 First_Line_Handle(uint8 *data)
            {
                uint8 i = 59;   // 控制行（第59行，图像底部第一行）
                uint8 Weight_Left, Weight_Right;    // 左右赛道宽度（用于判断有效边界）
                uint8 Mid_Left, Mid_Right;
                uint8 black_count = 0; // 黑色像素计数（用于判断是否为无效图像）
                Left_Add_Flag[59] = 0;
                Right_Add_Flag[59] = 0;     // 初始化补线标记（第59行默认不需要补线）
                Image_Para_Init(); // 初始化图像参数
                // 若第59行的中线位置（继承第60行）为黑色，可能是出赛道或不在赛道中心
                if (data[i*COL + Mid_Line[i+1]])
                {
                    // 统计第59行的黑色像素数量（判断是否为全黑图像）
                    for (uint8 j = 1; j < COL-2; j++)
                    {
                        if (data[i*COL + j] )
                        {
                            black_count++;
                        }
                        if (black_count > 100) // 黑色像素超过100，判定为无效图像
                        {
                            return 0;
                        }
                    }

                    // 从左侧查找边界，返回左赛道宽度；从右侧查找边界，返回右赛道宽度
                    Weight_Left = Traversal_Left(i, data, &Mid_Left, 1, (COL-2));
                    Weight_Right = Traversal_Right(i, data, &Mid_Right, 1, (COL-2));

                    // 左赛道宽度大于右赛道宽度且左宽度非0，使用左侧遍历获取边界
                    if (Weight_Left >= Weight_Right && Weight_Left)
                    {
                          Traversal_Left_Line(i, data, Left_Line, Right_Line);
                    }
                    // 右赛道宽度大于左赛道宽度且右宽度非0，使用右侧遍历获取边界
                    else if (Weight_Left < Weight_Right && Weight_Right)
                    {
                          Traversal_Right_Line(i, data, Left_Line, Right_Line);
                    }
                    else    // 未检测到有效边界，返回失败
                    {
                          return 0;
                    }
                }
                else // 第59行中线位置为白色（正常赛道），从中心向两侧查找边界
                {
                      Traversal_Mid_Line(i, data, Mid_Line[i+1], 1, (COL-2), Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
                }

                // 第60行边界继承第59行（第60行为虚拟行，用于辅助计算）
                Left_Line[60] = Left_Line[59];
                Right_Line[60] = Right_Line[59];
                Left_Add_Line[60] = Left_Add_Line[59];
                Right_Add_Line[60] = Right_Add_Line[59];

                // 若第59行左右均需补线，中线继承第60行；否则重新计算中线
                if (Left_Add_Flag[59] && Right_Add_Flag[59])
                {
                    Mid_Line[59] = Mid_Line[60];
                }
                else
                {
                    Mid_Line[59] = (Right_Line[59] + Left_Line[59]) / 2;
                    Mid_Line[60] = Mid_Line[59]; // 更新第60行中线，供下一次图像使用
                }
                // 赛道宽度继承（第60行继承第59行）
                Width_Real[60] = Width_Real[59];
                Width_Add[60] = Width_Add[59];
                Width_Min = Width_Add[59]; // 初始化最小赛道宽度为第59行宽度
                // 若中线列坐标超出合理范围（5~115），强制设为中心60
                if (Mid_Line[59] < 5 || Mid_Line[59] > 115)
                {
                    Mid_Line[59] = 60;
                    Mid_Line[60] = Mid_Line[59];
                }
                return 1; // 第一行处理成功
            }

            /*
            *   从左侧开始查找边界，返回赛道宽度
            *
            *   说明：本函数仅作为探测赛道使用，仅返回赛道宽度，不保存边界数据
            */
            uint8 Traversal_Left(uint8 i, uint8 *data, uint8 *Mid, uint8 Left_Min, uint8 Right_Max)
            {
                uint8 j, White_Flag = 0; // White_Flag=0：未找到左边界；=1：已找到左边界
                uint8 Left_Line = Left_Min, Right_Line = Right_Max; // 初始化左右边界为最小/最大列

                // 从Left_Min+Gray_Num列到Right_Max-Gray_Num列，逐列查找边界（跳过边缘灰度区域）
                for (j = Left_Min+Gray_Num; j <= Right_Max-Gray_Num; j+=1)
                {
                      if (!White_Flag)  // 先查找左边界
                      {
                          // 左边界可能掉线：若左边缘灰度区域为白色，直接设为左边界
                          if(data[i*COL + Left_Min+Gray_Num])
                          {
                                Left_Line = Left_Min+Gray_Num;
                                White_Flag = 1; // 左边界已找到，下一步找右边界
                                j += Gray_Num; // 跳过灰度区域，避免重复检测
                                continue;
                          }
                          // 检测到黑白跳变（黑色→白色），判定为左边界
                          if (data[i*COL + j+Gray_Num]!=data[i*COL + j])
                          {
                              Left_Line = j;
                              White_Flag = 1; // 左边界已找到，下一步找右边界
                              j += Gray_Num; // 跳过灰度区域
                              continue;
                          }
                      }
                      else // 已找到左边界，查找右边界
                      {
                          // 检测到白黑跳变（白色→黑色），判定为右边界
                          if (data[i*COL + j]!=data[i*COL + j+Gray_Num])
                          {
                              Right_Line = j;
                              break; // 左右边界均找到，结束循环
                          }
                      }
                }
                if (!White_Flag)    // 未找到左右边界，返回0
                {
                    return 0;
                }
                else // 找到边界，计算中线并返回赛道宽度
                {
                    *Mid = (Right_Line + Left_Line) / 2;
                    return (Right_Line - Left_Line);
                }
            }

            /*
            *   从右侧开始查找边界，返回赛道宽度
            *
            *   说明：本函数仅作为探测赛道使用，仅返回赛道宽度，不保存边界数据
            */
            uint8 Traversal_Right(uint8 i, uint8 *data, uint8 *Mid, uint8 Left_Min, uint8 Right_Max)
            {
                uint8 j, White_Flag = 0; // White_Flag=0：未找到右边界；=1：已找到右边界
                uint8 Left_Line = Left_Min, Right_Line = Right_Max; // 初始化左右边界为最小/最大列

                // 从Right_Max-Gray_Num列到Left_Min+Gray_Num列，倒序逐列查找边界（跳过边缘灰度区域）
                for (j = Right_Max-Gray_Num; j >= Left_Min+Gray_Num; j-=1)
                {
                    if (!White_Flag)        // 先查找右边界
                    {
                        // 右边界可能掉线：若右边缘灰度区域为白色，直接设为右边界
                        if(data[i*COL +  Right_Max-Gray_Num] )
                         {
                                Right_Line = Right_Max-Gray_Num;
                                White_Flag = 1; // 右边界已找到，下一步找左边界
                                j -= Gray_Num; // 跳过灰度区域
                                continue;
                         }
                         // 检测到黑白跳变（黑色→白色），判定为右边界
                        if (data[i*COL + j-Gray_Num]!=data[i*COL + j])
                        {
                            Right_Line = j;
                            White_Flag = 1; // 右边界已找到，下一步找左边界
                            j -= Gray_Num; // 跳过灰度区域
                            continue;
                        }
                    }
                    else // 已找到右边界，查找左边界
                    {
                        // 检测到白黑跳变（白色→黑色），判定为左边界
                        if (data[i*COL + j]!=data[i*COL + j - Gray_Num])
                        {
                            Left_Line = j;
                            break; // 左右边界均找到，结束循环
                        }
                    }
                }
                if (!White_Flag)    // 未找到左右边界，返回0
                {
                    return 0;
                }
                else // 找到边界，计算中线并返回赛道宽度
                {
                    *Mid = (Right_Line + Left_Line) / 2;
                    return (Right_Line - Left_Line);
                }
            }


            /*
            *   从左侧开始查找边界，保存赛道边界，返回1成功 0失败
            *
            *   说明：本函数使用后将保存边界数据到Left_Line/Right_Line数组
            */
            uint8 Traversal_Left_Line(uint8 i, uint8 *data, uint16 *Left_Line, uint16 *Right_Line)
            {
                uint8 j, White_Flag = 0; // White_Flag=0：未找到左边界；=1：已找到左边界

                // 初始化当前行左右边界为最小/最大列
                Left_Line[i] = 1;
                Right_Line[i] = (COL-1);

                // 从2列到COL-1-Gray_Num列，逐列查找边界（跳过边缘）
                for (j = 1+1; j < COL-1-Gray_Num; j += 1)
                {
                    if (!White_Flag)    // 先查找左边界
                    {
                        // 左边界可能掉线：若左边缘（2列）为白色，直接设为左边界
                        if(data[i*COL + 1+1] )
                        {
                                Left_Line[i] = 1+1;
                                White_Flag = 1; // 左边界已找到，下一步找右边界
                                j += Gray_Num; // 跳过灰度区域
                                continue;
                        }
                        // 检测到黑白跳变（黑色→白色），判定为左边界
                        if (data[i*COL + j+Gray_Num]!=data[i*COL + j])
                        {
                                Left_Line[i] = j;
                                White_Flag = 1; // 左边界已找到，下一步找右边界
                                j += Gray_Num; // 跳过灰度区域
                                continue;
                        }
                    }
                    else // 已找到左边界，查找右边界
                    {
                        // 检测到白黑跳变（白色→黑色），判定为右边界
                        if (data[i*COL + j]!=data[i*COL + j+Gray_Num])
                        {
                              Right_Line[i] = j-1;    // 右边界为跳变前一列
                              break; // 左右边界均找到，结束循环
                        }
                    }
                }
                // 若找到有效边界，更新补线边界和赛道宽度
                if (White_Flag  )
                {
                    Left_Add_Line[i] = Left_Line[i];
                    Right_Add_Line[i] = Right_Line[i];
                    Width_Real[i] = Right_Line[i] - Left_Line[i]; // 实际赛道宽度
                    Width_Add[i] = Width_Real[i]; // 补线赛道宽度（与实际一致）
                }

                return White_Flag;  // 返回查找结果（1成功，0失败）
            }
            /*
            *   从右侧开始查找边界，保存赛道边界，返回1成功 0失败
            *
            *   说明：本函数使用后将保存边界数据到Left_Line/Right_Line数组
            */
            uint8 Traversal_Right_Line(uint8 i, uint8 *data, uint16 *Left_Line, uint16 *Right_Line)
            {
                uint8 j, White_Flag = 0; // White_Flag=0：未找到右边界；=1：已找到右边界
                // 初始化当前行左右边界为最小/最大列
                Left_Line[i] = 1;
                Right_Line[i] = (COL-2);
                // 从COL-2列到3+Gray_Num列，倒序逐列查找边界（跳过边缘）
                for (j = (COL-1-1); j > 0+1+Gray_Num; j -= 1)
                {
                    if (!White_Flag)    // 先查找右边界
                    {
                          // 右边界可能掉线：若右边缘（COL-3列）为白色，直接设为右边界
                          if(data[i*COL +  (COL-3)] )
                          {
                              Right_Line[i] = (COL-2);
                              White_Flag = 1; // 右边界已找到，下一步找左边界
                              j -= Gray_Num; // 跳过灰度区域
                              continue;
                          }
                          // 检测到黑白跳变（黑色→白色），判定为右边界
                          if (data[i*COL + j-Gray_Num]!=data[i*COL + j])
                          {
                              Right_Line[i] = j;
                              White_Flag = 1;           // 右边界已找到，下一步找左边界
                              j -= Gray_Num; // 跳过灰度区域
                              continue;
                          }
                    }
                    else // 已找到右边界，查找左边界
                    {
                          // 检测到白黑跳变（白色→黑色），判定为左边界
                          if (data[i*COL + j]!=data[i*COL + j-Gray_Num])
                          {
                              Left_Line[i] = j+1; // 左边界为跳变后一列
                              break;        // 左右边界均找到，结束循环
                          }
                    }
                }
                // 若找到有效边界，更新补线边界和赛道宽度
                if (White_Flag )
                {
                    Left_Add_Line[i] = Left_Line[i];
                    Right_Add_Line[i] = Right_Line[i];
                    Width_Real[i] = Right_Line[i] - Left_Line[i]; // 实际赛道宽度
                    Width_Add[i] = Width_Real[i]; // 补线赛道宽度（与实际一致）
                }

                return White_Flag;      // 返回查找结果（1成功，0失败）
            }

            /*
            *   从中心向两侧搜索边界
            *
            *   说明：本函数使用后将保存边界数据到Left_Line/Right_Line等数组
            */
            void Traversal_Mid_Line(uint8 i, uint8 *data, uint8 Mid, uint8 Left_Min, uint8 Right_Max, uint16 *Left_Line, uint16 *Right_Line, uint16 *Left_Add_Line, uint16 *Right_Add_Line)
            {
                uint8 j;
                // 初始化补线标记（默认需要补线）
                Left_Add_Flag[i] = 1;
                Right_Add_Flag[i] = 1;
                // 初始化当前行左右边界为最小/最大列
                Right_Line[i] = Right_Max;
                Left_Line[i] = Left_Min;

                // 以Mid列为中心，向左查找左边界（倒序）
                for (j = Mid; j >= Left_Min+1+Gray_Num; j-=1)
                {
                    // 检测到黑→白跳变（左边界特征）
                    if (data[i*COL + j]!=data[i*COL + j - Gray_Num])
                    {
                        Left_Add_Flag[i] = 0;   // 左边界无需补线，清除补线标记
                        Left_Line[i] = j;       // 记录实际左边界
                        Left_Add_Line[i] = j;    // 补线左边界与实际一致
                        break;
                    }
                }
                // 以Mid列为中心，向右查找右边界
                for (j = Mid; j <= Right_Max-1*Gray_Num; j+=1)
                {
                    // 检测到白→黑跳变（右边界特征）
                    if (data[i*COL + j]!=data[i*COL + j + Gray_Num])
                    {
                        Right_Add_Flag[i] = 0;      // 右边界无需补线，清除补线标记
                        Right_Line[i] = j;      // 记录实际右边界
                        Right_Add_Line[i] = j;  // 补线右边界与实际一致
                        break;
                   }
                }
                // 若左边界需要补线
                if (Left_Add_Flag[i])
                {
                    if (i >= 57)    // 前3行（57-59行）：使用最后一行（59行）的边界数据补线
                    {
                        Left_Add_Line[i] = Left_Line[59];
                    }
                    else                     // 其他行：使用上一行（i+1）的补线边界补线
                    {
                        Left_Add_Line[i] = Left_Add_Line[i+1];
                    }
                }
                // 若右边界需要补线
                if (Right_Add_Flag[i])
                {
                    if (i >= 57)    // 前3行（57-59行）：使用最后一行（59行）的边界数据补线
                    {
                        Right_Add_Line[i] = Right_Line[59];
                    }
                    else // 其他行：使用上一行（i+1）的补线边界补线
                    {
                        Right_Add_Line[i] = Right_Add_Line[i+1];
                    }
                }

                // 计算实际赛道宽度和补线赛道宽度
                Width_Real[i] = Right_Line[i] - Left_Line[i];
                Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];

            }


            /*
            *   补线修复
            *
            *   说明：有始有终才使用，直接使用两点斜率进行补线
            *
            */
            void Line_Repair(uint8 Start, uint8 Stop, uint8 *data, uint16 *Line, uint16 *Line_Add, uint8 *Add_Flag, uint8 Mode)
            {
                float res;
                uint8 i;    // 控制行
                float Ka, Kb;

                    if (Stop)   // 有起始行和结束行（有效补线范围）
                    {
                            // 起始行若小于等于57，向后偏移2行（避免边缘误差）
                            if (Start <= 57)
                            {
                                Start +=2;
                            }
                            // 计算Start行与Stop行之间的斜率和截距
                            Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);
                            Kb = 1.0*Line_Add[Start] - (Ka * Start);

                            // 遍历补线范围（从Stop行到Start行），计算并更新补线坐标
                            for (i = Stop; i <= Start; )
                            {
                                i += 1;
                                res = i * Ka + Kb;
                                // 对补线坐标进行限幅（确保在2~COL-2范围内）
                                Line_Add[i] = range_protect((int32)res, 2, (COL-2));
                            }
                    }
            }

            /*
            *   补线修复（两点补线专用）
            *
            *   说明：任意两点补一条直线
            *
            */
            void Point_Repair_Line(uint8 Start, uint8 Stop,uint16 position_start, uint16 position_stop ,uint16 *Line_Add)
            {
                float res;
                uint8 i;    // 控制行
                float ka, kb;
                // 计算两点（Start行-position_start列、Stop行-position_stop列）之间的斜率和截距
                ka = 1.0*(position_start - position_stop) / (Start - Stop);
                kb = 1.0*position_start - (ka * Start);

                // 遍历补线范围（从Stop行到Start行），计算并更新补线坐标
                for (i = Stop; i <= Start; )
                {
                    i += 1;
                    res = i * ka + kb;
                    // 对补线坐标进行限幅（确保在2~COL-2范围内）
                    Line_Add[i] = range_protect((int32)res, 2, (COL-2));
                }
            }




            /******** 限幅保护（防止数值超出范围） *********/
            int32 range_protect(int32 duty, int32 min, int32 max)
            {
                if (duty >= max) // 数值大于最大值，返回最大值
                {
                    return max;
                }
                else if (duty <= min) // 数值小于最小值，返回最小值
                {
                    return min;
                }
                else // 数值在范围内，返回原数值
                {
                    return duty;
                }
            }
            /******** 从左侧查找左边界（边界丢失时备用） *********/
            uint8 zuobianjie( uint8 *data,uint8 Left_Min,uint8 Right_Max)
            {
                uint8 i,j;
                // 从第6行到第60行（倒序），在列2-30范围内查找左边界
                for(i=60;i>5;i--)
                {
                    for (j =2; j <= 30; j+=1)
                    {
                        if(data[i*COL + j]) // 检测到白色像素（左边界特征）
                        {
                            Right_Flag = 1; // 置位右边界标记（可能为逻辑复用）
                            Left_Add_Line[i] = j; // 更新左补线边界
                            break;
                        }
                    }
                }
            }
            /******** 从右侧查找右边界（边界丢失时备用） *********/
            uint8 youbianjie( uint8 *data, uint8 Left_Min, uint8 Right_Max)
            {
                    // 从第5行到第60行，在列90-110范围内查找右边界
                    for(i=60;i>=5;i--)
                    {
                        for (j = 110; j >= 90; j-=1)
                        {
                                if(data[i*COL + j] ) // 检测到白色像素（右边界特征）
                                {
                                    Right_Flag = 1; // 置位右边界标记
                                    Right_Add_Line[i]=j; // 更新右补线边界
                                    break;
                                }
                        }
                    }
            }

            // 检测菱形出环（判断是否完全离开菱形区域）
            void  Find_Rhombus_Out(uint8 *data)
            {
                    aaa=0;
                    // 在第15-60行、列57-63范围内统计白色像素数量
                    for(int i=15;i<=60;i++)
                    {
                        for(int j=57;j<=63;j++)
                        {
                            if(data[i*COL+j]) // 检测到白色像素
                            {
                                aaa++; // 计数白色像素
                            }
                        }
                    }
                    if(aaa<2) // 白色像素数量少于2，判定为已出菱形环
                    {
                        gpio_set_level(P33_10,1); // 舵机动作（可能为回正）
                        Rhombus_Out=1; // 置位菱形出环标记
                    }
            }

            // 左转控制逻辑（包含速度、转向角度、编码器计数控制）
            void Turn_Left(void)
            {
                // 初始状态：无转向、无停车，进入左转准备
                if(Flag_turn == 0&&Flag_stop==0)
                {
                    Flag_stop = 7; // 设置左转速度（7为自定义速度等级）
                    Flag_turn = 3; // 置位左转准备标记
                    Distance_Test_ON = 1; // 开启编码器计数（用于检测转向距离）
                }
                // 左转准备状态：编码器计数超过50，进入左转执行
                if(Flag_stop == 7&&Distance_Test>50)
                {
                    Distance_Test = 0; // 编码器计数复位
                    Distance_Test_ON = 0; // 关闭编码器计数

                    Light_Flag=2; // 左转指示灯标记（2为左转灯）

                    Flag_turn = 1; // 置位左转执行标记
                    Flag_stop = 2; // 设置左转执行速度（2为自定义速度等级）

                    Distance_ON = 1; // 开启主编码器计数（用于控制左转角度）
                    Light_Flag=2;
                }
                // 左转执行状态：主编码器计数超过165，进入右转回正
                if(Flag_turn == 1 && Distance > 165)
                {
                    Distance_ON = 0; // 关闭主编码器计数
                    Distance = 0; // 主编码器计数复位
                    Light_Flag=0; // 指示灯复位

                    Flag_turn = 2; // 置位右转回正标记
                    Flag_stop = 3; // 设置右转回正速度（3为自定义速度等级）
                    Distance_Test_ON = 1; // 开启编码器计数（用于检测回正距离）
                }

                // 右转回正状态：编码器计数超过90，完成回正
                if(Flag_turn == 2 && Distance_Test > 90 )
                {
                    gpio_set_level(P33_10,1); // 舵机关闭（或回正）
                    Distance_Test = 0; // 编码器计数复位
                    Distance_Test_ON = 0; // 关闭编码器计数

                    Flag_turn = 0; // 转向标记复位
                    Flag_stop = 5; // 设置直行速度（5为自定义速度等级）
                    Distance_ON = 1; // 开启主编码器计数（用于控制直行距离）
                }
                // 直行状态：主编码器计数超过400，左转流程结束
                if(Flag_turn == 0 && Distance > 400 )
                {
                    gpio_set_level(P33_10,0); // 舵机完全关闭
                    Distance_ON = 0; // 关闭主编码器计数
                    Distance = 0; // 主编码器计数复位

                    Flag_turn = 0; // 转向标记复位
                    Flag_stop=5; // 保持直行速度

                    Rhombus_Flag=0; // 菱形识别标记复位
                    Deal_Flag=2; // 切换到下一个处理流程（2为右转流程）
                }

            }
            // 左转控制逻辑2（与Turn_Left类似，调整了转向角度和速度参数）
            void Turn_Left_1(void)
            {
                // 初始状态：左转准备/无转向、直行速度/无转向，进入左转准备
                if((Flag_turn == 3||Flag_turn == 0)&&(Flag_stop==5||Flag_turn == 0)&&Distance_ON ==0)
                {
                    Flag_stop = 7; // 设置左转速度（7为自定义速度等级）
                    Flag_turn = 3; // 置位左转准备标记
                    Light_Flag=4; // 左转指示灯标记（4为自定义左转灯等级）
                    Distance_Test_ON = 1; // 开启编码器计数
                }

                // 左转准备状态：编码器计数超过50，进入左转执行
                if(Flag_stop == 7&&Distance_Test>50)
                {
                    Light_Flag=0; // 指示灯复位
                    Distance_Test = 0; // 编码器计数复位
                    Distance_Test_ON = 0; // 关闭编码器计数

                    Light_Flag=2; // 左转指示灯标记

                    Flag_turn = 4; // 置位左转执行标记（与Turn_Left的标记区分）
                    Flag_stop = 2; // 设置左转执行速度

                    Distance_ON = 1; // 开启主编码器计数
                }
                // 左转执行状态：主编码器计数超过170，进入右转回正
                if(Flag_turn == 4 && Distance > 170)
                {

                    Distance_ON = 0; // 关闭主编码器计数
                    Distance = 0; // 主编码器计数复位
                    Light_Flag=0; // 指示灯复位

                    Flag_turn = 2; // 置位右转回正标记
                    Flag_stop = 3; // 设置右转回正速度
                    Distance_Test_ON = 1; // 开启编码器计数
                }

                // 右转回正状态：编码器计数超过100，完成回正
                if(Flag_turn == 2 && Distance_Test > 100 )
                {
                    Distance_Test = 0; // 编码器计数复位
                    Distance_Test_ON = 0; // 关闭编码器计数

                    Flag_turn = 0; // 转向标记复位
                    Flag_stop = 5; // 设置直行速度
                    Distance_ON = 1; // 开启主编码器计数
                }
                // 直行状态：主编码器计数超过180，左转流程结束
                if(Flag_turn == 0 && Distance > 180 )
                {
                    gpio_set_level(P33_10,1); // 舵机关闭（或回正）
                    Distance_ON = 0; // 关闭主编码器计数
                    Distance = 0; // 主编码器计数复位

                    Flag_turn = 0; // 转向标记复位
                    Flag_stop=6; // 设置直行速度（6为自定义速度等级）

                    Rhombus_Flag=0; // 菱形识别标记复位
                    Deal_Flag=5; // 切换到下一个处理流程（5为斑马线流程）
                    Distance_Test_ON = 1; // 开启编码器计数
                }

            }
            // 右转控制逻辑（包含速度、转向角度、编码器计数控制）
            void Turn_Right(void)
            {
               // 初始状态：无转向、直行速度，进入右转准备
               if(Flag_turn == 0&&Flag_stop==5)
               {
                    Flag_turn = 2; // 置位右转执行标记
                    Flag_stop = 3; // 设置右转速度

                    Distance_ON = 1; // 开启主编码器计数
                    Light_Flag=3; // 右转指示灯标记（3为右转灯）
                }
                // 右转执行状态：主编码器计数超过175，进入左转回正
                if(Flag_turn == 2 && Distance >175)
                {
                    Light_Flag=0; // 指示灯复位
                    Distance_ON = 0; // 关闭主编码器计数
                    Distance = 0; // 主编码器计数复位
                    Light_Flag=0;


                    gpio_set_level(P33_10,1); // 舵机动作（右转）
                    Flag_turn = 1; // 置位左转回正标记
                    Flag_stop = 2; // 设置左转回正速度
                    Distance_Test_ON = 1; // 开启编码器计数
                }

                // 左转回正状态：编码器计数超过90，完成回正
                if(Flag_turn == 1 && Distance_Test >90)
                {
                    gpio_set_level(P33_10,0); // 舵机关闭（或回正）
                    Distance_Test = 0; // 编码器计数复位
                    Distance_Test_ON = 0; // 关闭编码器计数
                    gpio_set_level(P20_13,1); // 自定义IO口控制（可能为指示灯）
                    gpio_set_level(P21_4,1); // 自定义IO口控制（可能为指示灯）
                    Flag_turn = 10; // 置位右转完成标记
                    Flag_stop = 5; // 设置直行速度

                    gpio_set_level(P21_5, 0); // 自定义IO口复位

                    Distance_ON = 1; // 开启主编码器计数
                 }
                // 直行状态：主编码器计数超过200，右转流程结束
                if(Flag_turn == 10 && Distance > 200 )
                 {
                    Rhombus_Flag=0; // 菱形识别标记复位

                    Deal_Flag=30; // 切换到下一个处理流程（30为减速流程）
                    Flag_turn = 0; // 转向标记复位
                    Flag_stop = 0; // 速度标记复位
                    Distance_ON = 0; // 关闭主编码器计数
                    Distance = 0; // 主编码器计数复位
                 }

            }
            // 减速控制逻辑（检测到减速区域后执行减速，直到离开区域）
            void Decelerate(uint8 *data)
            {
                Flag_stop=5; // 设置减速后速度（5为自定义速度等级）
                Flag_turn=3; // 置位减速状态标记


                Find_rhombus_Out(data); // 检测是否离开菱形区域
                if(Rhombus_Out_Flag==1) // 已离开菱形区域，减速流程结束
                {
                   Flag_stop=5; // 保持直行速度
                   Flag_turn=0; // 转向标记复位
                   Deal_Flag=Deal_Flag+1; // 切换到下一个处理流程
                   Rhombus_Flag=0; // 菱形识别标记复位
                   Rhombus_Out_Flag=0; // 菱形出环标记复位
               }


            }
            // 斑马线处理逻辑（检测到斑马线后控制停车/通行）
            void Zebra_Crossing(uint8 *data)
            {
                // 初始状态：无转向，进入斑马线处理
                if(Flag_turn==0)
                {
                    gpio_set_level(P33_10,1); // 舵机动作（可能为停车准备）
                    Flag_turn=5; // 置位斑马线处理标记
                    Flag_stop=5; // 设置斑马线通行速度
                    Distance_ON=1; // 开启主编码器计数
                    Rhombus_Out=0; // 菱形出环标记复位
                }
                // 主编码器计数超过80，开始检测是否离开斑马线
                if(Distance>80)
                {
                Find_Rhombus_Out(data); // 检测是否离开菱形区域（斑马线可能与菱形关联）
                }
                // 已离开菱形区域，斑马线流程结束
                if(Rhombus_Out==1)
                {

                    Distance=0; // 主编码器计数复位
                    Distance_ON=0; // 关闭主编码器计数

                    Flag_stop=5; // 保持直行速度
                    Flag_turn=0; // 转向标记复位

                    Pwm_Min=843; // 设置PWM最小值（舵机或电机参数）
                    Pwm_Max=886; // 设置PWM最大值（舵机或电机参数）

                    Rhombus_Out=0; // 菱形出环标记复位
                    Rhombus_Flag=0; // 菱形识别标记复位
                    if(mode_2024==1) // 模式1：切换到菱形检测流程（6为菱形检测）
                    Deal_Flag=6;
                    else // 模式2：切换到下一个处理流程（4为自定义流程）
                    Deal_Flag=4;
                }
            }

