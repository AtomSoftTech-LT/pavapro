#include "periph.h"
#include "hw.h"
#include "periph.c"
#include "ff.h"
#include "asm.h"
#include "asm.c"
//#include "ff.c"

//28672B maximum

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
uint8_t isp_instr (uint32_t value);
unsigned char isp_read_page (unsigned int addr, unsigned char page_size, unsigned char * data);
unsigned long isp_get_devid (void);

void disp_rfsh(char * data, unsigned char cur_x, unsigned char cur_y, unsigned char cur_mode);
void clr_disp_buf (void);
#define  INPUT_SIZE  200
unsigned char input_file[INPUT_SIZE];
unsigned char flash_buffer[128];
#define  flash_buffer_2 input_file
//unsigned char flash_buffer_2[128];

#define  TERMINAL_SIZE INPUT_SIZE
#define  RX_BUFFER_SIZE 128
#define  usart_rx_buffer flash_buffer
unsigned char usart_rx_buffer_p,terminal_p,terminal_p2;

char disp_buffer[DISP_SIZE+0];

/*
unsigned int parse_char (unsigned char data);
unsigned char hexa_bin (unsigned char data);
unsigned int bin_cont (unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0);
unsigned char h_len, h_type, h_data[5];
unsigned int	h_addr;
unsigned char h_array[64];
*/
char h_temp[10];
unsigned char h_state, h_substate, h_data_c,h_array_p;
unsigned int addr,addr_highest;

#define	H_WAIT	0
#define	H_SIZE	1
#define	H_ADDR	2
#define	H_TYPE	3
#define	H_DATA	4
#define	H_END	5

unsigned char ret,i,db_point;
UINT bw;
UINT b_read;
FATFS FatFs;
FRESULT res;
FIL fi;
FIL fo;
char path[3];
char filename[12];
char filename2[12];
char file_line[20];
char line[20];

unsigned char main_state, menu_pointer, next_state;
unsigned char key_state, key_char,end_loop;

unsigned char isp_write_page (unsigned int addr, unsigned char page_size, unsigned char * data);
unsigned char disp_list_files (unsigned char * d_buf, char * path, unsigned char start, 
unsigned char max_files, unsigned char * listed_files, unsigned char start_line, unsigned char offset);

unsigned char page_size_w,page_empty,verify_ok,efuse,hfuse,lfuse,fuf_actual,filename_len,ignore_ff;
char *ptr;
char fuses_tok[8];
unsigned int addr_b;
unsigned long device_id;
unsigned int lines_assembled;

extern unsigned int opcode_1, opcode_2;

UINT16 file_len;
UINT16 p_actual_pointer, file_pointer, file_prep_pointer, actual_line;
unsigned char cursor_x,linelen,key_stat_old,editor_state,ed_state,filename_pointer;
unsigned int lister_pointer;

unsigned char editor_task (void);
void display (void);
void prepare_disp_buffer (UINT16 start_pointer);
unsigned char is_printable (unsigned char what);
UINT16 find_line (UINT16 line);
UINT16 get_line (UINT16 pointer, unsigned char * cur_position);
unsigned char get_linelen (UINT16 pointer);
void check_spec_keys (void);
void init_editor (void);
void insert_character (unsigned char what, UINT16 pointer);
void backspace (UINT16 pointer);
void clear_disp_buffer (void);
void clear_input_file (void);
UINT16 get_file_len (void);

void disp_rfsh(void);

ISR(USART_RX_vect)
{
   char ReceivedByte;
   ReceivedByte = UDR0; // Fetch the received byte value into the variable "ByteReceived"
   if (usart_rx_buffer_p<RX_BUFFER_SIZE)
     usart_rx_buffer[usart_rx_buffer_p++] = ReceivedByte;
}

void shift_array (char * array, unsigned int len, unsigned char n)
{

}

