#include "loc_rc522.h"
#include "stm8s.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
///////////////////////////////////////////######################################
#define CS_H GPIO_WriteHigh(GPIOC, GPIO_PIN_4)
#define CS_L GPIO_WriteLow(GPIOC, GPIO_PIN_4)
///////////////////
unsigned char serNum[5];        //orjinal deger 5
void delay(long x);
extern void SendDataToUart1(unsigned char *msg_string, unsigned char TCount);
/////////////////////////////
void RC522_Init(void)
{
  /* Enable SPI clock */
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);

  /* Set the MOSI,MISO and SCK at high level */
  GPIO_ExternalPullUpConfig(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_7 | GPIO_PIN_6 |GPIO_PIN_5), ENABLE);

  /* SD_SPI Configuration */
  SPI_Init( SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_4, SPI_MODE_MASTER,
           SPI_CLOCKPOLARITY_HIGH, SPI_CLOCKPHASE_2EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX,
           SPI_NSS_SOFT, 0x07);

  /* SD_SPI enable */
  SPI_Cmd( ENABLE);

  /* Set MSD ChipSelect pin in Output push-pull high level */
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_SLOW);
  GPIO_WriteHigh(GPIOC, GPIO_PIN_4);//CS
  //RST
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);//RST
  GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
}
/////////////////////////////////############################
#define MAXRLEN 18


/********************************************************************
//Dump debug info about the connected PCD to Serial
//show  all known Firmware versions.
void PDC_DumpVersionToSerial(unsigned int v){
  //get the MFRC522 firmware version.
  //v = ReadRawRC(VersionReg);
  unsigned char Version = v;
  //Lookup which version
  switch(v){
    case 0x88:  Version = 'U';break;//clone,UART1_SendData
    case 0x90:  Version = '0';break;//v0.0
    case 0x91:  Version = '1';break;//v1.0
    case 0x92:  Version = '2';break;//v2.0
    case 0x12:  Version = 'C';break;//counterfeit chip
  default:      Version = 'B';      //(unknown)
    
  }
  //when 0x00 or 0xFF is returned, communication probably failed
  if((v==0x00) || (v== 0xFF));
    //UART1_SendData(warning:)
}//End PCD_DumpVersionToSerial()*/

/**
 * Dumps card info (UID,SAK,Type) about the selected PICC to Serial.
 */
void PICC_DumpDetailsToSerial(unsigned char *pSnr){
  
  //UID
  for (int i = 0; i <sizeof(*pSnr); i++) {
    if(sizeof(*pSnr) < 0x10);
      //Serial.print(F(" 0"));
    //else
      //Serial.print(F(" "));
      //Serial.print(uid->uidByte[i], HEX);
  } 
}

/********************************************************************/
void PICC_DumptoSerial(unsigned char *pSnr){
  //unsigned char key[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  
  //PICC_DumpDetailsToSerial();
  /****Burada kaldim::PICC_DumpDetailsToSerial******/
  
  PcdHalt();
}
 
/* burasi yazilacak
void MFRC522::PICC_DumpMifareUltralightToSerial() {}
} // End PICC_DumpMifareUltralightToSerial()
*/


   //ATQBuf[0] = g_ucTempbuf[0];
   //ATQBuf[1] = g_ucTempbuf[1];
   /*if(ATQBuf[1] ==0x03){
      if(count == 0){
     count++;
     TestBuffer[0] = ATQBuf[0];
     TestBuffer[1] = ATQBuf[1];
      }
   }*/

/*
  static const char* const _TypeNamePICC[] =
{
  "Unknown type",
  "PICC compliant with ISO/IEC 14443-4",
  "PICC compliant with ISO/IEC 18092 (NFC)",
  "MIFARE Mini, 320 bytes",
  "MIFARE 1KB",
  "MIFARE 4KB",
  "MIFARE Ultralight or Ultralight C",
  "MIFARE Plus",
  "MIFARE TNP3XXX",

  /// not complete UID 
  "SAK indicates UID is not complete"
};

static const char* const _ErrorMessage[] =
{
  "Unknown error",
  "Success",
  "Error in communication",
  "Collision detected",
  "Timeout in communication",
  "A buffer is not big enough",
  "Internal error in the code, should not happen",
  "Invalid argument",
  "The CRC_A does not match",
  "A MIFARE PICC responded with NAK"
};
*/
/********************************************************************/
#define Mifare_UltraLight       0x4400
#define Mifare_One(S50)         0x0400
#define Mifare_One(S70)         0x0200
#define Mifare_Pro(X)           0x0800
#define Mifare_DESFire          0x4403

