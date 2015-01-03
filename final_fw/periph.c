#include "periph.h"
#include "hw.h"

void usart_tx_b(uint8_t data)
{
while (!(UCSR0A & _BV(UDRE0)));
UDR0 = data;
} 

void usart_tx_s(uint8_t * data)
{
while (*data!=0) 
  usart_tx_b(*data++);
} 


uint8_t usart_rx_rdy(void)
{
if (UCSR0A & _BV(RXC0))
  return 1;
else
  return 0;
}

uint8_t usart_rx_b(void)
{
return (uint8_t) UDR0;
} 

const char tbl1[12][6] PROGMEM = {
	{'*','1',0xFF,0xFF,0xFF},	{'a','b','c','2',0xFF,0xFF},	{'d','e','f','3',0xFF,0xFF},
	{'g','h','i','4',0xFF,0xFF},    {'j','k','l','5',0xFF,0xFF},	{'m','n','o','6',0xFF,0xFF},
	{'p','q','r','s','7',0xFF},	{'t','u','v','8',0xFF,0xFF},	{'w','x','y','z','9',0xFF},
	{'#','$','@','*','?',0xFF},	{'0','.',',','!',0xFF,0xFF},	{'(',')','_','&','%',0xFF},
	};

const char kbd_map_normal[17] PROGMEM = {0, 1,2,3,13, 4,5,6,14, 7,8,9,15, 10,11,12,16};
const char kbd_map_shift[17] PROGMEM = {0, 21,22,23,33, 24,25,26,34, 27,28,29,35, 30,31,32,36};


