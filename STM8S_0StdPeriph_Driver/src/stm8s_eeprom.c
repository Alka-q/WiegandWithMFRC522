#include <iostm8s103f3.h>
#include "stm8s.h"
#include "stm8s_flash.h"
//#include "STM8S_EEPROM.h"

void InitEEPROM (void)
{
  FLASH_CR1    =  0x00;
  FLASH_CR2    =  0x00;
  FLASH_NCR2   =  0xFF;
  FLASH_IAPSR &= ~0x08; 
  FLASH_IAPSR;
}
 
void Unlock_EEPROM (void)
{
  FLASH_DUKR = 0xAE;
  FLASH_DUKR = 0x56;
}
 
void Lock_EEPROM (void)
{
  FLASH_IAPSR = ~0x08;
}
 
void EraseByte_EEPROM (unsigned int adr)
{
  unsigned char * Pntr = (unsigned char *)adr;
  *Pntr = 0x00;
}
 
unsigned char ReadByte_EEPROM (unsigned int adr)
{
  unsigned char * Pntr = (unsigned char *)adr;
  return(*Pntr);
}
 
void WriteByte_EEPROM (unsigned int adr, unsigned char data)
{
  unsigned char * Pntr = (unsigned char *)adr;
  *Pntr = data;
}