/*
char _TypeNamePICC(unsigned char *pTagType){
  
  int LEN;
  unsigned char ATQBuf[2];
  
  ATQBuf[0] = *(pTagType);
  ATQBuf[1] = *(pTagType+1);
  
  if(ATQBuf[0]==0x44 && ATQBuf[1]==0x00){
    unsigned char MF_Type[] =" Mifare UL/UL-C ";
    LEN = strlen(MF_Type);
    SendDataToUart1(MF_Type, LEN);
  }
  else if(ATQBuf[0]==0x44 && ATQBuf[1]==0x03){
    unsigned char MF_Type[] =" Mifare DESFire ";
    LEN = strlen(MF_Type);
    SendDataToUart1(MF_Type, LEN);
  }
  else if(ATQBuf[0]==0x04 && ATQBuf[1]==0x00){
    unsigned char MF_Type[] =" Mifare One(S50) ";
    LEN = strlen(MF_Type);
    SendDataToUart1(MF_Type, LEN);
  }
  else if(ATQBuf[0]==0x02 && ATQBuf[1]==0x00){
    unsigned char MF_Type[] =" Mifare One(S70) ";
    LEN = strlen(MF_Type);
    SendDataToUart1(MF_Type, LEN);
  }
  else if(ATQBuf[0]==0x08 && ATQBuf[1]==0x00){
    unsigned char MF_Type[] =" Mifare Pro(X) ";
    LEN = strlen(MF_Type);
    SendDataToUart1(MF_Type, LEN);
  }
  else{
    unsigned char MF_Type[] = " Unknown Type ";
    LEN = strlen(MF_Type);
    SendDataToUart1(MF_Type, LEN);
  }
  
  return LEN;
}*/
/********************************************************************/

char GetStatusCodeName(unsigned char code, unsigned char *StatMes) {    //< One of the StatusCode enums.
  //unsigned char Message;
  unsigned int LEN;
  /*
  switch (code) {
  case STATUS_OK: 
      unsigned char Message[] = "Osman";//"Success.";
      LEN = strlen(Message);
      SendDataToUart1(Message, LEN);
    break;
  case STATUS_ERROR:		 
      unsigned char Message[] = "Error in communication.";
    break;
  case STATUS_COLLISION:	 
      Message = 'C'//"Collision detected.";
    break;
  case STATUS_TIMEOUT:	 
      Message = 'T'//"Timeout in communication.";
    break;
  case STATUS_NO_ROOM:	 
      Message = 'A'//"A buffer is not big enough.";
    break;
  case STATUS_INTERNAL_ERROR: 
    char Message[]= "Internal error in the code. Should not happen.";
    break;
  case STATUS_INVALID:	 
    char Message[]= "Invalid argument.";
    break;
  case STATUS_CRC_WRONG:	 
    char Message[]= "The CRC_A does not match.";
    break;
  case STATUS_MIFARE_NACK:	 
    char Message[]= "A MIFARE PICC responded with NAK.";
    break;
    default:			 
    char Message[]= "Unknown error";
  }*/
} // End GetStatusCodeName()


/********************************************************************/
/////////////////////////////////////////////////////////////////////
//?    ?:??
//????: req_code[IN]:????
//                0x52 = ?????????14443A????
//                0x26 = ??????????
//          pTagType[OUT]:??????
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
   char status;  
   unsigned int  unLen;
   unsigned char ucComMF522Buf[MAXRLEN]; 

   ClearBitMask(Status2Reg,0x08);
   WriteRawRC(BitFramingReg,0x07);
   SetBitMask(TxControlReg,0x03);
   ClearBitMask(CollReg, 0x80);         // ValuesAfterColl=1 => Bits received after collision are cleared.
 
   ucComMF522Buf[0] = req_code;
   
   unLen = 7;   //
   status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);
   
   if ((status == MI_OK) && (unLen == 0x10))
   {    
       *pTagType     = ucComMF522Buf[0];
       *(pTagType+1) = ucComMF522Buf[1];
   }
   else
   {   status = MI_ERR;   }
   
   return status;
}