const char font5x7[455] PROGMEM =
   {0x00, 0x00, 0x00, 0x00, 0x00 ,  // sp
    0x00, 0x00, 0x2f, 0x00, 0x00 ,  // !
    0x00, 0x07, 0x00, 0x07, 0x00 ,  // "
    0x14, 0x7f, 0x14, 0x7f, 0x14 ,  // #
    0x24, 0x2a, 0x7f, 0x2a, 0x12 ,  // $
    0xc4, 0xc8, 0x10, 0x26, 0x46 ,  // %
    0x36, 0x49, 0x55, 0x22, 0x50 ,  // &
    0x00, 0x05, 0x03, 0x00, 0x00 ,  // '
    0x00, 0x1c, 0x22, 0x41, 0x00 ,  // (
    0x00, 0x41, 0x22, 0x1c, 0x00 ,  // )
    0x14, 0x08, 0x3E, 0x08, 0x14 ,  // *
    0x08, 0x08, 0x3E, 0x08, 0x08 ,  // +
    0x00, 0x00, 0x50, 0x30, 0x00 ,  // ,
    0x10, 0x10, 0x10, 0x10, 0x10 ,  // -
    0x00, 0x60, 0x60, 0x00, 0x00 ,  // .
    0x20, 0x10, 0x08, 0x04, 0x02 ,  // /
    0x3E, 0x51, 0x49, 0x45, 0x3E ,  // 0
    0x00, 0x42, 0x7F, 0x40, 0x00 ,  // 1
    0x42, 0x61, 0x51, 0x49, 0x46 ,  // 2
    0x21, 0x41, 0x45, 0x4B, 0x31 ,  // 3
    0x18, 0x14, 0x12, 0x7F, 0x10 ,  // 4
    0x27, 0x45, 0x45, 0x45, 0x39 ,  // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30 ,  // 6
    0x01, 0x71, 0x09, 0x05, 0x03 ,  // 7
    0x36, 0x49, 0x49, 0x49, 0x36 ,  // 8
    0x06, 0x49, 0x49, 0x29, 0x1E ,  // 9
    0x00, 0x36, 0x36, 0x00, 0x00 ,  // :
    0x00, 0x56, 0x36, 0x00, 0x00 ,  // ;
    0x08, 0x14, 0x22, 0x41, 0x00 ,  // <
    0x14, 0x14, 0x14, 0x14, 0x14 ,  // =
    0x00, 0x41, 0x22, 0x14, 0x08 ,  // >
    0x02, 0x01, 0x51, 0x09, 0x06 ,  // ?
    0x32, 0x49, 0x59, 0x51, 0x3E ,  // @
    0x7E, 0x11, 0x11, 0x11, 0x7E ,  // A
    0x7F, 0x49, 0x49, 0x49, 0x36 ,  // B
    0x3E, 0x41, 0x41, 0x41, 0x22 ,  // C
    0x7F, 0x41, 0x41, 0x22, 0x1C ,  // D
    0x7F, 0x49, 0x49, 0x49, 0x41 ,  // E
    0x7F, 0x09, 0x09, 0x09, 0x01 ,  // F
    0x3E, 0x41, 0x49, 0x49, 0x7A ,  // G
    0x7F, 0x08, 0x08, 0x08, 0x7F ,  // H
    0x00, 0x41, 0x7F, 0x41, 0x00 ,  // I
    0x20, 0x40, 0x41, 0x3F, 0x01 ,  // J
    0x7F, 0x08, 0x14, 0x22, 0x41 ,  // K
    0x7F, 0x40, 0x40, 0x40, 0x40 ,  // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F ,  // M
    0x7F, 0x04, 0x08, 0x10, 0x7F ,  // N
    0x3E, 0x41, 0x41, 0x41, 0x3E ,  // O
    0x7F, 0x09, 0x09, 0x09, 0x06 ,  // P
    0x3E, 0x41, 0x51, 0x21, 0x5E ,  // Q
    0x7F, 0x09, 0x19, 0x29, 0x46 ,  // R
    0x46, 0x49, 0x49, 0x49, 0x31 ,  // S
    0x01, 0x01, 0x7F, 0x01, 0x01 ,  // T
    0x3F, 0x40, 0x40, 0x40, 0x3F ,  // U
    0x1F, 0x20, 0x40, 0x20, 0x1F ,  // V
    0x3F, 0x40, 0x38, 0x40, 0x3F ,  // W
    0x63, 0x14, 0x08, 0x14, 0x63 ,  // X
    0x07, 0x08, 0x70, 0x08, 0x07 ,  // Y
    0x61, 0x51, 0x49, 0x45, 0x43 ,  // Z
    0x00, 0x7F, 0x41, 0x41, 0x00 ,  // [
    0x55, 0x2A, 0x55, 0x2A, 0x55 ,  // 55
    0x00, 0x41, 0x41, 0x7F, 0x00 ,  // ]
    0x04, 0x02, 0x01, 0x02, 0x04 ,  // ^
    0x40, 0x40, 0x40, 0x40, 0x40 ,  // _
    0x00, 0x01, 0x02, 0x04, 0x00 ,  // '
    0x20, 0x54, 0x54, 0x54, 0x78 ,  // a
    0x7F, 0x48, 0x44, 0x44, 0x38 ,  // b
    0x38, 0x44, 0x44, 0x44, 0x20 ,  // c
    0x38, 0x44, 0x44, 0x48, 0x7F ,  // d
    0x38, 0x54, 0x54, 0x54, 0x18 ,  // e
    0x08, 0x7E, 0x09, 0x01, 0x02 ,  // f
    0x0C, 0x52, 0x52, 0x52, 0x3E ,  // g
    0x7F, 0x08, 0x04, 0x04, 0x78 ,  // h
    0x00, 0x44, 0x7D, 0x40, 0x00 ,  // i
    0x20, 0x40, 0x44, 0x3D, 0x00 ,  // j
    0x7F, 0x10, 0x28, 0x44, 0x00 ,  // k
    0x00, 0x41, 0x7F, 0x40, 0x00 ,  // l
    0x7C, 0x04, 0x18, 0x04, 0x78 ,  // m
    0x7C, 0x08, 0x04, 0x04, 0x78 ,  // n
    0x38, 0x44, 0x44, 0x44, 0x38 ,  // o
    0x7C, 0x14, 0x14, 0x14, 0x08 ,  // p
    0x08, 0x14, 0x14, 0x18, 0x7C ,  // q
    0x7C, 0x08, 0x04, 0x04, 0x08 ,  // r
    0x48, 0x54, 0x54, 0x54, 0x20 ,  // s
    0x04, 0x3F, 0x44, 0x40, 0x20 ,  // t
    0x3C, 0x40, 0x40, 0x20, 0x7C ,  // u
    0x1C, 0x20, 0x40, 0x20, 0x1C ,  // v
    0x3C, 0x40, 0x30, 0x40, 0x3C ,  // w
    0x44, 0x28, 0x10, 0x28, 0x44 ,  // x
    0x0C, 0x50, 0x50, 0x50, 0x3C ,  // y
    0x44, 0x64, 0x54, 0x4C, 0x44};  // z 


