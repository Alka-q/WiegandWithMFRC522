/////////////////////////////////////////////////////////////////////
//????
/////////////////////////////////////////////////////////////////////
void MFRC522_init();           //Deneme:https://github.com/asif-mahmud/MIFARE-RFID-with-AVR/blob/master/tests/3.card-serial-reading/src/main.c
char PcdReset(void);           //Incelendi
void PcdAntennaOn(void);        //Incelendi
void PcdAntennaOff(void);       //Incelendi
char PcdRequest(unsigned char req_code,unsigned char *pTagType);        //
char PcdAnticoll(unsigned char *pSnr);          //Only 1. Loop
char PcdAnticoll2(unsigned char *pSnr);         //Only 2. Loop
char PcdAnticoll_CL2(unsigned char *pSnr);      //1. and 2. Loop
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
char PcdComMF522_7byte(unsigned char Command, 
                 unsigned char *pInData,        //*sendData,    ///< Pointer to the data to transfer to the FIFO.
                 unsigned char InLenByte,       //sendLen       ///< Number of bytes to transfer to the FIFO.
                 unsigned char *pOutData,       //*backData     ///< nullptr or pointer to buffer if data should be read back after executing the command.
                 unsigned int  *pOutLenBit);     //validBits     ///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits. Default nullptr.
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData);
void WriteRawRC(unsigned char Address,unsigned char value);
unsigned char ReadRawRC(unsigned char Address);
void SetBitMask(unsigned char reg,unsigned char mask);
void ClearBitMask(unsigned char reg,unsigned char mask);

/******************CPP*********************
*******************************************
********byte buffer[18];*******************
********byte size = sizeof(buffer);********
*******************************************
*******************CPP********************/
// Support functions for debuging
/*
void PCD_DumpVersionToSerial();         //Fonksiyon yazilmadi
void PICC_DumpToSerial(Uid *uid);       //Fonksiyon yazilmadi
void PICC_DumpDetailsToSerial(Uid *uid);//Fonksiyon yazilmadi
void PICC_DumpMifareClassicToSerial(Uid *uid, PICC_Type piccType, MIFARE_Key *key);//Fonksiyon yazilmadi
void PICC_DumpMifareClassicSectorToSerial(Uid *uid, MIFARE_Key *key, byte sector);//Fonksiyon yazilmadi
void PICC_DumpMifareUltralightToSerial();
*/

/////////////////////////////////////////////////////////////////////
//MF522???
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //?????? // no action, cancels current command execution
#define PCD_AUTHENT           0x0E               //????   // performs the MIFARE standard authentication as a reader
#define PCD_RECEIVE           0x08               //????   // activates the receiver circuits
#define PCD_TRANSMIT          0x04               //????   // transmits data from the FIFO buffer
#define PCD_TRANSCEIVE        0x0C               //???????// transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
#define PCD_RESETPHASE        0x0F               //??     // resets the MFRC522
#define PCD_CALCCRC           0x03               //CRC??  // activates the CRC coprocessor or performs a self test

/////////////////////////////////////////////////////////////////////
//Mifare_One?????
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26    // REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.   
#define PICC_REQALL           0x52    // Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection.7bit frame.
#define PICC_CMD_CT           0x88    // Cascade Tag. Not really a command, but used during anti collision.
#define PICC_ANTICOLL1        0x93    // Anti collision/Select, Cascade Level 1
#define PICC_ANTICOLL2        0x95    // Anti collision/Select, Cascade Level 2
#define PICC_ANTICOLL3        0x97    // Anti collision/Select, Cascade Level 3
#define PICC_AUTHENT1A        0x60    // Perform authentication with Key A
#define PICC_AUTHENT1B        0x61    // Perform authentication with Key B
#define PICC_READ             0x30    // Reads one 16 byte block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
#define PICC_WRITE            0xA0    // Writes one 16 byte block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight
#define PICC_DECREMENT        0xC0    // Decrements the contents of a block and stores the result in the internal data register.
#define PICC_INCREMENT        0xC1    // Increments the contents of a block and stores the result in the internal data register.
#define PICC_RESTORE          0xC2    // Reads the contents of a block into the internal data register.
#define PICC_TRANSFER         0xB0    // Writes the contents of the internal data register to a block.
#define PICC_HALT             0x50    // HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.

