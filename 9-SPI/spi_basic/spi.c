//#include <stm32f10x.h> 
//#include "SPI.h" 
// 
//void spi_init(void) 
//{ 
// unsigned char div; 
// if(SPI0_EN) 
// { 
// SPI1-> CR1 &=		0x00; 				// T�m SPI0 ayarlar� s�f�rlan�yor 
//// SPI0_PIN 	&= 	0xFFFF00FF; 		// SPI0 pinleri ayarlan�yor 
//// SPI0_PIN 	|= 	0x00001500; 		//RX clk,ws, sda
// 
// SPI0_DIR=1<<SPI0_SEL; // Selection pini belirleniyor 
//	 
// //Ayarlar yap�l�rken chip aktif olmas�n istiyoruz 
// if(SPI0_SS==0) 
// SPI0_SET=1<<SPI0_SEL; 
//           // E�er chip y�ksek seviyede aktifse 
// else 
// SPI0_CLR=1<<SPI0_SEL; 
//           // E�er chip d���k seviyede aktifse 
// 
// // SPI0 h�z� belirleniyor 
// div=(PCLK/SPI0_CLK)+8; 
// if((div%2)!=0) // E�er frekans de�eri 3,5 gibiyse 
// div+=1; // 4 ve 6 gibi �ift rakama tamamlan�yor 
// S0SPCCR=div; // Frekans atan�yor 
// 
// 
// S0SPCR=(BitEnable0<<2)|(SPI0_CPHA<<3)|(SPI0_CPOL<<4)|(SPI0_MSTR<<5)|(
//SPI0_LSBF<<6)|(SPI0_BITS<<8); 
// } 
//} 
// 
//void spi_write(int veri) 
//{ 
// S0SPSR=0; 
// S0SPDR=veri; 
// while ( !(S0SPSR & (0x80)) ); 
//} 
// 
//int spi_read(void) 
//{ 
// int temp=0; 
// 
// S0SPDR = 0xFF; 
// while ( !(S0SPSR & (0x80)) ); 
// temp=S0SPDR; 
// 
// return temp; 
//} 