#define  SSD1306_EXTERNALVCC 2

const char disp_init_array[24] PROGMEM = 
{0xae,0x00,0x10,0x40,0x81,0xcf,0xa1,0xc8,0xa6,0xa8,0x3f,0xd3,0x00,0xd5,0x80,0xd9,0xf1,0xda,0x12,0xdb,0x40,0x8d,0x14,0xaf};

void disp_init (void)
{
unsigned char vccstate,i;
vccstate = 0;
DISP_PORT &= ~(1<<DISP_RST);
_delay_ms(1);
DISP_PORT |= (1<<DISP_RST);
_delay_ms(2);

for (i=0;i<24;i++)
  disp_cmd(pgm_read_word(&disp_init_array[i]));
/*
disp_cmd(0xae);//--turn off oled panel
disp_cmd(0x00);//---set low column address
disp_cmd(0x10);//---set high column address
disp_cmd(0x40);//--set start line address
disp_cmd(0x81);//--set contrast control register
disp_cmd(0xcf);
disp_cmd(0xa1);//--set x mapping (a0/a1)
disp_cmd(0xc8);//--set y mapping (c0/c8)
disp_cmd(0xa6);//--set normal display
disp_cmd(0xa8);//--set multiplex ratio(1 to 64)
disp_cmd(0x3f);//--1/64 duty
disp_cmd(0xd3);//-set display offset
disp_cmd(0x00);//-not offset

disp_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
disp_cmd(0x80);//--set divide ratio
disp_cmd(0xd9);//--set pre-charge period
disp_cmd(0xf1);
disp_cmd(0xda);//--set com pins hardware configuration
disp_cmd(0x12);
disp_cmd(0xdb);//--set vcomh
disp_cmd(0x40);
disp_cmd(0x8d);//--set Charge Pump enable/disable
disp_cmd(0x14);//--set(0x10) disable
disp_cmd(0xaf);//--turn on oled panel 
*/

}


void i2c_init(void)
{
    TWSR = 0;                         //bez delicky
    TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  //hodnota TWBR musi byt viac ako 10 t.j. FCPU > 3.6 MHz
}

/****************************************************
    start podmienka
    - smer prenosu
    - na zbernicu zapise adresu zariadenia
    - vrati 0 - po bezchybnom priebehu
    - vrati 1 - neuspesny start
    - vrati 2 - adresa nepotvrdena ACk bitom
****************************************************/

unsigned char i2c_start(unsigned char address, unsigned char dir)
{
	// posle start podmienku
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// caka na jej prijatie
	while(!(TWCR & (1<<TWINT)));

	// kontrola TWSR
	if (((TWSR & 0xF8) != TW_START) && ((TWSR & 0xF8) != TW_REP_START)) return 1;

	// odosle adresu zariadenia s urcenim smeru prenosu dat
	TWDR = address | dir;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// caka na dokoncenie presnosu
	while(!(TWCR & (1<<TWINT)));

	// kontrola TWSR
	if (((TWSR & 0xF8) != TW_MT_SLA_ACK) && ((TWSR & 0xF8) != TW_MR_SLA_ACK)) return 2;

	return 0;
}

