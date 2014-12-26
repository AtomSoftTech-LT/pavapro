#include <avr/io.h>
#include <util/delay.h>
#include "stdio.h"

#ifndef F_CPU
#define F_CPU 12000000UL
#endif

#ifndef HW_H
#define HW_H
typedef unsigned char	BYTE;
typedef unsigned long	DWORD;
typedef unsigned int	UINT;
typedef unsigned short	WORD;
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
//#define  DISP_SCK  1
//#define  DISP_DAT  2
#define  DISP_RST  0
//#define  DISP_DC   4
//#define  DISP_CS   3

#define _BAUD	9600	// Baud rate (9600 is default)
#define _UBRR	(F_CPU/16)/_BAUD - 1	// Used for UBRRL and UBRRH 

#define SCL_CLOCK  100000 //100 kHz

#endif