/////////////////////////////////////////////////////////////////////
//Card Types
/////////////////////////////////////////////////////////////////////
#define Mifare_UltraLight 	0x4400
#define Mifare_One_S50		0x0400
#define Mifare_One_S70		0x0200
#define Mifare_Pro_X		0x0800
#define Mifare_DESFire		0x4403

    // The commands used for MIFARE Ultralight (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
    // The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultralight.
#define PICC_CMD_UL_WRITE     0xA2    // Writes one 4 byte page to the PICC.

// PICC types we can detect. Remember to update PICC_GetTypeName() if you add more.
#define PICC_TYPE_UNKNOWN      = 0,
#define PICC_TYPE_ISO_14443_4  = 1,  // PICC compliant with ISO/IEC 14443-4
#define PICC_TYPE_ISO_18092    = 2,  // PICC compliant with ISO/IEC 18092 (NFC)
#define PICC_TYPE_MIFARE_MINI  = 3,  // MIFARE Classic protocol, 320 bytes
#define PICC_TYPE_MIFARE_1K    = 4,  // MIFARE Classic protocol, 1KB
#define PICC_TYPE_MIFARE_4K    = 5,  // MIFARE Classic protocol, 4KB
#define PICC_TYPE_MIFARE_UL    = 6,  // MIFARE Ultralight or Ultralight C
#define PICC_TYPE_MIFARE_PLUS  = 7,  // MIFARE Plus
#define PICC_TYPE_TNP3XXX      = 8,  // Only mentioned in NXP AN 10833 MIFARE Type Identification Procedure
#define PICC_TYPE_NOT_COMPLETE = 255 // SAK indicates UID is not complete.

// Return codes from the functions in this class. Remember to update GetStatusCodeName() if you add more.
//#define  STATUS_OK              = 1,  // Success
//#define  STATUS_ERROR           = 2,  // Error in communication
//#define  STATUS_COLLISION       = 3,  // Collision detected
//#define  STATUS_TIMEOUT         = 4,  // Timeout in communication.
//#define  STATUS_NO_ROOM         = 5,  // A buffer is not big enough.
//#define  STATUS_INTERNAL_ERROR  = 6,  // Internal error in the code. Should not happen ;-)
//#define  STATUS_INVALID         = 7,  // Invalid argument.
//#define  STATUS_CRC_WRONG       = 8,  // The CRC_A does not match
//#define  STATUS_MIFARE_NACK     = 9   // A MIFARE PICC responded with NAK.

/*
// A struct used for passing the UID of a PICC.
  typedef struct {
    uint8_t    size;     // Number of bytes in the UID. 4, 7 or 10.
    uint8_t    uidByte[10];
    uint8_t    sak;      // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
  } Uid;
 
  // A struct used for passing a MIFARE Crypto1 key
  typedef struct {
    uint8_t    keyByte[MF_KEY_SIZE];
  } MIFARE_Key;
 
  // Member variables
  Uid uid;                // Used by PICC_ReadCardSerial().
 
  // Size of the MFRC522 FIFO
  static const uint8_t FIFO_SIZE = 64;   // The FIFO is 64 bytes.
 */