/****************************************************
    opakovana start podmienka
****************************************************/

unsigned char i2c_rep_start(unsigned char address, unsigned char dir)
{
	return (i2c_start(address,dir));
}

/****************************************************
    stop podmienka
    - vrati 0 - po bezchybnom priebehu
    - vrati 1 - po neuspesnom stop
****************************************************/

unsigned char i2c_stop(void)
{
    	//odosle stop podmienku
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);

	//pocka na jej dokoncenie
	while(!(TWCR & (1<<TWSTO)));

	if ((TWSR & 0xF8) != TW_SR_STOP) return 1;

	return 0;
}

/****************************************************
    vysielanie byte-u na zbernicu
    - vrati 0 - po uspesnom vysielani - potvrdene s ACK
    - vrati 1 - data nepotvrdene ACK bitom
****************************************************/

unsigned char i2c_write(unsigned char data)
{
	//odosle data na predtym adresovane zariadenie
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);

	//caka na dokoncenie odosielania
	while(!(TWCR & (1<<TWINT)));

	//skontroluje stav TWSR
	if((TWSR & 0xF8) != TW_MT_DATA_ACK) return 1; 
	return 0; 
}

/****************************************************
    prijem byte-u zo zbernice - nie je posledny
    - vrati prijaty bajt a nasleduje dalsi bajt
****************************************************/

unsigned char i2c_read_Ack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));

        return TWDR;
}

/****************************************************
    prijem byte-u zo zbernice - posledny bajt
    - vrati prijaty bajt a nasleduje stop bit
****************************************************/

unsigned char i2c_read_nAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));

        return TWDR;
}

unsigned char ssd1306_command(unsigned char data)
{
i2c_start(0x78,I2C_WRITE);
i2c_write(0x00);  
i2c_write(data);  
i2c_stop(); 
}

unsigned char disp_data(unsigned char data)
{
i2c_start(0x78,I2C_WRITE);
i2c_write(0x40);  
i2c_write(data);  
i2c_stop(); 
}

void putslcd(char *data)
{
unsigned int counter;
	counter=0;
		while(counter<21)
  			{  
		  	char_print(*data);
			*data++;
			counter++;
		  	}
}

void char_print (unsigned char data)
{
unsigned char underline;
unsigned int	disp_loop_var;
unsigned int temp;
i2c_start(0x78,I2C_WRITE);
iic_write(0x40);
if (data&0x80) underline = 1;
	else underline = 0;
data = data&0x7F;
if (data<' ') data = ' ';
temp = ((unsigned int)(data - 0x20)*5);
for (disp_loop_var=0;disp_loop_var<5;disp_loop_var++)
	{
	if (underline==1)	iic_write((pgm_read_word(&font5x7[temp+disp_loop_var]) )^0xFF) ;	
	else 				iic_write(pgm_read_word(&font5x7[temp+disp_loop_var]) );	
	}
if (underline==1)
	iic_write(0xFF);	
else
	iic_write(0x00);	
iic_stop();

}


void disp_set_xy (unsigned char x, unsigned char y)
{
if (y>7) return;
if (x>127) return;
lcd_cmd (0xB0+y);
lcd_cmd (0x00 + (x&0x0F));
lcd_cmd (0x10 + ((x>>4)&0x0F));
}

void clrscr (unsigned char data)
{
unsigned char x,y;
for (y=0;y<8;y++)
	{
	disp_set_xy(0,y);
        i2c_start(0x78,I2C_WRITE);
	iic_write(0x40);
	for (x=0;x<128;x++)
		iic_write(data);
	iic_stop();
	}
}


//********************************keyboard handling************************
unsigned char kbd_r,kbd_r_old,kbd_me_timer,kbd_me_state,kbd_me_key_new,kbd_me_key_old,kbd_me_p,key_m,char_m,stat_m;


void kbd_write_col (uint8_t row)
{
row = row&0x0F;
row = ~row ;
i2c_start(KEYB_i2C_ADDR,I2C_WRITE);
i2c_write(row);  
i2c_stop(); 
}