int main (void)
{
  DIR dir;
  FILINFO fno;
  char *fn;
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
  
  UCSR0B |= (1 << RXCIE0);
  sei();
  usart_rx_buffer_p=0;
  terminal_p=0;
  /*
  while (1)
    {}  
*/
  path[0]='/';
  path[1]=0;

  main_state = 0;
  menu_pointer = 0;
  
  f_mount(&FatFs, "", 0);
  page_size_w = 64;//words
  ignore_ff = 1;
  while (1)
  {
    if (main_state==0)
    {
      main_state=1;
      menu_pointer = 0;
    }    
    if (main_state==1)
    {
      clr_disp_buf();
                                            //01234567890123456789*01234567890123456789*01234567890123456789*01234567890123456789*01234567890123456789*
      strcpy_P(disp_buffer+0*DISP_WIDTH,PSTR("Check target         Program FLASH        Program fuses        Edit file            List file"));
//      strcpy_P(disp_buffer+1*DISP_WIDTH,PSTR("Program FLASH"));
//      strcpy_P(disp_buffer+2*DISP_WIDTH,PSTR("Program fuses"));
//      strcpy_P(disp_buffer+3*DISP_WIDTH,PSTR("Edit file"));
//      strcpy_P(disp_buffer+4*DISP_WIDTH,PSTR("List file"));
                                            //01234567890123456789*01234567890123456789*01234567890123456789*01234567890123456789*01234567890123456789*
      strcpy_P(disp_buffer+5*DISP_WIDTH,PSTR("Assemble file        Settings             Serial terminal"));
//      strcpy_P(disp_buffer+6*DISP_WIDTH,PSTR("Settings"));
//      strcpy_P(disp_buffer+7*DISP_WIDTH,PSTR("Serial terminal"));
      disp_rfsh(disp_buffer,0,menu_pointer,2);
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        if (key_char==KEY_DN)
          if (menu_pointer<7) menu_pointer++;
        if (key_char==KEY_UP)
          if (menu_pointer>0) menu_pointer--;
        if (key_char==KEY_ENT)
        {
          if (menu_pointer==0) main_state = 70;
          else if (menu_pointer==7) main_state = 80;
          else main_state = menu_pointer*10;
          /*
          if (menu_pointer==1) main_state = 10;
          if (menu_pointer==2) main_state = 20;
          if (menu_pointer==3) main_state = 30;
          if (menu_pointer==4) main_state = 40;
          if (menu_pointer==5) main_state = 50;
          if (menu_pointer==6) main_state = 60;
          if (menu_pointer==7) main_state = 80;*/
        }
      }  
    }

    if (main_state==70)
    {
      clr_disp_buf();
      isp_progmode_enable(1);
      device_id = isp_get_devid();
      //sp sprintf (disp_buffer,"DEVID: %6.6lx",isp_get_devid());
      strcpy_P(disp_buffer+0*DISP_WIDTH,PSTR("DEVID"));
      htoa_24(disp_buffer+0*DISP_WIDTH + 7,device_id);
      isp_progmode_enable(0);
      disp_rfsh(disp_buffer,0,0,0);   
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        if (key_char==KEY_ENT) main_state = 0;
      }
    }

    //***************************programmer routines *********************************  

    if (main_state==10)
    {
      if (open_file(filename,path,"FLASH file")==1)
        main_state = 11;      
      else
        main_state = 0;
    }

    if (main_state==11)
    {
      if (f_open(&fi, filename, FA_READ) == FR_OK)
      {
        addr_b = 0;
        clr_disp_buf();
        isp_progmode_enable(1);
        device_id = isp_get_devid();
        strcpy_P(disp_buffer+0*DISP_WIDTH,PSTR("DEVID"));
        htoa_24(disp_buffer+0*DISP_WIDTH + 7,device_id);
        disp_rfsh(disp_buffer,0,0,0); 
        isp_instr(0xAC800000);
        while (isp_instr(0xF0000000)==0xFF);
        while (1)
        {
          for (i=0;i<page_size_w*2;i++) flash_buffer[i]=0xFF;
          if ((addr_b%256)==0)
          {
            strcpy_P(disp_buffer+1*DISP_WIDTH,PSTR("addr"));
            htoa_16(disp_buffer+1*DISP_WIDTH + 6,addr_b);
            disp_rfsh(disp_buffer,0,0,0);        
          }
          f_read(&fi,&flash_buffer,page_size_w*2,&b_read);
          if (b_read==page_size_w*2)
          {
            page_empty = 1;
            for (i=0;i<b_read;i++)
              if (flash_buffer[i]!=0xFF) page_empty = 0;
            if (page_empty==0)
              isp_write_page(addr_b,page_size_w,flash_buffer);
            else
            {
            }
            while (isp_instr(0xF0000000)==0xFF);
            addr_b = addr_b + (page_size_w*2);
          }
          else
          {
            if (b_read>0)
            {
              isp_write_page(addr_b,page_size_w,flash_buffer);
              while (isp_instr(0xF0000000)==0xFF);
            }
            break;        
          }
        }
        strcpy_P (disp_buffer+2*DISP_WIDTH,PSTR("verifying FLASH"));
        disp_rfsh(disp_buffer,0,0,0); 
        res = f_lseek(&fi, 0);
        addr_b = 0;
        verify_ok = 1;
        while (1)
        {
          if ((addr_b%256)==0)
          {
            strcpy_P(disp_buffer+3*DISP_WIDTH,PSTR("addr"));
            htoa_16(disp_buffer+3*DISP_WIDTH + 6,addr_b);
            disp_rfsh(disp_buffer,0,0,0);        
          }
          f_read(&fi,&flash_buffer,page_size_w*2,&b_read);
          page_empty = 1;
          for (i=0;i<b_read;i++) if (flash_buffer[i]!=0xFF) page_empty = 0;
          if (page_empty==0)
          {
            isp_read_page(addr_b,page_size_w,flash_buffer_2);
            for (i=0;i<b_read;i++)
            {
              if (flash_buffer_2[i]!=flash_buffer[i])
              {
                verify_ok = 0;
                strcpy (disp_buffer+4*DISP_WIDTH,"err");
                htoa_16(disp_buffer+0*DISP_WIDTH + 5,addr_b+i);
                disp_rfsh(disp_buffer,0,0,0);
                break;
              }
            }
          }
          if (verify_ok == 0)
            break;
          addr_b = addr_b + (page_size_w*2);
          if (b_read!=page_size_w*2)
          {
            break;
          }
        }
        if (verify_ok==1)
        {
          strcpy_P (disp_buffer+4*DISP_WIDTH,PSTR("OK"));
          disp_rfsh(disp_buffer,0,0,0);
        }
        f_close(&fi);
        isp_progmode_enable(0);
      }
      main_state = 12;
    }

    if (main_state==12)
    {
      disp_rfsh(disp_buffer,0,0,0);   
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        if (key_char==KEY_ENT) main_state = 0;
      }
    }

    //***************************fuse programmer routines *********************************  

    if (main_state==20)
    {
      if (open_file(filename,path,"select fuse file")==1)
        main_state = 21;
      else
        main_state = 0;
      clr_disp_buf();
    }

    if (main_state==21)
    {
      main_state = 12;
//      strcpy_P (disp_buffer+1*DISP_WIDTH,PSTR("invalid file"));
      if (f_open(&fi, filename, FA_READ) == FR_OK)
      {
        if (f_gets(file_line,30,&fi)!=0) 
        {
          fuf_actual = get_token(file_line,fuses_tok,0);
          if (strcmp(fuses_tok,"fuses")==0)
          {
            fuf_actual = get_token(file_line,fuses_tok,fuf_actual);
            efuse = convert_to_8_number(fuses_tok);
            fuf_actual = get_token(file_line,fuses_tok,fuf_actual);
            hfuse = convert_to_8_number(fuses_tok);
            fuf_actual = get_token(file_line,fuses_tok,fuf_actual);
            lfuse = convert_to_8_number(fuses_tok);
            strcpy_P (disp_buffer+1*DISP_WIDTH,PSTR("fuses"));
            htoa_8(disp_buffer+1*DISP_WIDTH + 6,efuse);
            htoa_8(disp_buffer+1*DISP_WIDTH + 9,hfuse);
            htoa_8(disp_buffer+1*DISP_WIDTH + 12,lfuse);
            strcpy_P (disp_buffer+2*DISP_WIDTH,PSTR("fuses write?"));
            disp_rfsh(disp_buffer,0,0,0);
            main_state = 22;
          }
        }
        else
        {
          strcpy_P (disp_buffer+1*DISP_WIDTH,PSTR("invalid file"));
          disp_rfsh(disp_buffer,0,0,0);
        }
        f_close(&fi);
      }
    }

    if (main_state==22)
    {
      disp_rfsh(disp_buffer,0,0,0);   
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        if (key_char==KEY_ESC) main_state = 0;
        if (key_char==KEY_ENT) 
        {
          isp_progmode_enable(1);
          isp_instr(0xACA40000 | efuse);
          isp_instr(0xACA80000 | hfuse);
          isp_instr(0xACA00000 | lfuse);
          isp_progmode_enable(0);
          strcpy_P (disp_buffer+3*DISP_WIDTH,PSTR("fuses written"));
          main_state = 23;
        }
      }
    }

    if (main_state==23)
    {
      disp_rfsh(disp_buffer,0,0,0);   
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        if (key_char==KEY_ENT) main_state = 0;
      }
    }

    if (main_state==30)
    {
      //usart_tx_b ('E');
      clr_disp_buf();
      for (i=0;i<INPUT_SIZE;i++) input_file[i]=0;
      file_pointer = 0;
      editor_state = 1;
      main_state = 31;
      if (open_file(filename,path,"editor")==1)
        {
  //      usart_tx_b ('S');
        if (f_open(&fi, filename, FA_READ) == FR_OK)
          {
            //usart_tx_b ('R');
            f_read(&fi,&input_file,INPUT_SIZE,&b_read);
            input_file[b_read]=0;
            main_state = 31;
            //usart_tx_s ((uint8_t*)input_file);
            f_close(&fi);
          }
        else
          {
//          usart_tx_b ('Q');
          main_state = 0;
          }
        }
      else
        {

        }
    }
    
    
    if (main_state==31)
    {
    if (editor_task()==2)
      {
      main_state = 32;
      menu_pointer=0;
      }
    }


    if (main_state==32)
    {
      clr_disp_buf();
      strcpy_P(disp_buffer+0*DISP_WIDTH,PSTR("return to editor"));
      strcpy_P(disp_buffer+1*DISP_WIDTH,PSTR("save to file"));
      strcpy_P(disp_buffer+2*DISP_WIDTH,PSTR("exit w/o save"));
      disp_rfsh(disp_buffer,0,menu_pointer,2);
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        if (key_char==KEY_DN)
          if (menu_pointer<3) menu_pointer++;
        if (key_char==KEY_UP)
          if (menu_pointer>0) menu_pointer--;
        if (key_char==KEY_ENT)
        {
          if (menu_pointer==0) main_state = 31;
          if (menu_pointer==1) main_state = 33;
          if (menu_pointer==2) main_state = 0;
        }
      }  
    }

    if (main_state==33)
    {
    key_state = kbd_tasks(&key_char,1);
  
    if (key_state>0)
      {
      if (key_state == 1)
        {
  //      usart_tx_b ('N');
  //      usart_tx_b (key_char);
        filename[filename_pointer] = key_char;
        }
      if (key_state == 2)
        {
 //       usart_tx_b ('R');
 //       usart_tx_b (key_char);
        filename[filename_pointer] = key_char;
        }
      if (key_state == 3)
        {
        filename_pointer++;
//        usart_tx_b ('F');
//        usart_tx_b (char_m);
        }
      if (key_state == 4)
        {
        if (key_char==KEY_ENT)
          {
          /*
          usart_tx_s ((uint8_t*)filename);
          usart_tx_hexa(file_len);
          */
          if (f_open(&fi,filename, FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
            {
            f_write(&fi,input_file,file_len,&bw);
            if (bw==file_len)
              strcpy_P (disp_buffer+3*DISP_WIDTH,PSTR("file written"));
            else
              strcpy_P (disp_buffer+3*DISP_WIDTH,PSTR("write failed"));
            f_close(&fi); 
            }
          else
            strcpy_P (disp_buffer+3*DISP_WIDTH,PSTR("can't open file"));
          disp_rfsh(disp_buffer,0,0,0);
          main_state = 31;
          }
        if (key_char==24)
          {
          if (filename_pointer>0) filename_pointer--;
          }
        if (key_char==26)
          {
          if (filename_pointer<12) filename_pointer++;
          }
          
        }
      }
    clr_disp_buf();
    strcpy_P (disp_buffer,PSTR("Save as"));
    strcpy(disp_buffer+2*DISP_WIDTH,filename);
    disp_buffer[2*DISP_WIDTH+filename_pointer] = disp_buffer[2*DISP_WIDTH+filename_pointer] | 0x80;
    disp_rfsh(disp_buffer,0,0,0);
    }


    if (main_state==40)
    {
      if (open_file(filename,path,"lister file")==1)
        {
        if (f_open(&fi,filename, FA_READ) == FR_OK)
          {
          main_state = 41;
          lister_pointer = 0;
          }
        else
          main_state = 0;
        }
      else
        {
        main_state = 0;
        }
    }

    if (main_state==41)
    {
      res = f_lseek(&fi, lister_pointer);
      f_read(&fi,&input_file,DISP_WIDTH*6,&b_read);
      input_file[b_read] = 0;
      main_state = 42;
    }

    if (main_state==42)
    {
      clr_disp_buf();
      strcpy (disp_buffer,(char *)input_file);
      strcpy (disp_buffer+7*DISP_WIDTH+9,(char *)filename);
      strcpy_P (disp_buffer+7*DISP_WIDTH,PSTR("L:"));
      itoa (lister_pointer/DISP_WIDTH,disp_buffer+DISP_WIDTH*7+3,10);
      disp_rfsh(disp_buffer,0,0,0);
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        main_state = 41;
        if (key_char==KEY_DN)
          lister_pointer = lister_pointer + DISP_WIDTH;
        if (key_char==KEY_UP)
          {
          if (lister_pointer>=DISP_WIDTH) lister_pointer = lister_pointer - DISP_WIDTH;
          else lister_pointer = 0;
          }
        if (key_char==KEY_PD)
          lister_pointer = lister_pointer + DISP_WIDTH*6;
        if (key_char==KEY_PU)
          {
          if (lister_pointer>=DISP_WIDTH*6) lister_pointer = lister_pointer - DISP_WIDTH*6;
          else lister_pointer = 0;
          }
        if (key_char==KEY_ESC)
          {
          main_state = 0;
          f_close(&fi);
          }
      }  
    }
    

    if (main_state==50)
    {
      if (open_file(filename,path,"select asm file")==1)
        {
        strcpy(filename2,filename);
  //      filename_len = strlen(filename2);
        filename2[strlen(filename2)-3] = 0;
        strcat(filename2,"OUT");
        main_state = 51;
        }
      else
        {
        main_state = 0;
        }
//      clr_disp_buf();
    }
    
    if (main_state==51)
    {
      address = 0;
      lines_assembled = 0;
      clr_disp_buf();
      strcpy_P (disp_buffer,PSTR("assembler"));
      disp_rfsh(disp_buffer,0,0,0);   
      if (f_open(&fi,filename, FA_READ) == FR_OK)
      {
        //usart_tx_b ('S');
        if (create_output(filename2)==1)
        {
          //usart_tx_b ('D');
          while (f_gets(line,30,&fi)!=0)
          {
            //usart_tx_s ((uint8_t*)line);
            
            if (line[0]==' ')
            {
              lines_assembled++;
              //line contains instruction
              opcodes = 1; //assume single byte instruction
              //printf("-input:%s",line);
              pos = get_token(line,tok,0);
              opcode_table();
//              usart_tx_hexa(opcodes);
//              usart_tx_hexa_16(opcode_1);
              //usart_tx_b (0x0A);
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
        //usart_tx_b ('E');
        //usart_tx_hexa_16(lines_assembled);
        strcpy_P (disp_buffer+1*DISP_WIDTH,PSTR("   lines assembled"));
        htoa_8(disp_buffer+1*DISP_WIDTH,lines_assembled);
      }
      main_state = 55;
    }
    
    if (main_state==55)
    {
      disp_rfsh(disp_buffer,0,0,0);   
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        if (key_char==KEY_ENT) main_state = 0;
      }
    }


/*
    if (main_state==50)
    {
      clr_disp_buf();
      strcpy ((char*)disp_buffer,"assembler");
      disp_rfsh(disp_buffer,0,0,0);   
      if (f_open(&fi, "src.asm", FA_READ) == FR_OK)
      {
          while (f_gets(line,30,&fi)!=0)
          {
            usart_tx_s ((uint8_t*)line);
          }

        f_close(&fi);  
        usart_tx_b ('E');
      }
      main_state = 0;
    }
    */
    if (main_state==60)
    {
    menu_pointer = 0;
    main_state = 61;
    }    
    
    if (main_state==61)
    {
      clr_disp_buf();
      strcpy_P(disp_buffer+0*DISP_WIDTH,PSTR("page size    words"));
      htoa_8(disp_buffer+0*DISP_WIDTH + 10,page_size_w);
      strcpy_P(disp_buffer+1*DISP_WIDTH,PSTR("ignore 0xFF: "));
      htoa_8(disp_buffer+1*DISP_WIDTH + 13,ignore_ff);
      disp_rfsh(disp_buffer,0,menu_pointer,2);
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
        if (key_char==KEY_DN)
          if (menu_pointer<1) menu_pointer++;
        if (key_char==KEY_UP)
          if (menu_pointer>0) menu_pointer--;
        if (key_char==KEY_ESC)
          main_state = 0;
        if (key_char==KEY_ENT)
        {
          if (menu_pointer==0) 
            {
            if (page_size_w == 16) page_size_w = 32;//words
            else if (page_size_w == 32) page_size_w = 64;//words
            else if (page_size_w == 64) page_size_w = 16;//words
            }
          if (menu_pointer==1) 
            {
              if (ignore_ff==0) ignore_ff = 1;
              else ignore_ff = 0;
            }
        }
      }  
    }

  if (main_state==80)
    {
      main_state=81;
      clr_disp_buf();
    }

  if (main_state==81)
    {
      cli();
      if (usart_rx_buffer_p>0)
        {
        terminal_p2 = 0;
        while (1)
          {
          if (usart_rx_buffer[terminal_p2]>=' ')
            disp_buffer[terminal_p++] = 0x7F&usart_rx_buffer[terminal_p2++];
          else
            {
            terminal_p = terminal_p + DISP_WIDTH - (terminal_p%DISP_WIDTH);
            terminal_p2++;
            }
          if (terminal_p>=7*DISP_WIDTH)
            {
            for (i=DISP_WIDTH;i<7*DISP_WIDTH;i++)
              disp_buffer[i-DISP_WIDTH] = disp_buffer[i];
            for (i=6*DISP_WIDTH;i<7*DISP_WIDTH;i++) disp_buffer[i] = ' ';
            terminal_p = terminal_p - DISP_WIDTH;
            }
          if (terminal_p2==usart_rx_buffer_p)
            break;
          }
        usart_rx_buffer_p = 0;
        }
      sei();
      disp_rfsh(disp_buffer,0,0,0);   
      key_state = kbd_tasks(&key_char,1);
      if (key_state == 4)
      {
      if (key_char==KEY_ESC)
        main_state = 0;       
      }
    }
  }
}


