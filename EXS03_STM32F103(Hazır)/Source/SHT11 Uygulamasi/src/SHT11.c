/*****************************************************************************
 *   SHT11.c:  Kaynak Dosya 
 *
 *   www.Expkits.com
 *   
 *	 Yazan:
 *   Coskun ERGAN
 *   13-Mart-2011  
 *
******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "SHT11.h"
#include "math.h"

#define DelayMs(x) delay_ms(x)

/***************** PORT *******************/

#define SHT_GPIO  GPIOB
#define SDA_PIN   9
#define SCL_PIN	  8

/***************************************/

#define Sda 	((GPIO_ReadInputDataBit(SHT_GPIO,(1<<SDA_PIN)))&0x01)

#define SDA(x) ((x) ? (GPIO_SetBits(SHT_GPIO,(1<<SDA_PIN)))  : (GPIO_ResetBits(SHT_GPIO,(1<<SDA_PIN))) );
#define SCL(x) ((x) ? (GPIO_SetBits(SHT_GPIO,(1<<SCL_PIN)))  : (GPIO_ResetBits(SHT_GPIO,(1<<SCL_PIN))) ); 

#define in   1
#define out  0

#define Tamam	1
#define	Hata	0

#define ISI 0x03
#define NEM 0x05

GPIO_InitTypeDef GPIO_InitStructure;

/******************************************************************************/
/******************************************************************************/
/***                      SHT11 Sensör Haberlesme Programi                  ***/
/******************************************************************************/
/******************************************************************************/
void ee_delay(void)
{  			
	unsigned long y;
	for(y=0;y<50;y++); 
}
/******************************************************************************/
void TRIS_SDA(char x)
{
    GPIO_InitStructure.GPIO_Pin = (1<<SDA_PIN);
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
if(!x)
  	{
  		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//Output
	}
else 
  	{ 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	}
  	GPIO_Init(SHT_GPIO, &GPIO_InitStructure);
}
/******************************************************************************/
void NAck(void)
{
 SDA(0);
 TRIS_SDA(out);
  ee_delay();
 SCL(1);                
 ee_delay();
 SCL(0);
  ee_delay();
 TRIS_SDA(in);
  ee_delay();			
}
/******************************************************************************/
char Ack(void)
{ 
 SCL(0);                 
 ee_delay();
 TRIS_SDA(in);			
 ee_delay();
 SCL(1);
  ee_delay();
   ee_delay(); 
   ee_delay();
 ee_delay();
 ee_delay();
 ee_delay();			
 return(Sda);
}
/******************************************************************************/
void start(void)
{
 SDA(1);
 SCL(0);	
 TRIS_SDA(out);		
 ee_delay(); 
 SCL(1);
 ee_delay();
 SDA(0);
 ee_delay();
  SCL(0);
 ee_delay();
  SCL(1);
 ee_delay();
  SDA(1);
 ee_delay();
  SCL(0);
 ee_delay();
   SDA(0);
 ee_delay();
 ee_delay();
}
/******************************************************************************/
void Gonder8Bit(unsigned char x)
{ 
 unsigned char i=0x80;
   TRIS_SDA(out);
 do{
   if (x&i){ SDA(1);}
   else {SDA(0);}

   ee_delay();
   SCL(1);
   ee_delay();
   SCL(0);
   ee_delay();
      

   i>>=1;
  }while(i!=0);
}
/****************************************************************************/
unsigned char Oku8Bit(void)
{
unsigned char x=0,i=0x80;	

 do
  {
   SCL(1);
   ee_delay();
   if (Sda) x=(x)|i;
   i>>=1;
   SCL(0);
   ee_delay();
  
  }while(i!=0);
  SCL(0);
 
  return x;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
void reset_sht11(void)
{
   unsigned long kont;
   SDA(1);
   SCL(1);
   TRIS_SDA(out);

   Gonder8Bit(0xFF); 
   Gonder8Bit(0xFF); 
   SCL(0);
   for(kont=0;kont<1000;kont++);  	   //bekle kendine gelsin
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
char Oku(unsigned char secim, float *ham)
{  
	unsigned int kont=0;

   start();	 	
   Gonder8Bit(secim);        
   if (Ack()) return 1;  
   SCL(0);

   ee_delay(); 
   while(Sda);	 
   ee_delay(); 

   kont=Oku8Bit();
   kont<<=8;           
   NAck(); 
   kont|=Oku8Bit();
   *ham=kont;
   NAck();
   Oku8Bit(); 
   Ack();
   return 0;
}																			
/******************************************************************************/
void hesapla(float *isi_ham, float *nem_ham)
{
  const float C1=-2.0468;          
  const float C2=+0.0367;           
  const float C3=-0.0000015955;    
  const float T1=+0.01;             
  const float T2=+0.00008;          
 
  float nem=*nem_ham;             
  float isi=*isi_ham;         
  float nem_lin;                     
  float nem_gercek;                  
  float isi_gercek; 
                   

  isi_gercek=isi*0.01 - 39.7; // 3v4 kalibrasyonS          
  nem_lin=C3*nem*nem + C2*nem + C1;      
  nem_gercek=(isi_gercek-25)*(T1+T2*nem)+nem_lin;   

  if(nem_gercek>99)nem_gercek=99;        
  if(nem_gercek<0.1)nem_gercek=0.1;  
               
  *nem_ham=nem_gercek;

  *isi_ham=isi_gercek;
 
}
/*****************************************************************************/
char sht11_oku(float *isi,float *nem)
{
	float nem_ham,isi_ham;
	unsigned char err=0;
               	 
	err|=Oku(ISI,&isi_ham);	 
	err|=Oku(NEM,&nem_ham); 
	
	hesapla(&isi_ham,&nem_ham);    
   
	*isi=isi_ham;
	*nem=nem_ham;

 	if(err)return (Hata);  
 	return (Tamam);
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
