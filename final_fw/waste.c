#ifdef DANGEROUS_SOURCE_CODE_WASTE_ENABLED

 
  while (kbd_read()==0);
  PORTB |= (1<<5);
  _delay_ms(500);  
  PORTB &= ~(1<<5);
  
  disp_set_xy(0,0);
  putslcd("OLED display       ");
  
  
  kbd_me_timer = 0;
  db_point = 3;
  disp_buffer[0]='A';
  disp_buffer[1]='B';
  disp_buffer[2]='C';
  while (1)
  {
  _delay_ms(10);
  key_state = kbd_tasks(&key_char,1);

  if (key_state>0)
    {
    if (key_state == 1)
      {
      usart_tx_b ('N');
      usart_tx_b (key_char);
      disp_buffer[db_point] = key_char;
      }
    if (key_state == 2)
      {
      usart_tx_b ('R');
      usart_tx_b (key_char);
      disp_buffer[db_point] = key_char;
      }
    if (key_state == 3)
      {
       db_point++;
      usart_tx_b ('F');
      usart_tx_b (char_m);
      }
    stat_m = 0;
    }
  disp_set_xy(0,0);
  putslcd((char *)disp_buffer);
   
  }




//*************************************************

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


//*************************************************
  while (1)
  {
   ret = kbd_read();
  usart_tx_b('A'+ret);
  _delay_ms(100);  
  }

//*************************************************
  disp_set_xy(0,0);
  putslcd("SD init              ");
  
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
    
    while (1);
//*************************************************
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
//*************************************************

  
  while (1)
    {
     ret = kbd_read();
    if (ret!=0)
      {
        PORTB |= (1<<5);
      while (kbd_read()!=0);
    _delay_ms(100);  
    PORTB &= ~(1<<5);

      }
    
    if (ret==1) 
      {
      usart_tx_b('1');
      usart_tx_b(':');
      isp_progmode_enable(1);
      usart_tx_hexa(isp_instr(0x30000000));
      usart_tx_hexa(isp_instr(0x30000100));
      usart_tx_hexa(isp_instr(0x30000200));
      usart_tx_hexa(isp_instr(0x30000300));
      isp_progmode_enable(0);
      usart_tx_b(0x0A);    
      }
    if (ret==2) 
      {
      usart_tx_b('2');
      isp_progmode_enable(1);
      for (i=0;i<32;i=i+1)
        {
        if ((i%4)==0) usart_tx_b(0x0A);
          usart_tx_hexa(isp_instr(0x28000000 | ((unsigned long)(i))<<8));
          usart_tx_hexa(isp_instr(0x20000000 | ((unsigned long)(i))<<8));
        }
      usart_tx_b(0x0A);
      isp_progmode_enable(0);
      }

    if (ret==3) 
      {
      //erase program memory
      usart_tx_b('3');
      usart_tx_b(':');
      isp_progmode_enable(1);
      isp_instr(0xAC800000);
      while (isp_instr(0xF0000000)==0xFF);
      isp_progmode_enable(0);
      usart_tx_b(0x0A);
      }

    if (ret==4) 
      {
      //load program memory
      usart_tx_b('4');
      usart_tx_b(':');
      //program
      usart_tx_b(0x0A);
      isp_progmode_enable(1);
      /*
      for (i=0;i<16;i=i+1)
        {
        _delay_us(100);
        isp_instr(0x40000000 | (program[i*2]) | ((unsigned long)(i))<<8);
        _delay_us(100);
        isp_instr(0x48000000 | (program[1+i*2]) | ((unsigned long)(i))<<8);
        }
      isp_instr(0x4C000000);
      */
/*
      isp_write_page(0,16,program);
      while (isp_instr(0xF0000000)==0xFF);
      isp_write_page(32,16,program);      
      while (isp_instr(0xF0000000)==0xFF);
*/
      isp_progmode_enable(0);
      usart_tx_b(0x0A);
      }
    }

//*************************************************
  kbd_me_timer = 0;
  db_point = 3;
  disp_buffer[0]='A';
  disp_buffer[1]='B';
  disp_buffer[2]='C';
  while (1)
  {
  _delay_ms(10);
  key_state = kbd_tasks(&key_char,1);

  if (key_state>0)
    {
    if (key_state == 1)
      {
      usart_tx_b ('N');
      usart_tx_b (key_char);
      disp_buffer[db_point] = key_char;
      }
    if (key_state == 2)
      {
      usart_tx_b ('R');
      usart_tx_b (key_char);
      disp_buffer[db_point] = key_char;
      }
    if (key_state == 3)
      {
       db_point++;
      usart_tx_b ('F');
      usart_tx_b (key_char);
      }
    if (key_state == 4)
      {
       usart_tx_b ('_');
       usart_tx_hexa_r(key_char); 
       usart_tx_b (0x0A);
      }      
      
    stat_m = 0;
    }
  disp_set_xy(0,0);
  putslcd((char *)disp_buffer);
   
  }


//*************************************************


//*************************************************


//*************************************************


//*************************************************


#endif
