#include <LPC21xx.h> 


// Mikrodenetleyinin P0.0 ile P0.1 pinlerini LED'lerin kontrol� i�in kullan
#define LED1 (1)
#define LED2 (1<<1)

// Mikrodenetleyinin P0.10 ile P0.11 pinlerini anahtarlarin kontrol� i�in kullan
#define ANAHTAR1 (1<<10)
#define ANAHTAR2 (1<<11)

// Anahtarlarin durumunu g�steren iki degisken tanimla
int anahtar_durum1, anahtar_durum2;

int main(void)
{
//B�t�n pinleri GPIO olarak ayarla
PINSEL0 = 0; 
PINSEL1 = 0; 

// Mikrodenetleyinin P0.0 ile P0.1 pinlerinini �ikis olarak ayarla
IODIR0 |= (LED1 | LED2);

// Mikrodenetleyinin P0.10 ile P0.11 pinlerini giris olarak ayarla
IODIR0&= ~(ANAHTAR1 | ANAHTAR2) ;


while(1)
{

//Anahtarlarin durumlarini oku
anahtar_durum1 = (IOPIN0 >> 10) &1;
anahtar_durum2 = (IOPIN0 >> 11) &1;

if (anahtar_durum1==1) //anahtar#1 basili ise
{
IOSET0 |= LED1;//LED1 yak
}
else //anahtar 1 basili degil ise
{
IOCLR0 |= LED1;//LED1 s�nd�r
}

if (anahtar_durum2==1) //anahtar 2 basili ise
{
IOSET0 |= LED2;//LED2 yak
}
else //anahtar 2 basili degil ise
{
IOCLR0 |= LED2;//LED2 s�nd�r
}
}

return 0;



}