/////////////////////////////////////////////////////////////////////
//?    ?:???
//????: pSnr[OUT]:?????,4 Bytes
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////  
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;  //PICC_ANTICOLL1
    ucComMF522Buf[1] = 0x20;    //ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)    //for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
             //*(pSnr+4)  = snr_check;                    //kontrol edilecek
         }
        //PcdAnticoll2(unsigned char *pSnr);
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}
/////////////////////////////////////////////////////////////////////
//?    ?:???
//????: pSnr[OUT]:?????,4 Bytes
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////  
char PcdAnticoll_CL2(unsigned char *pSnr)
{
  char status;
  unsigned char i, snr_check=0, BCC1=0, BCC2=0;
  unsigned int unLen;
  unsigned char ucComMF522Buf[MAXRLEN];
  
  ClearBitMask(Status1Reg,0x08);
  WriteRawRC(BitFramingReg,0x00);
  ClearBitMask(CollReg,0x80);
  
  ucComMF522Buf[0] = PICC_ANTICOLL1;
  ucComMF522Buf[1] = 0x20;
  
  status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);
  //status = PcdComMF522_7byte(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);
  
    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)    //for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
             *(pSnr+4)  = snr_check;                    //kontrol edilecek
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }

  if(ucComMF522Buf[0] == 0x88){
      
      ucComMF522Buf[0] = PICC_ANTICOLL2;
      ucComMF522Buf[1] = 0x20;
    
      PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);
      //PcdComMF522_7byte(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen);
      
      for(i=4; i<9;i++){
      *(pSnr+i)   = ucComMF522Buf[i];
      BCC2       ^= ucComMF522Buf[i];
      
      }
      if(BCC1 != ucComMF522Buf[i])
      {
        if(BCC2 != ucComMF522Buf[i] && ucComMF522Buf[0] == 0x88)
        {
          status = MI_ERR;
        }
      }
    }
      
  
  SetBitMask(CollReg,0x80);
  return status;
}

/////////////////////////////////////////////////////////////////////
//?    ?:????
//????: pSnr[IN]:?????,4??
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
    
    //ucComMF522Buf[7]   = *(pSnr + 7);
    
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

/////////////////////////////////////////////////////////////////////
//?    ?:??????
//????: auth_mode[IN]: ??????
//                 0x60 = ??A??
//                 0x61 = ??B?? 
//          addr[IN]:???
//          pKey[IN]:??
//          pSnr[IN]:?????,4??
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//?    ?:??M1?????
//????: addr[IN]:???
//          pData[OUT]:?????,16??
//?    ?: ????MI_OK
///////////////////////////////////////////////////////////////////// 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = MI_ERR;   }
    
    return status;
}
/////////////////////////////////////////////////////////////////////
//?    ?:????M1???
//????: addr[IN]:???
//          pData[IN]:?????,16??
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////                  
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    return status;
}

/////////////////////////////////////////////////////////////////////
//?    ?:?????
//????: dd_mode[IN]:???
//               0xC0 = ??
//               0xC1 = ??
//          addr[IN]:????
//          pValue[IN]:4???(?)?,????
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////                 
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
       // memcpy(ucComMF522Buf, pValue, 4);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}

/////////////////////////////////////////////////////////////////////
//?    ?:????
//????: sourceaddr[IN]:???
//          goaladdr[IN]:????
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status != MI_OK)
    {    return MI_ERR;   }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    return status;
}


/////////////////////////////////////////////////////////////////////
//?    ?:??????????
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
//?MF522??CRC16??
/////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}

