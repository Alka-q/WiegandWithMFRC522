//https://stackoverflow.com/questions/65171601/how-to-correctly-read-data-returned-by-mfrc522-via-spi-using-stm8s103f3

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//------------------------------------------------------------------------------
//#include <iostm8s103f3.h>
//#include "STM8S_EEPROM.h"
#include "stm8s_conf.h"
#include "stm8s_flash.h"
#include "loc_rc522.h"
//#include "LCD16x2.h"

#define PAUSE_1us {asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");}

#define ARRAY_SIZE      20 
#define MF_KEYA_RQA     0x03
#define MF_KEYB_RQA     0x01
#define MF_LEN_READ     0x24

#define LED_ON          GPIO_WriteLow(GPIOB, GPIO_PIN_5);
#define LED_OFF         GPIO_WriteHigh(GPIOB, GPIO_PIN_5);

void ComMifare_Uart1_ISR(void); /* Interrupt Service rutine */
void SendData2uSett(void);
void SetAddress(void);
void Sen_GET_SNR(void);
void Send_MF_Read_CMD(void);
void Send_MF_Write_CMD();       //Cihaza gönderilecek WRITE CMD 
void SendDataToUart1(unsigned char *msg_string, unsigned char TCount);
void SendData2Uart1(unsigned char *msg_string, unsigned char TCount);
void UART1_Settings(unsigned char *BaudR);       //replaceable baud speed.

void AuthenticateError();

volatile unsigned char ComMifare_in;    //UART1 Recieve Method

/*Error message*/
void CLK_Config(void);
void GPIO_setup(void);
void UART1_setup(void);
void Delay (uint16_t time);
void Delay_ms (uint16_t time);
//void Hex2Binary(unsigned char *msg_string, unsigned char *PoutData);
//void Hex2Binary(unsigned char *msg_string, unsigned char *FirstNum, unsigned char *SecNum);
void Hex2Binary(unsigned char WIC, unsigned char *msg_string, unsigned char *FirstNum, 
                unsigned char *SecNum, unsigned long int *BOutData, unsigned char *BinaryData);
void ExplosionHexDecSys(unsigned char *ComingHex, unsigned char HexLen, unsigned char *FirstNum, unsigned char *SecNum);
void SendHexGetBinary(unsigned char *pInHex, unsigned char *pOutBinary);

void Dec2Hex(unsigned int dec);
char dec2hex[10];
char hex[ARRAY_SIZE];

int i = 0, value, buf;

long long bufferSize;

unsigned char CmdBuf[16];
unsigned char g_ucTempbuf[20];
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char SectorTrailer[16] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xFF,0x07,0x80,0x69,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
unsigned char ValueBlock[16] = {0x64,0x00,0x00,0x00,0x98,0xFF,0xFF,0xFF,0x64,0x00,0x00,0x00,0x01,0xFE,0x01,0xFE};
unsigned char ValueBlockV2[16] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x19,0x67,0x8E,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};   
unsigned char data[20];

unsigned int FIFOData;
unsigned char TSector;
unsigned char addr;     
unsigned char TBlockAddr;       //Block Addr
unsigned char ATQBuf[2];        //ATQ Data
unsigned char WBlockBuf[16];    //Write Data
unsigned char InBufM[64];
unsigned char KeyAB;            //Key A or Key B
unsigned int BufCntM = 0;
unsigned char status;
unsigned char g_ucTempbuf[20];
unsigned char Ucnt = 0;
int cnt = 0;
unsigned char MWSendData[] =" Authenticate Error ";

void delayY(long x)
{
  while(x--)
  {
    asm("nop");
  }
}
unsigned char HexData[8];//
//unsigned char BinaryData[8];
unsigned char FirtsD[8];
unsigned char SeconD[8];
unsigned long int BinaryData[8];
unsigned char Binary01[8];
unsigned char Binary32bit[32];
unsigned char ReqALL = PICC_REQALL;