unsigned char editor_task (void)
{
file_len = get_file_len();
if (file_pointer>file_len) file_pointer = file_len-1;
if (file_pointer>INPUT_SIZE) file_pointer = INPUT_SIZE-1;
actual_line = get_line (file_pointer, &cursor_x);	//file pointer points to actual position in file, so compute line number
file_prep_pointer = find_line (actual_line);		//and from this, compute pointer to start of that line
if (file_prep_pointer>file_len) file_prep_pointer = file_len;
clr_disp_buf();
prepare_disp_buffer(file_prep_pointer);				//draw to window from this position
disp_rfsh(disp_buffer,0,0,0);
linelen = get_linelen(file_prep_pointer);
//display();
editor_state = 1;
//_delay_ms(10);
key_state = kbd_tasks(&key_char,1);
  if (key_state>0)
    {
    if (key_state == 1)
      {
/*      usart_tx_b ('N');
      usart_tx_b (key_char);*/
      insert_character (key_char, file_pointer);
      }
    if (key_state == 2)
      {
/*      usart_tx_b ('R');
      usart_tx_b (key_char);*/
      input_file[file_pointer] = key_char;
      }
    if (key_state == 3)
      {
      file_pointer++;
 /*     usart_tx_b ('F');
      usart_tx_b (key_char);*/
      }
    if (key_state == 4)
      {
      check_spec_keys(key_char);
      }  
    }
return editor_state;
}

