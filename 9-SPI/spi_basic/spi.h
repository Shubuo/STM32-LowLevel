#define PCLK 			12000000 	// PCLK deðeri 
#define SPI0_EN 		1 				// SPI0 açýlacak mý 
#define SPI0_CLK 		50000 		// SPI0 hýzý ne olacak 
#define SPI0_PIN 		PINSEL0 		// SPI'nýn bulunduðu PINSEL bloðu 
#define SPI0_DIR 		IO0DIR 		// SPI için enable biti nerede olacak 
#define SPI0_SEL 		7 				// SPI için enable biti hangi pin olacak 
#define SPI0_SET 		IO0SET 		// SPI için enable biti CLR ve SET bloklarý olacak 
#define SPI0_CLR 		IO0CLR 
#define SPI0_SS 		1 				// Kullanýlacak aletin enable'ý ne zaman aktif 
#define BitEnable0 	0 				// 8 bitten baþka uzunluk kullanýlacaksa bu bit 1 yapýlmalý 
#define SPI0_CPHA 	1 				// 0 ise data ilk saat darbesinde, 1 ise ikinci saat darbesinde örneklenir 
#define SPI0_CPOL 	0 				// IDLE seviyesi 
#define SPI0_MSTR 	1 				// Master için bu bit bir olacak 
#define SPI0_LSBF 	0 				// 0 ise önce yüksek bitler, 1 ise önce düþük bitler gönderilir 
#define SPI0_BITS 	8 				// 8-15 bit için 8-15 yazýlacak, 16bit için 0 
 
extern void spi_init(void); 
extern void spi_write(int veri); 
extern int 	spi_read(void); 