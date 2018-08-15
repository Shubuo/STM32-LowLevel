#include "lpc21xx.h"
#include "string.h"

void uart_hazirla (void)  
{
  PINSEL0 = 0x00000005;                   // RxD0 veTxD0 Etkinlestir       
  U0LCR = 0x83;                           // 8 bit, 1 bit bitis, parite yok
  U0DLL = 65;                             // 9600 baudrate, 10 Mhz sistem saati
  U0LCR = 0x03;                           // DLAB = 0 , alma ve verme saklayicilarina erisimi saglamak için
}
/* Gönderme fonksiyonu*/
void uart_gon( int veri)
{
    while (!(U0LSR & 0x20));  //U0THR saklayicisi bosalana kadar bekle.
    U0THR = veri; //veriyi saklayiciya yükle    
}

/* Alma fonksiyonu*/
int uart_al( int veri)
{
      while (!(U0LSR & 0x01));// Veri gelene kadar bekle
      return(U0RBR);                    //Gelen veriyi döndür.
}

char *mesaj="Selam FPGANedir";//gönderilecek mesaj
int msj_uzunlugu;
int i;

int main(void)
{
	  uart_hazirla ();
  	msj_uzunlugu =strlen(mesaj);
	  for(i=0;i< msj_uzunlugu;i++)
	  {		
		  uart_gon (*mesaj++);
		}
	
	  while(1);
    return 0;
};