/////////////////////////////////////////////////////////////////////
//MF522 FIFO????
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte
////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//MF522?????
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00   // reserved for future use             
#define     CommandReg            0x01   // starts and stops command execution   
#define     ComIEnReg             0x02   // enable and disable interrupt request control bits   
#define     DivlEnReg             0x03   // enable and disable interrupt request control bits   
#define     ComIrqReg             0x04   // interrupt request bits   
#define     DivIrqReg             0x05   // interrupt request bits
#define     ErrorReg              0x06   // error bits showing the error status of the last command executed   
#define     Status1Reg            0x07   // communication status bits   
#define     Status2Reg            0x08   // receiver and transmitter status bits   
#define     FIFODataReg           0x09   // input and output of 64 byte FIFO buffer
#define     FIFOLevelReg          0x0A   // number of bytes stored in the FIFO buffer
#define     WaterLevelReg         0x0B   // level for FIFO underflow and overflow warning
#define     ControlReg            0x0C   // miscellaneous control registers
#define     BitFramingReg         0x0D   // adjustments for bit-oriented frames
#define     CollReg               0x0E   // bit position of the first bit-collision detected on the RF interface
#define     RFU0F                 0x0F   // reserved for future use
// PAGE 1     
#define     RFU10                 0x10  // reserved for future use   
#define     ModeReg               0x11  // defines general modes for transmitting and receiving
#define     TxModeReg             0x12  // defines transmission data rate and framing
#define     RxModeReg             0x13  // defines reception data rate and framing
#define     TxControlReg          0x14  // controls the logical behavior of the antenna driver pins TX1 and TX2
#define     TxAutoReg             0x15  // controls the setting of the transmission modulation
#define     TxSelReg              0x16  // selects the internal sources for the antenna driver
#define     RxSelReg              0x17  // selects internal receiver settings
#define     RxThresholdReg        0x18  // selects thresholds for the bit decoder
#define     DemodReg              0x19  // defines demodulator settings
#define     RFU1A                 0x1A  // reserved for future use
#define     RFU1B                 0x1B  // reserved for future use
#define     MifareReg             0x1C  // controls some MIFARE communication transmit parameters
#define     RFU1D                 0x1D  // controls some MIFARE communication receive parameters
#define     RFU1E                 0x1E  // reserved for future use
#define     SerialSpeedReg        0x1F  // selects the speed of the serial UART interface          
// PAGE 2    
#define     RFU20                 0x20  // reserved for future use      //gelecekte kullanilmak üzere rezerve edilmistir
#define     CRCResultRegM         0x21  // shows the MSB and LSB values of the CRC calculation        //CRC hesaplamasinin MSB ve LSB degerlerini gösterir
#define     CRCResultRegL         0x22  
#define     RFU23                 0x23  // reserved for future use      //gelecekte kullanilmak üzere rezerve edilmistir
#define     ModWidthReg           0x24  // controls the ModWidth setting?       //ModWidth ayarini kontrol ediyor mu?
#define     RFU25                 0x25  // reserved for future use      //gelecekte kullanilmak üzere rezerve edilmistir
#define     RFCfgReg              0x26  // configures the receiver gain         //alici kazancini yapilandirir
#define     GsNReg                0x27  // selects the conductance of the antenna driver pins TX1 and TX2 for modulation        //modülasyon için anten sürücü pinleri TX1 ve TX2'nin iletkenligini seçer
#define     CWGsCfgReg            0x28  // defines the conductance of the p-driver output during periods of no modulation       //modülasyon olmadigi dönemlerde p sürücüsü çikisinin iletkenligini tanimlar
#define     ModGsCfgReg           0x29  // defines the conductance of the p-driver output during periods of modulation          //modülasyon periyotlari sirasinda p sürücüsü çikisinin iletkenligini tanimlar
#define     TModeReg              0x2A  // defines settings for the internal timer      //dahili zamanlayici için ayarlari tanimlar
#define     TPrescalerReg         0x2B  // the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.  //TPrescaler degerinin alt 8 biti. 4 yüksek bit TModeReg'dedir.
#define     TReloadRegH           0x2C  // defines the 16-bit timer reload value  //16 bit zamanlayici yeniden yükleme degerini tanimlar
#define     TReloadRegL           0x2D  
#define     TCounterValueRegH     0x2E  // shows the 16-bit timer value         //16 bit zamanlayici degerini gösterir 
#define     TCounterValueRegL     0x2F  
// PAGE 3      
#define     RFU30                 0x30  // reserved for future use         //gelecekte kullanilmak üzere rezerve edilmistir
#define     TestSel1Reg           0x31  // general test signal configuration    //genel test sinyali yapilandirmasi
#define     TestSel2Reg           0x32  // general test signal configuration    //genel test sinyali yapilandirmasi
#define     TestPinEnReg          0x33  // enables pin output driver on pins D1 to D7   //D1 ila D7 pinlerinde pin çikis sürücüsünü etkinlestirir
#define     TestPinValueReg       0x34  // defines the values for D1 to D7 when it is used as an I/O bus        //G/Ç veri yolu olarak kullanildiginda D1 ila D7 arasindaki degerleri tanimlar
#define     TestBusReg            0x35  // shows the status of the internal test bus    //dahili test veriyolunun durumunu gösterir
#define     AutoTestReg           0x36  // controls the digital self test       //dijital kendi kendine testi kontrol eder
#define     VersionReg            0x37  // shows the software version           //yazilim sürümünü gösterir
#define     AnalogTestReg         0x38  // controls the pins AUX1 and AUX2      //AUX1 ve AUX2 pinlerini kontrol eder
#define     TestDAC1Reg           0x39  // defines the test value for TestDAC1  //TestDAC1 için test degerini tanimlar
#define     TestDAC2Reg           0x3A  // defines the test value for TestDAC2  //TestDAC2 için test degerini tanimlar
#define     TestADCReg            0x3B  // shows the value of ADC I and Q channels     //ADC I ve Q kanallarinin degerini gösterir
#define     RFU3C                 0x3C  // reserved for production tests   //üretim testleri için ayrilmistir
#define     RFU3D                 0x3D  // reserved for production tests   //üretim testleri için ayrilmistir
#define     RFU3E                 0x3E  // reserved for production tests   //üretim testleri için ayrilmistir
#define     RFU3F                 0x3F  // reserved for production tests        //üretim testleri için ayrilmistir
/////////////////////////////////////////////////////////////////////
//?MF522??????????
/////////////////////////////////////////////////////////////////////
#define MI_OK                          0
#define MI_NOTAGERR                    (-1)
#define MI_ERR                         (-2)

