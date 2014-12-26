#include "periph.h"
#include "hw.h"
#include "periph.c"
#include "ff.h"
#include "asm.h"
#include "asm.c"
//#include "ff.c"

uint8_t rx_dat;

#define ISP_PORT  PORTC
#define ISP_DDR   DDRC
#define ISP_PIN   PINC
#define ISP_RST   3
#define ISP_SCK   2
#define ISP_MISO  1
#define ISP_MOSI  0

uint8_t isp_progmode_enable (uint8_t value);
void isp_rst (uint8_t value);
uint8_t isp_byte (uint8_t data);
void usart_tx_hexa (uint8_t value);
uint8_t isp_instr (uint32_t value);

unsigned char ret,i;
UINT bw;
FATFS FatFs;
FIL fi;
char line[30];


int main (void)
{
//  DISP_DDR |= (1<<DISP_CS);
//  DISP_PORT |= (1<<DISP_CS);
  i2c_init();
  disp_init();
  clrscr(0);
  
  DDRB |= (1<<5); 
  UBRR0H = ((_UBRR) & 0xF00);
  UBRR0L = (uint8_t) ((_UBRR) & 0xFF); 
  UCSR0B |= _BV(TXEN0);
  UCSR0B |= _BV(RXEN0);
 
  while (kbd_read()==0);
  PORTB |= (1<<5);
  _delay_ms(500);  
  PORTB &= ~(1<<5);
  usart_tx_b ('R');
 
  f_mount(&FatFs, "", 0);
  usart_tx_b ('M');
  if (f_open(&fi, "src.asm", FA_READ) == FR_OK)
    {
    usart_tx_b ('S');
    if (create_output("out.bin")==1)
      {
        usart_tx_b ('D');
        while (f_gets(line,30,&fi)!=0)
          {
          //usart_tx_b ('L');
          if (line[0]==' ')
	  {
	  //line contains instruction
	  opcodes = 1; //assume single byte instruction
	  //printf("-input:%s",line);
	  pos = get_token(line,tok,0);
	  opcode_table();
	  //printf("-op:%X\n",opcode_1);
    	  write_output(address,opcodes,opcode_1,opcode_2);
    	  address = address + (opcodes*2);
	  }
	else
	  {
	  //printf("-label");
  	  //line contains label
  	  }
          }
      close_output();
      }
      
    f_close(&fi);  
    usart_tx_b ('E');
    }
  while (1)
  {

  }
  
  while (1)
  {
   ret = kbd_read();
  usart_tx_b('A'+ret);
  _delay_ms(100);  
  }
  
  
  while (1)
    {
    if (isp_progmode_enable(1)==1)
      usart_tx_b('Y');
    else
      usart_tx_b('N');
    usart_tx_b(' ');    
    usart_tx_hexa(isp_instr(0x30000000));
    usart_tx_hexa(isp_instr(0x30000100));
    usart_tx_hexa(isp_instr(0x30000200));
    usart_tx_hexa(isp_instr(0x30000300));
    usart_tx_b(0x0A);    
    isp_progmode_enable(0);
    _delay_ms(500);  
    }
  
  disp_set_xy(0,0);
  putslcd("SD init              ");
/*
  f_mount(&FatFs, "", 0);		
  if (f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) 
    {
    disp_set_xy(0,1);
    putslcd("SD init OK           ");
    f_write(&Fil, "It works!\r\n", 11, &bw);	
    f_close(&Fil);
    if (bw == 11) 
      {
      disp_set_xy(0,2);
      putslcd("SD write             ");
      } 
    disp_set_xy(0,3);
    putslcd("OK                   ");
    }
    disp_set_xy(0,4);
    putslcd("File closed          ");
    */
    while (1);

    while(1) 
    {
        PORTB |= (1<<5);
        _delay_ms(50);
        for (i=0;i<8;i++)
          {
          disp_set_xy(0,i);
          putslcd("01234567890123456789Q");
          }
        PORTB &= ~(1<<5);
        _delay_ms(50);
        for (i=0;i<8;i++)
          {
          disp_set_xy(0,i);
          putslcd("Q01234567890123456789");
          }
        usart_tx_b('B');
        if (usart_rx_rdy())
          {
          rx_dat = usart_rx_b();
          usart_tx_b(rx_dat);
          }
    }
}