uint8_t kbd_read_row (void)
{
uint8_t temp;
i2c_start(KEYB_i2C_ADDR,I2C_READ);
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
  if ((i==3)&(temp&0x01))
    {
    shift = 1;
    temp = temp&0x0E;
    }
  if (temp==0x01)
    ret = (i*4)+4;
  if (temp==0x02)
    ret = (i*4)+3;
  if (temp==0x04)
    ret = (i*4)+2;
  if (temp==0x08)
    ret = (i*4)+1;
  }
if (shift==0)
  ret = pgm_read_word(&kbd_map_normal[ret]);
else
  ret = pgm_read_word(&kbd_map_shift[ret]);
return ret;
}


unsigned char kbd_tasks (unsigned char * kbd_out, unsigned char mode)
{
  kbd_r = kbd_read();
  stat_m = 0;
  if (mode==1)
  {
  if (kbd_r<13)  
    {
    if ((kbd_r>0)&(kbd_r_old==0))
      {
        kbd_me_key_new = kbd_r;
  //      usart_tx_b ('K');
        if (kbd_me_state==0)
           kbd_me_state=1;
        else
          {
          if (kbd_me_key_new!=kbd_me_key_old)
            kbd_me_state=10;
          else
            kbd_me_state=4;
          }
      }
     kbd_r_old = kbd_r;
    if (kbd_me_state==0)
      {
      }
    if (kbd_me_state==1)
      {
        kbd_me_timer=10;
        kbd_me_state=2;
        kbd_me_p = 0;
        char_m = pgm_read_word(&tbl1[(kbd_me_key_new-1)][0]);
        kbd_me_key_old = kbd_me_key_new;
        stat_m = 1;
      }
    if (kbd_me_state==2)
      {
        if (kbd_me_timer>0)
          kbd_me_timer--;
        else
          kbd_me_state=3;  
      }
    if (kbd_me_state==3)
      {
        //key timed out, valid
        //usart_tx_b ('T');
        kbd_me_state=0;
        key_m = char_m;
        stat_m = 3;
      }
    if (kbd_me_state==4)
      {
      kbd_me_timer=10;
      kbd_me_state=2;
      //usart_tx_b ('R');
      kbd_me_p++;
      char_m = pgm_read_word(&tbl1[(kbd_me_key_new-1)][kbd_me_p]);
      if (char_m==0xFF)
        {
        char_m = pgm_read_word(&tbl1[(kbd_me_key_new-1)][0]);
        kbd_me_p=0;
        }
      key_m = char_m;
      stat_m = 2;
      //the same key, next character
      }
    if (kbd_me_state==10)
      {
        //new key before key timed out, valid
  //      kbd_me_key_old
        kbd_me_state=1;
        key_m = char_m;
        stat_m = 3;
        //usart_tx_b ('X');
      }
    *kbd_out = char_m;
    return stat_m;
    }
  else
    {
    if ((kbd_r>0)&(kbd_r_old==0))
     { 
     *kbd_out = kbd_r;
     kbd_r_old = kbd_r;
     return 4; 
     }
    else
      kbd_r_old = kbd_r;
    }
  
  
  }
  else
  {
  return 0;
  }
}


void usart_tx_hexa_16 (uint16_t value)
{
usart_tx_b('0');
usart_tx_b('x');
usart_tx_hexa_r(value>>8);
usart_tx_hexa_r(value);
usart_tx_b(' ');

}

void htoa_8 (char * str, unsigned char input)
{
*str++ = conv_hex_ascii((input>>4)&0x0F);
*str++ = conv_hex_ascii((input>>0)&0x0F);
*str++ = 0;
}

void htoa_16 (char * str, unsigned int input)
{
*str++ = conv_hex_ascii((input>>12)&0x0F);
*str++ = conv_hex_ascii((input>>8)&0x0F);
*str++ = conv_hex_ascii((input>>4)&0x0F);
*str++ = conv_hex_ascii((input>>0)&0x0F);
*str++ = 0;
}

