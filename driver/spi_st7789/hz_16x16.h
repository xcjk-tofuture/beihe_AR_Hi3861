#ifndef __HZ16x16_H
#define __HZ16x16_H

// ------------------  汉字字模的数据结构定义 ------------------------ //
typedef struct typFNT_GB16                 // 汉字字模数据结构
{
       unsigned char Index[2];               // 汉字内码索引
       char Msk[32];                       // 点阵码数据
}typFNT_GB16;

/////////////////////////////////////////////////////////////////////////
// 汉字字模表                                                          //
// 汉字库: 宋体16.dot,横向取模左高位,数据排列:从左到右从上到下         //
/////////////////////////////////////////////////////////////////////////
struct typFNT_GB16 code_GB_16[] =          // 数据表
{
"欢", 0x00,0x80,0x00,0x80,0xFC,0x80,0x04,0xFC,
      0x45,0x04,0x46,0x48,0x28,0x40,0x28,0x40,
      0x10,0x40,0x28,0x40,0x24,0xA0,0x44,0xA0,
      0x81,0x10,0x01,0x08,0x02,0x0E,0x0C,0x04,

"迎", 0x00,0x00,0x41,0x84,0x26,0x7E,0x14,0x44,
      0x04,0x44,0x04,0x44,0xF4,0x44,0x14,0xC4,
      0x15,0x44,0x16,0x54,0x14,0x48,0x10,0x40,
      0x10,0x40,0x28,0x46,0x47,0xFC,0x00,0x00,

"使", 0x10,0x40,0x10,0x44,0x1F,0xFE,0x20,0x40,
      0x27,0xFC,0x64,0x44,0xA4,0x44,0x24,0x44,
      0x27,0xFC,0x24,0x44,0x22,0x40,0x21,0x80,
      0x20,0xC0,0x21,0x30,0x26,0x0E,0x28,0x04,

"用", 0x00,0x08,0x3F,0xFC,0x21,0x08,0x21,0x08,
      0x21,0x08,0x3F,0xF8,0x21,0x08,0x21,0x08,
      0x21,0x08,0x3F,0xF8,0x21,0x08,0x21,0x08,
      0x21,0x08,0x41,0x08,0x41,0x28,0x80,0x10
};

#endif