void prepare_disp_buffer (UINT16 start_pointer)
{
unsigned char writer_state;
unsigned char disp_x;
unsigned int disp_y;
UINT16 p;
writer_state = 0;
disp_y = 0;
disp_x = 0;
p = start_pointer;
while (1)
  {
  if (p>file_len)
    break;
  if (writer_state==0)
    {
    if (is_printable(input_file[p]))
      {
      disp_buffer[disp_y*21+disp_x] = input_file[p++];
      disp_x++;
      if (disp_x==21)
        {
        disp_x = 0;
        disp_y++;
        if (disp_y==7)
          writer_state=3;
        }
      }
    else
      writer_state=1;
    }
  if (writer_state==1)
    {
    if (is_printable(input_file[p])==0)
      p++;
    else
      {
      writer_state=0;
      disp_x = 0;
      disp_y++;
      if (disp_y==7)
        writer_state=3;
      }
    }
  if (writer_state==3)
    {
//      usart_tx_b ('E');
      break;
    }    
    
  }
disp_buffer[cursor_x] = disp_buffer[cursor_x]|0x80;
}

/*
void prepare_disp_buffer (UINT16 start_pointer)
{
UINT16 p;
unsigned long r;
unsigned char db_p,db_o;
p = start_pointer;
for (db_o=0;db_o<(21*8);db_o = db_o + 21)
	{
	db_p = 0;
	while ((is_printable(input_file[p]))&(db_p<21))
		{
		disp_buffer[db_o+db_p] = input_file[p];
		if (db_o==0) disp_buffer[db_o+cursor_x] = disp_buffer[db_o+cursor_x] | 0x80;
		db_p++;
		p++;
		}
	while (db_p<21)
		{
		disp_buffer[db_o+db_p] = ' ';
		if (db_o==0) disp_buffer[db_o+cursor_x] = disp_buffer[db_o+cursor_x] | 0x80;
		db_p++;
		}
	while (is_printable(input_file[p])==0)
		{
		p++;
		}
	}
//sprintf ((char *)disp_buffer+112,"                ");
p = file_pointer;
p = p * 100;
p = p / file_len;
r = file_len;
r = r * 100;
r = r / INPUT_SIZE;
//sprintf ((char *)disp_buffer+112,"L%d,%d%%,M%d%%   ",actual_line,((UINT16)(p)),((UINT16)(r)));
}

*/
void check_spec_keys (unsigned char key)
{
if (key==KEY_ESC)
	{
	editor_state = 2;
	}

if (key==KEY_BSP)
	{
	if (file_pointer>0)
		{
		file_pointer--;
		backspace(file_pointer);
		}
	}

if (key==KEY_ENT)
	{
//	input_file[file_pointer] = NEW_LINE;
	insert_character(NEW_LINE,file_pointer);
	file_pointer++;
	if (input_file[file_pointer] == 0x00)
		{
		input_file[file_pointer] = ' ';
		file_pointer++;
		}
	}
//rt
if (key==KEY_RT)
	{
	file_pointer++;
	}
//lt
if (key==KEY_LT)
	{
	if (file_pointer>0) file_pointer--;
	}
//dn
if (key==KEY_DN)
	{
	actual_line++;
	file_len = get_file_len();
	file_prep_pointer = find_line (actual_line);		//and from this, compute pointer to start of that line
	if (file_prep_pointer>file_len) file_prep_pointer = file_len;
	linelen = get_linelen(file_prep_pointer);
	if (linelen>cursor_x) 	file_pointer = file_prep_pointer + cursor_x;
		else file_pointer = file_prep_pointer + linelen;
	}
//up
if (key==KEY_UP)
	{
	if (actual_line>0)
		{
		actual_line--;
		file_prep_pointer = find_line (actual_line);		//and from this, compute pointer to start of that line
		file_pointer = file_prep_pointer;
		linelen = get_linelen(file_prep_pointer);
		if (linelen>cursor_x) 	file_pointer = file_prep_pointer + cursor_x;
			else file_pointer = file_prep_pointer + linelen;
		}
	}

}


