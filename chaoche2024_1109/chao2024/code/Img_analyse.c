#include "zf_common_headfile.h"

/*
*   缁撴灉鍙橀噺 鐢ㄤ簬鍦ㄥ睆骞曟樉绀�
*/
uint8 result_screen;
uint8 Rhombus_Out_Flag;
uint8 aaa=0,Rhombus_Out;

uint32 time_dl1b;
uint32 time_dl1b_s;
uint8 time_dl1b_on;
uint8 mode_2024,tiaoguang;
uint8 Stop_Out;

/*************************鍥惧儚璧涢亾鏁版嵁****************************/
uint16 Left_Line[61], Right_Line[61], Mid_Line[61]; // 鍘熷宸﹀彸杈圭晫鏁版嵁
uint16 Left_Add_Line[61], Right_Add_Line[61];       // 宸﹀彸杈圭晫琛ョ嚎鏁版嵁
uint8 Left_Add_Flag[61], Right_Add_Flag[61];        // 宸﹀彸杈圭晫琛ョ嚎鏍囧織浣�
uint16 Width_Real[61];                                  // 瀹為檯璧涢亾瀹藉害
int16 Width_Add[61];                                    // 琛ョ嚎璧涢亾瀹藉害
int16 Width_Min;                                        // 鏈�灏忚禌閬撳搴�

uint8 Left_Add_Start, Right_Add_Start;  // 宸﹀彸琛ョ嚎璧峰琛屽潗鏍�
uint8 Left_Add_Stop, Right_Add_Stop;    // 宸﹀彸琛ョ嚎缁撴潫琛屽潗鏍�

uint8 Left_Line_Start, Right_Line_Start;  //  宸﹀彸 杈圭晫绾� 瀹炵嚎璧峰鐐�
uint8 Left_Line_Break, Right_Line_Break;   //  宸﹀彸 杈圭晫绾� 鏂偣

int8 Out_Side = 0;  // 涓㈢嚎鎺у埗
int8 Line_Count;        // 璁板綍鎴愬姛璇嗗埆鍒扮殑璧涢亾琛屾暟

int Count=0;
uint8 Last_Point;
uint8 turnleft;
/***********************璧涢亾鏂滅巼************************/
float Left_Ka = 0, Right_Ka = 0;
float Left_Kb = 1, Right_Kb = (COL - 1);    //  鏈�灏忎簩涔樻硶鍙傛暟
float Mid_qulv,Right_qulv,Left_qulv;

/************鐜矝鏍囧織 *****************/
uint8 LDrop_Line_Num , RDrop_Line_Num;  // 宸﹀彸 杩炵画 涓㈣竟 鏁伴噺 2020.5.26
uint8 Left_Linear_Flag, Right_Linear_Flag;     //  宸﹀彸 杈圭晫绾� 鏄惁 鏄� 鐩寸嚎 鐨勬爣蹇� 1 涓� 鐩寸嚎

