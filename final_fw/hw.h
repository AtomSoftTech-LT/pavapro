#include <avr/io.h>
#include <util/delay.h>
//#include "stdio.h"
#include "string.h"
#include <avr/pgmspace.h>

/*
t2313 - 64 pages of 16 words by 2 bytes
m328 - 256 pages of 64 words by 2 bytes


*/

#define    DISP_WIDTH  21
#define    DISP_ROWS  8
#define    DISP_SIZE  DISP_WIDTH*DISP_ROWS

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#ifndef HW_H
#define HW_H
typedef unsigned char	BYTE;
typedef unsigned long	DWORD;
typedef unsigned int	UINT;
typedef unsigned short	WORD;

typedef unsigned int	UINT16;
/*
typedef int				INT;
typedef unsigned int	UINT;

typedef char			CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

typedef short			SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

typedef long			LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;
*/
#define  DISP_PORT  PORTB
#define  DISP_DDR  DDRB
#define  DISP_RST  4

#define  NEW_LINE 0x0A

#define _BAUD	9600	// Baud rate (9600 is default)
#define _UBRR	(F_CPU/16)/_BAUD - 1	// Used for UBRRL and UBRRH 

#define SCL_CLOCK  100000 //100 kHz

#endif
