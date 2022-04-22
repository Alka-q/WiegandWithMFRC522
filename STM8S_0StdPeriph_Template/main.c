
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

void ComMifare_Uart1_ISR(void); /* Interrupt Service rutine */
void SendData2uSett(void);
void SetAddress(void);
void Sen_GET_SNR(void);
void Send_MF_Read_CMD(void);
void Send_MF_Write_CMD();       //Cihaza gönderilecek WRITE CMD 
void SendDataToUart1(unsigned char *msg_string, unsigned char TCount);
void SendData2Uart1(unsigned char *msg_string, unsigned char TCount);
void UART1_Settings(unsigned char *BaudR);       //It can change baud speed.

void AuthenticateError();

volatile unsigned char ComMifare_in;    //UART1 Recieve Method

/*Error message*/
void CLK_Config(void);
void GPIO_setup(void);
void UART1_setup(void);
void Delay (uint16_t time);
void Delay_ms (uint16_t time);
void Hex2Binary(unsigned char *msg_string, unsigned char *PoutData);

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
      
    // GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
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
        
        for(int cnt = 0; cnt < 34; cnt++){
          g_ucTempbuf[cnt] = 0;

        }
     PcdHalt();
  }//End Of The While Loop
}//End Of The Main Foncion

void Hex2Binary(unsigned char *msg_string, unsigned char *PoutData)
{
  unsigned char GData[20];
  unsigned char SData[20];
  unsigned char i;
  for(i = 0; i < 8; i++)
  {
    GData[i] = *(msg_string + i);
  }
  /*
  switch (GData[i])
  {
  case 0 : SData[i] =   
  }
  */
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