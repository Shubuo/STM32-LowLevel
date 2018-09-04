#define PCLK 			12000000 	// PCLK de�eri 
#define SPI0_EN 		1 				// SPI0 a��lacak m� 
#define SPI0_CLK 		50000 		// SPI0 h�z� ne olacak 
#define SPI0_PIN 		PINSEL0 		// SPI'n�n bulundu�u PINSEL blo�u 
#define SPI0_DIR 		IO0DIR 		// SPI i�in enable biti nerede olacak 
#define SPI0_SEL 		7 				// SPI i�in enable biti hangi pin olacak 
#define SPI0_SET 		IO0SET 		// SPI i�in enable biti CLR ve SET bloklar� olacak 
#define SPI0_CLR 		IO0CLR 
#define SPI0_SS 		1 				// Kullan�lacak aletin enable'� ne zaman aktif 
#define BitEnable0 	0 				// 8 bitten ba�ka uzunluk kullan�lacaksa bu bit 1 yap�lmal� 
#define SPI0_CPHA 	1 				// 0 ise data ilk saat darbesinde, 1 ise ikinci saat darbesinde �rneklenir 
#define SPI0_CPOL 	0 				// IDLE seviyesi 
#define SPI0_MSTR 	1 				// Master i�in bu bit bir olacak 
#define SPI0_LSBF 	0 				// 0 ise �nce y�ksek bitler, 1 ise �nce d���k bitler g�nderilir 
#define SPI0_BITS 	8 				// 8-15 bit i�in 8-15 yaz�lacak, 16bit i�in 0 
 
extern void spi_init(void); 
extern void spi_write(int veri); 
extern int 	spi_read(void); 