void htoa_24 (char * str, unsigned long input)
{
*str++ = conv_hex_ascii((input>>20)&0x0F);
*str++ = conv_hex_ascii((input>>16)&0x0F);
*str++ = conv_hex_ascii((input>>12)&0x0F);
*str++ = conv_hex_ascii((input>>8)&0x0F);
*str++ = conv_hex_ascii((input>>4)&0x0F);
*str++ = conv_hex_ascii((input>>0)&0x0F);
*str++ = 0;
}


unsigned char conv_hex_ascii (unsigned char temp)
{
if (temp<10) temp = temp + '0';
else temp = temp + 'A'- 10;
return temp;
}

void usart_tx_hexa_r (uint8_t value)
{
uint8_t temp;
temp = value;
temp = ((temp>>4)&0x0F);
if (temp<10) temp = temp + '0';
else temp = temp + 'A'- 10;
usart_tx_b(temp);
temp = value;
temp = ((temp>>0)&0x0F);
if (temp<10) temp = temp + '0';
else temp = temp + 'A' - 10;
usart_tx_b(temp);
}

void usart_tx_hexa (uint8_t value)
{
usart_tx_b('0');
usart_tx_b('x');
usart_tx_hexa_r(value);
usart_tx_b(' ');
}



/*
intel hex file parser, unused now
unsigned int parse_char (unsigned char data)
{
unsigned int t1,t2;
unsigned int retval;
retval = 0;
if (data==':')
	{
	h_state = H_SIZE;
	h_substate = 0;
	return retval;
	}

if (h_state==H_SIZE)
	{
	h_data[h_substate++] = hexa_bin(data);
	if (h_substate==2)
		{
		h_state = H_ADDR;
		h_substate = 0;
		h_len = bin_cont(h_data[1],h_data[0],0,0);
		}
	return retval;
	}

if (h_state==H_ADDR)
	{
	h_data[h_substate++] = hexa_bin(data);
	if (h_substate==4)
		{
		h_state = H_TYPE;
		h_substate = 0;
		h_addr = bin_cont(h_data[3],h_data[2],h_data[1],h_data[0]);
		}

	return retval;
	}

if (h_state==H_TYPE)
	{
	h_data[h_substate++] = hexa_bin(data);
	if (h_substate==2)
		{
		h_state = H_DATA;
		h_substate = 0;
		h_type = bin_cont(h_data[1],h_data[0],0,0);
		h_array_p=0;
		if (h_type == 0x01)
			{
			h_state = H_END;	
			h_substate = 0;
			retval = 0;
			}
		}
	else
		{
		}
	return retval;
	}

if (h_state==H_DATA)
	{
	h_data[h_substate++] = hexa_bin(data);
	if (h_substate==2)
		{
		h_data_c = bin_cont(h_data[1],h_data[0],0,0);
		h_array[h_array_p++] = h_data_c;
		h_substate = 0;
		if (h_array_p==h_len)
			{
			if (addr_highest<(h_addr+h_len)) 
				addr_highest=(h_addr+h_len);
                        //now I have data ready
			//h_len, h_addr, h_array
			h_state = H_WAIT;
			}
		else
			{

			}
		}
	return retval;
	}
if (h_state==H_END)
	{
	h_substate++;
	if ((h_substate==2)|(data<' '))
		{
		retval = addr_highest;
		addr_highest = 0;
		h_state = H_WAIT;
		}
	return retval;
	}
return retval;
}

unsigned char hexa_bin (unsigned char data)
{
if ((data>='0')&(data<='9'))
	return data - '0';
if ((data>='A')&(data<='F'))
	return data - 'A' + 10;
else
	return 0;

}

unsigned int bin_cont (unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0)
{
unsigned int t1;
t1 = b0;
t1 = t1<<4;
t1 = t1 | b1;
t1 = t1<<4;
t1 = t1 | b2;
t1 = t1<<4;
t1 = t1 | b3;
return t1;
}

*/

