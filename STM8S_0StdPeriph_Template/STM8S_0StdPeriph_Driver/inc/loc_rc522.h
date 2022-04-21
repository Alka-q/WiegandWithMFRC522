/////////////////////////////////////////////////////////////////////
//????
/////////////////////////////////////////////////////////////////////
char PcdReset(void);
void PcdAntennaOn(void);
void PcdAntennaOff(void);
char PcdRequest(unsigned char req_code,unsigned char *pTagType);   
char PcdAnticoll(unsigned char *pSnr);
char PcdSelect(unsigned char *pSnr);         
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);     
char PcdRead(unsigned char addr,unsigned char *pData);     
char PcdWrite(unsigned char addr,unsigned char *pData);    
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue);   
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr);                                 
char PcdHalt(void);
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit);
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData);
void WriteRawRC(unsigned char Address,unsigned char value);
unsigned char ReadRawRC(unsigned char Address); 
void SetBitMask(unsigned char reg,unsigned char mask); 
void ClearBitMask(unsigned char reg,unsigned char mask);

/*From MW-110*/
void SendDataToMifareReader(unsigned char * msg_string, unsigned char TCount);

/////////////////////////////////////////////////////////////////////
//MF522???
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //??????
#define PCD_AUTHENT           0x0E               //????
#define PCD_RECEIVE           0x08               //????
#define PCD_TRANSMIT          0x04               //????
#define PCD_TRANSCEIVE        0x0C               //???????
#define PCD_RESETPHASE        0x0F               //??
#define PCD_CALCCRC           0x03               //CRC??

/////////////////////////////////////////////////////////////////////
//Mifare_One?????
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26           //????????????
#define PICC_REQALL           0x52 //0x03        //????????
#define PICC_ANTICOLL1        0x93 //0x04        //???
#define PICC_ANTICOLL2        0x95               //???
#define PICC_AUTHENT1A        0x60 //0x05        //??A??
#define PICC_AUTHENT1B        0x61               //??B??
#define PICC_READ             0x30 //0x20        //??
#define PICC_WRITE            0xA0 //0x21        //??
#define PICC_DECREMENT        0xC0 //0x23        //??
#define PICC_INCREMENT        0xC1 //0x24        //??
#define PICC_RESTORE          0xC2               //????????
#define PICC_TRANSFER         0xB0               //????????
#define PICC_HALT             0x50               //??

/////////////////////////////////////////////////////////////////////
//MF522 FIFO????
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

/////////////////////////////////////////////////////////////////////
//MF522?????
/////////////////////////////////////////////////////////////////////

// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F           
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     RFU3C                 0x3C
#define     RFU3D                 0x3D
#define     RFU3E                 0x3E
#define     RFU3F		  0x3F

/////////////////////////////////////////////////////////////////////
//?MF522??????????
/////////////////////////////////////////////////////////////////////

#define MI_OK                          0
#define MI_NOTAGERR                    (-1)
#define MI_ERR                         (-2)

//////////////////////
/*#define MF_READ           0x20
#define MF_WRITE          0x21
#define MF_INITVAL        0x22
#define MF_DECREMENT      0x23
#define MF_INCREMENT      0x24
#define MF_GET_SER_NUM    0x25
#define MF_TYPEA_TRANSFER 0x28
#define MF_READ_SERIAL    0x30

#define MF_KEYA_RQA   0x01
#define MF_KEYB_RQA   0x03

#define MF_ST_OK      0x00 // Command OK.
#define MF_ST_FAIL    0x01 // Command FAILURE

#define MF_LEN_SER_NUM      0x06 // 6 bytes 
#define MF_LEN_SER_NUM_UID7 0x09 // 9 bytes 
#define MF_LEN_READ         0x15 // 21 bytes
#define MF_LEN_READ_UID7    0x18 // 24 bytes
#define MF_LEN_WRITE        0x05 // 5 bytes
#define MF_LEN_INITVAL      0x05 // 5 bytes
#define MF_LEN_DECREMENT    0x09 // 9 bytes
#define MF_LEN_INCREMENT    0x09 // 9 bytes

#define BUFCNT_SER_NUM      11
#define BUFCNT_READ         26
#define BUFCNT_READ_SERIAL  26
#define BUFCNT_WRITE        10
#define BUFCNT_INITVAL      10
#define BUFCNT_DECREMENT    14
#define BUFCNT_INCREMENT    14*/
//////////////////////////

void Init_spi_software(void);
void SPIRC522_Init(void);

#define MF522_SCK_H     GPIO_WriteHigh(GPIOC,GPIO_PIN_5)
#define MF522_SCK_L     GPIO_WriteLow(GPIOC,GPIO_PIN_5)

#define MF522_MISO      GPIO_ReadInputData(GPIOC)

#define MF522_MOSI_H    GPIO_WriteHigh(GPIOC,GPIO_PIN_6)
#define MF522_MOSI_L    GPIO_WriteLow(GPIOC,GPIO_PIN_6)

#define MF522_RST_H     GPIO_WriteHigh(GPIOC, GPIO_PIN_3)
#define MF522_RST_L     GPIO_WriteLow(GPIOC, GPIO_PIN_3)