/////////////////////////////////////////////////////////////////////
//?    ?:??RC522
//?    ?: ????MI_OK
/////////////////////////////////////////////////////////////////////
char PcdReset(void)
{
    //MF522_RST=1;
    MF522_RST_H;
    __no_operation();
    //MF522_RST=0;
     MF522_RST_L;
    __no_operation();
    //MF522_RST=1;
    MF522_RST_H;
    __no_operation();
    WriteRawRC(CommandReg,PCD_RESETPHASE);
    __no_operation();
    
//    WriteRawRC(TModeReg,0x80);
//    WriteRawRC(ModeReg,0x3D);            //?Mifare???,CRC???0x6363
//    WriteRawRC(TReloadRegL,30);           
//    WriteRawRC(TReloadRegH,0);
//    
//    WriteRawRC(TPrescalerReg,0x3E);
   
    WriteRawRC(TModeReg, 0x80);			// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
    WriteRawRC(TPrescalerReg, 0xA9);		// TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25?s.
    WriteRawRC(TReloadRegH, 0x03);		// Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
    WriteRawRC(TReloadRegL, 0xE8);
    
    WriteRawRC(TxAutoReg, 0x40);		// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
    WriteRawRC(ModeReg, 0x3D);		// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
    PcdAntennaOn();						// Enable the antenna driver pins TX1 and TX2 
    return MI_OK;
}

/////////////////////////////////////////////////////////////////////
char test;
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    test=tmp;
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

/////////////////////////////////////////////////////////////////////
//?    ?:?RC522????
//????:reg[IN]:?????
//          mask[IN]:???
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

/////////////////////////////////////////////////////////////////////
//?    ?:??RC522?ISO14443???
//????:Command[IN]:RC522???
//          pInData[IN]:??RC522????????
//          InLenByte[IN]:?????????
//          pOutData[OUT]:??????????
//          *pOutLenBit[OUT]:????????
/////////////////////////////////////////////////////////////////////
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn   = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
       case PCD_AUTHENT:
          irqEn   = 0x12;
          waitFor = 0x10;
          break;
       case PCD_TRANSCEIVE:
          irqEn   = 0x77;
          waitFor = 0x30;
          break;
       default:
         break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pInData[i]);    }
    WriteRawRC(CommandReg, Command);
   
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }
    
    i = 600;//????????,??M1???????25ms
    do 
    {
         n = ReadRawRC(ComIrqReg);
         i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);
	      
    if (i!=0)
    {    
         if(!(ReadRawRC(ErrorReg)&0x1B))
         {
             status = MI_OK;
             if (n & irqEn & 0x01)
             {   status = MI_NOTAGERR;   }
             if (Command == PCD_TRANSCEIVE)
             {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOutData[i] = ReadRawRC(FIFODataReg);    }
            }
         }
         else
         {   status = MI_ERR;   }
        
   }
   
   SetBitMask(ControlReg,0x80);           // stop timer now
   WriteRawRC(CommandReg,PCD_IDLE); 
   return status;
}
/******************************/
/**************25ms************
int millis(){  // 25ms
  unsigned char timer;
  int i = 600;
    timer = ReadRawRC(ComIrqReg);       //Okuma önemsisiz.
  i--;
  return i;
}
**************25ms************/
/******************************/

/////////////////////////////////////////////////////////////////////
//?    ?:??RC522?ISO14443???
//????:Command[IN]:RC522???
//          pInData[IN]:??RC522????????                 7_Byte Için
//          InLenByte[IN]:?????????
//          pOutData[OUT]:??????????
//          *pOutLenBit[OUT]:????????
/////////////////////////////////////////////////////////////////////