void main(void)
{
  Dec2Hex(bufferSize);
  CLK_Config();
  GPIO_setup();
  /*MFRC522 Initiliaze*/
  SPIRC522_Init();
  PcdReset();
  while (1)
  {

/*  ATQBuf[2]; Card Type
    0x4400 = Mifare_UltraLight    
    0x0400 = Mifare_One(S50)    
    0x0200 = Mifare_One(S70)    
    0x0800 = Mifare_Pro(X)    
    0x4403 = Mifare_DESFire    */

    for(int cnt =0; cnt < 30; cnt++)
    {
      if(Binary32bit[cnt] == 0)
      { 
        LED_OFF;     
        //Delay_ms(1);
      }
      else
      {
        LED_ON;     
        //Delay_ms(1);
      }
      Delay(0xFFFF);
      
    }
     

//     GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
     status = PcdRequest(ReqALL, ATQBuf);  //(0x52 - 14443A), ,g_ucTempbuf
     if(status != MI_OK)
     {
       continue;
     }
     status = PcdAnticoll(g_ucTempbuf);
     if(status != MI_OK)
     {
       continue;
     }
     for(int IXI = 0; IXI < 8; IXI++)
     {
       HexData[IXI]= g_ucTempbuf[IXI];
     }     

////////////////////////////////////////////////////////////////////////////////
    for(unsigned char i=0; i<8; i++){
      
      Hex2Binary(i, HexData, FirtsD, SeconD, BinaryData, Binary01);
      
      if(i == 0)
      {
         for(unsigned char j=0; j <8; j++)
         {
           Binary32bit[j] = *(Binary01+j);
         }
      }
      
      else if(i == 1 )
      {
        for(unsigned char j=0; j <8; j++)
        {
          Binary32bit[j+8] = *(Binary01+j);
        }
      }
      else if(i == 2)
      {
         for(unsigned char j=0; j <8; j++)
         {
           Binary32bit[j+16] = *(Binary01+j);
         }
      }
      
      else if(i == 3)
      {
        for(unsigned char j=0; j <8; j++)
        {
          Binary32bit[j+24] = *(Binary01+j);
        }
      }
    }   
////////////////////////////////////////////////////////////////////////////////
    
     status = PcdSelect(g_ucTempbuf);
     if(status != MI_OK)
     {
       continue;
     }
     
     status = PcdAuthState(PICC_ANTICOLL1, 1, DefaultKey, g_ucTempbuf);
     if(status != MI_OK)
     {
       continue;
     }
 
        Delay(0xFFFF);

        ATQBuf[0] = 0;
        ATQBuf[1] = 0;
        
        for(int cnt = 0; cnt < 20; cnt++){
          g_ucTempbuf[cnt] = 0;

        }
     PcdHalt();
  }//End Of The While Loop
}//End Of The Main Foncion
void SendHexGetBinary(unsigned char *pInHex, unsigned char *pOutBinary)
{
  //Hex datasi girilip binaryy datasin alimasi beklenir.
}
void Hex2Binary(unsigned char WIC, unsigned char *msg_string, unsigned char *FirstNum, unsigned char *SecNum, 
                unsigned long int *BOutData, unsigned char *BinaryData)
{
  unsigned char GData[20];
  unsigned char FHtoB[20];
  unsigned char SHtoB[20];
  unsigned long int FData[20];
  unsigned long int SData[20];
  unsigned char SendSignal[8];
  unsigned char i;
  
//  for(WIC = 0; WIC < 8; WIC++)
//  { 
    GData[WIC] = *(msg_string + WIC);
    
    *(FirstNum + WIC) =  GData[WIC] % 16 ;       //ilk Basamak 

    *(SecNum + WIC) = GData[WIC] / 16 ;          //Ikinci Basnak
    
//    SonucData[i] = 16*BData[i] + SData[i];
//    *(PoutData + i) = SonucData[i];
    FHtoB[WIC] = *(FirstNum + WIC);
    SHtoB[WIC] = *(SecNum + WIC);
    
    switch (FHtoB[WIC])
    {
    case 0 : FData[WIC] = 0000, SendSignal[0]=0,SendSignal[1]=0,SendSignal[2]=0,SendSignal[3]=0; break;   
    case 1 : FData[WIC] = 0001, SendSignal[0]=0,SendSignal[1]=0,SendSignal[2]=0,SendSignal[3]=1; break;
    case 2 : FData[WIC] = 0010, SendSignal[0]=0,SendSignal[1]=0,SendSignal[2]=1,SendSignal[3]=1; break;
    case 3 : FData[WIC] = 0011, SendSignal[0]=0,SendSignal[1]=0,SendSignal[2]=1,SendSignal[3]=1; break;
    case 4 : FData[WIC] = 0100, SendSignal[0]=0,SendSignal[1]=1,SendSignal[2]=0,SendSignal[3]=0; break;
    case 5 : FData[WIC] = 0101, SendSignal[0]=0,SendSignal[1]=1,SendSignal[2]=0,SendSignal[3]=1; break;
    case 6 : FData[WIC] = 0110, SendSignal[0]=0,SendSignal[1]=1,SendSignal[2]=1,SendSignal[3]=0; break;
    case 7 : FData[WIC] = 0111, SendSignal[0]=0,SendSignal[1]=1,SendSignal[2]=1,SendSignal[3]=1; break;
    case 8 : FData[WIC] = 1000, SendSignal[0]=1,SendSignal[1]=0,SendSignal[2]=0,SendSignal[3]=0; break;
    case 9 : FData[WIC] = 1001, SendSignal[0]=1,SendSignal[1]=0,SendSignal[2]=0,SendSignal[3]=1; break;
    case 0x0A : FData[WIC] = 1010, SendSignal[0]=1,SendSignal[1]=0,SendSignal[2]=1,SendSignal[3]=0; break;
    case 0x0B : FData[WIC] = 1011, SendSignal[0]=1,SendSignal[1]=0,SendSignal[2]=1,SendSignal[3]=1; break;
    case 0x0C : FData[WIC] = 1100, SendSignal[0]=1,SendSignal[1]=1,SendSignal[2]=0,SendSignal[3]=0; break;
    case 0x0D : FData[WIC] = 1101, SendSignal[0]=1,SendSignal[1]=1,SendSignal[2]=0,SendSignal[3]=1; break;
    case 0x0E : FData[WIC] = 1110, SendSignal[0]=1,SendSignal[1]=1,SendSignal[2]=1,SendSignal[3]=0; break;
    case 0x0F : FData[WIC] = 1111, SendSignal[0]=1,SendSignal[1]=1,SendSignal[2]=1,SendSignal[3]=1; break;
    }
    
    switch (SHtoB[WIC])         
    {
    case 0 : SData[WIC] = 0000, SendSignal[4]=0,SendSignal[5]=0,SendSignal[6]=0,SendSignal[7]=0;  break;
    case 1 : SData[WIC] = 0001, SendSignal[4]=0,SendSignal[5]=0,SendSignal[6]=0,SendSignal[7]=1;  break;
    case 2 : SData[WIC] = 0010, SendSignal[4]=0,SendSignal[5]=0,SendSignal[6]=1,SendSignal[7]=1;  break;
    case 3 : SData[WIC] = 0011, SendSignal[4]=0,SendSignal[5]=0,SendSignal[6]=1,SendSignal[7]=1;  break;
    case 4 : SData[WIC] = 0100, SendSignal[4]=0,SendSignal[5]=1,SendSignal[6]=0,SendSignal[7]=0;  break;
    case 5 : SData[WIC] = 0101, SendSignal[4]=0,SendSignal[5]=1,SendSignal[6]=0,SendSignal[7]=1;  break;
    case 6 : SData[WIC] = 0110, SendSignal[4]=0,SendSignal[5]=1,SendSignal[6]=1,SendSignal[7]=0;  break;
    case 7 : SData[WIC] = 0111, SendSignal[4]=0,SendSignal[5]=1,SendSignal[6]=1,SendSignal[7]=1;  break;
    case 8 : SData[WIC] = 1000, SendSignal[4]=1,SendSignal[5]=0,SendSignal[6]=0,SendSignal[7]=0;  break;
    case 9 : SData[WIC] = 1001, SendSignal[4]=1,SendSignal[5]=0,SendSignal[6]=0,SendSignal[7]=1;  break;
    case 0x0A : SData[WIC] = 1010, SendSignal[4]=1,SendSignal[5]=0,SendSignal[6]=1,SendSignal[7]=0; break;
    case 0x0B : SData[WIC] = 1011, SendSignal[4]=1,SendSignal[5]=0,SendSignal[6]=1,SendSignal[7]=1; break;
    case 0x0C : SData[WIC] = 1100, SendSignal[4]=1,SendSignal[5]=1,SendSignal[6]=0,SendSignal[7]=0; break;
    case 0x0D : SData[WIC] = 1101, SendSignal[4]=1,SendSignal[5]=1,SendSignal[6]=0,SendSignal[7]=1; break;
    case 0x0E : SData[WIC] = 1110, SendSignal[4]=1,SendSignal[5]=1,SendSignal[6]=1,SendSignal[7]=0; break;
    case 0x0F : SData[WIC] = 1111, SendSignal[4]=1,SendSignal[5]=1,SendSignal[6]=1,SendSignal[7]=1; break;
    }

    *(BOutData + WIC) = ((SData[WIC]*2) + FData[WIC]);
    
    //*(BinaryData + WIC) = SendSignal[WIC];
  //} 
  for(int a = 0; a<8; a++){
    *(BinaryData + a) = SendSignal[a];
  }
}

