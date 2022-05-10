
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//------------------------------------------------------------------------------
#include "stm8s_conf.h"
#include "loc_rc522.h"

#define PAUSE_1us {asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");}

#define ARRAY_SIZE      20 
#define MF_KEYA_RQA     0x03
#define MF_KEYB_RQA     0x01
#define MF_LEN_READ     0x24

#define LED_ON          GPIO_WriteLow(GPIOB, GPIO_PIN_5);
#define LED_OFF         GPIO_WriteHigh(GPIOB, GPIO_PIN_5);

void ComMifare_Uart1_ISR(void);         /* Interrupt Service rutine */
void SendData2uSett(void);
void SetAddress(void);
void Sen_GET_SNR(void);
void Send_MF_Read_CMD(void);
void Send_MF_Write_CMD();       //Cihaza gönderilecek WRITE CMD 
void SendDataToUart1(unsigned char *msg_string, unsigned char TCount);
void SendData2Uart1(unsigned char *msg_string, unsigned char TCount);

/*Error message*/
void AuthenticateError();

volatile unsigned char ComMifare_in;    //UART1 Recieve Method

void CLK_Config(void);
void GPIO_setup(void);
void UART1_setup(void);
void Delay (uint16_t time);
void Delay_ms (uint16_t time);

void Hex2Binary(unsigned char WIC, unsigned char *msg_string, unsigned char *FirstNum, 
                unsigned char *SecNum, unsigned long int *BOutData, unsigned char *BinaryData);
void SendHexGetBinary(unsigned char *pInHex, unsigned char *pOutBinary);
void Dec2Hex(unsigned int dec);
char dec2hex[10];
char hex[ARRAY_SIZE];

int i = 0, value, buf;

unsigned char g_ucTempbuf[20];
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char bufferSize;
unsigned char ATQBuf[2];        //ATQ Data
unsigned char WBlockBuf[16];    //Write Data
unsigned char InBufM[64];
unsigned int BufCntM = 0;
unsigned char status;
unsigned char g_ucTempbuf[20];
int cnt = 0;
unsigned char MWSendData[] =" Authenticate Error ";
unsigned char HexData[8];//
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
     
     status = PcdRequest(ReqALL, ATQBuf);  //(0x52 - 14443A)
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
    for(unsigned char i=0; i<8; i++)
    {
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
	  
/* Örnek Led counter Kullanımı From stm32f103...
////////////////////////////////////////////////////////////////////////////////
    if(SndCnt == 1)
    {
      if(LedCount == (350))
      {
        HAL_GPIO_WritePin(cpu_led_GPIO_Port, cpu_led_Pin, GPIO_PIN_SET);
        HAL_Delay(100);
        
      }
      HAL_GPIO_WritePin(cpu_led_GPIO_Port, cpu_led_Pin, GPIO_PIN_RESET);
      
      if(LedCount == 350)
        LedCount = 0;
      
      LedCount++;
 
    }
    
// /*******************************Get WIEGAND Type********************************
///////////////////////////////////////////////////////////////////////////////
    if(SndCnt == 0 )
    {
      HAL_GPIO_WritePin(cpu_led_GPIO_Port, cpu_led_Pin, GPIO_PIN_RESET);
      HAL_Delay(60);//
      HAL_GPIO_WritePin(cpu_led_GPIO_Port, cpu_led_Pin, GPIO_PIN_SET);
      HAL_Delay(20);//
      
      led_12_ON;
      led_34_ON;

      //if(WIGType[14] > 0)
      //  SndCnt = 1 ;
      HAL_Delay(100);
      HAL_UART_Transmit(&huart1, (uint8_t*)ReqWIGType, sizeof(ReqWIGType), 50);

//      for(Wcnt = 0; Wcnt < 16; Wcnt++)
//      {
//        WIGType[Wcnt] = UART1_rxBuffer[Wcnt];
//      }
      
      if(WIGType[4] == 'W' && WIGType[6] == 'G' && WIGType[7] == 'T')
      {
        SndCnt = 1;
        
        led_12_OFF;
        led_34_OFF;
       // WIGType[Wcnt+9] = 0x00;
       // WIGType[Wcnt+12] = 0x00;
         
        for(Wcnt = 0; Wcnt < 9; Wcnt++)
         {
           UART1_rxBuffer[Wcnt] = 0;
         }
      }
      uartSay = 0;
    } */
///////////////////////////////////////////////////////////////////////////////////////////////	  
	  
    
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
    
    *(FirstNum + WIC) =  GData[WIC] % 16 ;       //First Num

    *(SecNum + WIC) = GData[WIC] / 16 ;          //Secound Num
    
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
      // SendBuf[5] = 0x4D mesaj byte'i.
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