//char PcdComMF522_7byte(unsigned char Command, 
//                 unsigned char *pInData,        //*sendData,    ///< Pointer to the data to transfer to the FIFO.
//                 unsigned char InLenByte,       //sendLen       ///< Number of bytes to transfer to the FIFO.
//                 unsigned char *pOutData,       //*backData     ///< nullptr or pointer to buffer if data should be read back after executing the command.
//                 unsigned int  *pOutLenBit)     //validBits     ///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits. Default nullptr.
//{
//  char status = MI_ERR;
//  unsigned char irqEn   = 0x00;
//  unsigned char waitFor = 0x00;
//  unsigned char lastBits;
//  unsigned int n;
//  unsigned int _validBits = 0;
//  unsigned long int i;
//  
//  //Prepare Values for BitFarmingReg
//  //unsigned int txLastBits = validBits ? *validBits :0;
//  //unsigned int bitFraming = (rxAlign << 4) + txLastBits;   // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]
//  
//  WriteRawRC(CommandReg, PCD_IDLE);     // Stop any active command.
//  WriteRawRC(ComIrqReg, 0x7F);          // Clear all seven interrupt request bits
//  SetBitMask(FIFOLevelReg, 0x80);       // FlushBuffer = 1, FIFO initialization
//  //WriteRawRC(FIFODataReg, sendLen, sendData);// Write sendData to the FIFO
//    for (i=0; i<InLenByte; i++)
//    {   WriteRawRC(FIFODataReg, pInData[i]);    }
//   WriteRawRC(BitFramingReg, 0x80);    //bitFraming ? 0x80 // Bit adjustments
//   WriteRawRC(CommandReg, Command);     // Execute the command
//   if(Command == PCD_TRANSCEIVE)
//     SetBitMask(BitFramingReg, 0x80);   //StartSend = 1, transmission of data starts.
//   
//   	// In PCD_Init() we set the TAuto flag in TModeReg. This means the timer
//	// automatically starts when the PCD stops transmitting.
//	//
//	// Wait here for the command to complete. The bits specified in the
//	// `waitIRq` parameter define what bits constitute a completed command.
//	// When they are set in the ComIrqReg register, then the command is
//	// considered complete. If the command is not indicated as complete in
//	// ~36ms, then consider the command as timed out.
//       switch (Command)
//    {
//       case PCD_AUTHENT:
//          irqEn   = 0x12;
//          waitFor = 0x10;
//          break;
//       case PCD_TRANSCEIVE:
//          irqEn   = 0x77;
//          waitFor = 0x30;
//          break;
//       default:
//         break;
//    }
//   const unsigned int deadline = millis() + 36;
//   bool completed = false;
//   do{
//      n = ReadRawRC(ComIrqReg);
//      if(n & waitFor){
//        completed = true;
//        break;
//      }
//      if(n & 0x01){             // Timer interrupt - nothing received in 25ms
//        return MI_ERR;
//      }
//   }while(deadline<36);    //(i!=0) && !(n&0x01) && !(n&waitFor)
//   
//    if (!completed) {
//          return STATUS_TIMEOUT;                //4 Olmasi gerek düzelt!!!!!
//    }
//    // Stop now if any errors except collisions were detected.
//    unsigned int errorRegValue = ReadRawRC(ErrorReg);   // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
//    if(errorRegValue & 0x13){    // BufferOvfl ParityErr ProtocolErr
//      return STATUS_ERROR;
//    }
//    
//    unsigned int _valitBirs = 0;
//    // If the caller wants data back, get it from the MFRC522.
//    if(pOutData && pInData){
//      n = ReadRawRC(FIFODataReg);       // Number of bytes in the FIFO
//      if(n > *pInData){
//        return STATUS_NO_ROOM;
//      }
//      *pOutData = n;                    // Number of bytes returned
//      ReadRawRC(FIFODataReg);
//      ReadRawRC(n);
//      ReadRawRC(*pOutData);
//     // ReadRawRC(rxAlign);
//      _validBits = ReadRawRC(ControlReg) & 0x07;
//      if(pOutLenBit){
//        *pOutLenBit =_validBits;
//      } 
//    }
//    //Tell abouth collision
//    if(errorRegValue & 0x08){   //CollErr
//      return STATUS_MIFARE_NACK;
//    }
//    //Perform CRC_A validation if requested.
//  //  if(pOutData && InLenByte && checkCRC){
//      // In this case a MIFARE Classic NAK is not OK.
//      if(InLenByte == 1 && _validBits == 4){
//       return STATUS_MIFARE_NACK;
//      }
//      //We need at least the CRC_A value and all 8 bits of the last byte must be recevied
//      if(InLenByte < 2 || _validBits != 0){
//        return STATUS_CRC_WRONG;
//      }
//      //Verify CRC_A - do our own calculation and store the control in controlBuffer.
//     unsigned char controlBuffer[2];
//      //status = CalulateCRC((pOutData-2),InLenByte ,&controlBuffer[0]);  
//    //if(status != MI_OK){
//      if(!CalulateCRC((pOutData-2),InLenByte ,&controlBuffer[0]);
//      //return status;
//     // }
//    //}
//  return MI_OK;
//}

/////////////////////////////////////////////////////////////////////
//????  
//?????????????????1ms???
/////////////////////////////////////////////////////////////////////
char test=0;
void PcdAntennaOn()
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    test=i;
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

/////////////////////////////////////////////////////////////////////
void PcdAntennaOff()
{
    ClearBitMask(TxControlReg, 0x03);
}