void backspace (UINT16 pointer)
{
UINT16 i;
if (pointer>=0) 
	if (pointer<INPUT_SIZE) 
		for (i=pointer; i<INPUT_SIZE;i++)
			{
			input_file[i] = input_file[i+1];
			}
}


void insert_character (unsigned char what, UINT16 pointer)
{
UINT16 i;
unsigned char temp;
for (i=INPUT_SIZE; i>pointer;i--)
	{
	temp = input_file[i-1];
	input_file[i] = temp;
	}
input_file[pointer] = what;
}


unsigned char is_printable (unsigned char what)
{
if ((what>=' ')&(what<='z')) 
	return 1;
else return 0;

}


//return line number from file pointer
UINT16 get_line (UINT16 pointer, unsigned char * cur_pos)
{
UINT16 line,pp;
unsigned char cp;
line = 0;
pp=0;
cp=0;
while (pp!=pointer)
 	{
	if (input_file[pp]==NEW_LINE) 
		{
		line++;
		cp=0;
		}
	else cp++;
	if (input_file[pp]==0x00) 
		{
		return 0xFFFF;
		}
	pp++;
	}
*cur_pos = cp;
return line;
}

unsigned char get_linelen (UINT16 pointer)
{
unsigned char linlen;
linlen=0;
while ((input_file[pointer]!=NEW_LINE)&(input_file[pointer]!=0x00))
	{
	pointer++;
	linlen++;
	if (linelen==255) return 255;
	}
return linlen;
}

