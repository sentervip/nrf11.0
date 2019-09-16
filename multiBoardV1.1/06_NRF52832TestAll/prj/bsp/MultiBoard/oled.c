#include "oled.h"
#include "app_util_platform.h"
#include "nrf_delay.h"
#include "multiboard_process.h"

const uint8_t gau8Number8x16[][16]=
{
	{0x00,0xE0,0xF0,0x18,0x08,0x18,0xF0,0xE0,0x00,0x0F,0x1F,0x30,0x20,0x30,0x1F,0x0F},/*"0",0*/
	{0x00,0x00,0x10,0x10,0xF8,0xF8,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x3F,0x20,0x20},/*"1",1*/
	{0x00,0x70,0x78,0x08,0x08,0x08,0xF8,0xF0,0x00,0x30,0x38,0x2C,0x26,0x23,0x31,0x30},/*"2",2*/
	{0x00,0x30,0x38,0x08,0x08,0x88,0xF8,0x70,0x00,0x18,0x38,0x21,0x21,0x23,0x3E,0x1C},/*"3",3*/
	{0x00,0x00,0x80,0xC0,0x70,0xF8,0xF8,0x00,0x00,0x06,0x07,0x25,0x24,0x3F,0x3F,0x24},/*"4",4*/
	{0x00,0xF8,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x19,0x39,0x20,0x20,0x31,0x1F,0x0E},/*"5",5*/
	{0x00,0xE0,0xF0,0x98,0x88,0x98,0x90,0x00,0x00,0x0F,0x1F,0x31,0x20,0x20,0x3F,0x1F},/*"6",6*/
	{0x00,0x18,0x18,0x08,0x88,0xE8,0x78,0x18,0x00,0x00,0x00,0x3E,0x3F,0x01,0x00,0x00},/*"7",7*/
	{0x00,0x70,0xF8,0x88,0x08,0x88,0xF8,0x70,0x00,0x1C,0x3E,0x23,0x21,0x23,0x3E,0x1C},/*"8",8*/
	{0x00,0xF0,0xF8,0x08,0x08,0x18,0xF0,0xE0,0x00,0x01,0x13,0x32,0x22,0x33,0x1F,0x0F},/*"9",9*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x30,0x00,0x00,0x00,0x00},/*".",10*/
};

const uint8_t gau8Number12x24[][36]=
{	
	{0x00,0x00,0x80,0xC0,0xE0,0x60,0x20,0x60,0xE0,0xC0,0x80,0x00,0x00,0xFE,0xFF,0xFF,
	 0x01,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFE,0x00,0x01,0x07,0x0F,0x1E,0x18,0x10,0x18,
	 0x1E,0x0F,0x07,0x01},/*"0",0*/
	{0x00,0x00,0x40,0x40,0x40,0xC0,0xE0,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x1F,0x1F,0x1F,
	 0x10,0x10,0x10,0x00},/*"1",1*/
	{0x00,0x80,0xC0,0x60,0x20,0x20,0x20,0x60,0xE0,0xC0,0x80,0x00,0x00,0x03,0x03,0x03,
	 0x00,0xC0,0xE0,0x70,0x3F,0x1F,0x07,0x00,0x00,0x18,0x1C,0x1E,0x13,0x11,0x10,0x10,
	 0x10,0x1E,0x1E,0x00},/*"2",2*/
	{0x00,0xC0,0xC0,0xE0,0x20,0x20,0x60,0xE0,0xC0,0x80,0x00,0x00,0x00,0x01,0x01,0x01,
	 0x10,0x10,0x18,0x3F,0xEF,0xE7,0x80,0x00,0x00,0x0E,0x0E,0x1E,0x10,0x10,0x10,0x18,
	 0x1F,0x0F,0x07,0x00},/*"3",3*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0xE0,0xE0,0x00,0x00,0x00,0xC0,0xE0,0xB8,
	 0x9C,0x87,0x83,0xFF,0xFF,0xFF,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x1F,
	 0x1F,0x1F,0x10,0x10},/*"4",4*/
	{0x00,0x00,0xE0,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x00,0x00,0x3F,0x3F,
	 0x18,0x08,0x08,0x18,0xF8,0xF0,0xE0,0x00,0x00,0x07,0x0F,0x1B,0x10,0x10,0x10,0x1C,
	 0x1F,0x0F,0x03,0x00},/*"5",5*/
	{0x00,0x00,0x80,0xC0,0xC0,0x60,0x20,0x20,0xE0,0xE0,0xC0,0x00,0x00,0xFC,0xFF,0xFF,
	 0x31,0x18,0x08,0x08,0x18,0xF8,0xF0,0xE0,0x00,0x01,0x07,0x0F,0x1C,0x18,0x10,0x10,
	 0x18,0x0F,0x0F,0x03},/*"6",6*/
	{0x00,0x00,0xC0,0xE0,0x60,0x20,0x20,0x20,0x20,0xE0,0xE0,0x60,0x00,0x00,0x01,0x01,
	 0x00,0x00,0xE0,0xF8,0x1F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F,
	 0x00,0x00,0x00,0x00},/*"7",7*/
	{0x00,0x80,0xC0,0xE0,0x60,0x20,0x20,0x20,0x60,0xE0,0xC0,0x80,0x00,0x83,0xCF,0xEF,
	 0x7C,0x18,0x30,0x30,0x78,0xEF,0xCF,0x83,0x00,0x07,0x0F,0x0F,0x18,0x10,0x10,0x10,
	 0x18,0x1F,0x0F,0x07},/*"8",8*/
	{0x00,0x00,0xC0,0xC0,0xE0,0x20,0x20,0x20,0xE0,0xC0,0x80,0x00,0x00,0x1F,0x3F,0x7F,
	 0x60,0x40,0x40,0x60,0x30,0xFF,0xFF,0xFE,0x00,0x00,0x0C,0x1C,0x1C,0x10,0x10,0x18,
	 0x0F,0x0F,0x03,0x00},/*"9",9*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x1C,0x1C,0x00,0x00,
	 0x00,0x00,0x00,0x00},/*".",10*/
	{0x0C,0x92,0xD2,0xCC,0xE0,0x60,0x20,0x20,0x60,0xE0,0xE0,0x00,0xFC,0xFF,0xFF,0x07,
	 0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x01,0x07,0x0F,0x1F,0x18,0x10,0x10,0x10,
	 0x18,0x0E,0x07,0x03},/*"��", 11*/	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x0C,
	 0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00},/*"-", 12*/
};