/////////////////////////////////////////////////////////////////////
void WriteRawRC(unsigned char Address, unsigned char value)
{  
    unsigned char i, ucAddr,temp;

//    MF522_SCK_L;
    CS_L;
    ucAddr = ((Address<<1)&0x7E);

//    for(i=8;i>0;i--)
//    {
//        //MF522_SI = ((ucAddr&0x80)==0x80);
//        if((ucAddr&0x80)==0x80)
//        MF522_MOSI_H;
//        else
//        MF522_MOSI_L;  
//        //MF522_SCK = 1;
//        MF522_SCK_H;
//        ucAddr <<= 1;
//        //MF522_SCK = 0;
//        MF522_SCK_L;
//    }
  SPI->DR = ucAddr;

  while ((SPI->SR & SPI_SR_TXE) == 0)
  {
    /* Wait while the byte is transmitted */
  }
  ///
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)
  {
  }
  /* Store the received byte in the RxBuffer2 */
  temp = SPI->DR; 
//    for(i=8;i>0;i--)
//    {
//        //MF522_SI = ((value&0x80)==0x80);
//        if((value&0x80)==0x80)
//        MF522_MOSI_H;
//        else
//        MF522_MOSI_L;
//        //MF522_SCK = 1;
//        MF522_SCK_H;
//        value <<= 1;
//        //MF522_SCK = 0;
//        MF522_SCK_L;
//    }
  SPI->DR = value;

  while ((SPI->SR & SPI_SR_TXE) == 0)
  {
    /* Wait while the byte is transmitted */
  }
  ///
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)
  {
  }
  /* Store the received byte in the RxBuffer2 */
  temp = SPI->DR;
    CS_H;
//    MF522_SCK_H;
}
/////////////////////////////////////////////////////////////////////
char temp=0;
unsigned char ReadRawRC(unsigned char Address)
{
     unsigned char i, ucAddr;
     unsigned char ucResult=0;

//     MF522_SCK_L;
     CS_L;
     ucAddr = ((Address<<1)&0x7E)|0x80;

//     for(i=8;i>0;i--)
//     {
//         //MF522_SI = ((ucAddr&0x80)==0x80);
//         if((ucAddr&0x80)==0x80)
//         MF522_MOSI_H;
//         else
//         MF522_MOSI_L; 
//         //MF522_SCK = 1;
//         MF522_SCK_H;
//         ucAddr <<= 1;
//         //MF522_SCK = 0;
//         MF522_SCK_L;
//     }
  SPI->DR = ucAddr;

  while ((SPI->SR & SPI_SR_TXE) == 0)
  {
    /* Wait while the byte is transmitted */
  }
 ///
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)
  {
  }
  /* Store the received byte in the RxBuffer2 */
  temp = SPI->DR;
//     for(i=8;i>0;i--)
//     {
//         //MF522_SCK = 1;
//         MF522_SCK_H;
//         ucResult <<= 1;
//         //ucResult|=(bit)MF522_SO;
//         if((MF522_MISO&0x80)==0x80)//1000.0000
//         ucResult|=1;
//         //MF522_SCK = 0;
//         MF522_SCK_L;
//     }
    SPI->DR = 0x00;

    while ((SPI->SR & SPI_SR_TXE) == 0)
    {
      /* Wait while the byte is transmitted */
    }
    /* Wait the byte is entirely received by SPI */
    while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET)
    {
    }
    /* Store the received byte in the RxBuffer2 */
    ucResult = SPI->DR; 
     CS_H;
     //MF522_SCK_H;
     return ucResult;
}

 ////////////////////////////////////