//return pointer to beginning of line from line number
UINT16 find_line (UINT16 line)
{
UINT16 pointer;
UINT16 actual_line;
actual_line = 0;
pointer = 0;
while (actual_line!=line)
	{
	if (input_file[pointer] == NEW_LINE) actual_line++;
	pointer++;
	if (pointer>1000) return 0xFFFF;
	}
return pointer;
}




void clear_input_file (void)
{
UINT16 i;
for (i=0;i<INPUT_SIZE;i++) input_file[i] = ' ';
input_file[1] = 0x00;
}

UINT16 get_file_len (void)
{
UINT16 i;
i=0;
while (input_file[i]>0) 
	{
	i++;
	}
return i;
}


//********************************************************************************************



void clr_disp_buf (void)
{
  unsigned int i;
  for (i=0;i<DISP_SIZE;i++) disp_buffer[i] = 0;
}

unsigned char open_file (char * filename, char * path, char * caption)
{
  unsigned char disp_list_start, disp_list_real,y_cur;
  unsigned char file_pointer;
  disp_list_start = 0;
  file_pointer = 0;
  clr_disp_buf();
  while (1)
  {
    disp_list_files(disp_buffer,path,disp_list_start,4,&disp_list_real,1,2);
    if ((disp_list_real<4)&(file_pointer>0))
    {
      file_pointer--;
      if (disp_list_start>0)
        disp_list_start--;
    }
    y_cur = 1+file_pointer-disp_list_start;
    key_state = kbd_tasks(&key_char,1);
    if (key_state == 4)
    {
      if (key_char==25)
        file_pointer++;
      if (key_char==22)
      {
        if (file_pointer>0)
          file_pointer--;
      }
      if (key_char==15)
      {
        strcpy (filename,(char *)(disp_buffer+2+21*(y_cur)));
        return 1;
      }
      if (key_char==13)
      {
        return 0;
      }

    }
    if (file_pointer>=(disp_list_start+4))
      disp_list_start++;
    if ((file_pointer<disp_list_start)&(disp_list_start>0))
      disp_list_start--;
    strcpy (disp_buffer,caption);
    disp_rfsh(disp_buffer,0,y_cur,2);
  }


}


  DIR dir;
  FILINFO fno;

