#include "hw.h"

#define TW_START   0x08
#define TW_REP_START   0x10
#define TW_MT_SLA_ACK   0x18
#define TW_MT_SLA_NACK   0x20
#define TW_MR_SLA_ACK   0x40
#define TW_SR_STOP   0xA0
#define TW_MT_DATA_ACK   0x28
#define I2C_READ 1
#define I2C_WRITE 0

//#define KEYB_i2C_ADDR  0x70
#define KEYB_i2C_ADDR  0x40

#define  KEY_ESC  13
#define  KEY_BSP  14
#define  KEY_ENT  15
#define  KEY_UP   22
#define  KEY_DN   25
#define  KEY_RT   26
#define  KEY_LT   24
#define  KEY_PU   33
#define  KEY_PD   34




void usart_tx_b(uint8_t data);
uint8_t usart_rx_rdy(void);
uint8_t usart_rx_b(void);
void usart_tx_s(uint8_t * data);

void i2c_init(void);
unsigned char i2c_start(unsigned char address, unsigned char dir);
unsigned char i2c_rep_start(unsigned char address, unsigned char dir);
unsigned char i2c_stop(void);
unsigned char i2c_write(unsigned char data);
unsigned char i2c_read_Ack(void);
unsigned char i2c_read_nAck(void);

uint8_t ssd1306_command(uint8_t u8data);
unsigned char disp_data(unsigned char data);

#define SSD1306_EXTERNALVCC 1
#define disp_cmd  ssd1306_command
#define lcd_cmd  ssd1306_command

#define iic_start  i2c_start
#define iic_stop  i2c_stop
#define iic_write  i2c_write


void char_print (unsigned char data);
void disp_set_xy (unsigned char x, unsigned char y);
void clrscr (unsigned char data);
void putslcd(char *data);
void disp_init (void);

unsigned char kbd_tasks (unsigned char * kbd_out, unsigned char mode);


#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2


void usart_tx_hexa (uint8_t value);
void usart_tx_hexa_16 (uint16_t value);
void usart_tx_hexa_r (uint8_t value);
void htoa_8 (char * str, unsigned char input);
void htoa_16 (char * str, unsigned int input);
void htoa_24 (char * str, unsigned long input);
unsigned char conv_hex_ascii (unsigned char temp);