/*
void Hex2Binary(unsigned char *msg_string, unsigned char *FirstNum, unsigned char *SecNum, unsigned long int *BOutData )
{
  unsigned char GData[20];
  unsigned char FHtoB[20];
  unsigned char SHtoB[20];
  unsigned long int FData[20];
  unsigned long int SData[20];

  unsigned char i;
  
  for(i = 0; i < 8; i++)
  { 
    GData[i] = *(msg_string + i);
    
    *(FirstNum + i) =  GData[i] % 16 ;       //ilk Basamak 

    *(SecNum + i) = GData[i] / 16 ;          //Ikinci Basnak
    
//    SonucData[i] = 16*BData[i] + SData[i];
//    *(PoutData + i) = SonucData[i];
    FHtoB[i] = *(FirstNum + i);
    SHtoB[i] = *(SecNum + i);
    
    switch (FHtoB[i])
    {
       case 0 : FData[i] = 0000; break;
       case 1 : FData[i] = 0001; break;
       case 2 : FData[i] = 0010; break;
       case 3 : FData[i] = 0011; break;
       case 4 : FData[i] = 0100; break;
       case 5 : FData[i] = 0101; break;
       case 6 : FData[i] = 0110; break;
       case 7 : FData[i] = 0111; break;
       case 8 : FData[i] = 1000; break;
       case 9 : FData[i] = 1001; break;
       case 0x0A : FData[i] = 1010; break;                  //case 'A' : SData[i] = 1010; break;
     // case 0x0a : SData[i] = 1010; break;                  //case 'a' : SData[i] = 1010; break;
       case 0x0B : FData[i] = 1011; break;                  //case 'B' : SData[i] = 1011; break;
       //case 0x0b : SData[i] = 1011; break;                  //case 'b' : SData[i] = 1011; break;
       case 0x0C : FData[i] = 1100; break;                  //case 'C' : SData[i] = 1100; break;
     // case 'c' : SData[i] = 1100; break;                  //case 'c' : SData[i] = 1100; break;
       case 0x0D : FData[i] = 1101; break;                  //case 'D' : SData[i] = 1101; break;
     // case 'd' : SData[i] = 1101; break;                  //case 'd' : SData[i] = 1101; break;
       case 0x0E : FData[i] = 1110; break;                  //case 'E' : SData[i] = 1110; break;
     // case 'e' : SData[i] = 1110; break;                  //case 'e' : SData[i] = 1110; break;
       case 0x0F : FData[i] = 1111; break;                  //case 'F' : SData[i] = 1111; break;
     // case 'f' : SData[i] = 1111; break;                  //case 'f' : SData[i] = 1111; break;
    }
    
    switch (SHtoB[i])
    {
       case 0 : SData[i] = 0000; break;
       case 1 : SData[i] = 0001; break;
       case 2 : SData[i] = 0010; break;
       case 3 : SData[i] = 0011; break;
       case 4 : SData[i] = 0100; break;
       case 5 : SData[i] = 0101; break;
       case 6 : SData[i] = 0110; break;
       case 7 : SData[i] = 0111; break;
       case 8 : SData[i] = 1000; break;
       case 9 : SData[i] = 1001; break;
       case 0x0A : SData[i] = 1010; break;                  //case 'A' : SData[i] = 1010; break;
     // case 0x0a : SData[i] = 1010; break;                  //case 'a' : SData[i] = 1010; break;
       case 0x0B : SData[i] = 1011; break;                  //case 'B' : SData[i] = 1011; break;
       //case 0x0b : SData[i] = 1011; break;                  //case 'b' : SData[i] = 1011; break;
       case 0x0C : SData[i] = 1100; break;                  //case 'C' : SData[i] = 1100; break;
     // case 'c' : SData[i] = 1100; break;                  //case 'c' : SData[i] = 1100; break;
       case 0x0D : SData[i] = 1101; break;                  //case 'D' : SData[i] = 1101; break;
     // case 'd' : SData[i] = 1101; break;                  //case 'd' : SData[i] = 1101; break;
       case 0x0E : SData[i] = 1110; break;                  //case 'E' : SData[i] = 1110; break;
     // case 'e' : SData[i] = 1110; break;                  //case 'e' : SData[i] = 1110; break;
       case 0x0F : SData[i] = 1111; break;                  //case 'F' : SData[i] = 1111; break;
     // case 'f' : SData[i] = 1111; break;                  //case 'f' : SData[i] = 1111; break;
    }
    
    *(BOutData + i) = ((SData[i]*2) + FData[i]);
    
  } 
}*/
/**********Hexadecimal sayi sitemini parçala************************/
void ExplosionHexDecSys(unsigned char *ComingHex, unsigned char HexLen, unsigned char *FirstNum, unsigned char *SecNum)
{
  unsigned char GData[20];
//  unsigned char BData[HexLen];
//  unsigned char SData[HexLen];
//  unsigned char SonucData[HexLen];
  unsigned char i;
  unsigned int Lenght;
  Lenght = strlen(ComingHex); //sizeof(ComingHex);
  Lenght = 20;
  //for(i = 0; i < HexLen; i++)
  for(i = 0; i < Lenght; i++)
  {
    GData[i] = *(ComingHex + i);
    
    *(FirstNum + i) =  GData[i] % 16 ;       //ilk Basamak 

    *(SecNum + i) = GData[i] / 16 ;          //Ikinci Basnak
    
//    SonucData[i] = 16*BData[i] + SData[i];
//    *(PoutData + i) = SonucData[i];
  }
}

