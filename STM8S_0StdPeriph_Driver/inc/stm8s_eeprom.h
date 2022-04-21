#ifndef __STM8S_EEPROM_H
#define __STM8S_EEPROM_H
 
void InitEEPROM (void);
void Unlock_EEPROM (void);
void Lock_EEPROM (void);
void EraseByte_EEPROM (unsigned int);
unsigned char ReadByte_EEPROM (unsigned int);
void WriteByte_EEPROM (unsigned int, unsigned char);
 
#endif /*__STM8S_EEPROM_H */