
#define DQ_GPIO         GPIOB
#define DQ_PIN			9
#define DQ_PIN_WRITE(x) ((x) ? (GPIO_SetBits(DQ_GPIO,(1<<DQ_PIN)))  : (GPIO_ResetBits(DQ_GPIO,(1<<DQ_PIN))) );
#define DQ_PIN_READ 	((GPIO_ReadInputDataBit(DQ_GPIO,(1<<DQ_PIN)))&0x01)


extern void onewire_reset(void);
extern void onewire_write(char data);
extern unsigned char onewire_read( void );
extern float ds18b20_read(void);