/******************************************************************************/
void SendDataToUart1(unsigned char *msg_string, unsigned char TCount){

  unsigned int ixi;
  Delay(1000);

  for(ixi=0; ixi<9999; ixi++){
    PAUSE_1us;
    PAUSE_1us;
    PAUSE_1us;
    PAUSE_1us;
    PAUSE_1us;
  }

  for(ixi=0;  ixi<TCount; ixi++){
    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
      UART1_SendData8(msg_string[ixi]);
  }
  while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  
  for(ixi=0; ixi<9999; ixi++){
    PAUSE_1us;
    PAUSE_1us;
    PAUSE_1us;
    PAUSE_1us;
    PAUSE_1us;
  }
  //BufCntM = 0;
  Delay(1000);
}
/******************************************************************************/
void SendData2Uart1(unsigned char *msg_string, unsigned char TCount){

  unsigned int ixi;
  Delay_ms(20);
  
  for(ixi=0;  ixi<TCount; ixi++){
    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
      UART1_SendData8(msg_string[ixi]);
  }
  while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
  
  Delay_ms(20);
}
/******************************************************************************/

/******************************************************************************/

void Delay (uint16_t time){
  
    while(time--);
}

void Delay_ms (uint16_t time){  
    //time = 1000 olursa 1 sn lik bir fonsiyon olusturulmus olur.
  for(int cnt =0; cnt < 16; cnt++){
    while(time--);
  }
}
/******************************************************************************/

/******************************************************************************/
  //UART1 Interrupt
void ComMifare_Uart1_ISR(void)
{
  if ( BufCntM < 34 ) //Buffer is full, No Receive
  {
    InBufM[BufCntM] = ComMifare_in;
    ++BufCntM;
  }
  Delay(1000);
  //BufCntM = 0;
}
/******************************************************************************/
/*
void Delay(uint32_t nCount){
    while(nCount != 0){
      nCount--;
  }
}
*/
/******************************** WAIT DELAY ***********************************/
/*
void WaitDelay(unsigned short TWaitTime)
{
  unsigned long TWT = TWaitTime * 30;
  
  while(TWT--)
  {
    asm("nop");
  }
}
*/
/*******************************************************************************/
///////////////////////////////////////////////////////
 void Dec2Hex(unsigned int bufferSize)
{ 
  bufferSize = 0x00;
  dec2hex[0]=bufferSize/16;
  if(dec2hex[0]>9)
  {
    dec2hex[0]=dec2hex[0]+55;
  }
  else dec2hex[0]=dec2hex[0]+48;
  /////////////////
  dec2hex[1]=bufferSize%16;
  if(dec2hex[1]>9)
  {
    dec2hex[1]=dec2hex[1]+55;
  }
  else dec2hex[1]=dec2hex[1]+48;
}

 ///////////////////////////////////////////////////////
void CLK_Config(void)
{
  /* Initialization of the clock */
  /* Clock divider to HSI/1 */
  CLK_DeInit();
 
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      //f_Master = HSI/1 = 16MHz
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);            //f_CPU = f_Master/1 = 16MHz
  while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY)!=SET);     //wait until HSI ready;
  
/*	CLK_HSECmd(DISABLE);
	CLK_LSICmd(DISABLE);
	CLK_HSICmd(ENABLE);
	while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) != FALSE);

	CLK_ClockSwitchCmd(ENABLE);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, 
	DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
	
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
*/
}

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************

      // Bursasi için SDK yazilmasi gerek.
      // ilk iki byte'i AA, 00, olmali.
      // SendBuf[5] = 0x4D mesaj kodu.
      // " SendUart1Setting(); " Fonksiyonu yazilacak.
 
*******************************************************************************/

void UART1_setup(void)
{
	UART1_DeInit();
	
	UART1_Init(9600, 
                        UART1_WORDLENGTH_8D, 
			UART1_STOPBITS_1, 
			UART1_PARITY_NO, 
			UART1_SYNCMODE_CLOCK_DISABLE, 
			UART1_MODE_TXRX_ENABLE);
	
        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
        
        enableInterrupts();
	UART1_Cmd(ENABLE);
}

