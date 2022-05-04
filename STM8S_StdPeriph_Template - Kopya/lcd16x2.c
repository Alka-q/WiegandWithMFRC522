#include "lcd16x2.h"
#include "stm8s.h"
////
#define RS_H  GPIO_WriteHigh(GPIOD, GPIO_PIN_6)
#define RS_L  GPIO_WriteLow(GPIOD, GPIO_PIN_6)
#define E_H   GPIO_WriteHigh(GPIOD, GPIO_PIN_7)
#define E_L   GPIO_WriteLow(GPIOD, GPIO_PIN_7)
///
#define DB4_H   GPIO_WriteHigh(GPIOD, GPIO_PIN_0)
#define DB4_L   GPIO_WriteLow(GPIOD, GPIO_PIN_0)
#define DB5_H   GPIO_WriteHigh(GPIOD, GPIO_PIN_1)
#define DB5_L   GPIO_WriteLow(GPIOD, GPIO_PIN_1)
#define DB6_H   GPIO_WriteHigh(GPIOD, GPIO_PIN_2)
#define DB6_L   GPIO_WriteLow(GPIOD, GPIO_PIN_2)
#define DB7_H   GPIO_WriteHigh(GPIOD, GPIO_PIN_3)
#define DB7_L   GPIO_WriteLow(GPIOD, GPIO_PIN_3)
//////////////////////////////////////////////////////////////////////////////////////////////////
 void LCD_Enable(void)
{
   E_H;
   Delay_Basic(6);
   E_L;
   Delay_Basic(100); 
}
///////////////////////////////////////////////////////////////////////////////////
 void LCD_Send4Bit( unsigned char  Data )
{
  if((Data>>0)&0x01) DB4_H; else DB4_L;
  if((Data>>1)&0x01) DB5_H; else DB5_L;
  if((Data>>2)&0x01) DB6_H; else DB6_L;
  if((Data>>3)&0x01) DB7_H; else DB7_L;
}
//////////////////////////////////////////////////////////////////////////////////
 void LCD_SendCommand (unsigned char  command )
{
    LCD_Send4Bit  ( command >>4 );   /* Gui 4 bit cao */
    LCD_Enable () ;
    LCD_Send4Bit  ( command  );      /* Gui 4 bit thap*/
    LCD_Enable () ;
}
//////////////////////////////////////////////////////////////////////////////////
 void LCD_Init ( void )
{
    /////////////////////
    GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_FAST); //RS
    GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST); //E
    E_L; RS_L;
    //
    GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST); //DB4
    GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_OUT_PP_LOW_FAST); //DB5
    GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST); //DB6
    GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST); //DB7
    /////////////////////////////////////////////////////////////////
    LCD_Send4Bit(0x00);
    Delay_Basic(40000);
    RS_L;
    LCD_Send4Bit(0x03);
    LCD_Enable();
    Delay_Basic(10000);
    LCD_Send4Bit(0x03);
    LCD_Enable();
    Delay_Basic(200);
    LCD_Enable();
    LCD_Send4Bit(0x02);
    LCD_Enable();
    LCD_SendCommand( 0x28 );         // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
    LCD_SendCommand( 0x0c);          // cho phep hien thi man hinh
    LCD_SendCommand( 0x06 );         // tang ID, khong dich khung hinh
    LCD_Clear();                     // xoa toan bo khung hinh
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//x = 0 - 15.y= 0,1.
void LCD_Gotoxy(unsigned char  x, unsigned char  y)
{
  unsigned char  address;
  if(!y)
  address = (0x80+x);
  else
  address = (0xC0+x);
  Delay_Basic(2000);
  LCD_SendCommand(address);
  Delay_Basic(10000);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_Clear(void)
{
  LCD_SendCommand(0x01);  
  Delay_Basic(10000);
}
//////////////////////////////////////////////////////////////////////////////////////////////////
 void LCD_PutChar ( unsigned char  Data )
{
   RS_H;
   LCD_SendCommand( Data );
   RS_L;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
void LCD_Puts (unsigned char *s)
{
   while (*s)
   {
      LCD_PutChar(*s);
      s++;
   }
}
////////////////////////////////////////////////////////////////////////////////////////
void Delay_Basic(unsigned short nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////
const char character[] = {2,6,14,30,14,6,2,0};

void LCD_mychar(unsigned char x, unsigned char y){
  
	LCD_SendCommand(0x40);       //Load the location where we want to store
	LCD_PutChar(2);      //Load row 1 data
	LCD_PutChar(6);      //Load row 2 data
	LCD_PutChar(14);      //Load row 3 data
	LCD_PutChar(30);      //Load row 4 data
	LCD_PutChar(14);      //Load row 5 data
	LCD_PutChar(6);      //Load row 6 data
	LCD_PutChar(2);      //Load row 7 data
	LCD_PutChar(0);      //Load row 8 data
        LCD_Gotoxy(x,y);
        LCD_PutChar(0);
        
}