uint8 Left_Ring_Flag,Right_Ring_Flag;    //  杩涜鐜矝璇嗗埆鐨勬爣蹇�
uint8 Ring_Flag= 0;
uint8 Flag1=0;          //0鐩撮亾  1 鐠板扯搴曢儴  2 鐠板扯绗簩鍏ュ彛  3 鐠板扯涓儴
uint8 Dirc;           //1宸�  2鍙�
uint8 Ring_Enter = 0;
uint8 Ring_Enter1 = 0;
uint8 Cycle_Flag;
/************鍋滆溅鏍囧織 *****************/
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
uint32 distance_0,distance_1,distance_2;                //杞崲鍑烘潵鐨勮窛绂�  鍗曚綅涓哄帢绫�
uint8 Distance_Left_Flag,Distance_Right_Flag;
uint8 Stadiometry_Flag1,Stadiometry_Flag2,Stdiometry_Flag3;
uint8 Light_Flag,Light_Flag1;
uint8 Num_Turn_L,Num_Turn_R;
int32 Time_Cal;
/*
 *
 *
*   鍥惧儚绠楁硶鍙傛暟鍒濆鍖�
*
*   璇存槑锛氫粎褰卞搷绗竴琛岀壒娈婂鐞�
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
*   鍥惧儚澶勭悊绠楁硶
*
*   璇存槑锛氬鐞嗘櫘閫氬浘鍍忥紝鍖呮嫭鍗佸瓧銆侀殰纰�
*/
void Image_Handle(uint8 *data)
{       //鍙橀噺璁剧疆
    uint8 i;    // 鎺у埗琛�
    uint8 j;
    uint8 res;
    Line_Count = 0; // 璧涢亾琛屾暟澶嶄綅
    Width_Min = COL-1;
    //        Count++;     //  涓嶇煡閬撶敤鏉ュ共鍢�  鎴戠敤鏉� 鏃堕棿璁℃暟
    //=====================================================
    Stop1=1;
    Left_Add_Start = 0;  // 宸﹀浣嶈ˉ绾胯捣濮嬭鍧愭爣
    Right_Add_Start = 0; // 鍙冲浣嶈ˉ绾胯捣濮嬭鍧愭爣
    Left_Add_Stop = 0;   // 宸﹁ˉ绾跨粓姝㈣鍧愭爣
    Right_Add_Stop = 0;  // 鍙宠ˉ绾跨粓姝㈣鍧愭爣

    Left_Line_Start = 0;   //宸﹀彸 杈圭晫绾� 瀹炵嚎璧峰鐐�
    Right_Line_Start = 0;
    Left_Line_Break = 0;   //  宸﹀彸 杈圭晫绾� 鏂偣
    Right_Line_Break = 0;

    LDrop_Line_Num = 0;    // 宸﹀彸 杩炵画 涓㈣竟 鏁伴噺
    RDrop_Line_Num = 0;
    //Out_Side = 0;         // 涓㈢嚎鎺у埗
    Line_Count = 59;      // 璁板綍鎴愬姛璇嗗埆鍒扮殑璧涢亾琛屾暟
    /***************************** 绗竴琛岀壒娈婂鐞� *****************************/
    if(!Right_Flag)
    {
        res = First_Line_Handle(data);
    }
    if (res == 0)
    {
        //            if (g_fDirectionSum[0] + g_fDirectionSum[1] < 50 )
        {
            //                        Out_Side = 1;
        }  // 涓㈢嚎 鍋滆溅#

        return ;
    }
    /*************************** 绗竴琛岀壒娈婂鐞嗙粨鏉� ***************************/
    for (i = 59;i >= 5;)
    {
        i -= 1;
        if (data[(i-2)*COL + Mid_Line[i+1]] && data[(i-1)*COL + Mid_Line[i+1]]) //  鍓嶈涓偣鍦ㄦ湰琛屼负榛戠偣锛屽彲鑳芥槸璧涢亾缁撴潫锛屼篃鍙兘鏄幆璺�
        {
            if(i <= 50)     //  闃叉鎵殑绾胯鏁板お灏�
                break;
        }
        else  // 浣跨敤鍓�2琛屼腑鐐瑰悜涓よ竟鎵弿杈圭晫
        {
            Traversal_Mid_Line(i, data, Mid_Line[i+1], 1, (COL-3), Left_Line, Right_Line, Left_Add_Line, Right_Add_Line);
        }
        /**************************** 琛ョ嚎妫�娴嬪紑濮� ****************************/
        if (Width_Real[i] > Width_Min+10)//&&(Left_Add_Flag[i]||Right_Add_Flag[i]))   // 璧涢亾瀹藉害鍙樺锛屽彲鑳芥槸鍗佸瓧
        {
            if (Left_Add_Line[i] <  Left_Add_Line[i+1])
            {
                if (!Left_Add_Flag[i])
                {
                    Left_Add_Flag[i] = 1;       // 寮哄埗璁ゅ畾涓洪渶瑕佽ˉ绾�
                }
            }
            if (Right_Add_Line[i] > Right_Add_Line[i+1])
            {
                if (!Right_Add_Flag[i])
                {
                    Right_Add_Flag[i] = 1;       // 寮哄埗璁ゅ畾涓洪渶瑕佽ˉ绾�
                }
            }
        }

        /******************** 绗簩杞� 琛ョ嚎妫�娴嬪紑濮� ******************************/
        if (Left_Add_Line[i] > Left_Add_Line[i+1]+10 && !Left_Add_Flag[i+1])  // 濡傛灉杩欐鐨勭偣 璺濈涓婁竴涓偣浣嶇疆澶繙 涓斾笂涓�涓偣涓嶈琛ョ嚎
        {
            if (!Left_Add_Flag[i])
            {
                Left_Add_Flag[i] = 1;       // 寮哄埗璁ゅ畾涓洪渶瑕佽ˉ绾�
            }
        }
        if (Right_Add_Line[i] < Right_Add_Line[i+1]-10 && !Right_Add_Flag[i+1])  // 濡傛灉杩欐鐨勭偣 璺濈涓婁竴涓偣浣嶇疆澶繙 涓斾笂涓�涓偣涓嶈琛ョ嚎
        {
            if (!Right_Add_Flag[i])
            {
                Right_Add_Flag[i] = 1;       // 寮哄埗璁ゅ畾涓洪渶瑕佽ˉ绾�
            }
        }

        /*************************** 绗竴杞ˉ绾垮紑濮� ***************************/
        if (Left_Add_Flag[i])   // 宸︿晶闇�瑕佽ˉ绾�
        {
            if (i > 10 && i < 53)
            {
                LDrop_Line_Num++;
                // 宸︿涪绾挎暟鐩�
            }
            if (i >= 55)    // 鍓�5琛岃ˉ绾夸笉鐢ㄨ绠�
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
                if (!Left_Add_Start)    // 涔嬪墠娌℃湁琛ョ嚎
                {
                    Left_Add_Start = i+1; // 璁板綍宸︿晶琛ョ嚎寮�濮嬭
                }
                if (Left_Line_Break && (Left_Line_Start -Left_Line_Break) >= 5)    // 濡傛灉 宸︽柇鐐瑰瓨鍦� 鐢ㄨ捣濮嬬偣鏂滅巼琛ョ嚎
                {
                    Curve2_Fitting(&Left_Ka, &Left_Kb,Left_Line_Start,Left_Line_Break,Left_Add_Line,1);
                }
                else
                    Curve_Fitting(&Left_Ka, &Left_Kb, &i, Left_Add_Line, Left_Add_Flag, 1);    //  琛ョ嚎
                Left_Add_Line[i] = Calculate_Add(i, Left_Ka, Left_Kb);  // 琛ョ嚎瀹屾垚
            }
        }
        else   //  涓嶉渶瑕佽ˉ绾�  璁颁笅琛ョ嚎缁撴潫琛�  鐢ㄦ潵鏈�鍚庤繘琛屼袱鐐硅ˉ绾�
        {
            if (!Left_Line_Start  && i > 50 && i < 56)    // 璁颁笅 宸﹀疄闄呰竟鐣岀嚎鐨勮捣鐐�
            {
                Left_Line_Start = i;
            }
            if (Left_Add_Start) // 宸茬粡寮�濮嬭ˉ绾�
            {
                if (!Left_Add_Stop && !Left_Add_Flag[i+1]) // && !Left_Add_Flag[i+2])
                {
                    if ((Left_Add_Line[i] > Left_Add_Line[i+1])) // && (Left_Add_Line[i+1] > Left_Add_Line[i+2]))
                    {
                        Left_Add_Stop = i+1;        // 璁板綍宸︿晶琛ョ嚎缁撴潫琛�
                    }
                }
            }
        }

        if (Right_Add_Flag[i])  // 鍙充晶闇�瑕佽ˉ绾�
        {
            if (i > 10 && i<53)
            {
                RDrop_Line_Num++;
            }
            if(Right_Flag) break;
            if (i >= 55)    // 鍓嶄笁琛岃ˉ绾夸笉绠�
            {
                if (!Right_Add_Start)
                {
                    Right_Ka = 0;
                    Right_Kb = Right_Add_Line[i];
                }

                Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);   // 浣跨敤鍓嶄竴甯у浘鍍忓彸杈圭晫鏂滅巼琛ョ嚎
            }
            else
            {
                if (!Right_Add_Start)       // 涔嬪墠娌℃湁琛ョ嚎
                {
                    Right_Add_Start = i+1;  // 璁板綍鍙充晶琛ョ嚎寮�濮嬭
                }
                if (Right_Line_Break && (Right_Line_Start - Right_Line_Break) >= 5)    // 濡傛灉 宸︽柇鐐瑰瓨鍦� 鐢ㄨ捣濮嬬偣鏂滅巼琛ョ嚎
                {
                    Curve2_Fitting(&Right_Ka, &Right_Kb,Right_Line_Start,Right_Line_Break,Right_Add_Line,2);
                }
                else
                    Curve_Fitting(&Right_Ka, &Right_Kb, &i, Right_Add_Line, Right_Add_Flag, 2);   // 浣跨敤涓ょ偣娉曟嫙鍚堢洿绾�

                Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);// 琛ョ嚎瀹屾垚
            }
        }
        else     //  涓嶉渶瑕佽ˉ绾�  璁颁笅琛ョ嚎缁撴潫琛�  鐢ㄦ潵鏈�鍚庤繘琛屼袱鐐硅ˉ绾�
        {
            if (!Right_Line_Start)    // 璁颁笅 宸﹀疄闄呰竟鐣岀嚎鐨勮捣鐐�
            {
                Right_Line_Start = i;
            }
            if (Right_Add_Start)    // 宸茬粡寮�濮嬭ˉ绾�
            {
                if (!Right_Add_Stop && !Right_Add_Flag[i+1]) // && !Right_Add_Flag[i+2])
                {
                    if ((Right_Add_Line[i] < Right_Add_Line[i+1]))// && (Right_Add_Line[i+1]<Right_Add_Line[i+2]))
                    {
                        Right_Add_Stop = i+1; // 璁板綍鍙充晶琛ョ嚎缁撴潫琛�
                    }
                }
            }
        }
        /*************************** 绗竴杞ˉ绾跨粨鏉� ***************************/
        Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];        // 閲嶆柊璁＄畻璧涢亾瀹藉害
        Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2;   // 璁＄畻涓嚎

        if (Width_Add[i] < Width_Min) //&& !Left_Add_Flag[i] && !Right_Add_Flag[i])
        {
            Width_Min = Width_Add[i];                               // 鏇存柊鏈�灏忚禌閬撳搴�
        }
        Line_Count = i;                                            //     璁板綍鎴愬姛璇嗗埆鍒扮殑璧涢亾琛屾暟
        if(Left_Add_Stop && Right_Add_Stop)                      //琛ョ嚎缁撴潫鍚庡張瑕佽ˉ绾胯繖涓嶈ˉ
        {
            if(Left_Add_Flag[i]||Right_Add_Flag[i])
            {
                Line_Count = i+2;
                break;
            }
        }
        //             if(Right_Add_Line[i] < 5)
        //                 Right_Add_Line[i] = Right_Add_Line[i+1];
    }//for绲愭潫



    /*************************** 绗簩杞ˉ绾夸慨澶嶅紑濮� ***************************/
    if (Left_Add_Stop)      // 宸﹁竟鐣岄渶瑕佽ˉ绾�
    {
        Line_Repair(Left_Add_Start, Left_Add_Stop, data, Left_Line, Left_Add_Line, Left_Add_Flag, 1);
    }
    if (Right_Add_Stop) // 鍙宠竟鐣岄渶瑕佽ˉ绾�
    {
        Line_Repair(Right_Add_Start, Right_Add_Stop, data, Right_Line, Right_Add_Line, Right_Add_Flag, 2);
    }
    /*************************** 绗簩杞ˉ绾夸慨澶嶇粨鏉� ***************************/
    for (i = 59; i >= Line_Count;)   // 閲嶆柊璁＄畻 涓��
    {
        i -= 1;
        Mid_Line[i] = (Right_Add_Line[i] + Left_Add_Line[i]) / 2+Differ ;

    }


    /************************************************  杩愬姩澶勭悊鍑芥暟2024   ******************************************************/
    /************************************************  杩愬姩澶勭悊鍑芥暟2024   ******************************************************/
    /************************************************  杩愬姩澶勭悊鍑芥暟2024   ******************************************************/
    /************************************************  杩愬姩澶勭悊鍑芥暟2024   ******************************************************/

        if(tiaoguang==1)
        {
            Tiao_Light(data);
        }
        if(mode_2024==2)
        {
            if(Deal_Flag==0)
            {

                Flag_turn=3;
                Flag_stop=0;
                Find_rhombus_Out(data);
                if(Rhombus_Out_Flag==1)
                {
                Flag_turn=0;
                Deal_Flag=1;
                Rhombus_Out_Flag=0;

                }
            }
            if(Deal_Flag==1&&Rhombus_Flag==0)//璇嗗埆鍒伴殰纰嶇墿
            {
                Find_Decelerate(data);
            }
            if(Deal_Flag==2&&Rhombus_Flag==0)//璇嗗埆鍒伴殰纰嶇墿
            {
                Find_Decelerate(data);
            }
            if(Deal_Flag==3&&Rhombus_Flag==0)//璇嗗埆鍒伴殰纰嶇墿
            {
                Find_Rhombus(data);
            }
            if(Deal_Flag==4&&Rhombus_Flag==0)//璇嗗埆鍒伴殰纰嶇墿
            {
                Find_rhombus(data);
            }
        }
        if(mode_2024==1)
        {
            if(Deal_Flag==0)
            {
                Flag_turn=3;
                Flag_stop=0;
                Find_rhombus_Out(data);
                if(Rhombus_Out_Flag==1)
                {
                    Flag_turn=0;
                    Deal_Flag=1;
                    Rhombus_Out_Flag=0;

                }
            }
            if(Deal_Flag==1&& dl1b_distance_mm<900&&Rhombus_Flag==0)//识锟斤拷锟较帮拷锟斤拷
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
                if(dl1b2_distance_mm<520)
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


    //////////////////////////////////////////////////////鎵ц鍖�/////////////////////////////////////////////////


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
                        Zebra_Crossing_2(data);//6
                        break;
                    }
                case 4:
                    {

                        Flag_stop=0;
                        Flag_turn=8;
                        //Rhombus_Flag=0;
                        Distance_ON=1;
                        Deal_Flag=5;
                        break;
                    }
                case 5:
                    {
                        Find_Stop_Left(data);
                        if(Distance>50&&Stop_Out )
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
                            Turn_Left();//6
                            break;
                        }
                    case 2:
                        {
                            Turn_Right();//0
                            break;
                        }
                    case 30:
                        {
                            Decelerate(data);//6
                            break;
                        }
                    case 4:
                        {
                            Turn_Left_1();//6
                            break;
                        }
                    case 5:
                        {
                            Zebra_Crossing(data);//6
                            break;
                        }
                    case 6:
                        {
                            Flag_stop=5;
                            Flag_turn=5;
            //                Rhombus_Flag=0;
                            Distance_ON=1;
                            Deal_Flag=7;
                            break;
                        }
                    case 7:
                        {
                            Find_Stop_Right(data);
                            if(Distance>50&&Stop_Out==1)
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
//        Turn_Left();
//    }
//    if(Deal_Flag==2&&Rhombus_Flag==1)
//    {
//        Turn_Right();
//    }
//    if(Deal_Flag==3&&Rhombus_Flag==1)
//    {
//        Decelerate(data);
//    }
//    if(Deal_Flag==4&&Rhombus_Flag==1)
//    {
//        Decelerate(data);
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
//        Flag_stop=1;
//    }
//    if(Flag_turn == 1 && Distance > 124 )
//    {
//        gpio_set_level(P33_10,0);//铚傞福鍣ㄥ叧闂�
//        Flag_turn = 2;//鍙宠浆鐭
//        Distance_ON = 0;
//        Distance = 0;
//        Flag_stop = 5;//鍙宠浆鐭閫熷害
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
//            Flag_turn = 2;//鍙宠浆
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
//        Distance_Test_ON = 0;//鏈潵鏄�1
//        gpio_set_level(P21_5, 0);
//    }
//


    //濡傛灉渚ц竟鐨凾OF璺濈鍙樺寲锛屽氨涓嶆墽琛岃彵褰紝鐩存帴鎷愬集锛岀洿鎺ヨ瘑鍒枒椹嚎锛屽仠杞�
//    if(Turn_Flag == 2 && Distance_Test > Distances)//瀵绘壘鑿卞舰
//    {
//        Measurement = 1;
//        Find_rhombus(data);
//    }
//    if(Turn_Flag == 2 && Rhombus_Flag == 1)//鎵惧埌绗竴涓彵褰�
//    {
//        Flag_turn = 3;
//        Distance_Test_ON = 0;
//        Distance_Test = 0;
//        gpio_set_level(P33_10,1);
//        Flag_stop = 0;
//        Turn_Flag = 3;//绗竴涓彵褰�
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
//            Turn_Flag = 4;//绗竴涓彵褰㈢粨鏉�
//            Distance_ON = 0;
//            Distance = 0;
//        }
//    }
//    if(Turn_Flag == 4 && !Dis_Flag)//瀵绘壘鑿卞舰
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
//        Turn_Flag = 5;//绗簩涓彵褰�
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
//            Turn_Flag = 6;//绗簩涓彵褰㈢粨鏉�
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
//            if(Turn_Flag > 2)//鑿卞舰涔嬪悗
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
//    if(Dis_Flag == 1)//璇嗗埆鍒版湁杞�
//    {
//        Dis_Flag = 2;
//    }
//    if(Dis_Flag == 2)
//    {
//        gpio_set_level(P33_10,1);
//        Distance_ON = 1;
//        Flag_turn = 1;//宸﹁浆
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



  //鏃舵尟鐟�
//    if(Dis_Flag == 4 && !Start_Line_Flag)
//    {
//        Flag_stop = 4;//鍔犻��
//        Flag_turn = 0;
//        Start_Line_Judg(data);//瀵绘壘鏂戦┈绾�
//    }
//    if(Dis_Flag == 4 && Start_Line_Flag == 1)
//    {
//        Flag_stop = 4;
//        gpio_set_level(P33_10,1);
//        Dis_Flag = 5;//鎵惧埌鏂戦┈绾�
//    }
//    if(Dis_Flag == 5)
//    {
//        Flag_stop = 4;
//        Flag_turn = 3;
//        gpio_set_level(P33_10,0);
//        Distance_ON = 1;
//        if(Distance > 25)//鍘熸湰200
//        {
//            Flag_stop = 1;//鍘熸湰涓�0
//            Dis_Flag = 6;
//            Distance_ON = 0;
//            Distance = 0;
//            Time_banmaxian=1;
//            Turn_Flag=10;
//        }
//    }


    //鏃舵尟鐟�
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
//        Dis_Flag = 7;//瑕佺粨鏉�        Time_banmaxian_Flag
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
//            if(Distance_Test > 20)//鍘熸湰60
//            {
//                Flag_turn = 3;
//                Dis_Flag = 10;
//                Flag_stop = 1;//鍋滆溅
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
}     //姝ゅ涓篿mage缁撴潫

            //浠ヤ笅涓哄瓙鍑芥暟