const uint8_t gau8twodot8x16[][16] = 
{
	{0x00,0x00,0x00,0x60,0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x00,0x00},/*":",0*/
};

const uint8_t gau8Number6x8[10][6] = 
{
	{0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00},// 0
    {0x00, 0x42, 0x7F, 0x40, 0x00, 0x00},// 1
    {0x42, 0x61, 0x51, 0x49, 0x46, 0x00},// 2
    {0x21, 0x41, 0x45, 0x4B, 0x31, 0x00},// 3
    {0x18, 0x14, 0x12, 0x7F, 0x10, 0x00},// 4
    {0x27, 0x45, 0x45, 0x45, 0x39, 0x00},// 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00},// 6
    {0x01, 0x71, 0x09, 0x05, 0x03, 0x00},// 7
    {0x36, 0x49, 0x49, 0x49, 0x36, 0x00},// 8
    {0x06, 0x49, 0x49, 0x29, 0x1E, 0x00},// 9
};

const uint8_t gau8ASCII8x16[][16] = 
{
	{0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x19,0x24,0x24,0x12,0x3F,0x20,0x00},/*"a",0*/
	{0x10,0xF0,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00},/*"b",1*/
	{0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00},/*"c",2*/
	{0x00,0x00,0x80,0x80,0x80,0x90,0xF0,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},/*"d",3*/
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x24,0x24,0x24,0x24,0x17,0x00},/*"e",4*/
	{0x00,0x80,0x80,0xE0,0x90,0x90,0x20,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"f",5*/
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00},/*"g",6*/
	{0x10,0xF0,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},/*"h",7*/
	{0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"i",8*/
	{0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00},/*"j",9*/
	{0x10,0xF0,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x06,0x29,0x30,0x20,0x00},/*"k",10*/
	{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"l",11*/
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F},/*"m",12*/
	{0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},/*"n",13*/
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},/*"o",14*/
	{0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0x91,0x20,0x20,0x11,0x0E,0x00},/*"p",15*/
	{0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0x91,0xFF,0x80},/*"q",16*/
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},/*"r",17*/
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},/*"s",18*/
	{0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x10,0x00},/*"t",19*/
	{0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},/*"u",20*/
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x03,0x0C,0x30,0x0C,0x03,0x00,0x00},/*"v",21*/
	{0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,0x01,0x0E,0x30,0x0C,0x07,0x38,0x06,0x01},/*"w",22*/
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},/*"s",23*/
	{0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x81,0x86,0x78,0x18,0x06,0x01,0x00},/*"y",24*/
	{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00},/*"z",25*/
};

