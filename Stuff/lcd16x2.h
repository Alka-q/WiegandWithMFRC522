/////////////////////////////////////
void LCD_Enable(void);
void LCD_Send4Bit( unsigned char  Data );
void LCD_SendCommand (unsigned char  command );
void LCD_Init ( void );
void LCD_Gotoxy(unsigned char  x, unsigned char  y);
void LCD_Clear(void);
void LCD_PutChar ( unsigned char  Data );
void LCD_Puts (unsigned char *s);
void Delay_Basic(unsigned short nCount);
void LCD_mychar(unsigned char x, unsigned char y);
/////////////////////////////////////////////////