void Init_spi_software(void);
void SPIRC522_Init(void);

#define MF522_SCK_H     GPIO_WriteHigh(GPIOC,GPIO_PIN_5)
#define MF522_SCK_L     GPIO_WriteLow(GPIOC,GPIO_PIN_5)

#define MF522_MISO      GPIO_ReadInputData(GPIOC)

#define MF522_MOSI_H    GPIO_WriteHigh(GPIOC,GPIO_PIN_6)
#define MF522_MOSI_L    GPIO_WriteLow(GPIOC,GPIO_PIN_6)

#define MF522_RST_H     GPIO_WriteHigh(GPIOC, GPIO_PIN_3)
#define MF522_RST_L     GPIO_WriteLow(GPIOC, GPIO_PIN_3)

#define STATUS_OK		0xA1
#define STATUS_ERROR		0xA2
#define STATUS_COLLISION	0xA3
#define STATUS_TIMEOUT	        0xA4
#define STATUS_NO_ROOM	        0xA5
#define STATUS_INTERNAL_ERROR   0xA6
#define STATUS_INVALID	        0xA7
#define STATUS_CRC_WRONG	0xA8
#define STATUS_MIFARE_NACK	0xA9


/***** Address  addr*****/
#define S0_B0   0;      //SerNum 4 Byte
#define S0_B1   1;
#define S0_B2   2;
#define S0_B3   3;      //Sector Trailer

#define S1_B0   4;
#define S1_B1   5;
#define S1_B2   6;
#define S1_B3   7;      //Sector Trailer

#define S2_B0   8;
#define S2_B1   9;
#define S2_B2   10;
#define S2_B3   11;     //Sector Trailer

#define S3_B0   12;
#define S3_B1   13;
#define S3_B2   14;
#define S3_B3   15;     //Sector Trailer

#define S4_B0   16;
#define S4_B1   17;
#define S4_B2   18;
#define S4_B3   19;     //Sector Trailer

#define S5_B0   20;
#define S5_B1   21;
#define S5_B2   22;
#define S5_B3   23;     //Sector Trailer

#define S6_B0   24;
#define S6_B1   25;
#define S6_B2   26;
#define S6_B3   27;     //Sector Trailer

#define S7_B0   28;
#define S7_B1   29;
#define S7_B2   30;
#define S7_B3   31;     //Sector Trailer

#define S8_B0   32;
#define S8_B1   33;
#define S8_B2   34;
#define S8_B3   35;     //Sector Trailer

#define S9_B0   36;
#define S9_B1   37;
#define S9_B2   38;
#define S9_B3   39;     //Sector Trailer

#define S10_B0   40;
#define S10_B1   41;
#define S10_B2   42;
#define S10_B3   43;    //Sector Trailer

#define S11_B0   44;
#define S11_B1   45;
#define S11_B2   46;
#define S11_B3   47;    //Sector Trailer

#define S12_B0   48;
#define S12_B1   49;
#define S12_B2   50;
#define S12_B3   51;    //Sector Trailer

#define S13_B0   52;
#define S13_B1   53;
#define S13_B2   54;
#define S13_B3   55;    //Sector Trailer

#define S14_B0   56;
#define S14_B1   57;
#define S14_B2   58;
#define S14_B3   59;    //Sector Trailer
  
#define S15_B0   60;
#define S15_B1   61;
#define S15_B2   62;
#define S15_B3   63;    //Sector Trailer