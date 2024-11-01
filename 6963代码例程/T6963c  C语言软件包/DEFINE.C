#include <reg51.h>   /* 特殊功能寄存器函数 */
#include <math.h>    /* 数学函数 */
#include <absacc.h>  /* 绝对地址访问 */
#include <string.h>  /* 字符串函数 */
/* 直接访问方式地址定义，根据用户平台接口修改 */
#define  d_add XBYTE[0x8000]   /* 数据通道 */
#define  c_add XBYTE[0x8100]   /* 指令通道 */

/* 程序参数定义,根据用户选择的LCM上的MDS3,MDS2的电平组合实现 */
    #define  para1 0x20;            /* MGLS160128,240128T */
/*  --------------------------- */
/*  #define  para1 0x28;            MGLS128128,DMF5002 */
/*  #define  para1 0x40;            MGLS12864T,24064   */
/*  定义标志位  */
 sbit Acc_0=ACC^0;
 sbit Acc_1=ACC^1;
 sbit Acc_2=ACC^2;
 sbit Acc_3=ACC^3;
 sbit Acc_6=ACC^6;
 unsigned int O_X,O_Y,com,dat1,dat2;  /* O_X:行地址，在文本方式下以字符行为单位。
                                             在图形方式下以点行为单位。
                                             在图形方式下以点行为单位。
                                         O_Y:列地址(0~77H)
                                         com:指令码寄存器
                                         dat1:数据寄存器1
                                         dat2:数据寄存器2   */

unsigned char code_1,attr;            /* code_1:字符代码
                                         attr:字符属性码(低4位有效) */
/***********************************************************************/
/*    中文字符库定义 ----适应T6963C控制器                              */
/*    用户根据系统的需要，使用清达光电提供的汉字提取软件建立中文字符库  */
/***********************************************************************/
 code unsigned char CGTAB[64]={ 0x00,0x40,0x37,0x10,0x81,0x61,0x22,0x0A,/*"液"= 80H*/
                                0x16,0x2B,0xE2,0x22,0x22,0x22,0x23,0x22,
                                0x80,0x44,0xFE,0x20,0x20,0x3C,0x44,0x64,
                                0x98,0x48,0x50,0x20,0x50,0x8E,0x04,0x00,
                                0x0F,0x08,0x08,0x0F,0x08,0x08,0x0F,0x08,/*"晶"= 84H*/
                                0x7E,0x42,0x42,0x7E,0x42,0x42,0x7E,0x42,
                                0xE0,0x20,0x20,0xE0,0x20,0x20,0xE0,0x24,
                                0xFE,0x84,0x84,0xFC,0x84,0x84,0xFC,0x84};

 code unsigned char CCTAB[4][32]={ 00x00,0x40,0x37,0x10,0x83,0x60,0x2F,0x08,    /* 清*/
                                   0x13,0x12,0x63,0x22,0x23,0x22,0x22,0x22,
                                   0x40,0x48,0xFC,0x40,0xF8,0x40,0xFE,0x00,
                                   0xF8,0x08,0xF8,0x08,0xF8,0x08,0x28,0x10,

                                   0x01,0x41,0x27,0x21,0x1F,0x04,0xE2,0x2F,    /* 达*/
                                   0x21,0x2F,0x21,0x3F,0x21,0x51,0x8F,0x00,
                                   0x00,0x00,0xE0,0x00,0xF0,0x20,0x40,0xF0,
                                   0x00,0xE0,0x00,0xF8,0x00,0x00,0xFE,0x00,

                                   0x01,0x21,0x11,0x09,0x09,0x01,0xFF,0x04,    /* 光*/
                                   0x04,0x04,0x04,0x08,0x08,0x10,0x20,0x40,
                                   0x00,0x08,0x0C,0x10,0x20,0x04,0xFE,0x40,
                                   0x40,0x40,0x40,0x40,0x42,0x42,0x3E,0x00,

                                   0x1F,0x41,0x7F,0x81,0x3D,0x01,0x3D,0x01,    /* 电*/
                                   0x3F,0x21,0x3F,0x21,0x3F,0x01,0x01,0x01,
                                   0xF0,0x04,0xFE,0x02,0x74,0x00,0x70,0x00,
                                   0xF8,0x08,0xF8,0x08,0xF8,0x02,0x02,0xFE
};

/*    西文字符代码----- 适应T6963C控制器     */
/*    此代码组显示为"Welcome you"            */
unsigned char  DTAB[11]={ 0x37,0x45,0x4c,0x43,0x4f,0x4d,0x45,0x00,0x39,0x4f,0x55};