void GPIO_setup(void)
{		
  GPIO_DeInit(GPIOB);	
        
        GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);
        
  GPIO_DeInit(GPIOD);
	
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);
}

void RCC522_DeInit(void)
{
  SPI_Cmd(DISABLE); /*!< SD_SPI disable */
  /*!< SD_SPI Peripheral clock disable */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);

  /*!< Configure SD_SPI pins: SCK */
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);

  /*!< Configure SD_SPI pins: MISO */
  GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);

  /*!< Configure SD_SPI pins: MOSI */
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_SLOW);

  /*!< Configure SD_SPI_CS_PIN pin: SD Card CS pin */
  GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);
}

/*
  unsigned char ManufacturerData_0[16];
  unsigned char BlocData_1[16];
  unsigned char BlocData_2[16];
  unsigned char BlocData_3[16];   //Sector Trailer
  unsigned char BlocData_4[16];
  unsigned char BlocData_5[16];
  unsigned char BlocData_6[16];
  unsigned char BlocData_7[16];   //Sector Trailer
  unsigned char BlocData_8[16];
  unsigned char BlocData_9[16];
  unsigned char BlocData_10[16];
  unsigned char BlocData_11[16];  //Sector Trailer
  unsigned char BlocData_12[16];
  unsigned char BlocData_13[16];
  unsigned char BlocData_14[16];
  unsigned char BlocData_15[16];  //Sector Trailer
  unsigned char BlocData_16[16];
*/

/**
  * @brief  Initializes the SD_SPI and CS pins.
  * @param  None
  * @retval None
  */
/////////////////////////////////////////////////
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************************************************************************************************/













