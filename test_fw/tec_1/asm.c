#include "asm.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"
#include "ff.h"

unsigned char error;
char str1[10],str2[10];
char tok[10];
unsigned char opcodes;
unsigned int opcode_1, opcode_2;
unsigned int address;

extern char line[30];
char pos;
unsigned int read;
FIL fo;


char create_output (char * name)
{
if (f_open(&fo, "out.bin", FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
  return 1;
else
  return 0;
}

char write_output (unsigned int addr, unsigned char opcodes, unsigned int opcode1, unsigned int opcode2)
{
unsigned char buffer[2];
UINT bw;
f_lseek(&fo,addr);
if (opcodes==1)
	{
	buffer[0] = opcode1>>0;
	buffer[1] = opcode1>>8;
	f_write(&fo,buffer,2,&bw);
        
	}
if (opcodes==2)
	{
	buffer[0] = opcode1>>0;
	buffer[1] = opcode1>>8;
	f_write(&fo,buffer,2,&bw);
	buffer[0] = opcode2>>0;
	buffer[1] = opcode2>>8;
	f_write(&fo,buffer,2,&bw);
	}

}

char close_output (void)
{
  
f_close(&fo); 

}


void opcode_table (void)
{
	if (strcmp(tok,"#org")==0) 	error = popcode_org(line,pos);
	//P,b group
	if (strcmp(tok,"sbi")==0) 	error = opcode_p_b(line,pos,0x9A00,&opcode_1);
	if (strcmp(tok,"cbi")==0) 	error = opcode_p_b(line,pos,0x9800,&opcode_1);
	if (strcmp(tok,"sbic")==0) error = opcode_p_b(line,pos,0x9900,&opcode_1);
	if (strcmp(tok,"sbis")==0) error = opcode_p_b(line,pos,0x9B00,&opcode_1);
	//Rd group. note that a lot of instructions this group doesn't really exist, like like LSL Rd = ADD Rd,Rd
	if (strcmp(tok,"asr")==0) 	error = opcode_Rd(line,pos,0x9405,&opcode_1);
	if (strcmp(tok,"swap")==0) error = opcode_Rd(line,pos,0x9402,&opcode_1);
	if (strcmp(tok,"ror")==0) error = opcode_Rd(line,pos,0x9407,&opcode_1);		
	if (strcmp(tok,"lsr")==0) error = opcode_Rd(line,pos,0x9406,&opcode_1);		
	if (strcmp(tok,"push")==0) error = opcode_Rd(line,pos,0x920F,&opcode_1);		
	if (strcmp(tok,"pop")==0) error = opcode_Rd(line,pos,0x900F,&opcode_1);		
	if (strcmp(tok,"inc")==0) error = opcode_Rd(line,pos,0x9403,&opcode_1);		
	if (strcmp(tok,"dec")==0) error = opcode_Rd(line,pos,0x940A,&opcode_1);		
	if (strcmp(tok,"ser")==0) error = opcode_Rd(line,pos,0xEF0F,&opcode_1);//Rd is only 4-bit!!!
	if (strcmp(tok,"com")==0) error = opcode_Rd(line,pos,0x9400,&opcode_1);		
	if (strcmp(tok,"neg")==0) error = opcode_Rd(line,pos,0x9401,&opcode_1);		
	//direct opcodes, no operands
	if (strcmp(tok,"clc")==0) opcode_1=0x9488;	
	if (strcmp(tok,"clh")==0) opcode_1=0x94D8;	
	if (strcmp(tok,"cli")==0) opcode_1=0x94F8;		
	if (strcmp(tok,"cln")==0) opcode_1=0x94A8;	
	if (strcmp(tok,"cls")==0) opcode_1=0x94C8;	
	if (strcmp(tok,"clt")==0) opcode_1=0x94E8;	
	if (strcmp(tok,"clv")==0) opcode_1=0x94B8;	
	if (strcmp(tok,"clz")==0) opcode_1=0x9498;	
	if (strcmp(tok,"sec")==0) opcode_1=0x9408;	
	if (strcmp(tok,"seh")==0) opcode_1=0x9458;	
	if (strcmp(tok,"sei")==0) opcode_1=0x9478;	
	if (strcmp(tok,"sen")==0) opcode_1=0x9428;	
	if (strcmp(tok,"ses")==0) opcode_1=0x9448;	
	if (strcmp(tok,"set")==0) opcode_1=0x9468;	
	if (strcmp(tok,"sev")==0) opcode_1=0x9438;	
	if (strcmp(tok,"sez")==0) opcode_1=0x9418;	
	if (strcmp(tok,"nop")==0) opcode_1=0x0000;	
	if (strcmp(tok,"sleep")==0) opcode_1=0x9588;	
	if (strcmp(tok,"ret")==0) opcode_1=0x9508;	
	if (strcmp(tok,"reti")==0) opcode_1=0x9518;	
	if (strcmp(tok,"spm")==0) opcode_1=0x95E8;	
	if (strcmp(tok,"ijmp")==0) opcode_1=0x9409;	
	if (strcmp(tok,"icall")==0) opcode_1=0x9509;	
	if (strcmp(tok,"break")==0) opcode_1=0x9598;	
	//+-63 branch
	if (strcmp(tok,"breq")==0) error = opcode_k7(line,pos,0xF001,&opcode_1);	
	if (strcmp(tok,"brge")==0) error = opcode_k7(line,pos,0xF404,&opcode_1);	
	if (strcmp(tok,"brhc")==0) error = opcode_k7(line,pos,0xF405,&opcode_1);	
	if (strcmp(tok,"brhs")==0) error = opcode_k7(line,pos,0xF005,&opcode_1);	
	if (strcmp(tok,"brid")==0) error = opcode_k7(line,pos,0xF407,&opcode_1);	
	if (strcmp(tok,"brie")==0) error = opcode_k7(line,pos,0xF007,&opcode_1);	
	if (strcmp(tok,"brlo")==0) error = opcode_k7(line,pos,0xF000,&opcode_1);	
	if (strcmp(tok,"brlt")==0) error = opcode_k7(line,pos,0xF004,&opcode_1);	
	if (strcmp(tok,"brmi")==0) error = opcode_k7(line,pos,0xF002,&opcode_1);	
	if (strcmp(tok,"brpl")==0) error = opcode_k7(line,pos,0xF402,&opcode_1);	
	if (strcmp(tok,"brsh")==0) error = opcode_k7(line,pos,0xF400,&opcode_1);	
	if (strcmp(tok,"brtc")==0) error = opcode_k7(line,pos,0xF406,&opcode_1);	
	if (strcmp(tok,"brts")==0) error = opcode_k7(line,pos,0xF006,&opcode_1);	
	if (strcmp(tok,"brvc")==0) error = opcode_k7(line,pos,0xF403,&opcode_1);	
	if (strcmp(tok,"brvs")==0) error = opcode_k7(line,pos,0xF003,&opcode_1);	
	if (strcmp(tok,"brcs")==0) error = opcode_k7(line,pos,0xF000,&opcode_1);	
	if (strcmp(tok,"brcc")==0) error = opcode_k7(line,pos,0xF400,&opcode_1);	
	//+-2048 branch
	if (strcmp(tok,"rjmp")==0) error = opcode_k12(line,pos,0xC000,&opcode_1);	
	if (strcmp(tok,"rcall")==0) error = opcode_k12(line,pos,0xD000,&opcode_1);	
	//2 operand instructions / Rd Rr
	if (strcmp(tok,"add")==0) error = opcode_Rd_Rr(line,pos,0x0C00,&opcode_1);	
	if (strcmp(tok,"adc")==0) error = opcode_Rd_Rr(line,pos,0x1C00,&opcode_1);	
	if (strcmp(tok,"and")==0) error = opcode_Rd_Rr(line,pos,0x2000,&opcode_1);	
	if (strcmp(tok,"eor")==0) error = opcode_Rd_Rr(line,pos,0x2400,&opcode_1);	
	if (strcmp(tok,"or")==0) error = opcode_Rd_Rr(line,pos,0x2800,&opcode_1);	
	if (strcmp(tok,"sbc")==0) error = opcode_Rd_Rr(line,pos,0x0800,&opcode_1);	
	if (strcmp(tok,"sub")==0) error = opcode_Rd_Rr(line,pos,0x1800,&opcode_1);	
	if (strcmp(tok,"mul")==0) error = opcode_Rd_Rr(line,pos,0x9C00,&opcode_1);	
	if (strcmp(tok,"cpse")==0) error = opcode_Rd_Rr(line,pos,0x1000,&opcode_1);	
	if (strcmp(tok,"cp")==0) error = opcode_Rd_Rr(line,pos,0x1400,&opcode_1);	
	if (strcmp(tok,"cpc")==0) error = opcode_Rd_Rr(line,pos,0x0400,&opcode_1);	
	if (strcmp(tok,"mov")==0) error = opcode_Rd_Rr(line,pos,0x2C00,&opcode_1);	
	//register immediate instructions Rd,k8
	if (strcmp(tok,"andi")==0) error = opcode_Rd_k8(line,pos,0x7000,&opcode_1,0);	
	if (strcmp(tok,"subi")==0) error = opcode_Rd_k8(line,pos,0x5000,&opcode_1,0);	
	if (strcmp(tok,"sbci")==0) error = opcode_Rd_k8(line,pos,0x4000,&opcode_1,0);	
	if (strcmp(tok,"ori")==0) error = opcode_Rd_k8(line,pos,0x6000,&opcode_1,0);	
	if (strcmp(tok,"cpi")==0) error = opcode_Rd_k8(line,pos,0x3000,&opcode_1,0);	
	if (strcmp(tok,"ldi")==0) error = opcode_Rd_k8(line,pos,0xE000,&opcode_1,0);	
	if (strcmp(tok,"sbr")==0) error = opcode_Rd_k8(line,pos,0x6000,&opcode_1,0);	//same as ori
	if (strcmp(tok,"cbr")==0) error = opcode_Rd_k8(line,pos,0x7000,&opcode_1,1);	//same as andi, K complemented
	// s
	if (strcmp(tok,"bclr")==0) error = opcode_s(line,pos,0x9488,&opcode_1);	
	if (strcmp(tok,"bset")==0) error = opcode_s(line,pos,0x9408,&opcode_1);	
	// in, out
	if (strcmp(tok,"in")==0) error = opcode_in(line,pos,&opcode_1);
	if (strcmp(tok,"out")==0) error = opcode_out(line,pos,&opcode_1);		
	//
}

//****************extract operands and modify opcode skeleton********************
char popcode_org (char *str, char actual)
{
unsigned int tmp;
unsigned int addr;
str1[0]=0;
actual = get_token(str,str1,actual);
if (strlen(str1)>0) addr = convert_to_16_number(str1);
else return K_ERR;
address = addr;
opcodes = 0;
//printf("--out:%s:%d %s:%d op:%X\n",str1,rd,str2,p,*opcode);
return OK;
}


char opcode_out (char *str, char actual, unsigned int * opcode)
{
unsigned int tmp;
unsigned char rd,p;
str1[0]=0;
str2[0]=0;
actual = get_token(str,str1,actual);
actual = get_token(str,str2,actual);
if (strlen(str1)>0) p = convert_to_8_number(str1);
else return P_ERR;
if (strlen(str2)>0) rd = convert_to_8_number(str2);
else return RD_ERR;
tmp = rd&0x1F;
tmp = tmp << 4;
tmp = tmp | (p&0x0F);
if (p&0x10) tmp = tmp & 0x0200;
if (p&0x20) tmp = tmp & 0x0400;
tmp = tmp | 0xB000;
*opcode = tmp;
//printf("--out:%s:%d %s:%d op:%X\n",str1,rd,str2,p,*opcode);
return OK;
}

char opcode_in (char *str, char actual, unsigned int * opcode)
{
unsigned int tmp;
unsigned char rd,p;
str1[0]=0;
str2[0]=0;
actual = get_token(str,str1,actual);
actual = get_token(str,str2,actual);
if (strlen(str1)>0) rd = convert_to_8_number(str1);
else return RD_ERR;
if (strlen(str2)>0) p = convert_to_8_number(str2);
else return P_ERR;
tmp = rd&0x1F;
tmp = tmp << 4;
tmp = tmp | (p&0x0F);
if (p&0x10) tmp = tmp & 0x0200;
if (p&0x20) tmp = tmp & 0x0400;
tmp = tmp | 0xB000;
*opcode = tmp;
//printf("--in:%s:%d %s:%d op:%X\n",str1,rd,str2,p,*opcode);
return OK;
}

char opcode_s (char *str, char actual, unsigned int skeleton, unsigned int * opcode)
{
unsigned int tmp;
unsigned char s;
str1[0]=0;
actual = get_token(str,str1,actual);
if (strlen(str1)>0) s = convert_to_8_number(str1);
else return S_ERR;
tmp = s&0x07;
tmp = tmp << 4;
*opcode = (skeleton | tmp);
//printf("--s:%s:%d op:%X\n",str1,s,*opcode);
return OK;
}

char opcode_p_b (char *str, char actual, unsigned int skeleton, unsigned int * opcode)
{
unsigned int tmp;
unsigned char p,b;
str1[0]=0;
str2[0]=0;
actual = get_token(str,str1,actual);
actual = get_token(str,str2,actual);
if (strlen(str1)>0) p = convert_to_8_number(str1);
else return P_ERR;
if (strlen(str2)>0) b = convert_to_8_number(str2);
else return B_ERR;
tmp = p;
tmp = tmp << 3;
*opcode = (skeleton | b | tmp);
//printf("--p_b:%s:%d %s:%d op:%X\n",str1,p,str2,b,*opcode);
return OK;
}

char opcode_Rd (char *str, char actual, unsigned int skeleton, unsigned int * opcode)
{
unsigned int tmp;
unsigned char rd;
str1[0]=0;
actual = get_token(str,str1,actual);
if (strlen(str1)>0) rd = convert_to_8_number(str1);
else return RD_ERR;
tmp = rd;
tmp = tmp << 4;
*opcode = (skeleton | tmp);
//printf("--Rd:%s:%d op:%X\n",str1,rd,*opcode);
return OK;
}

char opcode_Rd_Rr (char *str, char actual, unsigned int skeleton, unsigned int * opcode)
{
unsigned int tmp;
unsigned char rd,rr;
str1[0]=0;
str2[0]=0;
actual = get_token(str,str1,actual);
actual = get_token(str,str2,actual);
if (strlen(str1)>0) rd = convert_to_8_number(str1);
else return RD_ERR;
if (strlen(str2)>0) rr = convert_to_8_number(str2);
else return RR_ERR;
tmp = rd;
tmp = tmp << 4;
tmp = tmp | (rr&0x0F);
if (rr&0x10) tmp = tmp & 0x0200;
*opcode = (skeleton | tmp);
//printf("--Rd Rr:%s:%d %s:%d op:%X\n",str1,rd,str2,rr,*opcode);
return OK;
}

//Rd can be 0..15 or 16..32 higher bit is ignored
char opcode_Rd_k8 (char *str, char actual, unsigned int skeleton, unsigned int * opcode, unsigned char cpl)
{
unsigned int tmp;
unsigned char rd,k8;
str1[0]=0;
str2[0]=0;
actual = get_token(str,str1,actual);
actual = get_token(str,str2,actual);
if (strlen(str1)>0) rd = convert_to_8_number(str1);
else return RD_ERR;
if (strlen(str2)>0) k8 = convert_to_8_number(str2);
else return K8_ERR;
if (cpl!=0) k8 = ~k8;
tmp = k8;
tmp = (tmp << 4)&0x0F00;
tmp = tmp | (k8&0x0F);
tmp = tmp | ((rd&0x0F)<<4);
*opcode = (skeleton | tmp);
//printf("--Rd k8:%s:%d %s:%d op:%X\n",str1,rd,str2,k8,*opcode);
return OK;
}


char opcode_k7 (char *str, char actual, unsigned int skeleton, unsigned int * opcode)
{
unsigned int tmp;
unsigned char k7;
str1[0]=0;
actual = get_token(str,str1,actual);
if (strlen(str1)>0) k7 = convert_to_8_number(str1);
else return K7_ERR;
tmp = k7;
tmp = tmp << 3;
*opcode = (skeleton | tmp);
//printf("--k7:%s:%d op:%X\n",str1,k7,*opcode);
return OK;
}

char opcode_k12 (char *str, char actual, unsigned int skeleton, unsigned int * opcode)
{
unsigned int tmp;
unsigned int k12;
str1[0]=0;
actual = get_token(str,str1,actual);
if (strlen(str1)>0) k12 = convert_to_16_number(str1);
else return K12_ERR;
tmp = k12;
tmp = tmp << 3;
*opcode = (skeleton | tmp);
//printf("--k12:%s:%d op:%X\n",str1,k12,*opcode);
return OK;
}


//****************low level stuff********************

// Converts a hexadecimal string to integer
char xtoi_j(const char* xs, unsigned int* result)
{
 char szlen = strlen(xs);
 int xv, fact;
 char i;

 if (szlen > 0)
 {
  // Converting more than 32bit hexadecimal value?
  if (szlen>8) return 2; // exit

  // Begin conversion here
  *result = 0;
  fact = 1;

  // Run until no more character to convert
  for(i=szlen-1; i>=0 ;i--)
  {
   if (isxdigit(*(xs+i)))
   {
    if (*(xs+i)>=97)
    {
     xv = ( *(xs+i) - 97) + 10;
    }
    else if ( *(xs+i) >= 65)
    {
     xv = (*(xs+i) - 65) + 10;
    }
    else
    {
     xv = *(xs+i) - 48;
    }
    *result += (xv * fact);
    fact *= 16;
   }
   else
   {
    // Conversion was abnormally terminated
    // by non hexadecimal digit, hence
    // returning only the converted with
    // an error value 4 (illegal hex character)
    return 4;
   }
  }
 }

 // Nothing to convert
 return 1;
}


char get_token (char * source, char *dest, int actual)
{
unsigned char len,pos,i;
len = strlen((char*)source);
pos = actual;
i=0;
while (is_delimiter(source[pos])==1)
{
	if (pos<len)
	{
		pos++;
	}
	else return -1;
}

while (is_delimiter(source[pos])==0)
{
	if (pos<len)
	{
		dest[i] = source[pos];
		i++;
		pos++;
	}
	else return -1;
}
dest[i] = 0;
return pos;
}

char is_delimiter(unsigned char chr)
{
if ((chr<'!')|(chr==',')|(chr=='.')) return 1;
else return 0;
}

unsigned char convert_to_8_number (char * string)
{
unsigned char str_len,j,num;
unsigned int numi;
str_len = strlen(string);
num=0;
j=0;
if (str_len>0)
{
	if ((string[0]=='0')&(string[1]=='x'))
			{
			if (xtoi_j( string,&numi))
				return ((unsigned char) numi);
			else return 0;
			}
	else num = atoi( string);
	}
return num;
}

unsigned int convert_to_16_number (char * string)
{
unsigned char str_len,j;
unsigned int num;
str_len = strlen(string);
num=0;
j=0;
if (str_len>0)
{
	if ((string[0]=='0')&(string[1]=='x'))
			{
			if (xtoi_j( string,&num))
				return ( num);
			else return 0;
			}
	else num = atoi( string);
	}
return num;
}