void usart_tx_hexa (uint8_t value)
{
uint8_t temp;
temp = value;
usart_tx_b('0');
usart_tx_b('x');
temp = ((temp>>4)&0x0F);
if (temp<10) temp = temp + '0';
else temp = temp + 'A'- 10;
usart_tx_b(temp);
temp = value;
temp = ((temp>>0)&0x0F);
if (temp<10) temp = temp + '0';
else temp = temp + 'A' - 10;
usart_tx_b(temp);
usart_tx_b(' ');
}

uint8_t isp_progmode_enable (uint8_t value)
{
uint8_t temp;
if (value==1)
  {
  ISP_DDR |= (1<<ISP_SCK);
  ISP_DDR &= ~(1<<ISP_MISO);
  ISP_DDR |= (1<<ISP_MOSI);
  ISP_DDR |= (1<<ISP_RST);
  ISP_PORT &= ~(1<<ISP_SCK);
  isp_rst(0);
  _delay_ms(1);
  isp_rst(1);
  _delay_ms(1);
  isp_rst(0);
  _delay_ms(30);
  isp_byte(0xAC);
  isp_byte(0x53);
  temp = isp_byte(0x00);
  isp_byte(0x00);
//  usart_tx_hexa(temp);
  if (temp==0x53)
    return 1;
  else
    return 0;
  }
else
  {
  ISP_DDR &= ~(1<<ISP_SCK);
  ISP_DDR &= ~(1<<ISP_MISO);
  ISP_DDR &= ~(1<<ISP_MOSI);
  ISP_DDR &= ~(1<<ISP_RST);
  return 0;
  }
}



uint8_t isp_instr (uint32_t value)
{
isp_byte((value>>24)&0xFF);
isp_byte((value>>16)&0xFF);
isp_byte((value>>8)&0xFF);
return isp_byte((value>>0)&0xFF);
}

void isp_rst (uint8_t value)
{
if (value==0)
  ISP_PORT &= ~(1<<ISP_RST);
else
  ISP_PORT |= (1<<ISP_RST);
}

uint8_t isp_byte (uint8_t data)
{
uint8_t temp,i;
temp = 0;
for (i=0;i<8;i++)
  {
  if ((data&0x80)!=0)
    ISP_PORT |= (1<<ISP_MOSI);
  else
    ISP_PORT &= ~(1<<ISP_MOSI);
  data = data << 1;
  _delay_us(50);
  ISP_PORT |= (1<<ISP_SCK);
  _delay_us(50);
  temp = temp << 1;
  if ((ISP_PIN&(1<<ISP_MISO))!=0)
    temp = temp | 0x01;
  ISP_PORT &= ~(1<<ISP_SCK);
  }
return temp;
}


void kbd_write_col (uint8_t row)
{
row = row&0x0F;
row = ~row ;
i2c_start(0x70,I2C_WRITE);
i2c_write(row);  
i2c_stop(); 
}

uint8_t kbd_read_row (void)
{
uint8_t temp;
i2c_start(0x70,I2C_READ);
temp = i2c_read_nAck();  
i2c_stop(); 
temp = (~(temp>>4))&0x0F;
return temp;
}

uint8_t kbd_read (void)
{
uint8_t i,mask,temp,ret,shift;
shift = 0;
ret = 0;
for (i=0;i<4;i++)
  {
  mask = 1<<(3-i);
  kbd_write_col(mask);
  temp = kbd_read_row();
  if (temp==0x01)
    {
    ret = (i*4)+4;
    }
  if (temp==0x02)
    ret = (i*4)+3;
  if (temp==0x04)
    ret = (i*4)+2;
  if (temp==0x08)
    ret = (i*4)+1;
  }
return ret;
}