///////////////////////////////
////OKUMA PROGRAMI///////////
//
////https://stackoverflow.com/questions/65171601/how-to-correctly-read-data-returned-by-mfrc522-via-spi-using-stm8s103f3
//
///* Includes ------------------------------------------------------------------*/
////Hexadecimal to decimal converter.
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
////-------------------------------------------- 
//
//#include "stm8s_conf.h"
//#include "loc_rc522.h"
////#include "LCD16x2.h"
//
//// 1 US
//#define PAUSE_1us {asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");}
//#define ARRAY_SIZE  20 
//#define MF_KEYA_RQA     0x03
//#define MF_KEYB_RQA     0x01
//#define MF_LEN_READ     0x24
//
//void ComMifare_Uart1_ISR(void);
//void mifareHab(void);
//void Sen_GET_SNR(void);
//void Send_MF_Read_CMD(/*unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char * Key*/ void );
////void Send_MF_Write_CMD(unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char * Key, unsigned char * TBlockData);
//void SendDataToUart1(unsigned char *msg_string, unsigned char TCount);
//void AuthenticateError();
//void MF_Write_CMD();            //Weite Command
//void MF_Read_CMD();             //Read Command
//void MF_SerialNum_CMD();        //Serial Number Command
//void Send_MF_Write_CMD();       //Cihaza gönderilecek WRITE CMD 
///* Interrupt Service rutine */
///*Error message*/
//void CLK_Config(void);
//void GPIO_setup(void);
//void UART1_setup(void);
//void Delay (uint16_t time);
////void Delay(uint32_t nCount);
//void Dec2Hex(unsigned int dec);
//char dec2hex[10];
//char hex[ARRAY_SIZE];
//long long decimal = 0;
//long long base = 1;
//int size;        //----------------// Boyut için
//
//int i = 0, value, buf;
//char a=0,b=0;
//
//long long bufferSize;
//
//unsigned char Version;
//unsigned int v;
//unsigned int FIFOData;
//
//unsigned char CmdBuf[16];
//unsigned char InBufM[32];
//unsigned int BufCntM = 0;
//unsigned char status;
//unsigned char g_ucTempbuf[20];
//unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//unsigned char SectorTrailer[16] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xFF,0x07,0x80,0x69,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
//unsigned char ValueBlock[16] = {0x64,0x00,0x00,0x00,0x98,0xFF,0xFF,0xFF,0x64,0x00,0x00,0x00,0x01,0xFE,0x01,0xFE};
//unsigned char data1[16] = {0x4f, 0x73, 0x6d, 0x61, 0x6e, 0x20, 0x43, 0x65, 0x79 ,0x6c, 0x61, 0x6e};
//unsigned char data2[16] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0x17,0x81,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//unsigned char data[20];
//
//unsigned char data3t[20]={0x6e,0x73,0x61,0x6c, 0x65,0x69, 0xFF,0x6c, 0x6e,0xAA,0x75};
//unsigned char UID[7];//= {0,0,0,0};
//
//volatile unsigned char ComMifare_in;
//
//unsigned char TSector;
//unsigned char addr;
//unsigned char TBlockAddr;
//unsigned char ATQBuf[2];
//unsigned char WBlockBuf[16];
//
////unsigned char InBuflenght;
//
//unsigned char MWSendData[] =" Authenticate Error ";
//
///*unsigned char KeyData[0] = 0xAA;
//  unsigned char KeyData[1] = 0xBB;
//  unsigned char KeyData[2] = 0xCC;
//  unsigned char KeyData[3] = 0xDD;
//  unsigned char KeyData[4] = 0x86;
//  unsigned char KeyData[5] = 0x91;*/
//
//void delayY(long x)
//{
//  while(x--)
//  {
//    asm("nop");
//  }
//}
//
//unsigned char ReqALL = PICC_REQALL;
//
//void main(void)
//{
//  int Len = strlen(MWSendData);
//  Dec2Hex(bufferSize);
//  CLK_Config();
//  UART1_setup();
//  GPIO_setup();
//  SPIRC522_Init();
//  PcdReset();
//  //Init_spi_software();
//  while (1)
//  {
//    ComMifare_Uart1_ISR();
//    //if()
//    mifareHab(); //BufCntM = 0;
//  //InBufM[1] = 0x01;
///*******************************Make SNR***************************************/
//    if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x03 || InBufM[3] != 0x25){
//      /*
//      *continue;   
//      */
//    }
//    else{
//      //MF_SerialNum_CMD();
//     status = PcdRequest(PICC_REQALL, ATQBuf);  //(0x52 - 14443A), ,g_ucTempbuf
//     if(status != MI_OK){
//      continue;
//     }
//     status = PcdAnticoll(g_ucTempbuf);
//     if(status != MI_OK){
//       continue;
//     }
//     else{
//       Sen_GET_SNR();           //Send serial nummer
//     }
//     status = PcdSelect(g_ucTempbuf);
//     if(status != MI_OK){
//       continue;
//     }
//     Delay(0xFFFF);
//     /***********Buffer Sifirlama********************************/
//        ATQBuf[0] = 0;
//        ATQBuf[1] = 0;
//        for(int cnt = 0; cnt < 32; cnt++){
//          g_ucTempbuf[cnt] = 0;
//          InBufM[cnt] = 0;
//        }
//     /***********Buffer Sifirlama********************************/
//         PcdHalt();
//    }
///***************************END OF THE Make SNR********************************/
//////////////////////////////////////////////////////////////////////////////////
///*******************************MF_READ_CMD************************************/
//    InBufM[0] = 0xAA;
//    InBufM[1] = 0x00;
//    InBufM[2] = 0x0A;
//    InBufM[3] = 0x20;
//    if(InBufM[0] != 0xAA && InBufM[1] != 0x00 && InBufM[2] != 0x0A && InBufM[3] != 0x20){
//      //continue;
//    }
//    else{
//      //MF_Read_CMD();           //Read Data a Block of MF Card
//     status = PcdRequest(PICC_REQALL, ATQBuf);  //(0x52 - 14443A), ,g_ucTempbuf
//     if(status != MI_OK){
//       continue;
//     }
//     status = PcdAnticoll(g_ucTempbuf);
//     if(status != MI_OK){
//       continue;
//     }
//     else{
//       //Sen_GET_SNR();           //Send serial nummer
//     }
//     status = PcdSelect(g_ucTempbuf);
//     if(status != MI_OK){
//       continue;
//     }
//       
//       TBlockAddr = InBufM[5];     // Number of Block: 1
//       //TSector = InBufM[6];             //Sector
//       //Key Bytes
//       for(int cnt=0; cnt<6; cnt++){
//         CmdBuf[cnt] = InBufM[cnt+7];
//       }
//       
//        status = PcdAuthState(PICC_AUTHENT1A, TBlockAddr, DefaultKey/*DefaultKey*/, g_ucTempbuf); //SectorTrailer, DefaultKey, CmdBuf
//        if(status != MI_OK){
//          continue;
//        }else{
//          //Buffer sifirlama
//          for(int cnt=0; cnt<6; cnt++)
//            CmdBuf[cnt] = 0x00;
//        }
//        status = PcdRead(TBlockAddr, data);
//        if(status != MI_OK){
//          continue;
//        }else{
//          Send_MF_Read_CMD();
//        }
//     
//      /*************************MF_READ_CMD**********************************/
//     
//        ///////////////////DATA VE KART BILGISI YOLLAMA/////////////////////
//       Delay(0xFFFF);
//        //////////////////////DATA BUFFER SIFIRLAMA////////////////////////
//      for(unsigned int b = 0;b<16 ;b++){
//        data[b] = 0; 
//      }
//      Delay(0xFFFF);
//       /////////////////////////KART BUFFER SIFIRLAMA////////////////////
//       for(unsigned int d = 0;d<32 ;d++){
//         g_ucTempbuf[d] = 0;
//         InBufM[d]= 0;
//       } 
//       
//       PcdHalt();
//}
///************************END OF THE MF_READ_CMD********************************/
//////////////////////////////////////////////////////////////////////////////////
///*****************************MF_Write_CMD*************************************/
//    /*InBufM[0] = 0xAA;
//    InBufM[1] = 0x00;
//    InBufM[2] = 0x1A;
//    InBufM[3] = 0x21;*/
//    if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x1A || InBufM[3] != 0x21){            //Write Commond
//      continue;
//    }
//    else{
//      //MF_Write_CMD();        //Write Data a Block of MF Card
//           status = PcdAnticoll(g_ucTempbuf);
//     if(status != MI_OK){
//       //continue;
//     }
//     else{
//       //Sen_GET_SNR();           //Send serial number
//     }
//     
//     status = PcdSelect(g_ucTempbuf);
//     if(status != MI_OK){
//       //continue;
//     }
//     TBlockAddr = InBufM[5];     // Number of Block: 1
//     TBlockAddr = 0x0A; // addr = 10
//     //TSector = InBufM[6];             //Sector
//     //Key Bytes
//     for(int cnt=0; cnt<6; cnt++){
//       CmdBuf[cnt] = InBufM[cnt+7];
//     }
//      status = PcdAuthState(PICC_AUTHENT1A, TBlockAddr, DefaultKey/*CmdBuf*/, g_ucTempbuf); //SectorTrailer, DefaultKey
//      if(status != MI_OK){
//        //continue;
//      }
//      else{
//        //Buffer sifirlama
//        for(int cnt=0; cnt<6; cnt++)
//          CmdBuf[cnt] =  0;
//      } 
//      for(int cnt=0; cnt<16; cnt++){
//        WBlockBuf[cnt] = InBufM[cnt];//data1[cnt];
//      }
//      //
//      status = PcdWrite(TBlockAddr, WBlockBuf);
//      if(status != MI_OK){
//        //continue;
//        //WriteError
//      }
//      else{
//        Send_MF_Write_CMD();           //'Write' Send Data, 
//        TBlockAddr = 0x00;
//      }
//      /*******************MF_Write_CMD****************************/
//     
//      ///////////////////DATA VE KART BILGISI YOLLAMA/////////////////////
//       Delay(0xFFFF);
//     //////////////////////DATA BUFFER SIFIRLAMA////////////////////////
//      for(unsigned int b = 0;b<16 ;b++){
//        data[b] = 0; 
//      }
//        Delay(0xFFFF);
//          /////////////////////////KART BUFFER SIFIRLAMA////////////////////
//       for(unsigned int d = 0;d<20 ;d++){
//         g_ucTempbuf[d] =0;
//       } 
//       PcdHalt();
//}
///****************************END OF THE MF_Write_CMD***************************/
//////////////////////////////////////////////////////////////////////////////////
//  }//End Of The While 
//}//End Of The Main Foncion
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//
//void MF_SerialNum_CMD(){
//   
//
//     }
//
///*****************************MF_Write_CMD****************************/
//
//void MF_Write_CMD(){
//     //
//
//}
//  /////////////////////////////////////////////////////
//void MF_Read_CMD(){
//      
//
//}
//
//void AuthenticateError(){
//  unsigned char ERR[]= " Authenticate Error ";
//  unsigned int Len = sizeof(ERR);//strlen(ERR);
//  unsigned int mCount;
//  //sizeof(ERR);
//  BufCntM = 0;
//  
//  for(mCount = 0; mCount < Len; mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//      UART1_SendData8(ERR[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                   
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//}
//
///*******************************************************************************/
//
///*******************************************************************************/
///*
//void MapOfCards(unsigned char *DataOfBlock[16]){
//  
//  int i=0;
//  
//  for(i=0; i<16; i++){
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//    UART1_SendData8(*DataOfBlock[i]);
//  }
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//}
//*/
///*******************************************************************************/
//
//void mifareHab(void){
//  
//  unsigned char TmpBuf[8];
//  //BufCntM = 0;
///*
//    //•ùµ³ÿÿ‰
//  TmpBuf[0] = 0xAA ; //'•'       0x95
//  TmpBuf[1] = 0x00 ; //'ù'       0xf9
//  TmpBuf[2] = 0x03 ; //'µ'       0xb5
//  TmpBuf[3] = 0x25 ; //'³'       0xb3
//  TmpBuf[4] = 0x26 ; //'ÿ'       0xff
//  TmpBuf[5] = 0x00 ; //'ÿ'       0xff
//  TmpBuf[6] = 0x00 ; //'‰'       0x89
//  TmpBuf[7] = 0xBB ; //
//*/
//                    //0xAA     Modülün110'a gönderdigi veri 
//  TmpBuf[0] = 0xAA;  // 0x00
//  TmpBuf[1] = 0x00;  // 0x03
//  TmpBuf[2] = 0x02;  // 0x25
//  TmpBuf[3] = 0x01;  // 0x26
//  TmpBuf[4] = 0x83;  // 0x00
//  TmpBuf[5] = 0x80;  // 0x00
//  TmpBuf[6] = 0xBB;  // 0xBB
//  
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 7);
// // SendDataToUart1(TmpBuf, 7);
//}
//void Sen_GET_SNR(void){
//  
//  unsigned char TmpBuf[12];
//
//  TmpBuf[0]=0xAA;
//  TmpBuf[1]=0x00;  // bu indexin çikarilmasi gere//0X02
//  TmpBuf[2]=0x06;               //0x06
//  TmpBuf[3]=0X00;               //MF_Get_SNR(0x25)
//  TmpBuf[4]=0x00;               //Bir kart var
//      //Kart seri no
//      //Tersleme yapiliyor... seri no. için
//  TmpBuf[5]=g_ucTempbuf[3];//g_ucTempbuf[0];   
//  TmpBuf[6]=g_ucTempbuf[2];//g_ucTempbuf[1];   
//  TmpBuf[7]=g_ucTempbuf[1];//g_ucTempbuf[2];   
//  TmpBuf[8]=g_ucTempbuf[0];//g_ucTempbuf[3];   
//      //BCC
//  TmpBuf[9]=0;
//  for(int ixi=0; ixi<9; ixi++){
//    TmpBuf[9]^=TmpBuf[ixi];}
//    //ETX 
//  TmpBuf[10]=0xBB; 
//  
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 11);
//}
//
///******************************************************************************/
//
//void Send_MF_Read_CMD( void){ /*, unsigned Char TBlockData*/ /*unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char * Key*/
//  unsigned char TmpBuf[27];
//  unsigned char MI;
//  
//  TmpBuf[0]=0xAA;
//  TmpBuf[1]=0x00;
//  TmpBuf[2]=0x15;
//  TmpBuf[3]=0x00;  //MI_OK
//    //Kart seri no.
//  TmpBuf[4]=g_ucTempbuf[3];
//  TmpBuf[5]=g_ucTempbuf[2];
//  TmpBuf[6]=g_ucTempbuf[1];
//  TmpBuf[7]=g_ucTempbuf[0];
//    //Okunan blok 
//  for(int ixi=8; ixi<24;ixi++){
//    TmpBuf[ixi]=data[ixi-9];
//  }
//  
//  TmpBuf[25]=0;
//  for(MI=0; MI<25;MI++){
//    TmpBuf[25]^=TmpBuf[MI];}      //0x92
//  //ETX
//  TmpBuf[26]=0xBB;
//  
//  BufCntM = 0;
//  //do{
//    //status = PcdRead(1, data);
//    //if(status == MI_OK){
//        SendDataToUart1(TmpBuf, 27); 
//      //}
//  //}while(status == MI_OK);
//  //status=0;
//}
///******************************************************************************/
//void Send_MF_Write_CMD(){       //unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char * Key, unsigned char * TBlockData
// // unsigned char TmpBuf[10];
//  unsigned char TmpBuf[10];
//  
//  TmpBuf[0] = 0xAA;
//  TmpBuf[1] = 0x00;
//  TmpBuf[2] = 0x05;
//  TmpBuf[3] = 0x00;
//  TmpBuf[4] = g_ucTempbuf[3];
//  TmpBuf[5] = g_ucTempbuf[2];
//  TmpBuf[6] = g_ucTempbuf[1];
//  TmpBuf[7] = g_ucTempbuf[0];
//  TmpBuf[8] = 0x84;
//  TmpBuf[9] = 0xBB;
//  
//  sizeof(TmpBuf);
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, sizeof(TmpBuf));
//}
///******************************************************************************/
//
//void SendDataToUart1(unsigned char *msg_string, unsigned char TCount){
//
//  unsigned int ixi;
//  Delay(30000);
//
//  for(ixi=0; ixi<9999; ixi++){
//  PAUSE_1us;
//  PAUSE_1us;
//  PAUSE_1us;
//  PAUSE_1us;
//  PAUSE_1us;}
//
//  for(ixi=0;  ixi<TCount; ixi++){
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//      UART1_SendData8(msg_string[ixi]);
//  }
//  while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//  
//  for(ixi=0; ixi<9999; ixi++){
//  PAUSE_1us;
//  PAUSE_1us;
//  PAUSE_1us;
//  PAUSE_1us;
//  PAUSE_1us;}
//  
//  Delay(30000);
//}
//
//void Delay (uint16_t time){
//  
//    while(time--); 
//}
//
///****************************************************************/
//  //Interrupt
//void ComMifare_Uart1_ISR(void)
//{
//  if ( BufCntM < 32 ) //Buffer is full, No Receive
//  {
//    InBufM[BufCntM] = ComMifare_in;
//    ++BufCntM;
//    
//    /*if(BufCntM==32
//        BufCntM=0; */
//  }
//}
///***************************************************************/
//
///*
//void Delay(uint32_t nCount){
//  while(nCount != 0){
//    nCount--;
//  }
//}*/
// ///////////////////////////////////////////////////////
//
// void Dec2Hex(unsigned int bufferSize)
//{ 
//  bufferSize = 0x00;
//  dec2hex[0]=bufferSize/16;
//  if(dec2hex[0]>9)
//  {
//    dec2hex[0]=dec2hex[0]+55;
//  }
//  else dec2hex[0]=dec2hex[0]+48;
//  /////////////////
//  dec2hex[1]=bufferSize%16;
//  if(dec2hex[1]>9)
//  {
//    dec2hex[1]=dec2hex[1]+55;
//  }
//  else dec2hex[1]=dec2hex[1]+48;
//}
//
// ///////////////////////////////////////////////////////
//void CLK_Config(void)
//{
//  /* Initialization of the clock */
//  /* Clock divider to HSI/1 */
// CLK_DeInit();
// 
//  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      //f_Master = HSI/1 = 16MHz
//  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);            //f_CPU = f_Master/1 = 16MHz
//  while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY)!=SET);     //wait until HSI ready;
//	
///*	CLK_HSECmd(DISABLE);
//	CLK_LSICmd(DISABLE);
//	CLK_HSICmd(ENABLE);
//	while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY) != FALSE);
//	
//	CLK_ClockSwitchCmd(ENABLE);
//	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
//	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
//	
//	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, 
//	DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE);
//	
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);	
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);*/
//}
///////////////////////////////////////////////////////
//
//void UART1_setup(void)
//{
//	UART1_DeInit();
//	
//	UART1_Init(9600, 
//			UART1_WORDLENGTH_8D, 
//			UART1_STOPBITS_1, 
//			UART1_PARITY_NO, 
//			UART1_SYNCMODE_CLOCK_DISABLE, 
//			UART1_MODE_TXRX_ENABLE);
//	
//        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
//        enableInterrupts();
//	UART1_Cmd(ENABLE);
//}
//
//void GPIO_setup(void)
//{		
//  GPIO_DeInit(GPIOB);	
//        
//        GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);
//        
//  GPIO_DeInit(GPIOD);
//	
//	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
//	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);
//}
//
//void RCC522_DeInit(void)
//{
//  SPI_Cmd(DISABLE); /*!< SD_SPI disable */
//  /*!< SD_SPI Peripheral clock disable */
//  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
//
//  /*!< Configure SD_SPI pins: SCK */
//  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
//
//  /*!< Configure SD_SPI pins: MISO */
//  GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);
//
//  /*!< Configure SD_SPI pins: MOSI */
//  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_SLOW);
//
//  /*!< Configure SD_SPI_CS_PIN pin: SD Card CS pin */
//  GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);
//}
//
///**
//  * @brief  Initializes the SD_SPI and CS pins.
//  * @param  None
//  * @retval None
//  */
///////////////////////////////////////////////////
//#ifdef USE_FULL_ASSERT
//
///**
//  * @brief  Reports the name of the source file and the source line number
//  *   where the assert_param error has occurred.
//  * @param file: pointer to the source file name
//  * @param line: assert_param error line source number
//  * @retval : None
//  */
//void assert_failed(u8* file, u32 line)
//{ 
//  /* User can add his own implementation to report the file name and line number,
//     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//
//  /* Infinite loop */
//  while (1)
//  {
//  }
//}
//#endif
//
///******************************************************************************************************/
