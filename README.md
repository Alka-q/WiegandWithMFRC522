# STM8sf103Hex2Bin

#define LED_ON          GPIO_WriteLow(GPIOB, GPIO_PIN_5);
#define LED_OFF         GPIO_WriteHigh(GPIOB, GPIO_PIN_5);

/*mian*/
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////



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