void Init_spi_software(void)
{
  GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);//MISO
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_HIGH_SLOW);//MOSI
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_SLOW);//SCK
  
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_SLOW);//CS
  GPIO_WriteHigh(GPIOC, GPIO_PIN_4);//CS
  //RST
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);//RST
  GPIO_WriteHigh(GPIOC, GPIO_PIN_3);//RST
}
////////////////////////////////////
void delay(long x)
{
  while(x--)
  {
    asm("nop");
  }
}
//////////////////////////////
void SPIRC522_Init(void)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, ENABLE);
  
  GPIO_ExternalPullUpConfig(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_7 | GPIO_PIN_6 |GPIO_PIN_5), ENABLE);
  ///
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_SLOW);//CS
  GPIO_WriteHigh(GPIOC, GPIO_PIN_4);//CS
  //RST
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_SLOW);//RST
  GPIO_WriteHigh(GPIOC, GPIO_PIN_3);//RST
  /* Initialize SPI in Slave mode  */
  SPI_DeInit();
  
  SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_2, SPI_MODE_MASTER, SPI_CLOCKPOLARITY_LOW,
           SPI_CLOCKPHASE_1EDGE, SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT,(uint8_t)0x07);
  
  delay(0xfff);
	/* Enable the SPI*/
  SPI_Cmd(ENABLE);
  
}
/******************************************************************************************************/
void MFRC522_init(){
  
  /*
void TM_MFRC522_Init(void) 
{
	
  TM_MFRC522_InitPins(); 
	//CS high
	MFRC522_CS_HIGH; 
	
	TM_MFRC522_Reset();

	TM_MFRC522_WriteRegister(MFRC522_REG_T_MODE, 0x8D);
	TM_MFRC522_WriteRegister(MFRC522_REG_T_PRESCALER, 0x3E);
	TM_MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_L, 30);           
	TM_MFRC522_WriteRegister(MFRC522_REG_T_RELOAD_H, 0);

	// 48dB gain 
	//TM_MFRC522_WriteRegister(MFRC522_REG_RF_CFG, 0x70); 
	TM_MFRC522_WriteRegister(MFRC522_REG_TX_AUTO, 0x40);
	TM_MFRC522_WriteRegister(MFRC522_REG_MODE, 0x3D);

	TM_MFRC522_AntennaOn();		//Open the antenna
}
  */
  unsigned int unlen;
  PcdReset();
 
  //WriteRawRC(TxModeReg)
  WriteRawRC(TModeReg, 0x80);
  WriteRawRC(TPrescalerReg, 0xA9);
  WriteRawRC(TReloadRegH, 0x03);
  WriteRawRC(TReloadRegL, 0xE8);
  WriteRawRC(TxAutoReg, 0x40);
  WriteRawRC(ModeReg, 0x3D);
  
  unlen = ReadRawRC(TxControlReg);
  if(unlen&0x03){
    WriteRawRC(TxControlReg,unlen|0x03);
  }
  PcdAntennaOn();
}
/*********************************************************************************************************/

/////////////////////////////////////
/**
 * Calculates the bit pattern needed for the specified access bits. In the [C1 C2 C3] tuples C1 is MSB (=4) and C3 is LSB (=1).
 */
void MIFARE_SetAccessBits(unsigned char *accessBitBuffer,       //< Pointer to byte 6, 7 and 8 in the sector trailer. Bytes [0..2] will be set.
                          unsigned int g0,                      //< Access bits [C1 C2 C3] for block 0 (for sectors 0-31) or blocks 0-4 (for sectors 32-39)
                          unsigned int g1,                      //< Access bits C1 C2 C3] for block 1 (for sectors 0-31) or blocks 5-9 (for sectors 32-39)
                          unsigned int g2,                      //< Access bits C1 C2 C3] for block 2 (for sectors 0-31) or blocks 10-14 (for sectors 32-39)
                          unsigned int g3){                     //< Access bits C1 C2 C3] for the sector trailer, block 3 (for sectors 0-31) or block 15 (for sectors 32-39)

  unsigned int c1 = ((g3 & 4) << 1) | ((g2 & 4) << 0) | ((g1 & 4) >> 1) | ((g0 & 4) >> 2);
  unsigned int c2 = ((g3 & 2) << 2) | ((g2 & 2) << 1) | ((g1 & 2) << 0) | ((g0 & 2) >> 1);
  unsigned int c3 = ((g3 & 1) << 3) | ((g2 & 1) << 2) | ((g1 & 1) << 1) | ((g0 & 1) << 0);
  
  accessBitBuffer[0] = (~c2 & 0xF) << 4 | (~c1 & 0xF);
  accessBitBuffer[1] =          c1 << 4 | (~c3 & 0xF);
  accessBitBuffer[2] =          c3 << 4 | c2;
}// End MIFARE_SetAccessBits()
/////////////////////////////////////


/////////////////