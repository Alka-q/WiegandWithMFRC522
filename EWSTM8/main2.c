//
//
//#/******************************************************************************
//
//                            Online C Compiler.
//                Code, Compile, Run and Debug C program online.
//Write your code in this editor and press "Run" button to compile and execute it.
//
//*******************************************************************************/
//
//// Online C compiler to run C program online
//#include <stdio.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <time.h>
//#define BYTE unsigned char
//
//unsigned short UpdateCrc(unsigned char ch, unsigned short *lpwCrc)
//{
//    ch = (ch^(unsigned char)((*lpwCrc) & 0x00FF));
//    ch = (ch^(ch<<4));
//    *lpwCrc = (*lpwCrc >> 8)^((unsigned short)ch << 8)^((unsigned short)ch<<3)^((unsigned short)ch>>4);
//    return(*lpwCrc);
//}
//void ComputeCrc(unsigned short wCrcPreset, unsigned char *Data, int Length, unsigned short /*&*/usCRC)
//{
//    unsigned char chBlock;
//    do {
//    chBlock = *Data++;
//    UpdateCrc(chBlock, &wCrcPreset);
//    } while (--Length);
//    usCRC = wCrcPreset;
//    return;
//}
//void Convert7ByteUIDTo4ByteNUID(unsigned char *uc7ByteUID, unsigned char *uc4ByteUID)
//{
//    unsigned short CRCPreset = 0x6363;
//    unsigned short CRCCalculated = 0x0000;
//    
//    ComputeCrc(CRCPreset, uc7ByteUID, 3,CRCCalculated);
//    
//    uc4ByteUID[0] = (CRCCalculated >>8)&0xFF;//MSB
//    uc4ByteUID[1] = CRCCalculated &0xFF; //LSB
//    CRCPreset = CRCCalculated;
//    
//    ComputeCrc(CRCPreset, uc7ByteUID+3, 4,CRCCalculated);
//    
//    uc4ByteUID[2] = (CRCCalculated >>8)&0xFF;//MSB
//    uc4ByteUID[3] = CRCCalculated &0xFF; //LSB
//    uc4ByteUID[0] = uc4ByteUID[0]|0x0F;
//    uc4ByteUID[0] = uc4ByteUID[0]& 0xEF;
//}
//int main(void)
//{
//    int i;
//    unsigned char uc7ByteUID[7] = {0x04,0x18,0x3F,0x09,0x32,0x1B,0x85};//4F505D7D
//    unsigned char uc4ByteUID[4] = {0x00};
//    
//    Convert7ByteUIDTo4ByteNUID(uc7ByteUID,uc4ByteUID);
//    
//    printf("7-byte UID = ");
//    for(i = 0;i<7;i++)
//    printf("%02X",uc7ByteUID[i]);
//    printf("\t4-byte FNUID = ");
//    for(i = 0;i<4;i++)
//    printf("%02X",uc4ByteUID[i]);
//    
//    getch();
//    return(0);
//}

//ctrl + k yorum satirina alir
//ctrl + shift + k yorum satirni kaldirir
//


//https://stackoverflow.com/questions/65171601/how-to-correctly-read-data-returned-by-mfrc522-via-spi-using-stm8s103f3