unsigned char disp_list_files (char * d_buf, char * path, unsigned char start, unsigned char max_files, unsigned char * listed_files, unsigned char start_line, unsigned char offset)
{
  unsigned char i,j;
  char *fn;
  i=0;
  j=0;
  res = f_opendir(&dir, path); 
  if (res == FR_OK)
  {
    while (1)
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) 
      {
        break;
      }
      i++;
      if (i>start)
      {
        fn = (char *)(fno.fname);
        strcpy((char *)(d_buf+offset+21*(j+start_line)),fn);
        j++;
        if (j>=max_files)
        {
          break;
        }
      }
    }
    *listed_files = j;
    return 1;
  }
  else return 0;
}



void disp_rfsh(char * data, unsigned char cur_x, unsigned char cur_y, unsigned char mode)
{
  unsigned char i;
  unsigned char counter;
  for (i=0;i<8;i++)
  {
    disp_set_xy(0,i);
    counter=0;
    while(counter<21)
    {
      if (i==cur_y)
      {
        if ((mode==1)&(counter==cur_x))
          char_print(0x80|(*(data + counter + (21*i))));
        else if (mode==2)
          char_print(0x80|(*(data + counter + (21*i))));
        else char_print(*(data + counter + (21*i)));
      }
      else
        char_print(*(data + counter + (21*i)));
      counter++;
    }
  }
}

