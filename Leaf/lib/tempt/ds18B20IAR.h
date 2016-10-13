/*
* 	I build for country life.
* 	Author : Pham Dinh Van
*	----------------%-------------------
*	User case : GND TI --- GND DS18B20; VCC TI --- VCC DS18B20; P2.2 TI --- One-Wire bus Data DS18B20 
*/

#ifndef _DS18B20_LIB_H
#define _DS18B20_LIB_H 

#include <msp430.h>

union reg {	                          // define one byte with 8 bits 
    struct bit {                      // define bit on byte  
          unsigned char b0:1;					// the first bit on byte
          unsigned char b1:1;					// the second bit on byte 
          unsigned char b2:1;					// the third bit on byte
          unsigned char b3:1;					// the fourth bit on byte
          unsigned char b4:1;					// the fifth bit on byte
          unsigned char b5:1;					// the sixth bit on byte
          unsigned char b6:1;					// the seventh bit on byte
          unsigned char b7:1;					// the eighth bit on byte 
              }_BIT;
    unsigned char _BYTE;              // Name of byte
};

// The digital I/O registers 
union reg* _P2_DIRECT = (union reg*)0x2A ;		// P2-Direction Register _ address : 0x2Ah , name = _P2_DIRECT
union reg* _P2_OUT    = (union reg*)0x29 ;		// P2-Output Register    _ address : 0x29h , name = _P2_OUT
union reg* _P2_IN     = (union reg*)0x28 ;		// P2-Input Register 	   _ address : 0x28h , name = _P2_IN

#define     DQIN        (_P2_IN  -> _BIT.b2)
#define     DQOUT       (_P2_OUT -> _BIT.b2)

//@! -----------------------------------------

// ROM function commands 
#define READ_ROM            0x33
#define MATCH_ROM           0x55
#define SKIP_ROM            0xCC
#define SEARCH_ROM          0xF0
#define ALARM_SEARCH        0xEC 

//! DS18B20 function command set
// Temperature conversion commands 
#define CONVERT_T           0x44

// Memory function command
#define READ_SCRATCHPAD     0xBE
#define WRITE_SCRATCHPAD    0x4E
#define COPY_SCRATCHPAD     0x48
#define RECALL_E2           0xB8
#define READ_POWER_SUPPLY   0xB4 

//Define Resolutin for sensor
#define DS18B20_RESOLUTION          12   // select : 9, 10, 11, 12

//@! -----------------------------------------
//=======   Delay function 
void delay_18B20(unsigned int i);

//=======   Reset function
void DS18B20_Reset(void);

//=======   Read a byte 
unsigned char DS18B20_Read_Byte(void);

//=======   Write a byte
void DS18B20_Write_Byte(unsigned char dat);

//=======   Read Temperature 
float ReadTemperature(void);

//@! -----------------------------------------
//=======   Delay function
void delay_18B20(unsigned int i){
	while(i--);
}

//=======   Reset function
void DS18B20_Reset(void){
	_P2_DIRECT->_BIT.b2=1;
    DQOUT = 1;
    delay_18B20(8);
    DQOUT = 0;
    delay_18B20(80);
    DQOUT = 1;
    delay_18B20(14);
    _P2_DIRECT->_BIT.b2=0;
    delay_18B20(20);
}

//=======   Read a byte 
unsigned char DS18B20_Read_Byte(void){
	unsigned char i=0;
    unsigned char dat = 0;
    for (i=8;i>0;i--)
    {
        _P2_DIRECT->_BIT.b2=1;
          DQOUT = 0; 			// pull to a logic low level
          dat>>=1;
          DQOUT = 1; 			// pull to a logic high level
        _P2_DIRECT->_BIT.b2=0;
          if(DQIN)
          dat|=0x80;
          delay_18B20(4);
    }
    return(dat);
}

//=======   Write a byte
void DS18B20_Write_Byte(unsigned char dat){
	_P2_DIRECT->_BIT.b2=1;
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
  		DQOUT = 0;
  		DQOUT = dat&0x01;
  		delay_18B20(5);
  		DQOUT = 1;
  		dat>>=1;
	}
}

//=======   Read Temperature 
// with one DS18B20 on bus 
float ReadTemperature(void){
	unsigned int t;
	unsigned char a;
	unsigned char b;
 
  DS18B20_Reset();
  DS18B20_Write_Byte(SKIP_ROM);
  DS18B20_Write_Byte(CONVERT_T);

  delay_18B20(100);

  DS18B20_Reset();
  DS18B20_Write_Byte(SKIP_ROM);
  DS18B20_Write_Byte(READ_SCRATCHPAD);

  a = DS18B20_Read_Byte();
  b = DS18B20_Read_Byte();

  t = (b<<8) | a;

  if(t<0x8000)
  {

    return(t*0.0625);
  }
  else
  {
    t=(~t)+1;
    return(t*0.0625);
  }
}

#endif /*_DS18B20_LIB_H*/ 