uint8 Start_Line_Judg(uint8 *data)   // 鏂戦┈绾垮垽鍒�
{
    uint8 j;
    uint8 i,white_count,k,a;
    for(i = 25;i>15;i--)
    {
        k= 0,a=0 ;
        for(j = 20;j<100;j++)
        {
            if(data[i*COL + j])//鐧�
            {
                white_count=1;
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
    for(i = 45;i>40;i--)
    {
        for(j = 55;j<65;j++)
        {
            if(data[i*COL+j])
            {
                Stop_Flag=1;
            }
        }
    }

}
//鍒ゆ柇鑿卞舰
void Find_rhombus(uint8 *data)
{
    uint8 i,j,a=0;
    for(i=30;i<=55;i++)
    {
        for(j=58;j<62;j++)
        {
            if(data[i*COL+j])
            {
                a++;
            }
        }
        if(a>2)
        {
            Rhombus_Flag = 1;
            break;
        }
    }
}
void Find_Rhombus(uint8 *data)
{
    uint8 i,j,a=0;
    for(i=50;i>=25;i--)
    {
        for(j=52;j<68;j++)
        {
            if(data[i*COL+j])
            {
                a++;
            }
        }
        if(a>3)
        {
            Rhombus_Flag = 1;

            break;
        }
    }
}
void Find_Decelerate(uint8 *data)
{
    uint8 i,j,a=0;
    for(i=55;i>24;i--)
    {
        for(j=58;j<62;j++)
        {
            if(data[i*COL+j])
            {
                a++;
            }
        }
        if(a>2)
        {
            Rhombus_Flag = 1;

            if(Deal_Flag>=31)
            {
                Deal_Flag=4;
            }
            break;
        }
    }
}
void Find_rhombus_Out(uint8 *data)
{
    uint8 i,j,a=0;
    for(i=65;i>20;i--)
    {
        for(j=58;j<62;j++)
        {
            if(data[i*COL+j])
            {
                a++;

            }
        }
    }
    if(a<3)
    {
        Rhombus_Out_Flag =1;
    }
}
/*********鑷富杈圭晫*************************/
void New_boundary(uint8 *data, float *Ka, float *Kb)
{
    float ka,kb;
    uint8 res;
    Zhuan_Flag=1;
    if(Zhuan_Flag)
    {
    for(i=15;i<18;i++)
    {
        for(j=35;j>30;j--)
        {
            if(data[i*COL+j]&&!data[(i-1)*COL+j]&&!data[(i+1)*COL+j])
            {
                a=i;
                b=j;
            }
        }
    }
    a1=60;
    b1=98;
        ka = 1.0*(b1-b) / (a1-a);
        kb = 1.0*b - (ka*a);
        res = a * ka + kb;
        res = range_protect((int32)res, 1, (COL-2)); // 闄愬箙
        *Ka = ka;
        *Kb = kb;
    Right_Ka=*Ka;
    Right_Kb=*Kb;
    for(i=60;i>5;i--)
    {
        Right_Add_Line[i] = Calculate_Add(i, Right_Ka, Right_Kb);
        Left_Add_Line[i]=4;
        Mid_Line[i] = (Right_Add_Line[i]+Left_Add_Line[i]) / 2;
    }
    }
}
/*********杈呭姪鍒ゅ埆閿ユ《*************************/
uint8 Find_Cone_bucket(uint8 *data)
{
    uint8 i,k;
    for(j=0;j<10;j++)
    {
        for(i=60;i<64;i++)
        {
            if(data[i*COL+j])
            {
                k++;
                if(k>10)
                {
                Cone_bucket=1;
                zyuzhi = 38;
                }
            }
        }
    }
}
//  浠庢煇涓�鍒� 浠庡簳鍚戜笂鎼滃缁堢偣  杩斿洖 琛屾暟
uint8 Find_J_Black(uint8 *data, uint8 j,uint8 start,uint8 end)
{
    uint8 i;
    uint8 result = 0;

      for(i=start;i>=end;)
      {
        i -= 1;
        if(data[(i+Gray_Num)*COL + j]!=data[i*COL + j])
        {
             result = i;
             return result;
        }
      }
     return i;
}


/*
*   涓ょ偣娉曟眰鐩寸嚎鏂滅巼
*
*   璇存槑锛氭嫙鍚堢洿绾� y = Ka * x + Kb  鐨勬枩鐜�
*/
void Curve_Fitting(float *Ka, float *Kb, uint8 *Start, uint16 *Line, uint8 *Add_Flag, uint8 Mode)
{
    uint8 start=0,start2,start3;
    uint8 stop=0;
    float ka,kb;
    float res;
    for (uint8 i = *Start+2; i < 57; i++) // 浠庤琛ョ嚎鐨勭偣 寰�涓嬫壘鍒颁笉闇�瑕佽ˉ绾跨殑鐐� 璁颁负鏂滅巼璁＄畻璧风偣
    {
        if (!Add_Flag[i])
        {
            start = i;
            break;
        }
    }
    if (start)  // 鏂滅巼璁＄畻 璧风偣鎵惧埌
    {
        for (uint8 i = start+2; i < start+10; i++) // 浠庢枩鐜囪绠楄捣鐐� 寰�涓嬫壘 涓嶉渶瑕佽ˉ绾跨殑鐐� 璁颁负鏂滅巼璁＄畻缁堢偣
        {
            if (!Add_Flag[i])
            {
                stop = i+1;
                break;
            }
            if (i > 58)
            {
                break;
            }
        }
    if (stop)        // 缁堢偣鎵惧埌
        {
            ka = 1.0*(Line[stop] - Line[start]) / (stop - start);
            kb = 1.0*Line[start] - (ka*start);
        }
    }
    else // 娌℃壘鍒� 鏂滅巼璁＄畻璧风偣
    {
        ka = 0;
        kb = Line[*Start+1];
    }
    res = (*Start) * ka + kb;
    res = range_protect((int32)res, 1, (COL-2)); // 闄愬箙
    if (Mode ==1 )
    {
        if ((res - Line[*Start+1]) < -3) // 濡傛灉璁＄畻鍑烘潵鐨勮宸笌涓婁竴鐐圭殑璇樊澶ぇ 浣跨敤涓婁竴鐐圭殑鍧愭爣
        {
            Line[*Start] =  Line[*Start+1];
            *Ka = 0;
            *Kb = Line[*Start+1];
        }
        else
        {
            *Ka = ka;
            *Kb = kb;
            Line[*Start] = res;
        }
    }
    else if (Mode == 2)
    {
        if ((res - Line[*Start+1]) > 3) // 濡傛灉璁＄畻鍑烘潵鐨勮宸笌涓婁竴鐐圭殑璇樊澶ぇ 浣跨敤涓婁竴鐐圭殑鍧愭爣
        {
            Line[*Start] =  Line[*Start+1];
            *Ka = 0;
            *Kb = Line[*Start+1];
        }
        else
        {
            *Ka = ka;
            *Kb = kb;
            Line[*Start] = res;
        }
    }
}
/*
*   涓ょ偣娉曟眰鐩寸嚎
*
*   璇存槑锛氭嫙鍚堢洿绾� y = Ka * x + Kb   Mode == 1浠ｈ〃宸﹁竟鐣岋紝Mode == 2浠ｈ〃鍙宠竟鐣�
*/
void Curve2_Fitting(float *Ka, float *Kb, uint8 Start,uint8 End, uint16 *Line, uint8 Mode)
{
        if (Mode==1)
        {
             *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
             *Kb = 1.0*Line[End] - (*Ka * End);
        }
        else
        {
              *Ka = 1.0*((Line[Start]) - Line[End]) / (Start-End);
              *Kb = 1.0*Line[End] - (*Ka * End);
        }

}

/*
*   璁＄畻琛ョ嚎鍧愭爣
*
*   璇存槑锛氫娇鐢ㄤ袱鐐规硶璁＄畻鎷熷悎鍑虹殑琛ョ嚎鍧愭爣
*/
uint8 Calculate_Add(uint8 i, float Ka, float Kb)
{
    float res;
    uint8 Result;

    res = i * Ka + Kb;
    Result = range_protect((int32)res, 1, (COL-2));

    return Result;
}

//  棣栬澶勭悊
uint8 First_Line_Handle(uint8 *data)
{
    uint8 i = 59;   // 鎺у埗琛�
    uint8 Weight_Left, Weight_Right;    // 宸﹀彸璧涢亾瀹藉害
    uint8 Mid_Left, Mid_Right;
    uint8 black_count = 0;
    Left_Add_Flag[59] = 0;
    Right_Add_Flag[59] = 0;     //    鍒濆鍖栬ˉ绾挎爣蹇�
    Image_Para_Init();
    if (data[i*COL + Mid_Line[i+1]])// 60 琛岀粓鐐瑰湪 59 琛屼负 榛戣壊  鍙兘璺戝嚭璧涢亾 涔熷彲鑳戒笉鍦ㄨ禌閬撴渶涓績浣嶇疆
    {
        for (uint8 j = 1; j < COL-2; j++)
        {
            if (data[i*COL + j] )
            {
                black_count++;
            }
            if (black_count > 100)
            {
                return 0;
            }
        }

        Weight_Left = Traversal_Left(i, data, &Mid_Left, 1, (COL-2));       // 浠庡乏渚ф悳绱㈣竟鐣�  0
        Weight_Right = Traversal_Right(i, data, &Mid_Right, 1, (COL-2));// 浠庡彸渚ф悳绱㈣竟鐣�  0

        if (Weight_Left >= Weight_Right && Weight_Left)                 // 宸﹁禌閬撳搴﹀ぇ浜庡彸璧涢亾瀹藉害涓斾笉涓�0
        {
              Traversal_Left_Line(i, data, Left_Line, Right_Line);      // 浣跨敤宸﹂亶鍘嗚幏鍙栬禌閬撹竟鐣�
        }
        else if (Weight_Left < Weight_Right && Weight_Right)
        {
              Traversal_Right_Line(i, data, Left_Line, Right_Line);     // 浣跨敤鍙抽亶鍘嗚幏鍙栬禌閬撹竟鐣�
        }
        else    // 璇存槑娌℃煡鍒�
        {
              return 0;
        }
    }
    else
    {
          Traversal_Mid_Line(i, data, Mid_Line[i+1], 1, (COL-2), Left_Line, Right_Line, Left_Add_Line, Right_Add_Line); // 浠庡墠涓�琛屼腑鐐瑰悜涓よ竟鎵弿
    }

    Left_Line[60] = Left_Line[59];
    Right_Line[60] = Right_Line[59];
    Left_Add_Line[60] = Left_Add_Line[59];
    Right_Add_Line[60] = Right_Add_Line[59];

    if (Left_Add_Flag[59] && Right_Add_Flag[59])
    {
        Mid_Line[59] = Mid_Line[60];
    }
    else
    {
        Mid_Line[59] = (Right_Line[59] + Left_Line[59]) / 2;
        Mid_Line[60] = Mid_Line[59];                           // 鏇存柊绗�60琛岃櫄鎷熶腑鐐癸紝渚夸簬涓嬩竴甯у浘鍍忎娇鐢�
    }
    Width_Real[60] = Width_Real[59];
    Width_Add[60] = Width_Add[59];
    Width_Min = Width_Add[59];
    if (Mid_Line[59] < 5 || Mid_Line[59] > 115)  // 濡傛灉 鏈�浣庤鐨� 涓偣鍧愭爣 澶皬鎴栧お澶� 鑷繁琛ヤ竴涓�
    {
        Mid_Line[59] = 60;
        Mid_Line[60] = Mid_Line[59];
    }
    return 1;
}

/*
*   浠庡乏渚у紑濮嬫悳绱㈣竟鐣岋紝杩斿洖璧涢亾瀹藉害
*
*   璇存槑锛氭湰鍑芥暟浠呬粎浣滀负鎺㈡祴璧涢亾浣跨敤锛屼粎杩斿洖璧涢亾瀹藉害锛屼笉淇濆瓨杈圭晫鏁版嵁
*/
uint8 Traversal_Left(uint8 i, uint8 *data, uint8 *Mid, uint8 Left_Min, uint8 Right_Max)  //  Traversal_Left(i, data, &Mid_Left, 1, (COL-1)); i = 89
{
    uint8 j, White_Flag = 0;
    uint8 Left_Line = Left_Min, Right_Line = Right_Max;

    for (j = Left_Min+Gray_Num; j <= Right_Max-Gray_Num; j+=1)  // 杈圭晫鍧愭爣 1鍒� 119
    {
          if (!White_Flag)  // 鍏堟煡鎵惧乏杈圭晫
          {
              // 宸﹁竟鍙兘涓㈢嚎 濡傛灉鐩存帴涓㈢嚎 閭ｄ箞璁颁笅宸︾晫
              if(data[i*COL + Left_Min+Gray_Num])
              {
                    Left_Line = Left_Min+Gray_Num;// 璁板綍褰撳墠j鍊间负鏈宸﹁竟鐣�
                    White_Flag = 1;// 宸﹁竟鐣屽凡鎵惧埌锛屽繀鏈夊彸杈圭晫
                    j += Gray_Num;
                    continue;   // 缁撴潫鏈寰幆锛岃繘鍏ヤ笅娆″惊鐜�
              }
              if (data[i*COL + j+Gray_Num]!=data[i*COL + j])   // 妫�娴嬭禌閬撹竟鐣�
              {
                  Left_Line = j;// 璁板綍褰撳墠j鍊间负鏈宸﹁竟鐣�
                  White_Flag = 1;// 宸﹁竟鐣屽凡鎵惧埌锛屽繀鏈夊彸杈圭晫
                  j += Gray_Num;
                  continue; // 缁撴潫鏈寰幆锛岃繘鍏ヤ笅娆″惊鐜�
              }
          }
          else
          {
              if (data[i*COL + j]!=data[i*COL + j+Gray_Num])// 妫�娴嬭禌閬撹竟鐣�
              {
                  Right_Line = j;//璁板綍褰撳墠j鍊间负鏈鍙宠竟鐣�
                  break;    // 宸﹀彸杈圭晫閮芥壘鍒帮紝缁撴潫寰幆
              }
          }
    }
    if (!White_Flag)    // 鏈壘鍒板乏鍙宠竟鐣�
    {
        return 0;
    }
    else
    {
        *Mid = (Right_Line + Left_Line) / 2;
        return (Right_Line - Left_Line);
    }
}

/*
*   浠庡彸渚у紑濮嬫悳绱㈣竟鐣岋紝杩斿洖璧涢亾瀹藉害
*
*   璇存槑锛氭湰鍑芥暟浠呬粎浣滀负鎺㈡祴璧涢亾浣跨敤锛屼粎杩斿洖璧涢亾瀹藉害锛屼笉淇濆瓨杈圭晫鏁版嵁
*/
uint8 Traversal_Right(uint8 i, uint8 *data, uint8 *Mid, uint8 Left_Min, uint8 Right_Max)
{
    uint8 j, White_Flag = 0;
    uint8 Left_Line = Left_Min, Right_Line = Right_Max;

    for (j = Right_Max-Gray_Num; j >= Left_Min+Gray_Num; j-=1)  // 杈圭晫鍧愭爣 235 ~ 4
    {
        if (!White_Flag)        // 鍏堟煡鎵惧彸杈圭晫
        {
            // 鍙宠竟鍙兘涓㈢嚎 濡傛灉鐩存帴涓㈢嚎 閭ｄ箞璁颁笅鍙宠竟鐣�
            if(data[i*COL +  Right_Max-Gray_Num] )
             {
                    Right_Line = Right_Max-Gray_Num;// // 璁板綍褰撳墠j鍊间负鏈鍙宠竟鐣�
                    White_Flag = 1;// // 鍙宠竟鐣屽凡鎵惧埌锛屽繀鏈夊乏杈圭晫
                    j -= Gray_Num;
                    continue;   // 缁撴潫鏈寰幆锛岃繘鍏ヤ笅娆″惊鐜�
            }
            if (data[i*COL + j-Gray_Num]!=data[i*COL + j])     // 妫�娴嬪埌鐧界偣
            {
                Right_Line = j; // 璁板綍褰撳墠j鍊间负鏈鍙宠竟鐣�
                White_Flag = 1;     // 鍙宠竟鐣屽凡鎵惧埌锛屽繀鏈夊乏杈圭晫
                j -= Gray_Num;
                continue;// 缁撴潫鏈寰幆锛岃繘鍏ヤ笅娆″惊鐜�
            }
        }
        else
        {
            if (data[i*COL + j]!=data[i*COL + j - Gray_Num])   //妫�娴嬮粦鐐�
            {
                Left_Line = j;  //璁板綍褰撳墠j鍊间负鏈宸﹁竟鐣�
                break;      // 宸﹀彸杈圭晫閮芥壘鍒帮紝缁撴潫寰幆
            }
        }
    }
    if (!White_Flag)    // 鏈壘鍒板乏鍙宠竟鐣�
    {
        return 0;
    }
    else
    {
        *Mid = (Right_Line + Left_Line) / 2;
        return (Right_Line - Left_Line);
    }
}


/*
*   浠庡乏渚у紑濮嬫悳绱㈣竟鐣岋紝淇濆瓨璧涢亾杈圭晫锛岃繑鍥�1鎴愬姛 0澶辫触
*
*   璇存槑锛氭湰鍑芥暟浣跨敤鍚庡皢淇濆瓨杈圭晫鏁版嵁
*/
uint8 Traversal_Left_Line(uint8 i, uint8 *data, uint16 *Left_Line, uint16 *Right_Line)   // Traversal_Left_Line(i, data, Left_Line, Right_Line);
{
    uint8 j, White_Flag = 0;

    Left_Line[i] = 1;
    Right_Line[i] = (COL-1);

    for (j = 1+1; j < COL-1-Gray_Num; j += 1)       // 杈圭晫鍧愭爣 1鍒癈OL
    {
        if (!White_Flag)    // 鍏堟煡鎵惧乏杈圭晫
        {
            // 宸﹁竟鍙兘涓㈢嚎 濡傛灉鐩存帴涓㈢嚎 閭ｄ箞璁颁笅宸﹁竟灞�
            if(data[i*COL + 1+1] )
            {
                    Left_Line[i] = 1+1;// 璁板綍褰撳墠j鍊间负鏈宸﹁竟鐣�
                    White_Flag = 1;// 宸﹁竟鐣屽凡鎵惧埌锛屽繀鏈夊彸杈圭晫
                    j += Gray_Num;
                    continue;   // 缁撴潫鏈寰幆锛岃繘鍏ヤ笅娆″惊鐜�
            }
            if (data[i*COL + j+Gray_Num]!=data[i*COL + j]) // 妫�娴嬪埌鐧界偣
            {
                    Left_Line[i] = j;        //褰撳墠j鍊间负鏈宸﹁竟鐣�
                    White_Flag = 1;     // 宸﹁竟鐣屽凡鎵惧埌锛屽繀鏈夊彸杈圭晫
                    j += Gray_Num;
                    continue;   // 缁撴潫鏈寰幆锛岃繘鍏ヤ笅娆″惊鐜�
            }
        }
        else
        {
            if (data[i*COL + j]!=data[i*COL + j+Gray_Num]) //妫�娴嬮粦鐐�
            {
                  Right_Line[i] = j-1;    //璁板綍褰撳墠j鍊间负鏈鍙宠竟鐣�
                  break;    // 宸﹀彸杈圭晫閮芥壘鍒帮紝缁撴潫寰幆
            }
        }
    }
    if (White_Flag  )
    {
        Left_Add_Line[i] = Left_Line[i];
        Right_Add_Line[i] = Right_Line[i];
        Width_Real[i] = Right_Line[i] - Left_Line[i];
        Width_Add[i] = Width_Real[i];
    }

    return White_Flag;  // 杩斿洖鎼滅储缁撴灉
}
/*
*   浠庡彸渚у紑濮嬫悳绱㈣竟鐣岋紝淇濆瓨璧涢亾杈圭晫锛岃繑鍥�1鎴愬姛 0澶辫触
*
*   璇存槑锛氭湰鍑芥暟浣跨敤鍚庡皢淇濆瓨杈圭晫鏁版嵁
*/
uint8 Traversal_Right_Line(uint8 i, uint8 *data, uint16 *Left_Line, uint16 *Right_Line)
{
    uint8 j, White_Flag = 0;
    Left_Line[i] = 1;
    Right_Line[i] = (COL-2);
    for (j = (COL-1-1); j > 0+1+Gray_Num; j -= 1)   // 杈圭晫鍧愭爣 1鍒�79
    {
        if (!White_Flag)    // 鍏堟煡鎵惧彸杈圭晫
        {
              //    鍙宠竟鍙兘涓㈢嚎 濡傛灉鐩存帴涓㈢嚎 閭ｄ箞璁颁笅鍙宠竟鐣�
              if(data[i*COL +  (COL-3)] )
              {
                  Right_Line[i] = (COL-2);// // 璁板綍褰撳墠j鍊间负鏈鍙宠竟鐣�
                  White_Flag = 1;// // 鍙宠竟鐣屽凡鎵惧埌锛屽繀鏈夊乏杈圭晫
                  j -= Gray_Num;
                  continue; // 缁撴潫鏈寰幆锛岃繘鍏ヤ笅娆″惊鐜�
              }
              if (data[i*COL + j-Gray_Num]!=data[i*COL + j])   // 妫�娴嬪埌鐧界偣
              {
                  Right_Line[i] = j;    // 璁板綍褰撳墠j鍊间负鏈鍙宠竟鐣�
                  White_Flag = 1;           // 鍙宠竟鐣屽凡鎵惧埌锛屽繀鏈夊乏杈圭晫
                  j -= Gray_Num;
                  continue; // 缁撴潫鏈寰幆锛岃繘鍏ヤ笅娆″惊鐜�
              }
        }
        else
        {
              if (data[i*COL + j]!=data[i*COL + j-Gray_Num])       //妫�娴嬮粦鐐�
              {
                  Left_Line[i] = j+1;//璁板綍褰撳墠j鍊间负鏈宸﹁竟鐣�
                  break;        // 宸﹀彸杈圭晫閮芥壘鍒帮紝缁撴潫寰幆
              }
        }
    }
    if (White_Flag )
    {
        Left_Add_Line[i] = Left_Line[i];
        Right_Add_Line[i] = Right_Line[i];
        Width_Real[i] = Right_Line[i] - Left_Line[i];
        Width_Add[i] = Width_Real[i];
    }

    return White_Flag;      // 杩斿洖鎼滅储缁撴灉
}

/*
*   浠庝腑闂村悜涓よ竟鎼滅储杈圭晫
*
*   璇存槑锛氭湰鍑芥暟浣跨敤鍚庡皢淇濆瓨杈圭晫鏁版嵁
*/
void Traversal_Mid_Line(uint8 i, uint8 *data, uint8 Mid, uint8 Left_Min, uint8 Right_Max, uint16 *Left_Line, uint16 *Right_Line, uint16 *Left_Add_Line, uint16 *Right_Add_Line)
{
    uint8 j;
    Left_Add_Flag[i] = 1;// 鍒濆鍖栬ˉ绾挎爣蹇椾綅
    Right_Add_Flag[i] = 1;
    Right_Line[i] = Right_Max;    //   COL -1
    Left_Line[i] = Left_Min;    // 缁欏畾杈圭晫鍒濆鍊�   1

    for (j = Mid; j >= Left_Min+1+Gray_Num; j-=1)   // 浠ュ墠涓�琛屼腑鐐逛负璧风偣鍚戝乏鏌ユ壘杈圭晫
    {
        if (data[i*COL + j]!=data[i*COL + j - Gray_Num])   //妫�娴嬪埌榛戣浆鐧界偣
        {
            Left_Add_Flag[i] = 0;   //宸﹁竟鐣屼笉闇�瑕佽ˉ绾匡紝娓呴櫎鏍囧織浣�
            Left_Line[i] = j;       //璁板綍褰撳墠j鍊间负鏈瀹為檯宸﹁竟鐣�
            Left_Add_Line[i] = j;    // 璁板綍瀹為檯宸﹁竟鐣屼负琛ョ嚎宸﹁竟鐣�
            break;
        }
    }
    for (j = Mid; j <= Right_Max-1*Gray_Num; j+=1)  // 浠ュ墠涓�琛屼腑鐐逛负璧风偣鍚戝彸鏌ユ壘鍙宠竟鐣�
    {
        if (data[i*COL + j]!=data[i*COL + j + Gray_Num])   //妫�娴嬪埌榛戣浆鐧界偣
        {
            Right_Add_Flag[i] = 0;      //鍙宠竟鐣屼笉闇�瑕佽ˉ绾匡紝娓呴櫎鏍囧織浣�
            Right_Line[i] = j;      //璁板綍褰撳墠j鍊间负鏈鍙宠竟鐣�
            Right_Add_Line[i] = j;  // 璁板綍瀹為檯鍙宠竟鐣屼负琛ョ嚎鍙宠竟鐣�
            break;
       }
    }
    if (Left_Add_Flag[i])       // 宸﹁竟鐣岄渶瑕佽ˉ绾�
    {
        if (i >= 57)    // 鍓�3琛�
        {
            Left_Add_Line[i] = Left_Line[59];   // 浣跨敤搴曡鏁版嵁
        }
        else                     ///
        {
            Left_Add_Line[i] = Left_Add_Line[i+1];  // 浣跨敤鍓�1琛屽乏杈圭晫浣滀负鏈宸﹁竟鐣�
        }
    }
    if (Right_Add_Flag[i])  // 鍙宠竟鐣岄渶瑕佽ˉ绾�
    {
        if (i >= 57)    // 鍓�3琛�
        {
            Right_Add_Line[i] = Right_Line[59];// 浣跨敤搴曡鏁版嵁
        }
        else
        {
            Right_Add_Line[i] = Right_Add_Line[i+1];    // 浣跨敤鍓�1琛屽彸杈圭晫浣滀负鏈鍙宠竟鐣�
        }
    }

    Width_Real[i] = Right_Line[i] - Left_Line[i];               // 璁＄畻瀹為檯璧涢亾瀹藉害
    Width_Add[i] = Right_Add_Line[i] - Left_Add_Line[i];    // 璁＄畻琛ョ嚎璧涢亾瀹藉害

}


/*
*   琛ョ嚎淇
*
*   璇存槑锛氭湁濮嬫湁缁堟墠浣跨敤锛岀洿鎺ヤ娇鐢ㄤ袱鐐规枩鐜囪繘琛岃ˉ绾�
*
*/ //Line_Repair(Left_Add_Start, Left_Add_Stop, data, Left_Line, Left_Add_Line, Left_Add_Flag, 1);
void Line_Repair(uint8 Start, uint8 Stop, uint8 *data, uint16 *Line, uint16 *Line_Add, uint8 *Add_Flag, uint8 Mode)
{
    float res;
    uint8 i;    // 鎺у埗琛�
    float Ka, Kb;

        if (Stop)   // 鏈夊鏈夌粓
        {
            // if ((Right_Add_Stop >= 51 && Left_Add_Stop >= 51) || (Right_Add_Stop >= 51 && Left_Add_Start <= Right_Add_Stop) || (Left_Add_Stop >= 51 && Right_Add_Start <= Left_Add_Stop))//  鍙湁杈冨皯琛岄渶瑕佽ˉ绾匡紝涓嶈绠楁枩鐜囷紝鐩存帴绔栫洿鍚戜笅琛ョ嚎
            // {
            //     for (i = Stop-2; i <= 57; )
            //     {
            //         i += 1;
            //         Line_Add[i] = Line_Add[Stop];
            //     }
            // }
            // else     // 灏嗚捣濮嬭鍜岀粨鏉熻璁＄畻鏂滅巼琛ョ嚎
            // {
                if (Start <= 57)
                {
                    Start +=2;
                }
                Ka = 1.0*(Line_Add[Start] - Line_Add[Stop]) / (Start - Stop);
                Kb = 1.0*Line_Add[Start] - (Ka * Start);
                // if (Mode == 1)          // 鏂滅巼浼拌 闃叉璇ˉ绾�
                // {
                //     if (Ka < -2.5)
                //     {
                //         return;
                //     }
                // }
                // if (Mode == 2)
                // {
                //     if (Ka > 2.5)
                //     {
                //         return;
                //     }
                // }
                for (i = Stop; i <= Start; )
                {
                    i += 1;
                    res = i * Ka + Kb;
                    Line_Add[i] = range_protect((int32)res, 2, (COL-2));
                }
            // }
        }
    // }
}

/*
*   琛ョ嚎淇
*
*   璇存槑锛氫换鎰忎袱鐐硅ˉ涓�鏉＄洿绾�
*
*/   // Start Stop 寮�濮嬬粨鏉� 琛ョ嚎鐐圭殑 琛屾暟  position_start position_stop   琛ョ嚎鐐圭殑鍒楁暟    Line_Add 瑕佽ˉ鐨勭嚎
void Point_Repair_Line(uint8 Start, uint8 Stop,uint16 position_start, uint16 position_stop ,uint16 *Line_Add)
{
    float res;
    uint8 i;    // 鎺у埗琛�
    float ka, kb;
    ka = 1.0*(position_start - position_stop) / (Start - Stop);
    kb = 1.0*position_start - (ka * Start);

    for (i = Stop; i <= Start; )
    {
        i += 1;
        res = i * ka + kb;
        Line_Add[i] = range_protect((int32)res, 2, (COL-2));
    }
}




/******** 闄愬箙淇濇姢 *********/
int32 range_protect(int32 duty, int32 min, int32 max)//闄愬箙淇濇姢
{
    if (duty >= max)
    {
        return max;
    }
    else if (duty <= min)
    {
        return min;
    }
    else
    {
        return duty;
    }
}
/******** 浠庡乏鎵乏杈圭晫 *********/
uint8 zuobianjie( uint8 *data,uint8 Left_Min,uint8 Right_Max)
{
    uint8 i,j;
    for(i=60;i>5;i--)
    {
        for (j =2; j <= 30; j+=1)
        {
            if(data[i*COL + j])
            {
                Right_Flag = 1;
                Left_Add_Line[i] = j;
                break;
            }
        }
    }
//    for(i=60;i>=5;i--)
//    {
//        for (j = 110; j >= 90; j-=1)  // 杈圭晫鍧愭爣 235 ~ 4
//        {
//            if(data[i*COL + j] )
//            {
//                Right_Flag = 1;
//                Right_Add_Line[i]=j;
//                break;
//            }
//        }
//    }
//    for(i = 59; i >= 5;i--)
//    {
//        Mid_Line[i] = (Right_Add_Line[i]+Left_Add_Line[i]) / 2;
//    }
}
/******** 浠庡彸鎵彸杈圭晫 *********/
uint8 youbianjie( uint8 *data, uint8 Left_Min, uint8 Right_Max)
{
        for(i=60;i>=5;i--)
        {
            for (j = 110; j >= 90; j-=1)  // 杈圭晫鍧愭爣 235 ~ 4
            {
                    if(data[i*COL + j] )
                    {
                        Right_Flag = 1;
                        Right_Add_Line[i]=j;
                        break;
                    }
            }
        }
}

void  Find_Rhombus_Out(uint8 *data)
{
        aaa=0;
        for(int i=15;i<=60;i++)
        {
            for(int j=57;j<=63;j++)
            {
                if(data[i*COL+j])
                {
                    aaa++;
                }
            }
        }
        if(aaa<2)
        {
            gpio_set_level(P33_10,1);
            Rhombus_Out=1;
        }
}

void Turn_Left(void)
{
    if(Flag_turn == 0&&Flag_stop==0)
    {
        Flag_stop = 7;
        Flag_turn = 3;
        Distance_Test_ON = 1;
    }
    if(Flag_stop == 7&&Distance_Test>20)
    {
        Distance_Test = 0;
        Distance_Test_ON = 0;

        Flag_turn = 1;
        Flag_stop = 2;

        Distance_ON = 1;
        Light_Flag=2;
    }
    if(Flag_turn == 1 && Distance > 165)
    {
        Distance_ON = 0;
        Distance = 0;
        Light_Flag=0;

        Flag_turn = 6;//鍙宠浆鐭

        Flag_stop = 3;//鍙宠浆鐭閫熷害
        Distance_Test_ON = 1;
    }

    if(Flag_turn == 6 && Distance_Test > 85)//鍙宠浆澶嶄綅
    {
        gpio_set_level(P33_10,1);//铚傞福鍣ㄥ叧闂�
        Distance_Test = 0;
        Distance_Test_ON = 0;

        Flag_turn = 0;
        Flag_stop = 5;
        Distance_ON = 1;
    }
    if(Flag_turn == 0 && Distance >450  )//缁欎竴娈佃窛绂诲惊杩瑰浣�
    {
        gpio_set_level(P33_10,0);//铚傞福鍣ㄥ叧闂�
        Distance_ON = 0;
        Distance = 0;

        Flag_turn = 0;
        Flag_stop=5;

        Rhombus_Flag=0;
        Deal_Flag=2;
    }

}
void Turn_Left_1(void)
{
    if((Flag_turn == 3||Flag_turn == 0)&&(Flag_stop==5||Flag_turn == 0)&&Distance_ON ==0)
    {
      //铚傞福鍣ㄥ搷
        Flag_stop = 7;//宸﹁浆閫熷害
        Flag_turn = 3;
        Distance_Test_ON = 1;
//        if(Light_Flag==0)
//        {
//            Light_Flag=4;
//        }

    }

    if(Flag_stop == 7&&Distance_Test>50)
//        if(Flag_stop == 7&&((Distance>50&&Light_Flag!=4)||Distance>80))
    {
        Light_Flag=0;
        Distance_Test= 0;
        Distance_Test_ON = 0;

        Light_Flag=2;//宸﹁浆鍚戠伅

        Flag_turn = 4;//宸﹁浆
        Flag_stop = 2;//宸﹁浆閫熷害

        Distance_ON = 1;//寮�鍚噷绋嬭
    }
    if(Flag_turn == 4 && Distance > 175)//宸﹁浆//  婊＄數140
    {

        Distance_ON = 0;
        Distance = 0;
        Light_Flag=0;

        Flag_turn = 2;//鍙宠浆鐭

        Flag_stop = 3;//鍙宠浆鐭閫熷害
        Distance_Test_ON = 1;
    }

    if(Flag_turn == 2 && Distance_Test > 85 )//鍙宠浆澶嶄綅
    {
        Distance_Test = 0;
        Distance_Test_ON = 0;

        Flag_turn = 0;
        Flag_stop = 5;
        Distance_ON = 1;
    }
    if(Flag_turn == 0 && Distance > 250 )//缁欎竴娈佃窛绂诲惊杩瑰浣�
    {
        gpio_set_level(P33_10,1);//铚傞福鍣ㄥ叧闂�
        Distance_ON = 0;
        Distance = 0;

        Flag_turn = 0;
        Flag_stop=6;

        Rhombus_Flag=0;
        Deal_Flag=5;
        Distance_Test_ON = 1;
    }

}
void Turn_Right(void)
{
   if(Flag_turn == 0&&Flag_stop==5)
   {
        Flag_turn = 2;//鍙宠浆
        Flag_stop = 3;//鍙宠浆閫熷害
        Distance_ON = 1;//寮�鍚噷绋嬭
        Light_Flag=3;
    }
    if(Flag_turn == 2 && Distance >165)
    {
        Light_Flag=0;
        Distance_ON = 0;
        Distance = 0;
        gpio_set_level(P33_10,1);//铚傞福鍣�
        Flag_turn = 1;//宸﹁浆鐭

        Flag_stop = 2;//宸﹁浆鐭閫熷害
        Distance_Test_ON = 1;
    }

    if(Flag_turn == 1 && Distance_Test >85)
    {
        gpio_set_level(P33_10,0);//铚傞福鍣�
        Distance_Test = 0;
        Distance_Test_ON = 0;
        Flag_turn = 10;
        Flag_stop = 5;
        Distance_ON = 1;
     }
    if(Flag_turn == 10 && Distance > 300 )
     {
        Rhombus_Flag=0;

        Deal_Flag=30;
        Flag_turn = 0;
        Flag_stop = 0;
        Distance_ON = 0;
        Distance = 0;
     }

}
void Decelerate(uint8 *data)
{
    Flag_stop=5;
    Flag_turn=3;

    Find_rhombus_Out(data);
    if(Rhombus_Out_Flag==1)
    {
       Flag_stop=5;
       Flag_turn=0;
       Deal_Flag=Deal_Flag+1;
       Rhombus_Flag=0;
       Rhombus_Out_Flag=0;
   }
}
void Zebra_Crossing(uint8 *data)
{
    if(Flag_turn==0)
    {
        gpio_set_level(P33_10,1);
        Flag_turn=5;
        Flag_stop=5;
        Distance_ON=1;
        Rhombus_Out=0;
    }
    if(Distance>80)
    {
    Find_Rhombus_Out(data);
    }
    if(Rhombus_Out==1)
    {

        Distance=0;
        Distance_ON=0;

        Flag_stop=5;
        Flag_turn=0;

        Pwm_Min=820;
        Pwm_Max=884;

        Rhombus_Out=0;
        Rhombus_Flag=0;
        if(mode_2024==1)
        Deal_Flag=6;
        else
        Deal_Flag=4;
   //     Distance_Test_ON=1;
    }
}
void Zebra_Crossing_2(uint8 *data)
{
    if(Flag_turn==0)
    {
        gpio_set_level(P33_10,1);
        Flag_turn=8;
        Flag_stop=5;
        Distance_ON=1;
        Rhombus_Out=0;
    }
    if(Distance>80)
    {
    Find_Rhombus_Out(data);
    }
    if(Rhombus_Out==1)
    {

        Distance=0;
        Distance_ON=0;

        Flag_stop=5;
        Flag_turn=0;

        Pwm_Min=820;
        Pwm_Max=884;

        Rhombus_Out=0;
        Rhombus_Flag=0;
        if(mode_2024==1)
        Deal_Flag=6;
        else
        Deal_Flag=4;
   //     Distance_Test_ON=1;
    }
}
void  Find_Stop_Left(uint8 *data)
{
        aaa=0;
        for(int i=30;i<=65;i++)
        {
            for(int j=10;j<=60;j++)
            {
                if(data[i*COL+j])
                {
                    aaa++;
                }
            }
        }
        if(aaa<2)
        {
            Stop_Out=1;
        }
}
void  Find_Stop_Right(uint8 *data)
{
        aaa=0;
        for(int i=30;i<=65;i++)
        {
            for(int j=60;j<=110;j++)
            {
                if(data[i*COL+j])
                {
                    aaa++;
                }
            }
        }
        if(aaa<2)
        {
            Stop_Out=1;
        }
}
void  Tiao_Light(uint8 *data)
{
        uint8 i,j;
        uint32 a;
        a=0;
        for(i=30;i>5;i--)
        {
            for(j=55;j<65;j++)
            {
                if(data[i*COL+j])
                {
                    a++;
                    time_dl1b_on=1;
                }
            }
        }
        if(a>=1)
        {
            Differ_light++;
        }

}


/* ================= [2026-10-25] 前方车辆识别（项目一：超车与会车）=================
   前车是 1:8~1:15 遥控车。车道宽 45cm 约等于画面里 58px，一辆车约 26~38px 宽，
   出现在【本车道内部】，不像车道线那样只有 2~3px。

   难点：车壳颜色未知。白壳在二值图里是白的，深色壳是黑的（跟赛道一样黑）。
   所以这里不做“识别车”，而是【统计两条边线之间左右两半的白点占比】：
       正常赛道：中间只有一条细虚线 -> 占比很低
       有车挡着：浅色车 -> 占比骤升；深色车 -> 车道线被挡断，边界会异常
   两个值都输出，交给状态机判断，同时打到屏幕上方便调阈值。

   输出：
     car_pct_l / car_pct_r   左半/右半车道的白点占比（%）
     car_row_l / car_row_r   单行占比超标的“最靠下”那一行（越大越近）
*/
#define CAR_ROW_MIN     (22)   /* 太远的行只有几个像素 */
#define CAR_ROW_MAX     (54)
#define CAR_MIN_COLS    (8)    /* 单行至少要有这么多列才统计 */
#define CAR_ROW_PCT     (45)   /* 单行白点占比超过它算“这一行有东西” */

volatile uint8 car_pct_l = 0, car_pct_r = 0;
volatile uint8 car_row_l = 0, car_row_r = 0;

void Car_Scan(uint8 *data)
{
    uint8  i, j, mid;
    uint32 wl = 0, wr = 0;
    uint32 tl = 0, tr = 0;
    uint16 wrow, trow;

    car_row_l = 0;
    car_row_r = 0;

    if(diedai_img_avg > 190)          /* 整片白（过曝）时没有意义 */
    {
        car_pct_l = 0;
        car_pct_r = 0;
        return;
    }

    for(i = CAR_ROW_MIN; i <= CAR_ROW_MAX; i ++)
    {
        if(Left_Line[i] <= 2 || Right_Line[i] >= (COL - 3)) continue;   /* 边界不可信 */

        mid = (uint8)((Left_Line[i] + Right_Line[i]) >> 1);
        if(mid < 25 || mid > 95) continue;

        /* ---- 左半车道 ---- */
        wrow = 0;
        trow = 0;
        for(j = (uint8)(Left_Line[i] + 3); (uint16)j + 5 < (uint16)mid; j ++)
        {
            trow ++;
            if(data[i * COL + j]) wrow ++;
        }
        if(trow >= CAR_MIN_COLS)
        {
            wl += wrow;
            tl += trow;
            if((uint32)wrow * 100 >= (uint32)trow * CAR_ROW_PCT) car_row_l = i;
        }

        /* ---- 右半车道 ---- */
        wrow = 0;
        trow = 0;
        for(j = (uint8)(mid + 5); (uint16)j + 3 < Right_Line[i]; j ++)
        {
            trow ++;
            if(data[i * COL + j]) wrow ++;
        }
        if(trow >= CAR_MIN_COLS)
        {
            wr += wrow;
            tr += trow;
            if((uint32)wrow * 100 >= (uint32)trow * CAR_ROW_PCT) car_row_r = i;
        }
    }

    car_pct_l = tl ? (uint8)(wl * 100 / tl) : 0;
    car_pct_r = tr ? (uint8)(wr * 100 / tr) : 0;
}