unsigned char isp_read_page (unsigned int addr, unsigned char page_size, unsigned char * data)
{
  unsigned char dat1,dat2;
  unsigned long addr_sh;
  addr = addr / 2;
  for (i=0;i<page_size;i=i+1)
  {
    addr_sh = addr + i;
    addr_sh = addr_sh<<8;
    data[i*2] = isp_instr(0x20000000 | addr_sh);
    data[1+i*2] = isp_instr(0x28000000 | addr_sh);
  }
}


unsigned char isp_write_page (unsigned int addr, unsigned char page_size, unsigned char * data)
{
  //page size in words!
  //addr in bytes
  unsigned char i;
  unsigned long addr_l;
  addr = addr / 2;
  for (i=0;i<page_size;i=i+1)
  {
    isp_instr(0x40000000 | (data[i*2]) | ((unsigned long)(i))<<8);
    isp_instr(0x48000000 | (data[1+i*2]) | ((unsigned long)(i))<<8);
  }
  isp_instr(0x4C000000 | (((unsigned long)(addr))<<8));
  return 1;
}


unsigned long isp_get_devid (void)
{
  unsigned long id;
  id = isp_instr(0x30000000);
  id = id <<8;
  id = id | isp_instr(0x30000100);
  id = id <<8;
  id = id | isp_instr(0x30000200);
  return id;
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
    _delay_us(15);
    ISP_PORT |= (1<<ISP_SCK);
    _delay_us(15);
    temp = temp << 1;
    if ((ISP_PIN&(1<<ISP_MISO))!=0)
      temp = temp | 0x01;
    ISP_PORT &= ~(1<<ISP_SCK);
  }
  return temp;
}