const uint8_t gau8ASCII5x7[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // space
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

const uint8_t gau8Arrow5x7[][6] =
{
	{0x00,0x3E,0x1C,0x08,0x00}/*"Arrow",0*/
};

nrf_drv_spi_t gtSpi = NRF_DRV_SPI_INSTANCE(1);

void initSpiToOled(void)
{
	nrf_drv_spi_config_t ltSpiCfg = NRF_DRV_SPI_DEFAULT_CONFIG(1);
	ltSpiCfg.sck_pin = SPIM1_SCK_PIN;
	ltSpiCfg.mosi_pin = SPIM1_MOSI_PIN;
	ltSpiCfg.miso_pin = SPIM1_MISO_PIN;
	ltSpiCfg.frequency = NRF_DRV_SPI_FREQ_8M;
	ltSpiCfg.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;
	//nrf_drv_spi_uninit(&gtSpi);
	nrf_drv_spi_init(&gtSpi, &ltSpiCfg, NULL);
}
void initOledPin(void)
{
	initSpiToOled();
	nrf_gpio_cfg_output(SPIM1_SS_PIN);
	nrf_gpio_cfg_output(LCD_DC_PIN);
	nrf_gpio_cfg_output(LCD_RST_PIN);
}

void writeOledCmd(uint8_t fu8Cmd)
{
	LCD_CS_L;
	LCD_DC_CMD;
	nrf_drv_spi_transfer(&gtSpi, &fu8Cmd, 1, NULL, 0);
	LCD_DC_DATA;
	LCD_CS_H;
}

void writeOledData(uint8_t fu8Data)
{
	LCD_CS_L;
	LCD_DC_DATA;
	nrf_drv_spi_transfer(&gtSpi, &fu8Data, 1, NULL, 0);
	LCD_DC_CMD;
	LCD_CS_H;
}

void clrOledRam(void)
{
	writeOledCmd(0XB0);
	writeOledCmd(0X00);
	writeOledCmd(0X10);
	for(uint16_t i = 0; i < 1024; i ++){
		writeOledData(0x00);
		if((i & 0x007f) == 0x007f){
			writeOledCmd(0XB1 + (i >> 7));
			writeOledCmd(0X00);
			writeOledCmd(0X10);
		}
	}
}

void clrOledLine( uint8_t fu8Yloc)
{
	writeOledCmd(0xB0 | fu8Yloc);
	writeOledCmd(0x0f);
	writeOledCmd(0x10);
	for(uint8_t i = 0; i < 128; i ++){		
		writeOledData(0x00);
	}
}

void fillOledRam(uint8_t fu8ClrData)
{
	writeOledCmd(0XB0);
	writeOledCmd(0X00);
	writeOledCmd(0X12);
	for(uint16_t i = 0; i < 1024; i ++){
		writeOledData(fu8ClrData);
		if((i & 0x007f) == 0x007f){
			writeOledCmd(0XB1 + (i >> 7));
			writeOledCmd(0X00);
			writeOledCmd(0X10);
		}
	}
}

void closeOled(void)
{
	writeOledCmd(0XAE);
}

void openOled(void)
{
	writeOledCmd(0xAF);
}

void initOled(void)
{
	initOledPin();
	LCD_CS_H;
	nrf_delay_ms(10);
	LCD_RST_L;
	nrf_delay_ms(10);
	LCD_RST_H;
	writeOledCmd(0XAE);//display off
	writeOledCmd(0XD5);//set display clock and clock divice
	writeOledCmd(0X80);
	writeOledCmd(0XA8);//set multiplex ratio follow by data 15-63
	writeOledCmd(0X3F);
	writeOledCmd(0XD3);//set display offset,follow by data 0-63
	writeOledCmd(0X00);
	writeOledCmd(0X40);//set display start line,or by data 0-63
	writeOledCmd(0X00);//set higher column start address for paga address mode
	writeOledCmd(0X10);//set higher column start address for paga address mode
	writeOledCmd(0XA1);//A0:column address0 map to seg0,A1:column address127 map to seg0
	writeOledCmd(0XC8);//set com scan mode,C0:scan from COM0-COM[N - 1],C1:scan from COM[N - 1] to COM0
	writeOledCmd(0XDA);//set comm pin hardware configuration
	writeOledCmd(0X12);
	writeOledCmd(0X81);//set contrast control,follow by data 0-255
	writeOledCmd(0XCF);
	writeOledCmd(0XD9);//set pre-charge perid,follow by data
	writeOledCmd(0X22);
	writeOledCmd(0XDB);//set Vcomh deselect level,follow by data.0x00:0.65Vcc,0x20:0.77VCC,0x30:0.83VCC
	writeOledCmd(0x00);
	writeOledCmd(0XA4);//A4:display RAM,A5:display all,ignores RAM content
	writeOledCmd(0XA6);//A6:normal display,A7:inverse display
	writeOledCmd(0X8D);//set charge pump setting,must follow by data of 0X14 and 0xAF
	writeOledCmd(0X14);
	writeOledCmd(0XAF);
	clrOledRam();
}


void writeOled8x16Number(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number)
{
	fu8Xloc *= 8;
	for(uint8_t i = 0; i < 16; i ++){		
		if((i % 8) == 0){
			writeOledCmd(0xB0 | fu8Yloc);
			writeOledCmd(fu8Xloc & 0x0f);
			writeOledCmd(0x10 + (fu8Xloc >> 4));
			fu8Yloc ++;
		}
		writeOledData(gau8Number8x16[fu8Number][i]);
	}
}

void writeOled8x16Numbers(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum)
{
	while(*fpNum){
		writeOled8x16Number(fu8Xloc++, fu8Yloc, *fpNum++ - 0x30);
	}
}

void writeOled8x16ASCII(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number)
{
	fu8Xloc *= 8;
	for(uint8_t i = 0; i < 16; i ++){		
		if((i % 8) == 0){
			writeOledCmd(0xB0 | fu8Yloc);
			writeOledCmd(fu8Xloc & 0x0f);
			writeOledCmd(0x10 + (fu8Xloc >> 4));
			fu8Yloc ++;
		}
		writeOledData(gau8ASCII8x16[fu8Number][i]);
	}
}

void writeOled8x16ASCIIs(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum)
{
	while(*fpNum){
		writeOled8x16ASCII(fu8Xloc++, fu8Yloc, *fpNum++ - 'a');
	}
}

void writeOled5x7ASCII(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number)
{
	fu8Xloc *= 6;
	writeOledCmd(0xB0 | fu8Yloc);
	writeOledCmd(fu8Xloc & 0x0f);
	writeOledCmd(0x10 + (fu8Xloc >> 4));
	for(uint8_t i = 0; i < 6; i ++){		
		writeOledData(gau8ASCII5x7[fu8Number - 0x20][i]);
	}
}

void writeOled5x7Arrow(uint8_t fu8Xloc, uint8_t fu8Yloc, bool bClear)
{
	fu8Xloc *= 6;
	writeOledCmd(0xB0 | fu8Yloc);
	writeOledCmd(fu8Xloc & 0x0f);
	writeOledCmd(0x10 + (fu8Xloc >> 4));
	for(uint8_t i = 0; i < 6; i ++){		
		if(bClear){
			writeOledData(0x00);
		}
		else{
			writeOledData(gau8Arrow5x7[0][i]);
		}		
	}
}

void writeOled5x7ASCIIs(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum)
{
	while(*fpNum){
		writeOled5x7ASCII(fu8Xloc++, fu8Yloc, *fpNum++);
	}
}

void writeOled12x24Number(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number, int8_t fu8OffSet)
{
	fu8Xloc = fu8Xloc * 12 + fu8OffSet;
	for(uint8_t i = 0; i < 36; i ++){		
		if((i % 12) == 0){			
			writeOledCmd(0xB0 | fu8Yloc);
			writeOledCmd(fu8Xloc & 0x0f);
			writeOledCmd(0x10 + (fu8Xloc >> 4));
			fu8Yloc ++;
		}
		writeOledData(gau8Number12x24[fu8Number][i]);
	}

	if(fu8OffSet == 0)return;
	fu8Yloc -= 3;
	fu8Xloc += 12;
	if(fu8OffSet < 0){
		for(uint8_t i = 0; i < -fu8OffSet * 3; i ++){		
			if((i % 3) == 0){			
				writeOledCmd(0xB0 | fu8Yloc);
				writeOledCmd(fu8Xloc & 0x0f);
				writeOledCmd(0x10 + (fu8Xloc >> 4));
				fu8Yloc ++;
			}
			writeOledData(0x00);
		}
	}
}

void writeOled12x24NumberReserve(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number, int8_t fu8OffSet)
{
	fu8Xloc = fu8Xloc * 12 + fu8OffSet;
	for(uint8_t i = 0; i < 36; i ++){		
		if((i % 12) == 0){			
			writeOledCmd(0xB0 | fu8Yloc);
			writeOledCmd(fu8Xloc & 0x0f);
			writeOledCmd(0x12 + (fu8Xloc >> 4));
			fu8Yloc ++;
		}
		writeOledData(0xff ^ gau8Number12x24[fu8Number][i]);
	}

	if(fu8OffSet == 0)return;
	fu8Yloc -= 3;
	fu8Xloc += 12;
	if(fu8OffSet < 0){
		for(uint8_t i = 0; i < -fu8OffSet * 3; i ++){		
			if((i % 3) == 0){			
				writeOledCmd(0xB0 | fu8Yloc);
				writeOledCmd(fu8Xloc & 0x0f);
				writeOledCmd(0x10 + (fu8Xloc >> 4));
				fu8Yloc ++;
			}
			writeOledData(0xff);
		}
	}
}

void clrOled12x24Number(uint8_t fu8Xloc, uint8_t fu8Yloc, int8_t fu8OffSet)
{
	fu8Xloc = fu8Xloc * 12 + fu8OffSet;
	for(uint8_t i = 0; i < 36; i ++){		
		if((i % 12) == 0){			
			writeOledCmd(0xB0 | fu8Yloc);
			writeOledCmd(fu8Xloc & 0x0f);
			writeOledCmd(0x10 + (fu8Xloc >> 4));
			fu8Yloc ++;
		}
		writeOledData(0x00);
	}

	if(fu8OffSet == 0)return;

	fu8Yloc -= 3;
	fu8Xloc += 12;
	if(fu8OffSet < 0){
		for(uint8_t i = 0; i < -fu8OffSet * 3; i ++){		
			if((i % 3) == 0){			
				writeOledCmd(0xB0 | fu8Yloc);
				writeOledCmd(fu8Xloc & 0x0f);
				writeOledCmd(0x10 + (fu8Xloc >> 4));
				fu8Yloc ++;
			}
			writeOledData(0x00);
		}
	}
}

void writeOled12x24Numbers(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum)
{
	while(*fpNum){
		writeOled12x24Number(fu8Xloc++, fu8Yloc, *fpNum++ - 0x30, 0);
	}
}

void writeOled6x8Number(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t fu8Number, int8_t fu8OffSet)
{
	fu8Xloc = fu8Xloc * 6 + fu8OffSet;
	writeOledCmd(0xB0 | fu8Yloc);
	writeOledCmd(fu8Xloc & 0x0f);
	writeOledCmd(0x10 + (fu8Xloc >> 4));
	for(uint8_t i = 0; i < 6; i ++){			
		writeOledData(gau8Number6x8[fu8Number][i]);
	}
}

void writeOled6x8Numbers(uint8_t fu8Xloc, uint8_t fu8Yloc, uint8_t* fpNum)
{
	while(*fpNum){
		writeOled6x8Number(fu8Xloc++, fu8Yloc, *fpNum++ - 0x30, 0);
	}
}

void writeOled8x16TwoDot(uint8_t fu8Xloc, uint8_t fu8Yloc)
{
	fu8Xloc *= 8;
	for(uint8_t i = 0; i < 16; i ++){		
		//if((i & 0x07) == 0x07){
		if((i % 8) == 0){
			writeOledCmd(0xB0 | fu8Yloc);
			writeOledCmd(fu8Xloc & 0x0f);
			writeOledCmd(0x10 + (fu8Xloc >> 4));
			fu8Yloc ++;
		}
		writeOledData(gau8twodot8x16[0][i]);
	}
}

void clrOled8x16TwoDot(uint8_t fu8Xloc, uint8_t fu8Yloc)
{
	fu8Xloc *= 8;
	for(uint8_t i = 0; i < 16; i ++){		
		//if((i & 0x07) == 0x07){
		if((i % 8) == 0){
			writeOledCmd(0xB0 | fu8Yloc);
			writeOledCmd(fu8Xloc & 0x0f);
			writeOledCmd(0x10 + (fu8Xloc >> 4));
			fu8Yloc ++;
		}
		writeOledData(0x00);
	}
}

void welcomScreen(void)
{
	clrOledRam();
	gtSysVal.b1DispTime = false;
	writeOled5x7ASCIIs(0, 0, "Thank you for choosin");
	writeOled5x7ASCIIs(0, 2, "g BLE Multiboard and");
	writeOled5x7ASCIIs(0, 4, "rf_smart!^_^Press sel");
	writeOled5x7ASCIIs(0, 6, "ect key enter test!");
}