/* Includes ------------------------------------------------------------------*/

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
//
//#define ARRAY_SIZE      20 
//#define MF_KEYA_RQA     0x03
//#define MF_KEYB_RQA     0x01
//#define MF_LEN_READ     0x24
//
///*
//unsigned char TSector, 
//unsigned char TBlock, 
//unsigned char TKey, 
//unsigned char * Key*/ 
////void Send_MF_Write_CMD(unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char *Key, unsigned char *TBlockData);
//
//void ComMifare_Uart1_ISR(void); /* Interrupt Service rutine */
//
//void SendData2uSett(void);
//void SetAddress(void);
//void Sen_GET_SNR(void);
//void Send_MF_Read_CMD(void);
//void Send_MF_Write_CMD();       //Cihaza gönderilecek WRITE CMD 
//
//void SendDataToUart1(unsigned char *msg_string, unsigned char TCount);
//void SendData2Uart1(unsigned char *msg_string, unsigned char TCount);
////void UART1_Settings(unsigned char *BaudR);       //It can change baud speed.
//
//void AuthenticateError();
//
//volatile unsigned char ComMifare_in;    //UART1 Recieve Method
//  
////void MF_Write_CMD();            //Weite Command
////void MF_Read_CMD();             //Read Command
////void MF_SerialNum_CMD();        //Serial Number Command
//
///*Error message*/
//
//void CLK_Config(void);
//void GPIO_setup(void);
//void UART1_setup(void);
//void Delay (uint16_t time);
//void Delay_ms (uint16_t time);
//
////void Delay(uint32_t nCount);
//void Dec2Hex(unsigned int dec);
//char dec2hex[10];
//char hex[ARRAY_SIZE];
//
//int i = 0, value, buf;
//
//long long bufferSize;
//
//unsigned char CmdBuf[16];
//unsigned char g_ucTempbuf[20];
//unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//unsigned char SectorTrailer[16] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xFF,0x07,0x80,0x69,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
//unsigned char ValueBlock[16] = {0x64,0x00,0x00,0x00,0x98,0xFF,0xFF,0xFF,0x64,0x00,0x00,0x00,0x01,0xFE,0x01,0xFE};
//unsigned char ValueBlockV2[16] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x19,0x67,0x8E,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};   
//unsigned char data2[16]; //= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0x17,0x81,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//unsigned char data[20];
//unsigned char data1[16]; 
//
///*unsigned char ManufacturerData_0[16];
//  unsigned char BlocData_1[16];
//  unsigned char BlocData_2[16];
//  unsigned char BlocData_3[16];   //Sector Trailer
//  unsigned char BlocData_4[16];
//  unsigned char BlocData_5[16];
//  unsigned char BlocData_6[16];   
//  unsigned char BlocData_7[16];   //Sector Trailer
//  unsigned char BlocData_8[16];
//  unsigned char BlocData_9[16];   
//  unsigned char BlocData_10[16];  
//  unsigned char BlocData_11[16];  //Sector Trailer
//  unsigned char BlocData_12[16];  
//  unsigned char BlocData_13[16];  
//  unsigned char BlocData_14[16];
//  unsigned char BlocData_15[16];  //Sector Trailer
//  unsigned char BlocData_16[16];*/
//
//unsigned int FIFOData;
//unsigned char TSector;
//unsigned char addr;     
//unsigned char TBlockAddr;       //Block Addr
//unsigned char ATQBuf[2];        //ATQ Data
//unsigned char WBlockBuf[16];    //Write Data
//unsigned char KeyAB;            //Key A or Key B
//unsigned char InBufM[34];
//unsigned int BufCntM = 0;
//unsigned char status;
//unsigned char g_ucTempbuf[20];
//
//unsigned char MWSendData[] =" Authenticate Error ";
//
//void delayY(long x)
//{
//  while(x--)
//  {
//    asm("nop");
//  }
//}
//
///*
//   ConfigUart1 
//   u1brg = (unsigned char)(((f1_CLK_SPEED/16)/BAUD_RATE_1)-1);
//   #define	f1_CLK_SPEED	18432000            // Peripheral Clock Speed - set up in mcu_init()
//   #define BAUD_RATE	38400
//   #define BAUD_RATE_1	9600
//*/
//
//unsigned char ReqALL = PICC_REQALL;
//
//void main(void)
//{
//  //int Len = strlen(MWSendData);
//  Dec2Hex(bufferSize);
//  CLK_Config();
//  UART1_setup();
//  //SendSetBaudRate();
//  GPIO_setup();
//
//  /*MFRC522 Initiliaze*/
//  SPIRC522_Init();
//  PcdReset();
//  //Init_spi_software();
//  while (1)
//  {
//    
//    ComMifare_Uart1_ISR();
//    //BufCntM = 0;
//    SendData2uSett();
///*******************************Make SNR***************************************/
///*  ATQBuf[2]; Card Type
//    0x4400 = Mifare_UltraLight    
//    0x0400 = Mifare_One(S50)    
//    0x0200 = Mifare_One(S70)    
//    0x0800 = Mifare_Pro(X)    
//    0x4403 = Mifare_DESFire    */
//    
//     status = PcdRequest(PICC_REQALL, ATQBuf);  //(0x52 - 14443A), ,g_ucTempbuf
//     if(status != MI_OK){
//      continue;
//     }
//     
//     status = PcdAnticoll(g_ucTempbuf);
//     if(status != MI_OK){
//       continue;
//     }
//     
//     else
//     {
//       if(InBufM[0] == 0xBB)
//         InBufM[0] = 0xAA;
//       
//          if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x03 || InBufM[3] != 0x25){
//            /*
//            *continue;   
//            */
//          }
//          else
//          {
//              Sen_GET_SNR(); //Send serial nummer
//              BufCntM = 0;
//              KeyAB = PICC_AUTHENT1A;
//              TBlockAddr = 1;
//              
//              //Buffer sifirlama
//              for(int i = 0; i < 34; i++){
//                InBufM[i] = 0;
//              }
//              i=0;
//                do{
//                  g_ucTempbuf[i] = 0;
//                  i++;
//                }while(i < 4);
//          }
//     }
//     
//     status = PcdSelect(g_ucTempbuf);
//     if(status != MI_OK){
//       continue;
//     }
//        //Key Bytes
//        //MF_READ_Sector_KEY
//        if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x0A || InBufM[3] != 0x20){
//        //continue;
//        }
//        else
//        {
//          TBlockAddr = InBufM[5];     // Number of Block: 1
//          
//          for(int cnt=0; cnt<6; cnt++){
//            CmdBuf[cnt] = InBufM[cnt+7];        //MF_READ_Sector_KEY
//          }
//        }
//        
//        //MF_Write_Key
//        if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x1A || InBufM[3] != 0x21){            //Write Commond
//          //continue;
//        }
//        else{
//          /*Key A or B*/
//          if(InBufM[4] == 0x01){
//            KeyAB = PICC_AUTHENT1A;
//          }
//          else
//          {
//            KeyAB = PICC_AUTHENT1B;
//          }
//          
//          //InBufM[5];
//          TBlockAddr = InBufM[6];     // Number of Block: 1
//          
//          for(int cnt=0; cnt<6; cnt++){
//            CmdBuf[cnt] = InBufM[cnt+7];    //    MF_Write_Key
//          } 
//        }
//     
//          //TBlockAddr = 1;
//       
//        if(  TBlockAddr != 0&& TBlockAddr != 3&& TBlockAddr != 7&& TBlockAddr != 11&&
//          TBlockAddr != 15&& TBlockAddr != 19&& TBlockAddr != 23&& TBlockAddr != 27&&
//          TBlockAddr != 31&& TBlockAddr != 35&& TBlockAddr != 39&& TBlockAddr != 43&&
//          TBlockAddr != 47&& TBlockAddr != 51&& TBlockAddr != 55&& TBlockAddr != 59&&
//          TBlockAddr != 63  )
//        {
//              //KeyAB = 0x60;
//              status = PcdAuthState(KeyAB, TBlockAddr, DefaultKey/*CmdBuf*/, g_ucTempbuf); //SectorTrailer, DefaultKey, CmdBuf
//              if(status != MI_OK){
//                continue;
//              }
//              else{
//                //Buffer sifirlama
//                for(int cnt=0; cnt<6; cnt++)
//                  CmdBuf[cnt] = 0x00;
//              }
//          }
//          else
//          {
//              continue;
//          }
//     
//////////////////////////////////////////////////////////////////////////////////
///*******************************MF_READ_CMD************************************/
//        if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x0A || InBufM[3] != 0x20){
//        //continue;
//        }
//        else
//        {
//          status = PcdRead(TBlockAddr, data);
//          if(status != MI_OK){
//            continue;
//          }
//          else{
//            Send_MF_Read_CMD();
//            TBlockAddr = 0x00;
//          } 
//        }
///************************END OF THE MF_READ_CMD********************************/
//////////////////////////////////////////////////////////////////////////////////
///*****************************MF_Write_CMD*************************************/
//     if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x1A || InBufM[3] != 0x21){            //Write Commond
//       //continue;
//     }
//     else
//     {
//        for(int cnt=0; cnt<16; cnt++){
//          WBlockBuf[cnt] = InBufM[cnt+13];//data1[cnt];
//        }
//        //
//        status = PcdWrite(TBlockAddr, WBlockBuf);
//        if(status != MI_OK){
//          //continue;
//          //WriteError
//        }
//        else{
//            Send_MF_Write_CMD();           //'Write' Send Data, 
//            TBlockAddr = 0x00;
//          }
//      }
//      
///****************************END OF THE MF_Write_CMD***************************/
//////////////////////////////////////////////////////////////////////////////////  
//        Delay(0xFFF);
///***************************Buffer Sifirlama***********************************/
//        ATQBuf[0] = 0;
//        ATQBuf[1] = 0;
//        
//        for(int cnt = 0; cnt < 34; cnt++){
//          g_ucTempbuf[cnt] = 0;
//          InBufM[cnt] = 0;
//        }
//     PcdHalt();
//  
//  }//End Of The While Loop
//}//End Of The Main Foncion
//
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///*
//void SectorAddr(unsigned char *blockAddr, unsigned char *sectorAddr){
//  
//  unsigned char TmpBuf;
//  //sectorAddr 
//  if(sectorAddr == 0){
//    TmpBuf = 1 || 2 || 3 ; 
//  }
//  
//  *blockAddr = TmpBuf;
//}
//*/
///*
//  bana gelecek veri TmpBuf[]
//*/
//
///*
//  void MF_SerialNum_CMD(){
//
//  }
//
//  void MF_Write_CMD(){
//
//  }
//
//  void MF_Read_CMD(){
//      
//  }
//*/
//
///****Error mesajlari tek fonsiyon olarak üretilebilir.(Counter vs. seklinde)****/
///*******************************************************************************/
//void AuthenticateError(){
//  
//  unsigned char ERR[]= " Authenticate Error ";
//  unsigned int Len = strlen(ERR);//strlen(ERR);
//  unsigned int mCount;
//
//  //sizeof(ERR);
//  BufCntM = 0;
//  
//  for(mCount = 0; mCount < Len; mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//      UART1_SendData8(ERR[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                 
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//}
///*******************************************************************************/
//
///*******************************************************************************/
//void ReadError(){
//  
//  unsigned char ERR[]= " Read Error ";
//  unsigned int Len = strlen(ERR);//strlen(ERR);
//  unsigned int mCount;
//  
//  //sizeof(ERR);
//  BufCntM = 0;
//  
//  for(mCount = 0; mCount < Len; mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//      UART1_SendData8(ERR[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                   
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//}
///*******************************************************************************/
//
///*******************************************************************************/
//void WriteError(){
//  
//  unsigned char ERR[]= " Write Error ";
//  unsigned int Len = strlen(ERR);//strlen(ERR);
//  unsigned int mCount;
//  
//  //sizeof(ERR);
//  BufCntM = 0;
//  
//  for(mCount = 0; mCount < Len; mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//      UART1_SendData8(ERR[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                   
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//}
///*******************************************************************************/
//
///*******************************************************************************/
///*
//    void MapOfCards(unsigned char *DataOfBlock){
//  
//    int i=0;
//  
//    for(i=0; i<16; i++){
//      while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//      UART1_SendData8(*DataOfBlock[i]);
//    }
//      while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//  }
//*/
///*******************************************************************************/
//void SendData2uSett(void){
//  
//  //Baud Rate Speed
//  unsigned char TmpBuf[7];
//  
//  TmpBuf[0] = 0xAA;
//  TmpBuf[1] = 0x00;
//  TmpBuf[2] = 0x02;     //0x02
//  TmpBuf[3] = 0x00;     //0x81
//  TmpBuf[4] = InBufM[4];     //0x83
//  
//  //BCC
//  TmpBuf[5] = 0;
//  for(uint8_t cnt = 0; cnt < 5; cnt++ ){
//    TmpBuf[5] ^= TmpBuf[cnt];
//  }
//  
//  //ETX
//  TmpBuf[6] = 0xBB;
//  
//  BufCntM = 0;
//  
//  if(InBufM[0] == 0xBB)
//    InBufM[0] == 0xAA;
//  
//  //if(InBufM[0] == 0xAA && InBufM[1] == 0x00 && InBufM[2] == 0x02 && InBufM[3] == 0x81) // Arayüz uygulamsi yapilir ise aktif edilmesi gerekir.
//    SendDataToUart1(TmpBuf, 7);
//}
///*******************************************************************************/
//
///*******************************************************************************/
//void SetAddress(void){  //0x80
//  
//  unsigned char TmpBuf[7];
//  
//  TmpBuf[0] = 0xAA;
//  TmpBuf[1] = 0x00;
//  TmpBuf[2] = 0x02;
//  TmpBuf[3] = 0x00;
//  TmpBuf[4] = InBufM[4];
//  
//  TmpBuf[5] = 0;
//  for(uint8_t cnt = 0; cnt<5; cnt++){
//    TmpBuf[5] ^= TmpBuf[cnt];
//  }
//  
//  TmpBuf[6] = 0xBB;
//  
//  BufCntM = 0;
//  if(InBufM[0] == 0xAA && InBufM[1] == 0x00 && InBufM[2] == 0x02 && InBufM[3] == 0x80)
//    SendDataToUart1(TmpBuf, 7); 
//}  
///*******************************************************************************/
//
///*******************************************************************************/
//void Sen_GET_SNR(void){
//  
//  unsigned char TmpBuf[11];
//
//  TmpBuf[0]=0xAA;
//  TmpBuf[1]=0x00;               // bu indexin çikarilmasi gere//0X02
//  TmpBuf[2]=0x06;               //0x06
//  TmpBuf[3]=0x00;               //MF_Get_SNR(0x25)
//  TmpBuf[4]=0x00;               //Bir kart var
//  
//  //Kart seri no
//  //Tersleme yapiliyor... seri no. için
//  TmpBuf[5] = g_ucTempbuf[3];//g_ucTempbuf[0];   
//  TmpBuf[6] = g_ucTempbuf[2];//g_ucTempbuf[1];   
//  TmpBuf[7] = g_ucTempbuf[1];//g_ucTempbuf[2];   
//  TmpBuf[8] = g_ucTempbuf[0];//g_ucTempbuf[3];  
//  
//  //BCC
//  TmpBuf[9]=0;
//  for(int ixi=0; ixi<9; ixi++){
//    TmpBuf[9]^=TmpBuf[ixi];
//  }
//  
//  //ETX 
//  TmpBuf[10]=0xBB; 
//  
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 11);
//}
///*******************************************************************************/
//
///*******************************************************************************/
//void Send_MF_Read_CMD( void){ /*, unsigned Char TBlockData*/ /*unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char * Key*/
//  unsigned char TmpBuf[27];
//  unsigned char MI;
//  
//  TmpBuf[0]=0xAA;       //STX
//  TmpBuf[1]=0x00;       //Station ID
//  TmpBuf[2]=0x15;       //Data Length
//  TmpBuf[3]=0x00;       //MI_OK
//  
//  // Card Serial Number.
//  TmpBuf[4]=g_ucTempbuf[3];
//  TmpBuf[5]=g_ucTempbuf[2];
//  TmpBuf[6]=g_ucTempbuf[1];
//  TmpBuf[7]=g_ucTempbuf[0];
//  
//  //Okunan blok 
//  for(int ixi=8; ixi<24;ixi++){
//    TmpBuf[ixi]=data[ixi-9];
//  }
//  
//  //BCC
//  TmpBuf[25]=0;
//  for(MI=0; MI<25;MI++){
//    TmpBuf[25]^=TmpBuf[MI];}      //0x92
//  
//  //ETX
//  TmpBuf[26]=0xBB;
//  
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 27); 
//}
///******************************************************************************/
//
///******************************************************************************/
//void Send_MF_Write_CMD(){       //unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char * Key, unsigned char * TBlockData
//  
//  // unsigned char TmpBuf[10];
//  unsigned char TmpBuf[10];
//  
//  TmpBuf[0] = 0xAA;
//  TmpBuf[1] = 0x00;
//  TmpBuf[2] = 0x05;
//  TmpBuf[3] = 0x00;
//  
//  //Card Serial Number
//  TmpBuf[4] = g_ucTempbuf[3];
//  TmpBuf[5] = g_ucTempbuf[2];
//  TmpBuf[6] = g_ucTempbuf[1];
//  TmpBuf[7] = g_ucTempbuf[0];
//  
//  //BCC
//  TmpBuf[8] = 0;
//  for(int cnt = 0; cnt <8; cnt++){
//    TmpBuf[8] ^= TmpBuf[cnt]; 
//  }
//  //TmpBuf[8] = 0x84;
//  
//  //ETX
//  TmpBuf[9] = 0xBB;
//  //sizeof(TmpBuf)
//  //sizeof(TmpBuf);
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 10);
//}
///******************************************************************************/
//
///******************************************************************************/
//void SendSetBaudRate(){
//  
//  unsigned char TmpBuf[7];
//  
//  TmpBuf[0] = 0xAA;
//  TmpBuf[1] = 0x00;
//  TmpBuf[2] = 0x02;
//  TmpBuf[3] = 0x00;     //
//  TmpBuf[4] = InBufM[4];        //Uart Config
//  
//  //BCC
//  TmpBuf[5] = 0x00;
//  for(int cnt = 0; cnt < 5; cnt++){
//    TmpBuf[5] ^= TmpBuf[cnt];
//  }
//  
//  //ETX
//  TmpBuf[6] = 0xBB;
//  
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 7);
//}
///******************************************************************************/
//
///******************************************************************************/
//void SendDataToUart1(unsigned char *msg_string, unsigned char TCount){
//
//  unsigned int ixi;
//  Delay(1000);
//
//  for(ixi=0; ixi<9999; ixi++){
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//  }
//
//  for(ixi=0;  ixi<TCount; ixi++){
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//      UART1_SendData8(msg_string[ixi]);
//  }
//  while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//  
//  for(ixi=0; ixi<9999; ixi++){
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//  }
//  //BufCntM = 0;
//  Delay(1000);
//}
//
//void SendData2Uart1(unsigned char *msg_string, unsigned char TCount){
//
//  unsigned int ixi;
//  Delay(1000);
//
//  for(ixi=0;  ixi<TCount; ixi++){
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//      UART1_SendData8(msg_string[ixi]);
//  }
//  while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//  
//  Delay(1000);
//}
///******************************************************************************/
//
///******************************************************************************/
//
//void Delay (uint16_t time){
//  
//    while(time--);
//}
//
//void Delay_ms (uint16_t time){  
//  
//    //time = 1000 olursa 1 sn lik bir fonsiyon olusturulmus olur.
//    for(int cnt =0; cnt < 16; cnt++)
//    while(time--);
//}
///******************************************************************************/
//
///******************************************************************************/
//  //UART1 Interrupt
//void ComMifare_Uart1_ISR(void)
//{
//  if ( BufCntM < 34 ) //Buffer is full, No Receive
//  {
//    InBufM[BufCntM] = ComMifare_in;
//    ++BufCntM;
//  }
//  Delay(1000);
//  //BufCntM = 0;
//}
///******************************************************************************/
//
///*
//void Delay(uint32_t nCount){
//    while(nCount != 0){
//      nCount--;
//  }
//}
//*/
//
//
///******************************** WAIT DELAY ***********************************/
///*
//void WaitDelay(unsigned short TWaitTime)
//{
//  unsigned long TWT = TWaitTime * 30;
//  
//  while(TWT--)
//  {
//    asm("nop");
//  }
//}
//*/
///*******************************************************************************/
//
/////////////////////////////////////////////////////////
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
//  CLK_DeInit();
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
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
//*/
//}
//
//////////////////////////////////////////////////////////////////////////////////
///*******************************************************************************
//
//      // Bursasi için SDK yazilmasi gerek.
//      // ilk iki byte'i AA, 00, olmali.
//      // SendBuf[5] = 0x4D mesaj kodu.
//      // " SendUart1Setting(); " Fonksiyonu yazilacak.
// 
//*******************************************************************************/
//
//void UART1_Settings(unsigned char *BaudR)
//{
//  unsigned char baudR;
//  uint32_t uartSpeed;
//  unsigned char UartMes[] = " 0,1,2,3,4 say. gir! ";
//  unsigned char TmpBuf[7];
//  
//  uartSpeed = *( BaudR + 4 );
//  
//  // 0x00 = 9600 bps
//  // 0x01 = 19200 bps
//  // 0x02 = 38400 bps
//  // 0x03 = 57600 bps
//  // 0x04 = 115200 bps
//
//  if( baudR == 0x00 )
//  {
//    uartSpeed = 9600;
//  }
//  else if( baudR == 0x01 )
//  {
//    uartSpeed = 19200;
//  }
//  else if( baudR == 0x02 )
//  {
//    uartSpeed = 38400;
//  }
//  else if( baudR == 0x03 )
//  {
//    uartSpeed = 57600;
//  }
//  else if( baudR == 0x04 )
//  {
//    uartSpeed = 115200;
//  }
//  else
//  {
//    /**Mesaj kodu gönderilebilir. For Ex. SendBuf[5] = 0x4D(Lütfen 0,1,2,3,4 sayilarindan birini giriniz.)**/
//    /*
//    //UART1 Error message.
//    for(int mCount = 0; mCount < sizeof(UartMes); mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit    
//      //for future business
//      //UART1_SendData8(UartMes[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                   
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//  */
//  }
//  if(InBufM[0] == 0xAA && InBufM[1] == 0x00 && InBufM[2]== 0x02 && InBufM[3] == 0x81/*u_settings command*/){
//    
//        UART1_DeInit();
//	
//	UART1_Init(uartSpeed, 
//                        UART1_WORDLENGTH_8D, 
//			UART1_STOPBITS_1, 
//			UART1_PARITY_NO, 
//			UART1_SYNCMODE_CLOCK_DISABLE, 
//			UART1_MODE_TXRX_ENABLE);
//	
//        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
//        
//        enableInterrupts();
//	UART1_Cmd(ENABLE);
//  
//        TmpBuf[0] = 0xAA;       //STX
//        TmpBuf[1] = 0x00;       //Station ID
//        TmpBuf[2] = 0x02;       
//        TmpBuf[3] = 0x00;       //U_OK
//        TmpBuf[4] = 0x00;       // 9600bps
//        
//        //BCC
//        TmpBuf[5] = 0;
//        for(uint8_t cnt=0; cnt<6; cnt++){
//          TmpBuf[5] ^= TmpBuf[cnt];
//        }
//        
//        //ETX
//        TmpBuf[6] = 0xBB;
//        
//        BufCntM = 0;
//        SendDataToUart1(TmpBuf, sizeof(TmpBuf));
//  }	     
//}
//
//void UART1_setup(void)
//{
//	UART1_DeInit();
//	
//	UART1_Init(9600, 
//                        UART1_WORDLENGTH_8D, 
//			UART1_STOPBITS_1, 
//			UART1_PARITY_NO, 
//			UART1_SYNCMODE_CLOCK_DISABLE, 
//			UART1_MODE_TXRX_ENABLE);
//	
//        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
//        
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

//Çalisir Durumda Mifare WorkSpace
/********************************************************************************************************/

//https://stackoverflow.com/questions/65171601/how-to-correctly-read-data-returned-by-mfrc522-via-spi-using-stm8s103f3

/* Includes ------------------------------------------------------------------*/

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
////------------------------------------------------------------------------------
////#include <iostm8s103f3.h>
//#include "STM8S_EEPROM.h"
//#include "stm8s_conf.h"
//#include "stm8s_flash.h"
//#include "loc_rc522.h"
////#include "LCD16x2.h"
//
//// 1 US
//#define PAUSE_1us {asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");asm ("NOP");}
//
//#define ARRAY_SIZE      20 
//#define MF_KEYA_RQA     0x03
//#define MF_KEYB_RQA     0x01
//#define MF_LEN_READ     0x24
//#define WUPA            0x1010010
//// CT = Cascade Tag, to indicate a following cascade level.
//// BCC = Block Check Character, it is calculated as exclusive-or over the 4 previous bytes.
//// CTRL + K ->  '//' LINE OF BLOCK
//// CTRL + SHIFT + K ->  '//' KALDIRILIR
//
//void ComMifare_Uart1_ISR(void); /* Interrupt Service rutine */
//void SendData2uSett(void);
//void SetAddress(void);
//void Sen_GET_SNR(void);
//void Send_MF_Read_CMD(void);
//void Send_MF_Write_CMD();       //Cihaza gönderilecek WRITE CMD 
//void SendDataToUart1(unsigned char *msg_string, unsigned char TCount);
//void SendData2Uart1(unsigned char *msg_string, unsigned char TCount);
//void UART1_Settings(unsigned char *BaudR);       //It can change baud speed.
//
//void AuthenticateError();
//
//volatile unsigned char ComMifare_in;    //UART1 Recieve Method
////void MF_Write_CMD();            //Weite Command
////void MF_Read_CMD();             //Read Command
////void MF_SerialNum_CMD();        //Serial Number Command
//
///*Error message*/
//void CLK_Config(void);
//void GPIO_setup(void);
//void UART1_setup(void);
//void Delay (uint16_t time);
//void Delay_ms (uint16_t time);
//
////void Delay(uint32_t nCount);
//void Dec2Hex(unsigned int dec);
//char dec2hex[10];
//char hex[ARRAY_SIZE];
//
//int i = 0, value, buf;
//
//long long bufferSize;
//
//unsigned char CmdBuf[16];
//unsigned char g_ucTempbuf[20];
//unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//unsigned char SectorTrailer[16] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xFF,0x07,0x80,0x69,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
//unsigned char ValueBlock[16] = {0x64,0x00,0x00,0x00,0x98,0xFF,0xFF,0xFF,0x64,0x00,0x00,0x00,0x01,0xFE,0x01,0xFE};
//unsigned char ValueBlockV2[16] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x19,0x67,0x8E,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};   
//unsigned char data2[16]; //= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xEE,0x17,0x81,0x69,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//unsigned char data[20];
//unsigned char data1[16]; 
//
//unsigned int FIFOData;
//unsigned char TSector;
//unsigned char addr;     
//unsigned char TBlockAddr;       //Block Addr
//unsigned char ATQBuf[2];        //ATQ Data
//unsigned char WBlockBuf[16];    //Write Data
//unsigned char KeyAB;            //Key A or Key B
//unsigned char InBufM[34];
//unsigned char InBufM2[8];
//unsigned int BufCntM = 0;
//unsigned char status;
//unsigned char g_ucTempbuf[20];
//unsigned char Ucnt = 0;
//int cnt = 0;
//unsigned char MWSendData[] =" Authenticate Error ";
//unsigned char eepromByte[16];
//
//void delayY(long x)
//{
//  while(x--)
//  {
//    asm("nop");
//  }
//}
///*
//   ConfigUart1 
//   u1brg = (unsigned char)(((f1_CLK_SPEED/16)/BAUD_RATE_1)-1);
//   #define	f1_CLK_SPEED	18432000            // Peripheral Clock Speed - set up in mcu_init()
//   #define BAUD_RATE	38400
//   #define BAUD_RATE_1	9600
//*/
//
//unsigned char ReqALL = PICC_REQALL;
//
//void main(void)
//{
//  //int Len = strlen(MWSendData);
//  Dec2Hex(bufferSize);
//  CLK_Config();
//  //FLASH_DeInit();
//  //FLASH_Unlock(FLASH_MEMTYPE_DATA);     /*!< Data EEPROM memory, (uint8_t)0xF7 */
//  //FLASH_Unlock(FLASH_MEMTYPE_PROG);     /*!< Data Flash memory */
//  InitEEPROM();
//  Unlock_EEPROM();
//  //UART1_setup();
//  //SendSetBaudRate();
//  GPIO_setup();
//
//  /*MFRC522 Initiliaze*/
//  SPIRC522_Init();
//  PcdReset();
//  //Init_spi_software();
//  while (1)
//  {
//   //UART1_Settings(unsigned char *BaudR)
//   //Baud rate sorguluyor
//   if(Ucnt == 0)
//   {  
//   //EEPROM Okuma
//      for(int eCnt=0x4000; eCnt<0x4015; eCnt++){
//        eepromByte[eCnt-0x4000] = ReadByte_EEPROM(eCnt);
//      }
//        InBufM2[4] = eepromByte[0];
//        UART1_Settings(InBufM2);   //Baslangiç için 9600bps
//        Delay_ms(50);
//        ComMifare_Uart1_ISR();
//        if(Ucnt == 1)
//          goto label;
//        
//        InBufM2[4] = eepromByte[1];
//        UART1_Settings(InBufM2);   //Baslangiç için 19200bps
//        Delay_ms(50);
//        ComMifare_Uart1_ISR();
//        if(Ucnt == 1)
//          goto label;
//        
//        InBufM2[4] = eepromByte[2];
//        UART1_Settings(InBufM2);   //Baslangiç için 38400bps
//        Delay_ms(50);
//        ComMifare_Uart1_ISR();
//        if(Ucnt == 1)
//          goto label;
//          
//        InBufM2[4] = eepromByte[3];
//        UART1_Settings(InBufM2);   //Baslangiç için 57600bps
//        Delay_ms(50);
//        ComMifare_Uart1_ISR();
//        if(Ucnt == 1)
//          goto label;
//        
//        InBufM2[4] = eepromByte[4];
//        UART1_Settings(InBufM2);   //Baslangiç için 115200bps
//        Delay_ms(50);
//        ComMifare_Uart1_ISR();
//        if(Ucnt == 1) 
//          goto label;
//   }
//   
//    //UART1_Settings(unsigned char *BaudR)
//    label:
//    if(Ucnt == 1){
//      ComMifare_Uart1_ISR();
//    }
//    //ComMifare_Uart1_ISR();
//      SendData2uSett();
//      Lock_EEPROM();
//  
////      FLASH_Lock(FLASH_MEMTYPE_DATA);
////      FLASH_Lock(FLASH_MEMTYPE_PROG);
///*******************************Make SNR***************************************/
//      
///*  ATQBuf[2]; Card Type
//    0x4400 = Mifare_UltraLight    
//    0x0400 = Mifare_One(S50)    
//    0x0200 = Mifare_One(S70)    
//    0x0800 = Mifare_Pro(X)    
//    0x4403 = Mifare_DESFire    */
//      
//    // GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
//     status = PcdRequest(ReqALL, ATQBuf);  //(0x52 - 14443A), ,g_ucTempbuf
//     if(status != MI_OK){
//      continue;
//     }
//     
//     status = PcdAnticoll_CL2(g_ucTempbuf);// PcdAnticoll_CL2(g_ucTempbuf);
//     if(status != MI_OK){
//       continue;
//     }
//     else
//     {
//       if(InBufM[0] == 0xBB)
//         InBufM[0] = 0xAA;
//          if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x03 || InBufM[3] != 0x25){
//            /*
//            *continue;   
//            */
//          }
//          else
//          {
//             Sen_GET_SNR(); //Send serial nummer
//             BufCntM = 0;
//             KeyAB = PICC_AUTHENT1A;
//             TBlockAddr = 1;
//             //Buffer sifirlama
//             for(int i = 0; i < 34; i++){
//               InBufM[i] = 0;
//             }
//               i=0;
//               do{
//                 //g_ucTempbuf[i] = 0;
//                 i++;
//               }while(i < 8); 
//          }
//     }
//     status = PcdSelect(g_ucTempbuf);
//     if(status != MI_OK){
//       continue;
//     }
//        //Key Bytes
//        //MF_READ_Sector_KEY
//        if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x0A || InBufM[3] != 0x20){
//        //continue;
//        }
//        else
//        {
//          TBlockAddr = InBufM[5];               // Number of Block: 1
//          
//          for(int Scnt=0; Scnt<6; Scnt++){
//            CmdBuf[Scnt] = InBufM[Scnt+7];        //MF_READ_Sector_KEY
//          }
//        }
//     
//        //MF_Write_Key
//        if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x1A || InBufM[3] != 0x21){            //Write Commond
//          //continue;
//        }
//        else
//        {
//          /*Key A or B*/
//          if(InBufM[4] == 0x01)
//          {
//            KeyAB = PICC_AUTHENT1A;
//          }
//          else
//          {
//            KeyAB = PICC_AUTHENT1B;
//          }
//          //InBufM[5];
//          TBlockAddr = InBufM[6];           // Number of Block: 1
//          
//          for(int Scnt=0; Scnt<6; Scnt++){
//            CmdBuf[Scnt] = InBufM[Scnt+7];    //    MF_Write_Key
//          }
//        }
//          //TBlockAddr = 1;
//        if(  TBlockAddr != 0&& TBlockAddr != 3&& TBlockAddr != 7&& TBlockAddr != 11&&
//          TBlockAddr != 15&& TBlockAddr != 19&& TBlockAddr != 23&& TBlockAddr != 27&&
//          TBlockAddr != 31&& TBlockAddr != 35&& TBlockAddr != 39&& TBlockAddr != 43&&
//          TBlockAddr != 47&& TBlockAddr != 51&& TBlockAddr != 55&& TBlockAddr != 59&&
//          TBlockAddr != 63  )
//        {
//              //KeyAB = 0x60;
//              status = PcdAuthState(KeyAB, TBlockAddr, DefaultKey/*CmdBuf*/, g_ucTempbuf); //SectorTrailer, DefaultKey, CmdBuf
//              if(status != MI_OK){
//                continue;
//              }
//              else
//              {
//                //Buffer sifirlama
//                for(int Scnt=0; Scnt<6; Scnt++)
//                  CmdBuf[Scnt] = 0x00;
//              }
//          }
//          else{
//              continue;
//              //Buffer sifirlama
//              for(int Scnt=0; Scnt<6; Scnt++)
//                CmdBuf[Scnt] = 0x00;
//          }
//     
//////////////////////////////////////////////////////////////////////////////////
///*******************************MF_READ_CMD************************************/
//        if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x0A || InBufM[3] != 0x20){
//        //continue;
//        }
//        else
//        {
//          status = PcdRead(TBlockAddr, data);
//          if(status != MI_OK){
//            continue;
//          }
//          else{
//            Send_MF_Read_CMD();
//            TBlockAddr = 0x00;
//          } 
//        }
///************************END OF THE MF_READ_CMD********************************/
//////////////////////////////////////////////////////////////////////////////////
///*****************************MF_Write_CMD*************************************/
//     if(InBufM[0] != 0xAA || InBufM[1] != 0x00 || InBufM[2] != 0x1A || InBufM[3] != 0x21){            //Write Commond
//       //continue;
//     }
//     else
//     {
//        for(int cnt=0; cnt<16; cnt++){
//          WBlockBuf[cnt] = InBufM[cnt+13];//data1[cnt];
//        }
//        //
//        status = PcdWrite(TBlockAddr, WBlockBuf);
//        if(status != MI_OK){
//          //continue;
//          //WriteError
//        }
//        else
//        {
//            Send_MF_Write_CMD();           //'Write' Send Data, 
//            TBlockAddr = 0x00;
//        }
//      }
///****************************END OF THE MF_Write_CMD***************************/
//////////////////////////////////////////////////////////////////////////////////  
//        Delay(0xFFF);
///***************************Buffer Sifirlama***********************************/
//        ATQBuf[0] = 0;
//        ATQBuf[1] = 0;
//        
//        for(int cnt = 0; cnt < 34; cnt++){
//          g_ucTempbuf[cnt] = 0;
//          InBufM[cnt] = 0;
//        }
//     PcdHalt();
//  }//End Of The While Loop
//}//End Of The Main Foncion
//
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///*
//void SectorAddr(unsigned char *blockAddr, unsigned char *sectorAddr){
//  
//  unsigned char TmpBuf;
//  //sectorAddr 
//  if(sectorAddr == 0){
//    TmpBuf = 1 || 2 || 3 ; 
//  }
//  
//  *blockAddr = TmpBuf;
//}
//*/
///*
//  bana gelecek veri TmpBuf[]
//*/
//
///*
//  void MF_SerialNum_CMD(){
//
//  }
//
//  void MF_Write_CMD(){
//
//  }
//
//  void MF_Read_CMD(){
//      
//  }
//*/
//
///****Error mesajlari tek fonsiyon olarak üretilebilir.(Counter vs. seklinde)****/
///*******************************************************************************/
//void AuthenticateError(){
//  
//  unsigned char ERR[]= " Authenticate Error ";
//  unsigned int Len = strlen(ERR);//strlen(ERR);
//  unsigned int mCount;
//
//  //sizeof(ERR);
//  BufCntM = 0;
//  
//  for(mCount = 0; mCount < Len; mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//      UART1_SendData8(ERR[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                 
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//}
///*******************************************************************************/
//
///*******************************************************************************/
//void ReadError(){
//  
//  unsigned char ERR[]= " Read Error ";
//  unsigned int Len = strlen(ERR);//strlen(ERR);
//  unsigned int mCount;
//  
//  //sizeof(ERR);
//  BufCntM = 0;
//  
//  for(mCount = 0; mCount < Len; mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//      UART1_SendData8(ERR[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                   
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//}
///*******************************************************************************/
//
///*******************************************************************************/
//void WriteError(){
//  
//  unsigned char ERR[]= " Write Error ";
//  unsigned int Len = strlen(ERR);//strlen(ERR);
//  unsigned int mCount;
//  
//  //sizeof(ERR);
//  BufCntM = 0;
//  
//  for(mCount = 0; mCount < Len; mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//      UART1_SendData8(ERR[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                   
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//}
///*******************************************************************************/
//
///*******************************************************************************/
///*
//    void MapOfCards(unsigned char *DataOfBlock){
//  
//    int i=0;
//  
//    for(i=0; i<16; i++){
//      while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//      UART1_SendData8(*DataOfBlock[i]);
//    }
//      while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//  }
//*/
///*******************************************************************************/
//void SendData2uSett(void){
//  
//  //Baud Rate Speed
//  unsigned char TmpBuf[7];
//  
//  TmpBuf[0] = 0xAA;
//  TmpBuf[1] = 0x00;
//  TmpBuf[2] = 0x02;     //0x02
//  TmpBuf[3] = 0x00;     //0x81
//  TmpBuf[4] = InBufM[4];     //0x83
//  
//  //BCC
//  TmpBuf[5] = 0;
//  for(uint8_t cnt = 0; cnt < 5; cnt++ ){
//    TmpBuf[5] ^= TmpBuf[cnt];
//  }
//  
//  //ETX
//  TmpBuf[6] = 0xBB;
//  
//  BufCntM = 0;
//  if(InBufM[0] == 0xBB)
//    InBufM[0] == 0xAA;
//  
////if(InBufM[0] == 0xAA && InBufM[1] == 0x00 && InBufM[2] == 0x02 && InBufM[3] == 0x81) // Arayüz uygulamsi yapilir ise aktif edilmesi gerekir.
//    SendDataToUart1(TmpBuf, 7);
//}
///*******************************************************************************/
//
///*******************************************************************************/
//void SetAddress(void){  //0x80
//  
//  unsigned char TmpBuf[7];
//  
//  TmpBuf[0] = 0xAA;
//  TmpBuf[1] = 0x00;
//  TmpBuf[2] = 0x02;
//  TmpBuf[3] = 0x00;
//  TmpBuf[4] = InBufM[4];
//  
//  TmpBuf[5] = 0;
//  for(uint8_t cnt = 0; cnt<5; cnt++){
//    TmpBuf[5] ^= TmpBuf[cnt];
//  }
//  
//  TmpBuf[6] = 0xBB;
//  
//  BufCntM = 0;
//  if(InBufM[0] == 0xAA && InBufM[1] == 0x00 && InBufM[2] == 0x02 && InBufM[3] == 0x80)
//    SendDataToUart1(TmpBuf, 7); 
//}  
///*******************************************************************************/
//
///*******************************************************************************/
//void Sen_GET_SNR(void){
//  
//  unsigned char TmpBuf[11];
//  
//  TmpBuf[0]=0xAA;       //STX
//  TmpBuf[1]=0x00;       //Station ID            // bu indexin çikarilmasi gere//0X02
//  TmpBuf[2]=0x06;       //Data Length           //0x06
//  TmpBuf[3]=0x00;       //MI_OK                 //MF_Get_SNR(0x25)
//  TmpBuf[4]=0x00;                               //Bir kart var
//  
//  //Kart seri no
//  //Tersleme yapiliyor... seri no. için
//  TmpBuf[5] = g_ucTempbuf[3];//g_ucTempbuf[0];   
//  TmpBuf[6] = g_ucTempbuf[2];//g_ucTempbuf[1];   
//  TmpBuf[7] = g_ucTempbuf[1];//g_ucTempbuf[2];   
//  TmpBuf[8] = g_ucTempbuf[0];//g_ucTempbuf[3];  
//  
//  //BCC
//  TmpBuf[9]=0;
//  for(int ixi=0; ixi<9; ixi++){
//    TmpBuf[9]^=TmpBuf[ixi];
//  }
//  
//  //ETX 
//  TmpBuf[10]=0xBB; 
//  
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 11);
//  //SendData2Uart1(TmpBuf, 11);
//}
///*******************************************************************************/
//
///*******************************************************************************/
//void Send_MF_Read_CMD( void){ /*, unsigned Char TBlockData*/ /*unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char * Key*/
//  unsigned char TmpBuf[27];
//  unsigned char MI;
//  
//  TmpBuf[0]=0xAA;       //STX
//  TmpBuf[1]=0x00;       //Station ID
//  TmpBuf[2]=0x15;       //Data Length
//  TmpBuf[3]=0x00;       //MI_OK
//  
//  // Card Serial Number.
//  TmpBuf[4]=g_ucTempbuf[3];
//  TmpBuf[5]=g_ucTempbuf[2];
//  TmpBuf[6]=g_ucTempbuf[1];
//  TmpBuf[7]=g_ucTempbuf[0];
//  
//  //Okunan blok 
//  for(int ixi=8; ixi<24;ixi++){
//    TmpBuf[ixi]=data[ixi-9];
//  }
//  
//  //BCC
//  TmpBuf[25]=0;
//  for(MI=0; MI<25;MI++){
//    TmpBuf[25] ^= TmpBuf[MI];}      //0x92
//  
//  //ETX
//  TmpBuf[26]=0xBB;
//  
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 27); 
//}
///******************************************************************************/
//
///******************************************************************************/
//void Send_MF_Write_CMD(){       //unsigned char TSector, unsigned char TBlock, unsigned char TKey, unsigned char * Key, unsigned char * TBlockData
//  
//  // unsigned char TmpBuf[10];
//  unsigned char TmpBuf[10];
//  
//  TmpBuf[0] = 0xAA;     //STX
//  TmpBuf[1] = 0x00;     //Station ID
//  TmpBuf[2] = 0x05;     //Data Length
//  TmpBuf[3] = 0x00;     //MI_OK
//  
//  //Card Serial Number
//  TmpBuf[4] = g_ucTempbuf[3];
//  TmpBuf[5] = g_ucTempbuf[2];
//  TmpBuf[6] = g_ucTempbuf[1];
//  TmpBuf[7] = g_ucTempbuf[0];
//  
//  //BCC
//  TmpBuf[8] = 0;
//  for(int cnt = 0; cnt <8; cnt++){
//    TmpBuf[8] ^= TmpBuf[cnt]; 
//  }
//  //TmpBuf[8] = 0x84;
//  
//  //ETX
//  TmpBuf[9] = 0xBB;
//  //sizeof(TmpBuf)
//  //sizeof(TmpBuf);
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 10);
//}
///******************************************************************************/
//
///******************************************************************************/
//void SendSetBaudRate(){
//  
//  unsigned char TmpBuf[7];
//  
//  TmpBuf[0] = 0xAA;     //STX
//  TmpBuf[1] = 0x00;     //Station ID
//  TmpBuf[2] = 0x02;     //Data Length
//  TmpBuf[3] = 0x00;     //MI_OK
//  TmpBuf[4] = InBufM[4];        //Uart Config
//  
//  //BCC
//  TmpBuf[5] = 0x00;
//  for(int cnt = 0; cnt < 5; cnt++){
//    TmpBuf[5] ^= TmpBuf[cnt];
//  }
//  //ETX
//  TmpBuf[6] = 0xBB;
//  
//  BufCntM = 0;
//  SendDataToUart1(TmpBuf, 7);
//}
///******************************************************************************/
//
///******************************************************************************/
//void SendDataToUart1(unsigned char *msg_string, unsigned char TCount){
//
//  unsigned int ixi;
//  Delay(1000);
//
//  for(ixi=0; ixi<9999; ixi++){
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//  }
//
//  for(ixi=0;  ixi<TCount; ixi++){
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//      UART1_SendData8(msg_string[ixi]);
//  }
//  while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//  
//  for(ixi=0; ixi<9999; ixi++){
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//    PAUSE_1us;
//  }
//  //BufCntM = 0;
//  Delay(1000);
//}
///******************************************************************************/
//void SendData2Uart1(unsigned char *msg_string, unsigned char TCount){
//
//  unsigned int ixi;
//  Delay_ms(20);
//  
//  for(ixi=0;  ixi<TCount; ixi++){
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//      UART1_SendData8(msg_string[ixi]);
//  }
//  while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
//  
//  Delay_ms(20);
//}
///******************************************************************************/
//
///******************************************************************************/
//
//void Delay (uint16_t time){
//  
//    while(time--);
//}
//
//void Delay_ms (uint16_t time){  
//    //time = 1000 olursa 1 sn lik bir fonsiyon olusturulmus olur.
//  for(int cnt =0; cnt < 16; cnt++){
//    while(time--);
//  }
//}
///******************************************************************************/
//
///******************************************************************************/
//  //UART1 Interrupt
//void ComMifare_Uart1_ISR(void)
//{
//  if ( BufCntM < 34 ) //Buffer is full, No Receive
//  {
//    InBufM[BufCntM] = ComMifare_in;
//    ++BufCntM;
//  }
//  Delay(1000);
//  //BufCntM = 0;
//}
///******************************************************************************/
///*
//void Delay(uint32_t nCount){
//    while(nCount != 0){
//      nCount--;
//  }
//}
//*/
///******************************** WAIT DELAY ***********************************/
///*
//void WaitDelay(unsigned short TWaitTime)
//{
//  unsigned long TWT = TWaitTime * 30;
//  
//  while(TWT--)
//  {
//    asm("nop");
//  }
//}
//*/
///*******************************************************************************/
/////////////////////////////////////////////////////////
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
//  CLK_DeInit();
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
//	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
//*/
//}
//
//////////////////////////////////////////////////////////////////////////////////
///*******************************************************************************
//
//      // Bursasi için SDK yazilmasi gerek.
//      // ilk iki byte'i AA, 00, olmali.
//      // SendBuf[5] = 0x4D mesaj kodu.
//      // " SendUart1Setting(); " Fonksiyonu yazilacak.
// 
//*******************************************************************************/
//
//void UART1_Settings(unsigned char *BaudR)
//{
//  unsigned char baudR;
//  uint32_t uartSpeed;
//  unsigned char UartMes[] = " 0,1,2,3,4 say. gir! ";
//  unsigned char TmpBuf[7];
//  
//  baudR = *( BaudR + 4 );
//  
//  // 0x00 = 9600 bps
//  // 0x01 = 19200 bps
//  // 0x02 = 38400 bps
//  // 0x03 = 57600 bps
//  // 0x04 = 115200 bps
//
//  if( baudR == 0x00 )
//  {
//    uartSpeed = 9600;
//  }
//  else if( baudR == 0x01 )
//  {
//    uartSpeed = 19200;
//  }
//  else if( baudR == 0x02 )
//  {
//    uartSpeed = 38400;
//  }
//  else if( baudR == 0x03 )
//  {
//    uartSpeed = 57600;
//  }
//  else if( baudR == 0x04 )
//  {
//    uartSpeed = 115200;
//  }
//  else
//  {
//    /**Mesaj kodu gönderilebilir. For Ex. SendBuf[5] = 0x4D(Lütfen 0,1,2,3,4 sayilarindan birini giriniz.)**/
//    /*
//    //UART1 Error message.
//    for(int mCount = 0; mCount < sizeof(UartMes); mCount++){  
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit    
//      //for future business
//      //UART1_SendData8(UartMes[mCount]);                     // Write one byte in the USARTy Transmit Data Register
//    }                                                   
//    while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);  // Wait until end of current transmit
//  */
//  }
//  //if(InBufM[0] == 0xAA && InBufM[1] == 0x00 && InBufM[2]== 0x02 && InBufM[3] == 0x81/*u_settings command*/){
//    
//        UART1_DeInit();
//	
//	UART1_Init(uartSpeed, 
//                        UART1_WORDLENGTH_8D, 
//			UART1_STOPBITS_1, 
//			UART1_PARITY_NO, 
//			UART1_SYNCMODE_CLOCK_DISABLE, 
//			UART1_MODE_TXRX_ENABLE);
//	
//        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
//        
//        enableInterrupts();
//	UART1_Cmd(ENABLE);
//        
//        Delay_ms(50);
//        ComMifare_Uart1_ISR();
//        //BufCntM = 0;
//        //if(InBufM[0] == 0xAA && InBufM[1] == 0x00 && InBufM[2]== 0x02 && InBufM[3] == 0x81/*u_settings command*/){
//        if(InBufM[0] == 0xAA && InBufM[1] == 0x00 && InBufM[2]== 0x03 && InBufM[3] == 0x25/*u_settings command*/){
//          
//          InBufM[4] = 0x00;
//          TmpBuf[0] = 0xAA;       //STX
//          TmpBuf[1] = 0x00;       //Station ID
//          TmpBuf[2] = 0x02;       
//          TmpBuf[3] = 0x00;       //U_OK
//          TmpBuf[4] = InBufM[4];  // Baud Speed
//          
//          //BCC
//          TmpBuf[5] = 0;
//          for(uint8_t cnt=0; cnt<6; cnt++){
//            TmpBuf[5] ^= TmpBuf[cnt];
//          }
//          
//          //ETX
//          TmpBuf[6] = 0xBB;
//          
//          BufCntM = 0;
//        
//          SendDataToUart1(TmpBuf, 7);
//          Ucnt = 1;             //Settin Uart Hide
//        }
//}
//
//void UART1_setup(void)
//{
//	UART1_DeInit();
//	
//	UART1_Init(9600, 
//                        UART1_WORDLENGTH_8D, 
//			UART1_STOPBITS_1, 
//			UART1_PARITY_NO, 
//			UART1_SYNCMODE_CLOCK_DISABLE, 
//			UART1_MODE_TXRX_ENABLE);
//	
//        UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
//        
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
///*
//  unsigned char ManufacturerData_0[16];
//  unsigned char BlocData_1[16];
//  unsigned char BlocData_2[16];
//  unsigned char BlocData_3[16];   //Sector Trailer
//  unsigned char BlocData_4[16];
//  unsigned char BlocData_5[16];
//  unsigned char BlocData_6[16];
//  unsigned char BlocData_7[16];   //Sector Trailer
//  unsigned char BlocData_8[16];
//  unsigned char BlocData_9[16];
//  unsigned char BlocData_10[16];
//  unsigned char BlocData_11[16];  //Sector Trailer
//  unsigned char BlocData_12[16];
//  unsigned char BlocData_13[16];
//  unsigned char BlocData_14[16];
//  unsigned char BlocData_15[16];  //Sector Trailer
//  unsigned char BlocData_16[16];
//*/
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