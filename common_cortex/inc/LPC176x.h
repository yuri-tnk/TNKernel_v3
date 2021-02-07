#ifndef LPC176X_H
#define LPC176X_H

/******/

#define  rRSID      (*(volatile unsigned long *) 0x400FC180UL)
#define  rEXTINT    (*(volatile unsigned long *) 0x400FC140UL) //-- External Interrupt flag register
#define  rEXTMODE   (*(volatile unsigned long *) 0x400FC148UL) //-- External Interrupt Mode register

#define  rEXTPOLAR  (*(volatile unsigned long *) 0x400FC14CUL) //-- External Interrupt Polarity register

#define  rSCS       (*(volatile unsigned long *) 0x400FC1A0UL) //-- System Controls and Status register

#define  rCLKSRCSEL (*(volatile unsigned long *) 0x400FC10CUL) //-- Clock Source Select register
#define  rPLL0CON   (*(volatile unsigned long *) 0x400FC080UL) //-- PLL0 Control register
#define  rPLL0CFG   (*(volatile unsigned long *) 0x400FC084UL) //-- PLL0 Configuration register
#define  rPLL0STAT  (*(volatile unsigned long *) 0x400FC088UL) //-- PLL0 Status register
#define  rPLL0FEED  (*(volatile unsigned long *) 0x400FC08CUL) //-- PLL0 Feed register
#define  rPLL1CON   (*(volatile unsigned long *) 0x400FC0A0UL) //-- PLL1 Control register
#define  rPLL1CFG   (*(volatile unsigned long *) 0x400FC0A4UL) //-- PLL1 Configuration register
#define  rPLL1STAT  (*(volatile unsigned long *) 0x400FC0A8UL) //-- PLL1 Status register
#define  rPLL1FEED  (*(volatile unsigned long *) 0x400FC0ACUL) //-- PLL1 Feed register
#define  rCCLKCFG   (*(volatile unsigned long *) 0x400FC104UL) //-- CPU Clock Configuration register
#define  rUSBCLKCFG (*(volatile unsigned long *) 0x400FC108UL) //-- USB Clock Configuration register
#define  rIRCTRIM   (*(volatile unsigned long *) 0x400FC1A4UL) //-- IRC Trim Register
#define  rPCLKSEL0  (*(volatile unsigned long *) 0x400FC1A8UL) //-- Peripheral Clock Selection register 0
#define  rPCLKSEL1  (*(volatile unsigned long *) 0x400FC1ACUL) //-- Peripheral Clock Selection register 1
#define  rPCON      (*(volatile unsigned long *) 0x400FC0C0UL) //-- Power Control Register
#define  rPCONP     (*(volatile unsigned long *) 0x400FC0C4UL) //-- Power Control for Peripherals Register
#define  rCLKOUTCFG (*(volatile unsigned long *) 0x400FC1C8UL)

#define  rFLASHCFG  (*(volatile unsigned long *) 0x400FC000UL)

#define  rISER0     (*(volatile unsigned long *) 0xE000E100UL) //-- Interrupt Set-Enable Register 0 register
#define  rISER1     (*(volatile unsigned long *) 0xE000E104UL) //-- Interrupt Set-Enable Register 1 register
#define  rICER0     (*(volatile unsigned long *) 0xE000E180UL) //-- Interrupt Clear-Enable Register 0
#define  rICER1     (*(volatile unsigned long *) 0xE000E184UL) //-- Interrupt Clear-Enable Register 1 register
#define  rISPR0     (*(volatile unsigned long *) 0xE000E200UL) //-- Interrupt Set-Pending Register 0 register
#define  rISPR1     (*(volatile unsigned long *) 0xE000E204UL) //-- Interrupt Set-Pending Register 1 register
#define  rICPR0     (*(volatile unsigned long *) 0xE000E280UL) //-- Interrupt Clear-Pending Register 0 register
#define  rICPR1     (*(volatile unsigned long *) 0xE000E284UL) //-- Interrupt Clear-Pending Register 1 register
#define  rIABR0     (*(volatile unsigned long *) 0xE000E300UL) //-- Interrupt Active Bit Register 0
#define  rIABR1     (*(volatile unsigned long *) 0xE000E304UL) //-- Interrupt Active Bit Register 1
#define  rIPR0      (*(volatile unsigned long *) 0xE000E400UL) //-- Interrupt Priority Register 0
#define  rIPR1      (*(volatile unsigned long *) 0xE000E404UL) //-- Interrupt Priority Register 1
#define  rIPR2      (*(volatile unsigned long *) 0xE000E408UL) //-- Interrupt Priority Register 2
#define  rIPR3      (*(volatile unsigned long *) 0xE000E40CUL) //-- Interrupt Priority Register 3
#define  rIPR4      (*(volatile unsigned long *) 0xE000E410UL) //-- Interrupt Priority Register 4
#define  rIPR5      (*(volatile unsigned long *) 0xE000E414UL) //-- Interrupt Priority Register 5
#define  rIPR6      (*(volatile unsigned long *) 0xE000E418UL) //-- Interrupt Priority Register 6
#define  rIPR7      (*(volatile unsigned long *) 0xE000E41CUL) //-- Interrupt Priority Register 7
#define  rIPR8      (*(volatile unsigned long *) 0xE000E420UL) //-- Interrupt Priority Register 8
#define  rSTIR      (*(volatile unsigned long *) 0xE000EF00UL) //-- Software Trigger Interrupt Register
                                                           
#define  rPINSEL0   (*(volatile unsigned long *) 0x4002C000UL) //-- Pin Function Select register  0
#define  rPINSEL1   (*(volatile unsigned long *) 0x4002C004UL) //-- Pin Function Select Register  1
#define  rPINSEL2   (*(volatile unsigned long *) 0x4002C008UL) //-- Pin Function Select register  2
#define  rPINSEL3   (*(volatile unsigned long *) 0x4002C00CUL) //-- Pin Function Select Register  3
#define  rPINSEL4   (*(volatile unsigned long *) 0x4002C010UL) //-- Pin Function Select Register  4
#define  rPINSEL7   (*(volatile unsigned long *) 0x4002C01CUL) //-- Pin Function Select Register  7
#define  rPINSEL9   (*(volatile unsigned long *) 0x4002C024UL) //-- Pin Function Select Register  9
#define  rPINSEL10  (*(volatile unsigned long *) 0x4002C028UL) //-- Pin Function Select Register 10
                                                           
#define  rPINMODE0  (*(volatile unsigned long *) 0x4002C040UL) //-- Pin Mode select register 0
#define  rPINMODE1  (*(volatile unsigned long *) 0x4002C044UL) //-- Pin Mode select register 1
#define  rPINMODE2  (*(volatile unsigned long *) 0x4002C048UL) //-- Pin Mode select register 2
#define  rPINMODE3  (*(volatile unsigned long *) 0x4002C04CUL) //-- Pin Mode select register 3
#define  rPINMODE4  (*(volatile unsigned long *) 0x4002C050UL) //-- Pin Mode select register 4
#define  rPINMODE7  (*(volatile unsigned long *) 0x4002C05CUL) //-- Pin Mode select register 7
#define  rPINMODE9  (*(volatile unsigned long *) 0x4002C064UL) //-- Pin Mode select register 9

#define  rPINMODE_OD0  (*(volatile unsigned long *) 0x4002C068UL) //-- Open Drain Pin Mode select register 0
#define  rPINMODE_OD1  (*(volatile unsigned long *) 0x4002C06CUL) //-- Open Drain Pin Mode select register 1
#define  rPINMODE_OD2  (*(volatile unsigned long *) 0x4002C070UL) //-- Open Drain Pin Mode select register 2
#define  rPINMODE_OD3  (*(volatile unsigned long *) 0x4002C074UL) //-- Open Drain Pin Mode select register 3
#define  rPINMODE_OD4  (*(volatile unsigned long *) 0x4002C078UL) //-- Open Drain Pin Mode select register 4
                                                              
#define  rI2CPADCFG  (*(volatile unsigned long *)   0x4002C07CUL) //-- I2C Pin Configuration register
                                                              
//----  GPIO port Direction registers                         
                                                              
#define  rFIO0DIR   (*(volatile unsigned long *)  0x2009C000UL) 
#define  rFIO0DIR0  (*(volatile unsigned char *)  0x2009C000UL) 
#define  rFIO0DIR1  (*(volatile unsigned char *)  0x2009C001UL)
#define  rFIO0DIR2  (*(volatile unsigned char *)  0x2009C002UL)
#define  rFIO0DIR3  (*(volatile unsigned char *)  0x2009C003UL)
#define  rFIO0DIRL  (*(volatile unsigned short *) 0x2009C000UL)
#define  rFIO0DIRU  (*(volatile unsigned short *) 0x2009C002UL)
#define  rFIO0DIRH  rFIO0DIRU

#define  rFIO1DIR   (*(volatile unsigned long *)  0x2009C020UL)
#define  rFIO1DIR0  (*(volatile unsigned char *)  0x2009C020UL)
#define  rFIO1DIR1  (*(volatile unsigned char *)  0x2009C021UL)
#define  rFIO1DIR2  (*(volatile unsigned char *)  0x2009C022UL)
#define  rFIO1DIR3  (*(volatile unsigned char *)  0x2009C023UL)
#define  rFIO1DIRL  (*(volatile unsigned short *) 0x2009C020UL)
#define  rFIO1DIRU  (*(volatile unsigned short *) 0x2009C022UL)
#define  rFIO1DIRH  rFIO1DIRU

#define  rFIO2DIR   (*(volatile unsigned long *)  0x2009C040UL)
#define  rFIO2DIR0  (*(volatile unsigned char *)  0x2009C040UL)
#define  rFIO2DIR1  (*(volatile unsigned char *)  0x2009C041UL)
#define  rFIO2DIR2  (*(volatile unsigned char *)  0x2009C042UL)
#define  rFIO2DIR3  (*(volatile unsigned char *)  0x2009C043UL)
#define  rFIO2DIRL  (*(volatile unsigned short *) 0x2009C040UL)
#define  rFIO2DIRU  (*(volatile unsigned short *) 0x2009C042UL)
#define  rFIO2DIRH  rFIO2DIRU

#define  rFIO3DIR   (*(volatile unsigned long *)  0x2009C060UL)
#define  rFIO3DIR0  (*(volatile unsigned char *)  0x2009C060UL)
#define  rFIO3DIR1  (*(volatile unsigned char *)  0x2009C061UL)
#define  rFIO3DIR2  (*(volatile unsigned char *)  0x2009C062UL)
#define  rFIO3DIR3  (*(volatile unsigned char *)  0x2009C063UL)
#define  rFIO3DIRL  (*(volatile unsigned short *) 0x2009C060UL)
#define  rFIO3DIRU  (*(volatile unsigned short *) 0x2009C062UL)
#define  rFIO3DIRH  rFIO3DIRU

#define  rFIO4DIR   (*(volatile unsigned long *)  0x2009C080UL)
#define  rFIO4DIR0  (*(volatile unsigned char *)  0x2009C080UL)
#define  rFIO4DIR1  (*(volatile unsigned char *)  0x2009C081UL)
#define  rFIO4DIR2  (*(volatile unsigned char *)  0x2009C082UL)
#define  rFIO4DIR3  (*(volatile unsigned char *)  0x2009C083UL)
#define  rFIO4DIRL  (*(volatile unsigned short *) 0x2009C080UL)
#define  rFIO4DIRU  (*(volatile unsigned short *) 0x2009C082UL)
#define  rFIO4DIRH  rFIO4DIRU

//---- Fast GPIO port Mask register

#define 	rFIO0MASK   (*(volatile unsigned long *)  0x2009C010UL)
#define 	rFIO0MASK0  (*(volatile unsigned char *)  0x2009C010UL)
#define 	rFIO0MASK1  (*(volatile unsigned char *)  0x2009C011UL)
#define 	rFIO0MASK2  (*(volatile unsigned char *)  0x2009C012UL)
#define 	rFIO0MASK3  (*(volatile unsigned char *)  0x2009C013UL)
#define 	rFIO0MASKL  (*(volatile unsigned short *) 0x2009C010UL)
#define 	rFIO0MASKU  (*(volatile unsigned short *) 0x2009C012UL)
#define 	rFIO0MASKH  rFIO0MASKU

#define 	rFIO1MASK   (*(volatile unsigned long *)  0x2009C030UL)
#define 	rFIO1MASK0  (*(volatile unsigned char *)  0x2009C030UL)
#define 	rFIO1MASK1  (*(volatile unsigned char *)  0x2009C031UL)
#define 	rFIO1MASK2  (*(volatile unsigned char *)  0x2009C032UL)
#define 	rFIO1MASK3  (*(volatile unsigned char *)  0x2009C033UL)
#define 	rFIO1MASKL  (*(volatile unsigned short *) 0x2009C030UL)
#define 	rFIO1MASKU  (*(volatile unsigned short *) 0x2009C032UL)
#define 	rFIO1MASKH  rFIO1MASKU

#define 	rFIO2MASK   (*(volatile unsigned long *)  0x2009C050UL)
#define 	rFIO2MASK0  (*(volatile unsigned char *)  0x2009C050UL)
#define 	rFIO2MASK1  (*(volatile unsigned char *)  0x2009C051UL)
#define 	rFIO2MASK2  (*(volatile unsigned char *)  0x2009C052UL)
#define 	rFIO2MASK3  (*(volatile unsigned char *)  0x2009C053UL)
#define 	rFIO2MASKL  (*(volatile unsigned short *) 0x2009C050UL)
#define 	rFIO2MASKU  (*(volatile unsigned short *) 0x2009C052UL)
#define 	rFIO2MASKH  rFIO2MASKU

#define 	rFIO3MASK   (*(volatile unsigned long *)  0x2009C070UL)
#define 	rFIO3MASK0  (*(volatile unsigned char *)  0x2009C070UL)
#define 	rFIO3MASK1  (*(volatile unsigned char *)  0x2009C071UL)
#define 	rFIO3MASK2  (*(volatile unsigned char *)  0x2009C072UL)
#define 	rFIO3MASK3  (*(volatile unsigned char *)  0x2009C073UL)
#define 	rFIO3MASKL  (*(volatile unsigned short *) 0x2009C070UL)
#define 	rFIO3MASKU  (*(volatile unsigned short *) 0x2009C072UL)
#define 	rFIO3MASKH  rFIO3MASKU

#define 	rFIO4MASK   (*(volatile unsigned long *)  0x2009C090UL)
#define 	rFIO4MASK0  (*(volatile unsigned char *)  0x2009C090UL)
#define 	rFIO4MASK1  (*(volatile unsigned char *)  0x2009C091UL)
#define 	rFIO4MASK2  (*(volatile unsigned char *)  0x2009C092UL)
#define 	rFIO4MASK3  (*(volatile unsigned char *)  0x2009C093UL)
#define 	rFIO4MASKL  (*(volatile unsigned short *) 0x2009C090UL)
#define 	rFIO4MASKU  (*(volatile unsigned short *) 0x2009C092UL)
#define 	rFIO4MASKH  rFIO4MASKU

//---- GPIO port Pin value register

#define  rFIO0PIN   (*(volatile unsigned long *)  0x2009C014UL)
#define  rFIO0PIN0  (*(volatile unsigned char *)  0x2009C014UL)
#define  rFIO0PIN1  (*(volatile unsigned char *)  0x2009C015UL)
#define  rFIO0PIN2  (*(volatile unsigned char *)  0x2009C016UL)
#define  rFIO0PIN3  (*(volatile unsigned char *)  0x2009C017UL)
#define  rFIO0PINL  (*(volatile unsigned short *) 0x2009C014UL)
#define  rFIO0PINU  (*(volatile unsigned short *) 0x2009C016UL)
#define  rFIO0PINH  rFIO0PINU

#define  rFIO1PIN   (*(volatile unsigned long *)  0x2009C034UL)
#define  rFIO1PIN0  (*(volatile unsigned char *)  0x2009C034UL)
#define  rFIO1PIN1  (*(volatile unsigned char *)  0x2009C035UL)
#define  rFIO1PIN2  (*(volatile unsigned char *)  0x2009C036UL)
#define  rFIO1PIN3  (*(volatile unsigned char *)  0x2009C037UL)
#define  rFIO1PINL  (*(volatile unsigned short *) 0x2009C034UL)
#define  rFIO1PINU  (*(volatile unsigned short *) 0x2009C036UL)
#define  rFIO1PINH  rFIO1PINU

#define  rFIO2PIN   (*(volatile unsigned long *)  0x2009C054UL)
#define  rFIO2PIN0  (*(volatile unsigned char *)  0x2009C054UL)
#define  rFIO2PIN1  (*(volatile unsigned char *)  0x2009C055UL)
#define  rFIO2PIN2  (*(volatile unsigned char *)  0x2009C056UL)
#define  rFIO2PIN3  (*(volatile unsigned char *)  0x2009C057UL)
#define  rFIO2PINL  (*(volatile unsigned short *) 0x2009C054UL)
#define  rFIO2PINU  (*(volatile unsigned short *) 0x2009C056UL)
#define  rFIO2PINH  rFIO2PINU

#define  rFIO3PIN   (*(volatile unsigned long *)  0x2009C074UL)
#define  rFIO3PIN0  (*(volatile unsigned char *)  0x2009C074UL)
#define  rFIO3PIN1  (*(volatile unsigned char *)  0x2009C075UL)
#define  rFIO3PIN2  (*(volatile unsigned char *)  0x2009C076UL)
#define  rFIO3PIN3  (*(volatile unsigned char *)  0x2009C077UL)
#define  rFIO3PINL  (*(volatile unsigned short *) 0x2009C074UL)
#define  rFIO3PINU  (*(volatile unsigned short *) 0x2009C076UL)
#define  rFIO3PINH  rFIO3PINU

#define  rFIO4PIN   (*(volatile unsigned long *)  0x2009C094UL)
#define  rFIO4PIN0  (*(volatile unsigned char *)  0x2009C094UL)
#define  rFIO4PIN1  (*(volatile unsigned char *)  0x2009C095UL)
#define  rFIO4PIN2  (*(volatile unsigned char *)  0x2009C096UL)
#define  rFIO4PIN3  (*(volatile unsigned char *)  0x2009C097UL)
#define  rFIO4PINL  (*(volatile unsigned short *) 0x2009C094UL)
#define  rFIO4PINU  (*(volatile unsigned short *) 0x2009C096UL)
#define  rFIO4PINH  rFIO4PINU

//---- GPIO port output Set registers

#define  rFIO0SET   (*(volatile unsigned long *)  0x2009C018UL)
#define  rFIO0SET0  (*(volatile unsigned char *)  0x2009C018UL)
#define  rFIO0SET1  (*(volatile unsigned char *)  0x2009C019UL)
#define  rFIO0SET2  (*(volatile unsigned char *)  0x2009C01AUL)
#define  rFIO0SET3  (*(volatile unsigned char *)  0x2009C01BUL)
#define  rFIO0SETL  (*(volatile unsigned short *) 0x2009C018UL)
#define  rFIO0SETU  (*(volatile unsigned short *) 0x2009C01AUL)
#define  rFIO0SETH  rFIO0SETU

#define  rFIO1SET   (*(volatile unsigned long *)  0x2009C038UL)
#define  rFIO1SET0  (*(volatile unsigned char *)  0x2009C038UL)
#define  rFIO1SET1  (*(volatile unsigned char *)  0x2009C039UL)
#define  rFIO1SET2  (*(volatile unsigned char *)  0x2009C03AUL)
#define  rFIO1SET3  (*(volatile unsigned char *)  0x2009C03BUL)
#define  rFIO1SETL  (*(volatile unsigned short *) 0x2009C038UL)
#define  rFIO1SETU  (*(volatile unsigned short *) 0x2009C03AUL)
#define  rFIO1SETH  rFIO1SETU

#define  rFIO2SET   (*(volatile unsigned long *)  0x2009C058UL)
#define  rFIO2SET0  (*(volatile unsigned char *)  0x2009C058UL)
#define  rFIO2SET1  (*(volatile unsigned char *)  0x2009C059UL)
#define  rFIO2SET2  (*(volatile unsigned char *)  0x2009C05AUL)
#define  rFIO2SET3  (*(volatile unsigned char *)  0x2009C05BUL)
#define  rFIO2SETL  (*(volatile unsigned short *) 0x2009C058UL)
#define  rFIO2SETU  (*(volatile unsigned short *) 0x2009C05AUL)
#define  rFIO2SETH  rFIO2SETU

#define  rFIO3SET   (*(volatile unsigned long *)  0x2009C078UL)
#define  rFIO3SET0  (*(volatile unsigned char *)  0x2009C078UL)
#define  rFIO3SET1  (*(volatile unsigned char *)  0x2009C079UL)
#define  rFIO3SET2  (*(volatile unsigned char *)  0x2009C07AUL)
#define  rFIO3SET3  (*(volatile unsigned char *)  0x2009C07BUL)
#define  rFIO3SETL  (*(volatile unsigned short *) 0x2009C078UL)
#define  rFIO3SETU  (*(volatile unsigned short *) 0x2009C07AUL)
#define  rFIO3SETH  rFIO3SETU

#define  rFIO4SET   (*(volatile unsigned long *)  0x2009C098UL)
#define  rFIO4SET0  (*(volatile unsigned char *)  0x2009C098UL)
#define  rFIO4SET1  (*(volatile unsigned char *)  0x2009C099UL)
#define  rFIO4SET2  (*(volatile unsigned char *)  0x2009C09AUL)
#define  rFIO4SET3  (*(volatile unsigned char *)  0x2009C09BUL)
#define  rFIO4SETL  (*(volatile unsigned short *) 0x2009C098UL)
#define  rFIO4SETU  (*(volatile unsigned short *) 0x2009C09AUL)
#define  rFIO4SETH  rFIO4SETU

//---- GPIO port output Clear register

#define  rFIO0CLR   (*(volatile unsigned long *)  0x2009C01CUL)
#define  rFIO0CLR0  (*(volatile unsigned char *)  0x2009C01CUL)
#define  rFIO0CLR1  (*(volatile unsigned char *)  0x2009C01DUL)
#define  rFIO0CLR2  (*(volatile unsigned char *)  0x2009C01EUL)
#define  rFIO0CLR3  (*(volatile unsigned char *)  0x2009C01FUL)
#define  rFIO0CLRL  (*(volatile unsigned short *) 0x2009C01CUL)
#define  rFIO0CLRU  (*(volatile unsigned short *) 0x2009C01EUL)
#define  rFIO0CLRH  rFIO0CLRU

#define  rFIO1CLR   (*(volatile unsigned long *)  0x2009C03CUL)
#define  rFIO1CLR0  (*(volatile unsigned char *)  0x2009C03CUL)
#define  rFIO1CLR1  (*(volatile unsigned char *)  0x2009C03DUL)
#define  rFIO1CLR2  (*(volatile unsigned char *)  0x2009C03EUL)
#define  rFIO1CLR3  (*(volatile unsigned char *)  0x2009C03FUL)
#define  rFIO1CLRL  (*(volatile unsigned short *) 0x2009C03CUL)
#define  rFIO1CLRU  (*(volatile unsigned short *) 0x2009C03EUL)
#define  rFIO1CLRH  rFIO1CLRU

#define  rFIO2CLR   (*(volatile unsigned long *)  0x2009C05CUL)
#define  rFIO2CLR0  (*(volatile unsigned char *)  0x2009C05CUL)
#define  rFIO2CLR1  (*(volatile unsigned char *)  0x2009C05DUL)
#define  rFIO2CLR2  (*(volatile unsigned char *)  0x2009C05EUL)
#define  rFIO2CLR3  (*(volatile unsigned char *)  0x2009C05FUL)
#define  rFIO2CLRL  (*(volatile unsigned short *) 0x2009C05CUL)
#define  rFIO2CLRU  (*(volatile unsigned short *) 0x2009C05EUL)
#define  rFIO2CLRH  rFIO2CLRU

#define  rFIO3CLR   (*(volatile unsigned long *)  0x2009C07CUL)
#define  rFIO3CLR0  (*(volatile unsigned char *)  0x2009C07CUL)
#define  rFIO3CLR1  (*(volatile unsigned char *)  0x2009C07DUL)
#define  rFIO3CLR2  (*(volatile unsigned char *)  0x2009C07EUL)
#define  rFIO3CLR3  (*(volatile unsigned char *)  0x2009C07FUL)
#define  rFIO3CLRL  (*(volatile unsigned short *) 0x2009C07CUL)
#define  rFIO3CLRU  (*(volatile unsigned short *) 0x2009C07EUL)
#define  rFIO3CLRH  rFIO3CLRU

#define  rFIO4CLR   (*(volatile unsigned long *)  0x2009C09CUL)
#define  rFIO4CLR0  (*(volatile unsigned char *)  0x2009C09CUL)
#define  rFIO4CLR1  (*(volatile unsigned char *)  0x2009C09DUL)
#define  rFIO4CLR2  (*(volatile unsigned char *)  0x2009C09EUL)
#define  rFIO4CLR3  (*(volatile unsigned char *)  0x2009C09FUL)
#define  rFIO4CLRL  (*(volatile unsigned short *) 0x2009C09CUL)
#define  rFIO4CLRU  (*(volatile unsigned short *) 0x2009C09EUL)
#define  rFIO4CLRH  rFIO4CLRU

//---- GPIO Interrupt

#define 	rIOIntStatus  (*(volatile unsigned long *)  0x40028080UL) //-- GPIO overall Interrupt Status register
#define 	rIO0IntEnR    (*(volatile unsigned long *)  0x40028090UL) //-- GPIO Interrupt Enable for port 0 Rising Edge
#define 	rIO2IntEnR    (*(volatile unsigned long *)  0x400280B0UL) //-- GPIO Interrupt Enable for port 2 Rising Edge
#define 	rIO0IntEnF    (*(volatile unsigned long *)  0x40028094UL) //-- GPIO Interrupt Enable for port 0 Falling Edge
#define 	rIO2IntEnF    (*(volatile unsigned long *)  0x400280B4UL) //-- GPIO Interrupt Enable for port 2 Falling Edge
#define 	rIO0IntStatR  (*(volatile unsigned long *)  0x40028084UL) //-- GPIO Interrupt Status for port 0 Rising Edge Interrupt
#define 	rIO2IntStatR  (*(volatile unsigned long *)  0x400280A4UL) //-- GPIO Interrupt Status for port 2 Rising Edge Interrupt
#define 	rIO0IntStatF  (*(volatile unsigned long *)  0x40028088UL) //-- GPIO Interrupt Status for port 0 Falling Edge Interrupt
#define 	rIO2IntStatF  (*(volatile unsigned long *)  0x400280A8UL) //-- GPIO Interrupt Status for port 2 Falling Edge Interrupt
#define 	rIO0IntClr    (*(volatile unsigned long *)  0x4002808CUL) //-- GPIO Interrupt Clear register for port 0
#define 	rIO2IntClr    (*(volatile unsigned long *)  0x400280ACUL) //-- GPIO Interrupt Clear register for port 0
                                                               
//----  Ethernet MAC                                           
                                                               
#define 	rMAC1         (*(volatile unsigned long *)  0x50000000UL) //-- MAC Configuration Register 1
#define 	rMAC2         (*(volatile unsigned long *)  0x50000004UL) //-- MAC Configuration Register 2
#define 	rIPGT         (*(volatile unsigned long *)  0x50000008UL) //-- Back-to-Back Inter-Packet-Gap Register
#define 	rIPGR         (*(volatile unsigned long *)  0x5000000CUL) //-- Non Back-to-Back Inter-Packet-Gap Register
#define 	rCLRT         (*(volatile unsigned long *)  0x50000010UL) //-- Collision Window / Retry Register
#define 	rMAXF         (*(volatile unsigned long *)  0x50000014UL) //-- Maximum Frame Register
#define 	rSUPP         (*(volatile unsigned long *)  0x50000018UL) //-- PHY Support Register
#define 	rTEST         (*(volatile unsigned long *)  0x5000001CUL) //-- Test Register
#define 	rMCFG         (*(volatile unsigned long *)  0x50000020UL) //-- MII Mgmt Configuration Register
#define 	rMCMD         (*(volatile unsigned long *)  0x50000024UL) //-- MII Mgmt Command Register
#define 	rMADR         (*(volatile unsigned long *)  0x50000028UL) //-- MII Mgmt Address Register
#define 	rMWTD         (*(volatile unsigned long *)  0x5000002CUL) //-- MII Mgmt Write Data Register
#define 	rMRDD         (*(volatile unsigned long *)  0x50000030UL) //-- MII Mgmt Read Data Register
#define 	rMIND         (*(volatile unsigned long *)  0x50000034UL) //-- MII Mgmt Indicators Register
#define 	rSA0          (*(volatile unsigned long *)  0x50000040UL) //-- Station Address 0 Register
#define 	rSA1          (*(volatile unsigned long *)  0x50000044UL) //-- Station Address 1 Register
#define 	rSA2          (*(volatile unsigned long *)  0x50000048UL) //-- Station Address 2 Register

#define 	rCommand             (*(volatile unsigned long *)  0x50000100UL) //-- Command Register
#define 	rStatus              (*(volatile unsigned long *)  0x50000104UL) //-- Status Register
#define 	rRxDescriptor        (*(volatile unsigned long *)  0x50000108UL) //-- Receive Descriptor Base Address Register
#define 	rRxStatus            (*(volatile unsigned long *)  0x5000010CUL) //-- Receive Status Base Address Register
#define 	rRxDescriptorNumber  (*(volatile unsigned long *)  0x50000110UL) //-- Receive Number of Descriptors Register
#define 	rRxProduceIndex      (*(volatile unsigned long *)  0x50000114UL) //-- Receive Produce Index Register
#define 	rRxConsumeIndex      (*(volatile unsigned long *)  0x50000118UL) //-- Receive Consume Index Register
#define 	rTxDescriptor        (*(volatile unsigned long *)  0x5000011CUL) //-- Transmit Descriptor Base Address Register
#define 	rTxStatus            (*(volatile unsigned long *)  0x50000120UL) //-- Transmit Status Base Address Register
#define 	rTxDescriptorNumber  (*(volatile unsigned long *)  0x50000124UL) //-- Transmit Number of Descriptors Register
#define 	rTxProduceIndex      (*(volatile unsigned long *)  0x50000128UL) //-- Transmit Produce Index Register
#define 	rTxConsumeIndex      (*(volatile unsigned long *)  0x5000012CUL) //-- Transmit Consume Index Register
#define 	rTSV0                (*(volatile unsigned long *)  0x50000158UL) //-- Transmit Status Vector 0 Register
#define 	rTSV1                (*(volatile unsigned long *)  0x5000015CUL) //-- Transmit Status Vector 1 Register
#define 	rRSV                 (*(volatile unsigned long *)  0x50000160UL) //-- Receive Status Vector Register
#define 	rFlowControlCounter  (*(volatile unsigned long *)  0x50000170UL) //-- Flow Control Counter Register
#define 	rFlowControlStatus   (*(volatile unsigned long *)  0x50000174UL) //-- Flow Control Status Register
#define 	rRxFilterCtrl        (*(volatile unsigned long *)  0x50000200UL) //-- Receive Filter Control Register
#define 	rRxFilterWoLStatus   (*(volatile unsigned long *)  0x50000204UL) //-- Receive Filter WoL Status Register
#define 	rRxFilterWoLClear    (*(volatile unsigned long *)  0x50000208UL) //-- Receive Filter WoL Clear Register
#define 	rHashFilterL         (*(volatile unsigned long *)  0x50000210UL) //-- Hash Filter Table LSBs Register
#define 	rHashFilterH         (*(volatile unsigned long *)  0x50000214UL) //-- Hash Filter Table MSBs Register
#define 	rIntStatus           (*(volatile unsigned long *)  0x50000FE0UL) //-- Interrupt Status Register
#define 	rIntEnable           (*(volatile unsigned long *)  0x50000FE4UL) //-- Interrupt Enable Register
#define 	rIntClear            (*(volatile unsigned long *)  0x50000FE8UL) //-- Interrupt Clear Register
#define 	rIntSet              (*(volatile unsigned long *)  0x50000FECUL) //-- Interrupt Set Register
#define 	rPowerDown           (*(volatile unsigned long *)  0x50000FF4UL) //-- Power-Down Register
                                                                      
//----  USB

#define 	rUSBClkCtrl       (*(volatile unsigned long *)  0x5000CFF4UL) //-- USB Clock Control register
#define 	rUSBClkSt         (*(volatile unsigned long *)  0x5000CFF8UL) //-- USB Clock Status register
#define 	rUSBIntSt         (*(volatile unsigned long *)  0x5000C1C0UL) //-- USB Interrupt Status register
#define 	rUSBDevIntSt      (*(volatile unsigned long *)  0x5000C200UL) //-- USB Device Interrupt Status register
#define 	rUSBDevIntEn      (*(volatile unsigned long *)  0x5000C204UL) //-- USB Device Interrupt Enable register
#define 	rUSBDevIntClr     (*(volatile unsigned long *)  0x5000C208UL) //-- USB Device Interrupt Clear register
#define 	rUSBDevIntSet     (*(volatile unsigned long *)  0x5000C20CUL) //-- USB Device Interrupt Set register
#define 	rUSBDevIntPri     (*(volatile unsigned long *)  0x5000C22CUL) //-- USB Device Interrupt Priority register
#define 	rUSBEpIntSt       (*(volatile unsigned long *)  0x5000C230UL) //-- USB Endpoint Interrupt Status register
#define 	rUSBEpIntEn       (*(volatile unsigned long *)  0x5000C234UL) //-- USB Endpoint Interrupt Enable register
#define 	rUSBEpIntClr      (*(volatile unsigned long *)  0x5000C238UL) //-- USB Endpoint Interrupt Clear register
#define 	rUSBEpIntSet      (*(volatile unsigned long *)  0x5000C23CUL) //-- USB Endpoint Interrupt Set register
#define 	rUSBEpIntPri      (*(volatile unsigned long *)  0x5000C240UL) //-- USB Endpoint Interrupt Priority register
#define 	rUSBReEp          (*(volatile unsigned long *)  0x5000C244UL) //-- USB Realize Endpoint register
#define 	rUSBEpIn          (*(volatile unsigned long *)  0x5000C248UL) //-- USB Endpoint Index register
#define 	rUSBMaxPSize      (*(volatile unsigned long *)  0x5000C24CUL) //-- USB MaxPacketSize register
#define 	rUSBRxData        (*(volatile unsigned long *)  0x5000C218UL) //-- USB Receive Data register
#define 	rUSBRxPLen        (*(volatile unsigned long *)  0x5000C220UL) //-- USB Receive Packet Length register
#define 	rUSBTxData        (*(volatile unsigned long *)  0x5000C21CUL) //-- USB Transmit Data register
#define 	rUSBTxPLen        (*(volatile unsigned long *)  0x5000C224UL) //-- USB Transmit Packet Length register
#define 	rUSBCtrl          (*(volatile unsigned long *)  0x5000C228UL) //-- USB Control register
#define 	rUSBCmdCode       (*(volatile unsigned long *)  0x5000C210UL) //-- USB Command Code register
#define 	rUSBCmdData       (*(volatile unsigned long *)  0x5000C214UL) //-- USB Command Data register
#define 	rUSBDMARSt        (*(volatile unsigned long *)  0x5000C250UL) //-- USB DMA Request Status register
#define 	rUSBDMARClr       (*(volatile unsigned long *)  0x5000C254UL) //-- USB DMA Request Clear register
#define 	rUSBDMARSet       (*(volatile unsigned long *)  0x5000C258UL) //-- USB DMA Request Set register
#define 	rUSBUDCAH         (*(volatile unsigned long *)  0x5000C280UL) //-- USB UDCA Head register
#define 	rUSBEpDMASt       (*(volatile unsigned long *)  0x5000C284UL) //-- USB EP DMA Status register
#define 	rUSBEpDMAEn       (*(volatile unsigned long *)  0x5000C288UL) //-- USB EP DMA Enable register
#define 	rUSBEpDMADis      (*(volatile unsigned long *)  0x5000C28CUL) //-- USB EP DMA Disable register
#define 	rUSBDMAIntSt      (*(volatile unsigned long *)  0x5000C290UL) //-- USB DMA Interrupt Status register
#define 	rUSBDMAIntEn      (*(volatile unsigned long *)  0x5000C294UL) //-- USB DMA Interrupt Enable register
#define 	rUSBEoTIntSt      (*(volatile unsigned long *)  0x5000C2A0UL) //-- USB End of Transfer Interrupt Status register
#define 	rUSBEoTIntClr     (*(volatile unsigned long *)  0x5000C2A4UL) //-- USB End of Transfer Interrupt Clear register
#define 	rUSBEoTIntSet     (*(volatile unsigned long *)  0x5000C2A8UL) //-- USB End of Transfer Interrupt Set register
#define 	rUSBNDDRIntSt     (*(volatile unsigned long *)  0x5000C2ACUL) //-- USB New DD Request Interrupt Status register
#define 	rUSBNDDRIntClr    (*(volatile unsigned long *)  0x5000C2B0UL) //-- USB New DD Request Interrupt Clear register
#define 	rUSBNDDRIntSet    (*(volatile unsigned long *)  0x5000C2B4UL) //-- USB New DD Request Interrupt Set register
#define 	rUSBSysErrIntSt   (*(volatile unsigned long *)  0x5000C2B8UL) //-- USB System Error Interrupt Status register
#define 	rUSBSysErrIntClr  (*(volatile unsigned long *)  0x5000C2BCUL) //-- USB System Error Interrupt Clear register
#define 	rUSBSysErrIntSet  (*(volatile unsigned long *)  0x5000C2C0UL) //-- USB System Error Interrupt Set register
                                                                   
#define 	rOTGIntSt    (*(volatile unsigned long *)  0x5000C100UL) //-- OTG Interrupt Status Register
#define 	rOTGIntEn    (*(volatile unsigned long *)  0x5000C104UL) //-- OTG Interrupt Enable Register
//--- !!!!                                                    
#define 	rOTGIntSet   (*(volatile unsigned long *)  0x5000C108UL) //-- OTG Interrupt Set Register
//--------------                                              
#define 	rOTGIntClr   (*(volatile unsigned long *)  0x5000C10CUL) //-- OTG Interrupt Clear Register
#define 	rOTGStCtrl   (*(volatile unsigned long *)  0x5000C110UL) //-- OTG Status and Control Register
#define 	rOTGTmr      (*(volatile unsigned long *)  0x5000C114UL) //-- OTG Timer Register
#define 	rOTGClkCtrl  (*(volatile unsigned long *)  0x5000CFF4UL) //-- OTG Clock Control Register
#define 	rOTGClkSt    (*(volatile unsigned long *)  0x5000CFF8UL) //-- OTG Clock Status Register
#define 	rI2C_RX      (*(volatile unsigned long *)  0x5000C300UL) //-- I2C Receive Register
#define 	rI2C_TX      (*(volatile unsigned long *)  0x5000C300UL) //-- I2C Transmit Register
#define 	rI2C_STS     (*(volatile unsigned long *)  0x5000C304UL) //-- I2C Status Register
#define 	rI2C_CTL     (*(volatile unsigned long *)  0x5000C308UL) //-- I2C Control Register
#define 	rI2C_CLKHI   (*(volatile unsigned long *)  0x5000C30CUL) //-- I2C Clock High Register
#define 	rI2C_CLKLO   (*(volatile unsigned long *)  0x5000C310UL) //-- I2C Clock Low Register

//----  UART0, UART2, UART3

#define 	rU0RBR      (*(volatile unsigned long *)  0x4000C000UL) //-- UARTn Receiver Buffer Register
#define 	rU0THR      (*(volatile unsigned long *)  0x4000C000UL) //-- UARTn Transmit Holding Register
#define 	rU0DLL      (*(volatile unsigned long *)  0x4000C000UL) //-- UARTn Divisor Latch LSB register
#define 	rU0DLM      (*(volatile unsigned long *)  0x4000C004UL) //-- Latch MSB register
#define 	rU0IER      (*(volatile unsigned long *)  0x4000C004UL) //-- UARTn Interrupt Enable Register
#define 	rU0IIR      (*(volatile unsigned long *)  0x4000C008UL) //-- UARTn Interrupt Identification Register
#define 	rU0FCR      (*(volatile unsigned long *)  0x4000C008UL) //-- UARTn FIFO Control Register
#define 	rU0LCR      (*(volatile unsigned long *)  0x4000C00CUL) //-- UARTn Line Control Register
#define 	rU0LSR      (*(volatile unsigned long *)  0x4000C014UL) //-- UARTn Line Status Register
#define 	rU0SCR      (*(volatile unsigned long *)  0x4000C01CUL) //-- UARTn Scratch Pad Register
#define 	rU0ACR      (*(volatile unsigned long *)  0x4000C020UL) //-- UARTn Auto-baud Control Register
#define 	rU0ICR      (*(volatile unsigned long *)  0x4000C024UL) //-- UARTn IrDA Control Register
#define 	rU0FDR      (*(volatile unsigned long *)  0x4000C028UL) //-- UARTn Fractional Divider Register
#define 	rU0TER      (*(volatile unsigned long *)  0x4000C030UL) //-- UARTn Transmit Enable Register
#define 	rU0FIFOLVL  (*(volatile unsigned long *)  0x4000C058UL) //-- UARTn FIFO Level register

#define 	rU2RBR      (*(volatile unsigned long *)  0x40098000UL) //-- UARTn Receiver Buffer Register
#define 	rU2THR      (*(volatile unsigned long *)  0x40098000UL) //-- UARTn Transmit Holding Register
#define 	rU2DLL      (*(volatile unsigned long *)  0x40098000UL) //-- UARTn Divisor Latch LSB register
#define 	rU2DLM      (*(volatile unsigned long *)  0x40098004UL) //-- Latch MSB register
#define 	rU2IER      (*(volatile unsigned long *)  0x40098004UL) //-- UARTn Interrupt Enable Register
#define 	rU2IIR      (*(volatile unsigned long *)  0x40098008UL) //-- UARTn Interrupt Identification Register
#define 	rU2FCR      (*(volatile unsigned long *)  0x40098008UL) //-- UARTn FIFO Control Register
#define 	rU2LCR      (*(volatile unsigned long *)  0x4009800CUL) //-- UARTn Line Control Register
#define 	rU2LSR      (*(volatile unsigned long *)  0x40098014UL) //-- UARTn Line Status Register
#define 	rU2SCR      (*(volatile unsigned long *)  0x4009801CUL) //-- UARTn Scratch Pad Register
#define 	rU2ACR      (*(volatile unsigned long *)  0x40098020UL) //-- UARTn Auto-baud Control Register
#define 	rU2ICR      (*(volatile unsigned long *)  0x40098024UL) //-- UARTn IrDA Control Register
#define 	rU2FDR      (*(volatile unsigned long *)  0x40098028UL) //-- UARTn Fractional Divider Register
#define 	rU2TER      (*(volatile unsigned long *)  0x40098030UL) //-- UARTn Transmit Enable Register
#define 	rU2FIFOLVL  (*(volatile unsigned long *)  0x40098058UL) //-- UARTn FIFO Level register

#define 	rU3RBR      (*(volatile unsigned long *)  0x4009C000UL) //-- UARTn Receiver Buffer Register
#define 	rU3THR      (*(volatile unsigned long *)  0x4009C000UL) //-- UARTn Transmit Holding Register
#define 	rU3DLL      (*(volatile unsigned long *)  0x4009C000UL) //-- UARTn Divisor Latch LSB register
#define 	rU3DLM      (*(volatile unsigned long *)  0x4009C004UL) //-- Latch MSB register
#define 	rU3IER      (*(volatile unsigned long *)  0x4009C004UL) //-- UARTn Interrupt Enable Register
#define 	rU3IIR      (*(volatile unsigned long *)  0x4009C008UL) //-- UARTn Interrupt Identification Register
#define 	rU3FCR      (*(volatile unsigned long *)  0x4009C008UL) //-- UARTn FIFO Control Register
#define 	rU3LCR      (*(volatile unsigned long *)  0x4009C00CUL) //-- UARTn Line Control Register
#define 	rU3LSR      (*(volatile unsigned long *)  0x4009C014UL) //-- UARTn Line Status Register
#define 	rU3SCR      (*(volatile unsigned long *)  0x4009C01CUL) //-- UARTn Scratch Pad Register
#define 	rU3ACR      (*(volatile unsigned long *)  0x4009C020UL) //-- UARTn Auto-baud Control Register
#define 	rU3ICR      (*(volatile unsigned long *)  0x4009C024UL) //-- UARTn IrDA Control Register
#define 	rU3FDR      (*(volatile unsigned long *)  0x4009C028UL) //-- UARTn Fractional Divider Register
#define 	rU3TER      (*(volatile unsigned long *)  0x4009C030UL) //-- UARTn Transmit Enable Register
#define 	rU3FIFOLVL  (*(volatile unsigned long *)  0x4009C058UL) //-- UARTn FIFO Level register

//----  UART1

#define 	rU1RBR  (*(volatile unsigned long *)  0x40010000UL) //-- UART1 Receiver Buffer Register
#define 	rU1THR  (*(volatile unsigned long *)  0x40010000UL) //-- UART1 Transmitter Holding Register
#define 	rU1DLL  (*(volatile unsigned long *)  0x40010000UL) //-- UART1 Divisor Latch LSB and MSB Registers
#define 	rU1DLM  (*(volatile unsigned long *)  0x40010004UL)
#define 	rU1IER  (*(volatile unsigned long *)  0x40010004UL) //-- UART1 Interrupt Enable Register
#define 	rU1IIR  (*(volatile unsigned long *)  0x40010008UL) //-- UART1 Interrupt Identification Register
#define 	rU1FCR  (*(volatile unsigned long *)  0x40010008UL) //-- UART1 FIFO Control Register
#define 	rU1LCR  (*(volatile unsigned long *)  0x4001000CUL) //-- UART1 Line Control Register
#define 	rU1MCR  (*(volatile unsigned long *)  0x40010010UL) //-- UART1 Modem Control Register
#define 	rU1LSR  (*(volatile unsigned long *)  0x40010014UL) //-- UART1 Line Status Register
#define 	rU1MSR  (*(volatile unsigned long *)  0x40010018UL) //-- UART1 Modem Status Register
#define 	rU1SCR  (*(volatile unsigned long *)  0x4001001CUL) //-- UART1 Scratch Pad Register
#define 	rU1ACR  (*(volatile unsigned long *)  0x40010020UL) //-- UART1 Auto-baud Control Register
#define 	rU1FDR  (*(volatile unsigned long *)  0x40010028UL) //-- UART1 Fractional Divider Register
#define 	rU1TER  (*(volatile unsigned long *)  0x40010030UL) //-- UART1 Transmit Enable Register
#define 	rU1RS485CTRL (*(volatile unsigned long *)  0x4001004CUL) //-- UART1 RS485 Control register
#define 	rU1ADRMATCH  (*(volatile unsigned long *)  0x40010050UL) //-- UART1 RS-485 Address Match register
#define 	rU1RS485ADRMATCH  rU1ADRMATCH                        
#define 	rU1RS485DLY  (*(volatile unsigned long *)  0x40010054UL) //-- UART1 RS-485 Delay value register
#define 	rU1FIFOLVL   (*(volatile unsigned long *)  0x40010058UL) //-- UART1 FIFO Level register
                                                              
//---- CAN                                                    

#define 	rAFMR         (*(volatile unsigned long *)  0x4003C000UL) //-- Acceptance Filter Mode Register
#define 	rSFF_sa       (*(volatile unsigned long *)  0x4003C004UL) //-- Standard Frame Individual Start Address register
#define 	rSFF_GRP_sa   (*(volatile unsigned long *)  0x4003C008UL) //-- Standard Frame Group Start Address register
#define 	rEFF_sa       (*(volatile unsigned long *)  0x4003C00CUL) //-- Extended Frame Start Address register
#define 	rEFF_GRP_sa   (*(volatile unsigned long *)  0x4003C010UL) //-- Extended Frame Group Start Address register
#define 	rENDofTable   (*(volatile unsigned long *)  0x4003C014UL) //-- End of AF Tables register
#define 	rLUTerrAd     (*(volatile unsigned long *)  0x4003C018UL) //-- LUT Error Address register
#define 	rLUTerr       (*(volatile unsigned long *)  0x4003C01CUL) //-- LUT Error register
#define 	rFCANIE       (*(volatile unsigned long *)  0x4003C020UL) //-- Global FullCANInterrupt Enable register
#define 	rFCANIC0      (*(volatile unsigned long *)  0x4003C024UL) //-- FullCAN Interrupt and Capture registers
#define 	rFCANIC1      (*(volatile unsigned long *)  0x4003C028UL)
#define 	rCANTxSR      (*(volatile unsigned long *)  0x40040000UL) //-- Central Transmit Status Register
#define 	rCANRxSR      (*(volatile unsigned long *)  0x40040004UL) //-- Central Receive Status Register
#define 	rCANMSR       (*(volatile unsigned long *)  0x40040008UL) //-- Central Miscellaneous Status Register

#define 	rCAN1MOD    (*(volatile unsigned long *)  0x40044000UL) //-- CAN Mode register
#define 	rCAN1CMR    (*(volatile unsigned long *)  0x40044004UL) //-- CAN Command Register
#define 	rCAN1GSR    (*(volatile unsigned long *)  0x40044008UL) //-- CAN Global Status Register
#define 	rCAN1ICR    (*(volatile unsigned long *)  0x4004400CUL) //-- CAN Interrupt and Capture Register
#define 	rCAN1IER    (*(volatile unsigned long *)  0x40044010UL) //-- CAN Interrupt Enable Register
#define 	rCAN1BTR    (*(volatile unsigned long *)  0x40044014UL) //-- CAN Bus Timing Register
#define 	rCAN1EWL    (*(volatile unsigned long *)  0x40044018UL) //-- CAN Error Warning Limit register
#define 	rCAN1SR     (*(volatile unsigned long *)  0x4004401CUL) //-- CAN Status Register
#define 	rCAN1RFS    (*(volatile unsigned long *)  0x40044020UL) //-- CAN Receive Frame Status register
#define 	rCAN1RID    (*(volatile unsigned long *)  0x40044024UL) //-- CAN Receive Identifier register
#define 	rCAN1RDA    (*(volatile unsigned long *)  0x40044028UL) //-- CAN Receive Data register A
#define 	rCAN1RDB    (*(volatile unsigned long *)  0x4004402CUL) //-- CAN Receive Data register B
#define 	rCAN1TFI1   (*(volatile unsigned long *)  0x40044030UL) //-- CAN Transmit Frame Information register
#define 	rCAN1TID1   (*(volatile unsigned long *)  0x40044034UL) //-- CAN Transmit Identifier register
#define 	rCAN1TDA1   (*(volatile unsigned long *)  0x40044038UL) //-- CAN Transmit Data register A
#define 	rCAN1TDB1   (*(volatile unsigned long *)  0x4004403CUL) //-- CAN Transmit Data register B
#define 	rCAN1TFI2   (*(volatile unsigned long *)  0x40044040UL)
#define 	rCAN1TID2   (*(volatile unsigned long *)  0x40044044UL)
#define 	rCAN1TDA2   (*(volatile unsigned long *)  0x40044048UL)
#define 	rCAN1TDB2   (*(volatile unsigned long *)  0x4004404CUL)
#define 	rCAN1TFI3   (*(volatile unsigned long *)  0x40044050UL)
#define 	rCAN1TID3   (*(volatile unsigned long *)  0x40044054UL)
#define 	rCAN1TDA3   (*(volatile unsigned long *)  0x40044058UL)
#define 	rCAN1TDB3   (*(volatile unsigned long *)  0x4004405CUL)

#define 	rCAN2MOD    (*(volatile unsigned long *)  0x40048000UL) //-- CAN Mode register
#define 	rCAN2CMR    (*(volatile unsigned long *)  0x40048004UL) //-- CAN Command Register
#define 	rCAN2GSR    (*(volatile unsigned long *)  0x40048008UL) //-- CAN Global Status Register
#define 	rCAN2ICR    (*(volatile unsigned long *)  0x4004800CUL) //-- CAN Interrupt and Capture Register
#define 	rCAN2IER    (*(volatile unsigned long *)  0x40048010UL) //-- CAN Interrupt Enable Register
#define 	rCAN2BTR    (*(volatile unsigned long *)  0x40048014UL) //-- CAN Bus Timing Register
#define 	rCAN2EWL    (*(volatile unsigned long *)  0x40048018UL) //-- CAN Error Warning Limit register
#define 	rCAN2SR     (*(volatile unsigned long *)  0x4004801CUL) //-- CAN Status Register
#define 	rCAN2RFS    (*(volatile unsigned long *)  0x40048020UL) //-- CAN Receive Frame Status register
#define 	rCAN2RID    (*(volatile unsigned long *)  0x40048024UL) //-- CAN Receive Identifier register
#define 	rCAN2RDA    (*(volatile unsigned long *)  0x40048028UL) //-- CAN Receive Data register A
#define 	rCAN2RDB    (*(volatile unsigned long *)  0x4004802CUL) //-- CAN Receive Data register B
#define 	rCAN2TFI1   (*(volatile unsigned long *)  0x40048030UL) //-- CAN Transmit Frame Information register
#define 	rCAN2TID1   (*(volatile unsigned long *)  0x40048034UL) //-- CAN Transmit Identifier register
#define 	rCAN2TDA1   (*(volatile unsigned long *)  0x40048038UL) //-- CAN Transmit Data register A
#define 	rCAN2TDB1   (*(volatile unsigned long *)  0x4004803CUL) //-- CAN Transmit Data register B
#define 	rCAN2TFI2   (*(volatile unsigned long *)  0x40048040UL)
#define 	rCAN2TID2   (*(volatile unsigned long *)  0x40048044UL)
#define 	rCAN2TDA2   (*(volatile unsigned long *)  0x40048048UL)
#define 	rCAN2TDB2   (*(volatile unsigned long *)  0x4004804CUL)
#define 	rCAN2TFI3   (*(volatile unsigned long *)  0x40048050UL)
#define 	rCAN2TID3   (*(volatile unsigned long *)  0x40048054UL)
#define 	rCAN2TDA3   (*(volatile unsigned long *)  0x40048058UL)
#define 	rCAN2TDB3   (*(volatile unsigned long *)  0x4004805CUL)

#define 	rCANSLEEPCLR    (*(volatile unsigned long *)  0x400FC110UL) //-- CAN Sleep Clear register
#define 	rCANWAKEFLAGS   (*(volatile unsigned long *)  0x400FC114UL) //-- CAN Wake-up Flags register

//---- SPI

#define 	rS0SPCR   (*(volatile unsigned long *)  0x40020000UL) //-- SPI Control Register
#define 	rS0SPSR   (*(volatile unsigned long *)  0x40020004UL) //-- SPI Status Register
#define 	rS0SPDR   (*(volatile unsigned long *)  0x40020008UL) //-- SPI Data Register
#define 	rS0SPCCR  (*(volatile unsigned long *)  0x4002000CUL) //-- SPI Clock Counter Register
//#define 	SPTCR    (*(volatile unsigned long *)  0x40020010) //-- SPI Test Control Register
//#define 	SPTSR    (*(volatile unsigned long *)  0x40020014) //-- SPI Test Status Register
#define 	S0SPINT  (*(volatile unsigned long *)  0x4002001CUL) //-- SPI Interrupt Register

//---- SSP

#define 	rSSP0CR0    (*(volatile unsigned long *)  0x40088000UL) //-- SSP0 Control Register 0
#define 	rSSP0CR1    (*(volatile unsigned long *)  0x40088004UL) //-- SSP0 Control Register 1
#define 	rSSP0DR     (*(volatile unsigned long *)  0x40088008UL) //-- SSP0 Data Register
#define 	rSSP0SR     (*(volatile unsigned long *)  0x4008800CUL) //-- SSP0 Status Register
#define 	rSSP0CPSR   (*(volatile unsigned long *)  0x40088010UL) //-- SSP0 Clock Prescale Register
#define 	rSSP0IMSC   (*(volatile unsigned long *)  0x40088014UL) //-- SSP0 Interrupt Mask Set/Clear Register
#define 	rSSP0RIS    (*(volatile unsigned long *)  0x40088018UL) //-- SSP0 Raw Interrupt Status Register
#define 	rSSP0MIS    (*(volatile unsigned long *)  0x4008801CUL) //-- SSP0 Masked Interrupt Status Register
#define 	rSSP0ICR    (*(volatile unsigned long *)  0x40088020UL) //-- SSP0 Interrupt Clear Register
#define 	rSSP0DMACR  (*(volatile unsigned long *)  0x40088024UL) //-- SSP0 DMA Control Register
                                                             
                                                             
#define 	rSSP1CR0    (*(volatile unsigned long *)  0x40030000UL) //-- SSP1 Control Register 0
#define 	rSSP1CR1    (*(volatile unsigned long *)  0x40030004UL) //-- SSP1 Control Register 1
#define 	rSSP1DR     (*(volatile unsigned long *)  0x40030008UL) //-- SSP1 Data Register
#define 	rSSP1SR     (*(volatile unsigned long *)  0x4003000CUL) //-- SSP1 Status Register
#define 	rSSP1CPSR   (*(volatile unsigned long *)  0x40030010UL) //-- SSP1 Clock Prescale Register
#define 	rSSP1IMSC   (*(volatile unsigned long *)  0x40030014UL) //-- SSP1 Interrupt Mask Set/Clear Register
#define 	rSSP1RIS    (*(volatile unsigned long *)  0x40030018UL) //-- SSP1 Raw Interrupt Status Register
#define 	rSSP1MIS    (*(volatile unsigned long *)  0x4003001CUL) //-- SSP1 Masked Interrupt Status Register
#define 	rSSP1ICR    (*(volatile unsigned long *)  0x40030020UL) //-- SSP1 Interrupt Clear Register
#define 	rSSP1DMACR  (*(volatile unsigned long *)  0x40030024UL) //-- SSP1 DMA Control Register

//---- I2C

#define 	rI2C0CONSET      (*(volatile unsigned long *)  0x4001C000UL) //-- I2C Control Set register
#define 	rI2C0STAT        (*(volatile unsigned long *)  0x4001C004UL) //-- I2C Status register
#define 	rI2C0DAT         (*(volatile unsigned long *)  0x4001C008UL) //-- I2C Data register
#define 	rI2C0ADR0        (*(volatile unsigned long *)  0x4001C00CUL) //-- I2C Slave Address registers
#define 	rI2C0SCLH        (*(volatile unsigned long *)  0x4001C010UL) //-- I2C SCL HIGH duty cycle register
#define 	rI2C0SCLL        (*(volatile unsigned long *)  0x4001C014UL) //-- I2C SCL Low duty cycle register
#define 	rI2C0CONCLR      (*(volatile unsigned long *)  0x4001C018UL) //-- I2C Control Clear register
#define 	rI2C0MMCTRL      (*(volatile unsigned long *)  0x4001C01CUL) //-- I2C Monitor mode control register
#define 	rI2C0ADR1        (*(volatile unsigned long *)  0x4001C020UL)
#define 	rI2C0ADR2        (*(volatile unsigned long *)  0x4001C024UL)
#define 	rI2C0ADR3        (*(volatile unsigned long *)  0x4001C028UL)
#define 	rI2C0DATA_BUFFER (*(volatile unsigned long *)  0x4001C02CUL) //-- I2C Data buffer register
#define 	rI2C0MASK0       (*(volatile unsigned long *)  0x4001C030UL) //-- I2C Mask registers
#define 	rI2C0MASK1       (*(volatile unsigned long *)  0x4001C034UL)
#define 	rI2C0MASK2       (*(volatile unsigned long *)  0x4001C038UL)
#define 	rI2C0MASK3       (*(volatile unsigned long *)  0x4001C03CUL)

#define 	rI2C1CONSET       (*(volatile unsigned long *)  0x4005C000UL) //-- I2C Control Set register
#define 	rI2C1STAT         (*(volatile unsigned long *)  0x4005C004UL) //-- I2C Status register
#define 	rI2C1DAT          (*(volatile unsigned long *)  0x4005C008UL) //-- I2C Data register
#define 	rI2C1ADR0         (*(volatile unsigned long *)  0x4005C00CUL) //-- I2C Slave Address registers
#define 	rI2C1SCLH         (*(volatile unsigned long *)  0x4005C010UL) //-- I2C SCL HIGH duty cycle register
#define 	rI2C1SCLL         (*(volatile unsigned long *)  0x4005C014UL) //-- I2C SCL Low duty cycle register
#define 	rI2C1CONCLR       (*(volatile unsigned long *)  0x4005C018UL) //-- I2C Control Clear register
#define 	rI2C1MMCTRL       (*(volatile unsigned long *)  0x4005C01CUL) //-- I2C Monitor mode control register
#define 	rI2C1ADR1         (*(volatile unsigned long *)  0x4005C020UL)
#define 	rI2C1ADR2         (*(volatile unsigned long *)  0x4005C024UL)
#define 	rI2C1ADR3         (*(volatile unsigned long *)  0x4005C028UL)
#define 	rI2C1DATA_BUFFER  (*(volatile unsigned long *)  0x4005C02CUL) //-- I2C Data buffer register
#define 	rI2C1MASK0        (*(volatile unsigned long *)  0x4005C030UL) //-- I2C Mask registers
#define 	rI2C1MASK1        (*(volatile unsigned long *)  0x4005C034UL)
#define 	rI2C1MASK2        (*(volatile unsigned long *)  0x4005C038UL)
#define 	rI2C1MASK3        (*(volatile unsigned long *)  0x4005C03CUL)
                                                                   
#define 	rI2C2CONSET       (*(volatile unsigned long *)  0x400A0000UL) //-- I2C Control Set register
#define 	rI2C2STAT         (*(volatile unsigned long *)  0x400A0004UL) //-- I2C Status register
#define 	rI2C2DAT          (*(volatile unsigned long *)  0x400A0008UL) //-- I2C Data register
#define 	rI2C2ADR0         (*(volatile unsigned long *)  0x400A000CUL) //-- I2C Slave Address registers
#define 	rI2C2SCLH         (*(volatile unsigned long *)  0x400A0010UL) //-- I2C SCL HIGH duty cycle register
#define 	rI2C2SCLL         (*(volatile unsigned long *)  0x400A0014UL) //-- I2C SCL Low duty cycle register
#define 	rI2C2CONCLR       (*(volatile unsigned long *)  0x400A0018UL) //-- I2C Control Clear register
#define 	rI2C2MMCTRL       (*(volatile unsigned long *)  0x400A001CUL) //-- I2C Monitor mode control register
#define 	rI2C2ADR1         (*(volatile unsigned long *)  0x400A0020UL)
#define 	rI2C2ADR2         (*(volatile unsigned long *)  0x400A0024UL)
#define 	rI2C2ADR3         (*(volatile unsigned long *)  0x400A0028UL)
#define 	rI2C2DATA_BUFFER  (*(volatile unsigned long *)  0x400A002CUL) //-- I2C Data buffer register
#define 	rI2C2MASK0        (*(volatile unsigned long *)  0x400A0030UL) //-- I2C Mask registers
#define 	rI2C2MASK1        (*(volatile unsigned long *)  0x400A0034UL)
#define 	rI2C2MASK2        (*(volatile unsigned long *)  0x400A0038UL)
#define 	rI2C2MASK3        (*(volatile unsigned long *)  0x400A003CUL)
                                                                   
//---- I2S                                                         

#define 	rI2SDAO        (*(volatile unsigned long *)  0x400A8000UL) //-- Digital Audio Output register
#define 	rI2SDAI        (*(volatile unsigned long *)  0x400A8004UL) //-- Digital Audio Input register
#define 	rI2STXFIFO     (*(volatile unsigned long *)  0x400A8008UL) //-- Transmit FIFO register
#define 	rI2SRXFIFO     (*(volatile unsigned long *)  0x400A800CUL) //-- Receive FIFO register
#define 	rI2SSTATE      (*(volatile unsigned long *)  0x400A8010UL) //-- Status Feedback register
#define 	rI2SDMA1       (*(volatile unsigned long *)  0x400A8014UL) //-- DMA Configuration Register 1
#define 	rI2SDMA2       (*(volatile unsigned long *)  0x400A8018UL) //-- DMA Configuration Register 2
#define 	rI2SIRQ        (*(volatile unsigned long *)  0x400A801CUL) //-- Interrupt Request Control register
#define 	rI2STXRATE     (*(volatile unsigned long *)  0x400A8020UL) //-- Transmit Clock Rate register
#define 	rI2SRXRATE     (*(volatile unsigned long *)  0x400A8024UL) //-- Receive Clock Rate register
#define 	rI2STXBITRATE  (*(volatile unsigned long *)  0x400A8028UL) //-- Transmit Clock Bit Rate register
#define 	rI2SRXBITRATE  (*(volatile unsigned long *)  0x400A802CUL) //-- Receive Clock Bit Rate register
#define 	rI2STXMODE     (*(volatile unsigned long *)  0x400A8030UL) //-- Transmit Mode Control register
#define 	rI2SRXMODE     (*(volatile unsigned long *)  0x400A8034UL) //-- Receive Mode Control register

//---- Timers

#define 	rT0IR    (*(volatile unsigned long *)  0x40004000UL) //-- Interrupt Register
#define 	rT0TCR   (*(volatile unsigned long *)  0x40004004UL) //-- Timer Control Register
#define 	rT0TC    (*(volatile unsigned long *)  0x40004008UL) //-- Timer Counter registers
#define 	rT0PR    (*(volatile unsigned long *)  0x4000400CUL) //-- Prescale register
#define 	rT0PC    (*(volatile unsigned long *)  0x40004010UL) //-- Prescale Counter register
#define 	rT0MCR   (*(volatile unsigned long *)  0x40004014UL) //-- Match Control Register
#define 	rT0MR0   (*(volatile unsigned long *)  0x40004018UL) //-- Match Register 0
#define 	rT0MR1   (*(volatile unsigned long *)  0x4000401CUL) //-- Match Register 1
#define 	rT0MR2   (*(volatile unsigned long *)  0x40004020UL) //-- Match Register 2
#define 	rT0MR3   (*(volatile unsigned long *)  0x40004024UL) //-- Match Register 3
#define 	rT0CCR   (*(volatile unsigned long *)  0x40004028UL) //-- Capture Control Register
#define 	rT0CR0   (*(volatile unsigned long *)  0x4000402CUL) //-- Capture Register 0
#define 	rT0CR1   (*(volatile unsigned long *)  0x40004030UL) //-- Capture Register 1
#define 	rT0EMR   (*(volatile unsigned long *)  0x4000403CUL) //-- External Match Register
#define 	rT0CTCR  (*(volatile unsigned long *)  0x40004070UL) //-- Count Control Register

#define 	rT1IR    (*(volatile unsigned long *)  0x40008000UL) //-- Interrupt Register
#define 	rT1TCR   (*(volatile unsigned long *)  0x40008004UL) //-- Timer Control Register
#define 	rT1TC    (*(volatile unsigned long *)  0x40008008UL) //-- Timer Counter registers
#define 	rT1PR    (*(volatile unsigned long *)  0x4000800CUL) //-- Prescale register
#define 	rT1PC    (*(volatile unsigned long *)  0x40008010UL) //-- Prescale Counter register
#define 	rT1MCR   (*(volatile unsigned long *)  0x40008014UL) //-- Match Control Register
#define 	rT1MR0   (*(volatile unsigned long *)  0x40008018UL) //-- Match Register 0
#define 	rT1MR1   (*(volatile unsigned long *)  0x4000801CUL) //-- Match Register 1
#define 	rT1MR2   (*(volatile unsigned long *)  0x40008020UL) //-- Match Register 2
#define 	rT1MR3   (*(volatile unsigned long *)  0x40008024UL) //-- Match Register 3
#define 	rT1CCR   (*(volatile unsigned long *)  0x40008028UL) //-- Capture Control Register
#define 	rT1CR0   (*(volatile unsigned long *)  0x4000802CUL) //-- Capture Register 0
#define 	rT1CR1   (*(volatile unsigned long *)  0x40008030UL) //-- Capture Register 1
#define 	rT1EMR   (*(volatile unsigned long *)  0x4000803CUL) //-- External Match Register
#define 	rT1CTCR  (*(volatile unsigned long *)  0x40008070UL) //-- Count Control Register

#define 	rT2IR    (*(volatile unsigned long *)  0x40090000UL) //-- Interrupt Register
#define 	rT2TCR   (*(volatile unsigned long *)  0x40090004UL) //-- Timer Control Register
#define 	rT2TC    (*(volatile unsigned long *)  0x40090008UL) //-- Timer Counter registers
#define 	rT2PR    (*(volatile unsigned long *)  0x4009000CUL) //-- Prescale register
#define 	rT2PC    (*(volatile unsigned long *)  0x40090010UL) //-- Prescale Counter register
#define 	rT2MCR   (*(volatile unsigned long *)  0x40090014UL) //-- Match Control Register
#define 	rT2MR0   (*(volatile unsigned long *)  0x40090018UL) //-- Match Register 0
#define 	rT2MR1   (*(volatile unsigned long *)  0x4009001CUL) //-- Match Register 1
#define 	rT2MR2   (*(volatile unsigned long *)  0x40090020UL) //-- Match Register 2
#define 	rT2MR3   (*(volatile unsigned long *)  0x40090024UL) //-- Match Register 3
#define 	rT2CCR   (*(volatile unsigned long *)  0x40090028UL) //-- Capture Control Register
#define 	rT2CR0   (*(volatile unsigned long *)  0x4009002CUL) //-- Capture Register 0
#define 	rT2CR1   (*(volatile unsigned long *)  0x40090030UL) //-- Capture Register 1
#define 	rT2EMR   (*(volatile unsigned long *)  0x4009003CUL) //-- External Match Register
#define 	rT2CTCR  (*(volatile unsigned long *)  0x40090070UL) //-- Count Control Register

#define 	rT3IR    (*(volatile unsigned long *)  0x40094000UL) //-- Interrupt Register
#define 	rT3TCR   (*(volatile unsigned long *)  0x40094004UL) //-- Timer Control Register
#define 	rT3TC    (*(volatile unsigned long *)  0x40094008UL) //-- Timer Counter registers
#define 	rT3PR    (*(volatile unsigned long *)  0x4009400CUL) //-- Prescale register
#define 	rT3PC    (*(volatile unsigned long *)  0x40094010UL) //-- Prescale Counter register
#define 	rT3MCR   (*(volatile unsigned long *)  0x40094014UL) //-- Match Control Register
#define 	rT3MR0   (*(volatile unsigned long *)  0x40094018UL) //-- Match Register 0
#define 	rT3MR1   (*(volatile unsigned long *)  0x4009401CUL) //-- Match Register 0
#define 	rT3MR2   (*(volatile unsigned long *)  0x40094020UL) //-- Match Register 0
#define 	rT3MR3   (*(volatile unsigned long *)  0x40094024UL) //-- Match Register 0
#define 	rT3CCR   (*(volatile unsigned long *)  0x40094028UL) //-- Capture Control Register
#define 	rT3CR0   (*(volatile unsigned long *)  0x4009402CUL) //-- Capture Register 0
#define 	rT3CR1   (*(volatile unsigned long *)  0x40094030UL) //-- Capture Register 1
#define 	rT3EMR   (*(volatile unsigned long *)  0x4009403CUL) //-- External Match Register
#define 	rT3CTCR  (*(volatile unsigned long *)  0x40094070UL) //-- Count Control Register

//---- Sys int timer

#define 	rRICOMPVAL  (*(volatile unsigned long *)  0x400B0000UL) //-- RI Compare Value register
#define 	rRIMASK     (*(volatile unsigned long *)  0x400B0004UL) //-- RI Mask register
#define 	rRICTRL     (*(volatile unsigned long *)  0x400B0008UL) //-- RI Control register
#define 	rRICOUNTER  (*(volatile unsigned long *)  0x400B000CUL) //-- RI 32-bit counter
                                                             
//---- Sys tick timer                                        
                                                             
#define 	rSTCTRL    (*(volatile unsigned long *)  0xE000E010UL) //-- System Timer Control and status register
#define 	rSTRELOAD  (*(volatile unsigned long *)  0xE000E014UL) //-- System Timer Reload value register
#define 	rSTCURR    (*(volatile unsigned long *)  0xE000E018UL) //-- System Timer Current value register
#define 	rSTCALIB   (*(volatile unsigned long *)  0xE000E01CUL) //-- System Timer Calibration value register
                                                            
//---- PWM                                                  
                                                            
#define 	rPWM1IR    (*(volatile unsigned long *)  0x40018000UL) //-- PWM Interrupt Register
#define 	rPWM1TCR   (*(volatile unsigned long *)  0x40018004UL) //-- PWM Timer Control Register
#define 	rPWM1TC    (*(volatile unsigned long *)  0x40018008UL)
#define 	rPWM1PR    (*(volatile unsigned long *)  0x4001800CUL)
#define 	rPWM1PC    (*(volatile unsigned long *)  0x40018010UL)
#define 	rPWM1MCR   (*(volatile unsigned long *)  0x40018014UL) //-- PWM Match Control Register
#define 	rPWM1MR0   (*(volatile unsigned long *)  0x40018018UL)
#define 	rPWM1MR1   (*(volatile unsigned long *)  0x4001801CUL)
#define 	rPWM1MR2   (*(volatile unsigned long *)  0x40018020UL)
#define 	rPWM1MR3   (*(volatile unsigned long *)  0x40018024UL)
#define 	rPWM1CCR   (*(volatile unsigned long *)  0x40018028UL) //-- PWM Capture Control Register
#define 	rPWM1CR0   (*(volatile unsigned long *)  0x4001802CUL)
#define 	rPWM1CR1   (*(volatile unsigned long *)  0x40018030UL)
#define 	rPWM1CR2   (*(volatile unsigned long *)  0x40018034UL)
#define 	rPWM1CR3   (*(volatile unsigned long *)  0x40018038UL)
#define 	rPWM1MR4   (*(volatile unsigned long *)  0x40018040UL)
#define 	rPWM1MR5   (*(volatile unsigned long *)  0x40018044UL)
#define 	rPWM1MR6   (*(volatile unsigned long *)  0x40018048UL)
#define 	rPWM1PCR   (*(volatile unsigned long *)  0x4001804CUL) //-- PWM Control Register
#define 	rPWM1LER   (*(volatile unsigned long *)  0x40018050UL) //-- PWM Latch Enable Register
#define 	rPWM1CTCR  (*(volatile unsigned long *)  0x40018070UL) //-- PWM Count Control Register

//---- Motor control PWM

#define 	rMCCON         (*(volatile unsigned long *)  0x400B8000UL) //-- MCPWM Control read address
#define 	rMCCON_SET     (*(volatile unsigned long *)  0x400B8004UL) //-- MCPWM Control set address
#define 	rMCCON_CLR     (*(volatile unsigned long *)  0x400B8008UL) //-- MCPWM Control clear address
#define 	rMCCAPCON      (*(volatile unsigned long *)  0x400B800CUL) //-- MCPWM Capture Control read address
#define 	rMCCAPCON_SET  (*(volatile unsigned long *)  0x400B8010UL) //-- MCPWM Capture Control set address
#define 	rMCCAPCON_CLR  (*(volatile unsigned long *)  0x400B8014UL) //-- MCPWM Capture control clear address
#define 	rMCTC0         (*(volatile unsigned long *)  0x400B8018UL) //-- MCPWM Timer/Counter 0-2 registers
#define 	rMCTC1         (*(volatile unsigned long *)  0x400B801CUL)
#define 	rMCTC2         (*(volatile unsigned long *)  0x400B8020UL)
#define 	rMCLIM0        (*(volatile unsigned long *)  0x400B8024UL) //-- MCPWM Limit 0-2 registers
#define 	rMCLIM1        (*(volatile unsigned long *)  0x400B8028UL)
#define 	rMCLIM2        (*(volatile unsigned long *)  0x400B802CUL)
#define 	rMCMAT0        (*(volatile unsigned long *)  0x400B8030UL) //-- MCPWM Match 0-2 registers
#define 	rMCMAT1        (*(volatile unsigned long *)  0x400B8034UL)
#define 	rMCMAT2        (*(volatile unsigned long *)  0x400B8038UL)
#define 	rMCDT          (*(volatile unsigned long *)  0x400B803CUL) //-- MCPWM Dead-time register
#define 	rMCCP          (*(volatile unsigned long *)  0x400B8040UL) //-- MCPWM Communication Pattern register
#define 	rMCCAP0        (*(volatile unsigned long *)  0x400B8044UL) //-- MCPWM Capture read addresses
#define 	rMCCAP1        (*(volatile unsigned long *)  0x400B8048UL)
#define 	rMCCAP2        (*(volatile unsigned long *)  0x400B804CUL)
#define 	rMCINTEN       (*(volatile unsigned long *)  0x400B8050UL) //-- MCPWM Interrupt Enable read address
#define 	rMCINTEN_SET   (*(volatile unsigned long *)  0x400B8054UL) //-- MCPWM Interrupt Enable set address
#define 	rMCINTEN_CLR   (*(volatile unsigned long *)  0x400B8058UL) //-- MCPWM Interrupt Enable clear address
#define 	rMCCNTCON      (*(volatile unsigned long *)  0x400B805CUL) //-- MCPWM Count Control read address
#define 	rMCCNTCON_SET  (*(volatile unsigned long *)  0x400B8060UL) //-- MCPWM Count Control set address
#define 	rMCCNTCON_CLR  (*(volatile unsigned long *)  0x400B8064UL) //-- MCPWM Count Control clear address
#define 	rMCINTF        (*(volatile unsigned long *)  0x400B8068UL) //-- MCPWM Interrupt Flags read address
#define 	rMCINTF_SET    (*(volatile unsigned long *)  0x400B806CUL) //-- MCPWM Interrupt Flags set address
#define 	rMCINTF_CLR    (*(volatile unsigned long *)  0x400B8070UL) //-- MCPWM Interrupt Flags clear address
#define 	rMCCAP_CLR     (*(volatile unsigned long *)  0x400B8074UL) //--    MCPWM Capture clear address

//---- Quadrature Encoder Interface

#define 	rQEICON     (*(volatile unsigned long *)  0x400BC000UL) //-- QEI Control register
#define 	rQEICONF    (*(volatile unsigned long *)  0x400BC008UL) //-- QEI Configuration register
#define 	rQEISTAT    (*(volatile unsigned long *)  0x400BC004UL) //-- QEI Status register
#define 	rQEIPOS     (*(volatile unsigned long *)  0x400BC00CUL) //-- QEI Position register
#define 	rQEIMAXPOS  (*(volatile unsigned long *)  0x400BC010UL) //-- QEI Maximum Position register
#define 	rCMPOS0     (*(volatile unsigned long *)  0x400BC014UL) //-- QEI Position Compare register 0
#define 	rCMPOS1     (*(volatile unsigned long *)  0x400BC018UL) //-- QEI Position Compare register 1
#define 	rCMPOS2     (*(volatile unsigned long *)  0x400BC01CUL) //-- QEI Position Compare register 2
#define 	rINXCNT     (*(volatile unsigned long *)  0x400BC020UL) //-- QEI Index Count register
#define 	rINXCMP     (*(volatile unsigned long *)  0x400BC024UL) //-- QEI Index Compare register
#define 	rQEILOAD    (*(volatile unsigned long *)  0x400BC028UL) //-- QEI Timer Reload register
#define 	rQEITIME    (*(volatile unsigned long *)  0x400BC02CUL) //-- QEI Timer register
#define 	rQEIVEL     (*(volatile unsigned long *)  0x400BC030UL) //-- QEI Velocity register
#define 	rQEICAP     (*(volatile unsigned long *)  0x400BC034UL) //-- QEI Velocity Capture register
#define 	rVELCOMP    (*(volatile unsigned long *)  0x400BC038UL) //-- QEI Velocity Compare register
#define 	rFILTER     (*(volatile unsigned long *)  0x400BC03CUL) //-- QEI Digital Filter register
                                                             
#define 	rQEIINSTAT  (*(volatile unsigned long *)  0x400BCFE0UL) //-- QEI Interrupt status register
#define 	rQEISET     (*(volatile unsigned long *)  0x400BCFECUL) //-- QEI Interrupt Set register
#define 	rQEICLR     (*(volatile unsigned long *)  0x400BCFE8UL) //-- QEI Interrupt Clear register
#define 	rQEIIE      (*(volatile unsigned long *)  0x400BCFE4UL) //-- QEI Interrupt Enable register
#define 	rQEIIES     (*(volatile unsigned long *)  0x400BCFDCUL) //-- QEI Interrupt Enable Set register
#define 	rQEIIEC     (*(volatile unsigned long *)  0x400BCFD8UL) //-- QEI Interrupt Enable Clear register
                                                             
//---- RTC                                                   
                                                             
#define 	rILR          (*(volatile unsigned long *)  0x40024000UL) //-- Interrupt Location Register
#define 	rCCR          (*(volatile unsigned long *)  0x40024008UL) //-- Clock Control Register
#define 	rCIIR         (*(volatile unsigned long *)  0x4002400CUL) //-- Counter Increment Interrupt Register
#define 	rAMR          (*(volatile unsigned long *)  0x40024010UL) //-- Alarm Mask Register
#define 	rRTC_AUX      (*(volatile unsigned long *)  0x4002405CUL) //-- RTC Auxiliary control register
#define 	rRTC_AUXEN    (*(volatile unsigned long *)  0x40024058UL) //-- RTC Auxiliary Enable register
#define 	rCTIME0       (*(volatile unsigned long *)  0x40024014UL) //-- Consolidated Time Register 0
#define 	rCTIME1       (*(volatile unsigned long *)  0x40024018UL) //-- Consolidated Time Register 1
#define 	rCTIME2       (*(volatile unsigned long *)  0x4002401CUL) //-- Consolidated Time Register 2
#define 	rSEC          (*(volatile unsigned long *)  0x40024020UL)
#define 	rMIN          (*(volatile unsigned long *)  0x40024024UL)
#define 	rHOUR         (*(volatile unsigned long *)  0x40024028UL)
#define 	rDOM          (*(volatile unsigned long *)  0x4002402CUL)
#define 	rDOW          (*(volatile unsigned long *)  0x40024030UL)
#define 	rDOY          (*(volatile unsigned long *)  0x40024034UL)
#define 	rMONTH        (*(volatile unsigned long *)  0x40024038UL)
#define 	rYEAR         (*(volatile unsigned long *)  0x4002403CUL)
#define 	rCALIBRATION  (*(volatile unsigned long *)  0x40024040UL) //-- Calibration register
#define 	rGPREG0       (*(volatile unsigned long *)  0x40024044UL) //-- General purpose registers 0 to 4
#define 	rGPREG1       (*(volatile unsigned long *)  0x40024048UL)
#define 	rGPREG2       (*(volatile unsigned long *)  0x4002404CUL)
#define 	rGPREG3       (*(volatile unsigned long *)  0x40024050UL)
#define 	rGPREG4       (*(volatile unsigned long *)  0x40024054UL)
#define 	rALSEC        (*(volatile unsigned long *)  0x40024060UL)
#define 	rALMIN        (*(volatile unsigned long *)  0x40024064UL)
#define 	rALHOUR       (*(volatile unsigned long *)  0x40024068UL)
#define 	rALDOM        (*(volatile unsigned long *)  0x4002406CUL)
#define 	rALDOW        (*(volatile unsigned long *)  0x40024070UL)
#define 	rALDOY        (*(volatile unsigned long *)  0x40024074UL)
#define 	rALMON        (*(volatile unsigned long *)  0x40024078UL)
#define 	rALYEAR       (*(volatile unsigned long *)  0x4002407CUL)
                                                               
//----  Watchdog                                               
                                                               
#define 	rWDMOD     (*(volatile unsigned long *)  0x40000000UL) //--  Watchdog Mode register
#define 	rWDTC      (*(volatile unsigned long *)  0x40000004UL) //--  Watchdog Timer Constant register
#define 	rWDFEED    (*(volatile unsigned long *)  0x40000008UL) //--  Watchdog Feed register
#define 	rWDTV      (*(volatile unsigned long *)  0x4000000CUL) //--  Watchdog Timer Value register
#define 	rWDCLKSEL  (*(volatile unsigned long *)  0x40000010UL) //--  Watchdog Timer Clock Source Selection register
                                                            
//---- ADC                                                  

#define 	rAD0CR      (*(volatile unsigned long *)  0x40034000UL) //-- A/D Control Register
#define 	rAD0GDR     (*(volatile unsigned long *)  0x40034004UL) //-- A/D Global Data Register
#define 	rAD0INTEN   (*(volatile unsigned long *)  0x4003400CUL) //-- A/D Interrupt Enable register
#define 	rAD0DR0     (*(volatile unsigned long *)  0x40034010UL) //-- A/D Data Registers
#define 	rAD0DR1     (*(volatile unsigned long *)  0x40034014UL)
#define 	rAD0DR2     (*(volatile unsigned long *)  0x40034018UL)
#define 	rAD0DR3     (*(volatile unsigned long *)  0x4003401CUL)
#define 	rAD0DR4     (*(volatile unsigned long *)  0x40034020UL)
#define 	rAD0DR5     (*(volatile unsigned long *)  0x40034024UL)
#define 	rAD0DR6     (*(volatile unsigned long *)  0x40034028UL)
#define 	rAD0DR7     (*(volatile unsigned long *)  0x4003402CUL)
#define 	rAD0STAT    (*(volatile unsigned long *)  0x40034030UL) //-- A/D Status register
#define 	rADTRIM     (*(volatile unsigned long *)  0x40034034UL) //-- A/D Trim register
                                                             
//---- DAC

#define 	rDACR       (*(volatile unsigned long *)  0x4008C000UL) //-- D/A Converter Register
#define 	rDACCTRL    (*(volatile unsigned long *)  0x4008C004UL) //-- D/A Converter Control register
#define 	rDACCNTVAL  (*(volatile unsigned long *)  0x4008C008UL) //-- D/A Converter Counter Value register

//---- DMA controler

#define 	rDMACIntStat        (*(volatile unsigned long *)  0x50004000UL) //-- DMA Interrupt Status register
#define 	rDMACIntTCStat      (*(volatile unsigned long *)  0x50004004UL) //-- DMA Interrupt Terminal Count Request Status register
#define 	rDMACIntTCClear     (*(volatile unsigned long *)  0x50004008UL) //-- DMA Interrupt Terminal Count Request Clear register
#define 	rDMACIntErrStat     (*(volatile unsigned long *)  0x5000400CUL) //-- DMA Interrupt Error Status register
#define 	rDMACIntErrClr      (*(volatile unsigned long *)  0x50004010UL) //-- DMA Interrupt Error Clear register
#define 	rDMACRawIntTCStat   (*(volatile unsigned long *)  0x50004014UL) //-- DMA Raw Interrupt Terminal Count Status register
#define 	rDMACRawIntErrStat  (*(volatile unsigned long *)  0x50004018UL) //-- DMA Raw Error Interrupt Status register
#define 	rDMACEnbldChns      (*(volatile unsigned long *)  0x5000401CUL) //-- DMA Enabled Channel register
#define 	rDMACSoftBReq       (*(volatile unsigned long *)  0x50004020UL) //-- DMA Software Burst Request register
#define 	rDMACSoftSReq       (*(volatile unsigned long *)  0x50004024UL) //-- DMA Software Single Request register
#define 	rDMACSoftLBReq      (*(volatile unsigned long *)  0x50004028UL) //-- DMA Software Last Burst Request register
#define 	rDMACSoftLSReq      (*(volatile unsigned long *)  0x5000402CUL) //-- DMA Software Last Single Request register
#define 	rDMACConfig         (*(volatile unsigned long *)  0x50004030UL) //-- DMA Configuration register
#define 	rDMACSync           (*(volatile unsigned long *)  0x50004034UL) //-- DMA Synchronization register
#define 	rDMAREQSEL          (*(volatile unsigned long *)  0x400FC1C4UL) //-- DMA Request Select register

#define 	rDMACC0SrcAddr      (*(volatile unsigned long *)  0x50004100UL) //-- DMA Channel Source Address registers
#define 	rDMACC0DestAddr     (*(volatile unsigned long *)  0x50004104UL) //-- DMA Channel Destination Address registers
#define 	rDMACC0LLI          (*(volatile unsigned long *)  0x50004108UL) //-- DMA Channel Linked List Item registers
#define 	rDMACC0Control      (*(volatile unsigned long *)  0x5000410CUL) //-- DMA channel control registers
#define 	rDMACC0Config       (*(volatile unsigned long *)  0x50004110UL) //-- DMA Channel Configuration registers
                                                                     
#define 	rDMACC1SrcAddr      (*(volatile unsigned long *)  0x50004120UL) //-- DMA Channel Source Address registers
#define 	rDMACC1DestAddr     (*(volatile unsigned long *)  0x50004124UL) //-- DMA Channel Destination Address registers
#define 	rDMACC1LLI          (*(volatile unsigned long *)  0x50004128UL) //-- DMA Channel Linked List Item registers
#define 	rDMACC1Control      (*(volatile unsigned long *)  0x5000412CUL) //-- DMA channel control registers
#define 	rDMACC1Config       (*(volatile unsigned long *)  0x50004130UL) //-- DMA Channel Configuration registers
                                                                     
#define 	rDMACC2SrcAddr      (*(volatile unsigned long *)  0x50004140UL) //-- DMA Channel Source Address registers
#define 	rDMACC2DestAddr     (*(volatile unsigned long *)  0x50004144UL) //-- DMA Channel Destination Address registers
#define 	rDMACC2LLI          (*(volatile unsigned long *)  0x50004148UL) //-- DMA Channel Linked List Item registers
#define 	rDMACC2Control      (*(volatile unsigned long *)  0x5000414CUL) //-- DMA channel control registers
#define 	rDMACC2Config       (*(volatile unsigned long *)  0x50004150UL) //-- DMA Channel Configuration registers
                                                                     
#define 	rDMACC3SrcAddr      (*(volatile unsigned long *)  0x50004160UL) //-- DMA Channel Source Address registers
#define 	rDMACC3DestAddr     (*(volatile unsigned long *)  0x50004164UL) //-- DMA Channel Destination Address registers
#define 	rDMACC3LLI          (*(volatile unsigned long *)  0x50004168UL) //-- DMA Channel Linked List Item registers
#define 	rDMACC3Control      (*(volatile unsigned long *)  0x5000416CUL) //-- DMA channel control registers
#define 	rDMACC3Config       (*(volatile unsigned long *)  0x50004170UL) //-- DMA Channel Configuration registers
                                                                     
#define 	rDMACC4SrcAddr      (*(volatile unsigned long *)  0x50004180UL) //-- DMA Channel Source Address registers
#define 	rDMACC4DestAddr     (*(volatile unsigned long *)  0x50004184UL) //-- DMA Channel Destination Address registers
#define 	rDMACC4LLI          (*(volatile unsigned long *)  0x50004188UL) //-- DMA Channel Linked List Item registers
#define 	rDMACC4Control      (*(volatile unsigned long *)  0x5000418CUL) //-- DMA channel control registers
#define 	rDMACC4Config       (*(volatile unsigned long *)  0x50004190UL) //-- DMA Channel Configuration registers
                                                                     
#define 	rDMACC5SrcAddr      (*(volatile unsigned long *)  0x500041A0UL) //-- DMA Channel Source Address registers
#define 	rDMACC5DestAddr     (*(volatile unsigned long *)  0x500041A4UL) //-- DMA Channel Destination Address registers
#define 	rDMACC5LLI          (*(volatile unsigned long *)  0x500041A8UL) //-- DMA Channel Linked List Item registers
#define 	rDMACC5Control      (*(volatile unsigned long *)  0x500041ACUL) //-- DMA channel control registers
#define 	rDMACC5Config       (*(volatile unsigned long *)  0x500041B0UL) //-- DMA Channel Configuration registers

#define 	rDMACC6SrcAddr      (*(volatile unsigned long *)  0x500041C0UL) //-- DMA Channel Source Address registers
#define 	rDMACC6DestAddr     (*(volatile unsigned long *)  0x500041C4UL) //-- DMA Channel Destination Address registers
#define 	rDMACC6LLI          (*(volatile unsigned long *)  0x500041C8UL) //-- DMA Channel Linked List Item registers
#define 	rDMACC6Control      (*(volatile unsigned long *)  0x500041CCUL) //-- DMA channel control registers
#define 	rDMACC6Config       (*(volatile unsigned long *)  0x500041D0UL) //-- DMA Channel Configuration registers
                                                                     
#define 	rDMACC7SrcAddr      (*(volatile unsigned long *)  0x500041E0UL) //-- DMA Channel Source Address registers
#define 	rDMACC7DestAddr     (*(volatile unsigned long *)  0x500041E4UL) //-- DMA Channel Destination Address registers
#define 	rDMACC7LLI          (*(volatile unsigned long *)  0x500041E8UL) //-- DMA Channel Linked List Item registers
#define 	rDMACC7Control      (*(volatile unsigned long *)  0x500041ECUL) //-- DMA channel control registers
#define 	rDMACC7Config       (*(volatile unsigned long *)  0x500041F0UL) //-- DMA Channel Configuration registers
                                                                     
//---- Cortex-M3  core                                               
                                                                     
#define         NVIC_PRIO_BITS    4UL         /*!< standard definition for NVIC Priority Bits */

typedef enum IRQn
{
   /******  Cortex-M3 Processor Exceptions Numbers ***************************************************/

  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

  /******  LPC17xx Specific Interrupt Numbers *******************************************************/

  WDT_IRQn                      = 0,        /*!< Watchdog Timer Interrupt                         */
  TIMER0_IRQn                   = 1,        /*!< Timer0 Interrupt                                 */
  TIMER1_IRQn                   = 2,        /*!< Timer1 Interrupt                                 */
  TIMER2_IRQn                   = 3,        /*!< Timer2 Interrupt                                 */
  TIMER3_IRQn                   = 4,        /*!< Timer3 Interrupt                                 */
  UART0_IRQn                    = 5,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 6,        /*!< UART1 Interrupt                                  */
  UART2_IRQn                    = 7,        /*!< UART2 Interrupt                                  */
  UART3_IRQn                    = 8,        /*!< UART3 Interrupt                                  */
  PWM1_IRQn                     = 9,        /*!< PWM1 Interrupt                                   */
  I2C0_IRQn                     = 10,       /*!< I2C0 Interrupt                                   */
  I2C1_IRQn                     = 11,       /*!< I2C1 Interrupt                                   */
  I2C2_IRQn                     = 12,       /*!< I2C2 Interrupt                                   */
  SPI_IRQn                      = 13,       /*!< SPI Interrupt                                    */
  SSP0_IRQn                     = 14,       /*!< SSP0 Interrupt                                   */
  SSP1_IRQn                     = 15,       /*!< SSP1 Interrupt                                   */
  PLL0_IRQn                     = 16,       /*!< PLL0 Lock (Main PLL) Interrupt                   */
  RTC_IRQn                      = 17,       /*!< Real Time Clock Interrupt                        */
  EINT0_IRQn                    = 18,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 19,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 20,       /*!< External Interrupt 2 Interrupt                   */
  EINT3_IRQn                    = 21,       /*!< External Interrupt 3 Interrupt                   */
  ADC_IRQn                      = 22,       /*!< A/D Converter Interrupt                          */
  BOD_IRQn                      = 23,       /*!< Brown-Out Detect Interrupt                       */
  USB_IRQn                      = 24,       /*!< USB Interrupt                                    */
  CAN_IRQn                      = 25,       /*!< CAN Interrupt                                    */
  DMA_IRQn                      = 26,       /*!< General Purpose DMA Interrupt                    */
  I2S_IRQn                      = 27,       /*!< I2S Interrupt                                    */
  ENET_IRQn                     = 28,       /*!< Ethernet Interrupt                               */
  RIT_IRQn                      = 29,       /*!< Repetitive Interrupt Timer Interrupt             */
  MCPWM_IRQn                    = 30,       /*!< Motor Control PWM Interrupt                      */
  QEI_IRQn                      = 31,       /*!< Quadrature Encoder Interface Interrupt           */
  PLL1_IRQn                     = 32,       /*!< PLL1 Lock (USB PLL) Interrupt                    */
} IRQn_Type;

/* memory mapping struct for Nested Vectored Interrupt Controller (NVIC) */

typedef struct
{
   volatile unsigned int ISER[8];                      /*!< Interrupt Set Enable Register            */
   unsigned int RESERVED0[24];
   volatile unsigned int ICER[8];                      /*!< Interrupt Clear Enable Register          */
   unsigned int RSERVED1[24];
   volatile unsigned int ISPR[8];                      /*!< Interrupt Set Pending Register           */
   unsigned int RESERVED2[24];
   volatile unsigned int ICPR[8];                      /*!< Interrupt Clear Pending Register         */
   unsigned int RESERVED3[24];
   volatile unsigned int IABR[8];                      /*!< Interrupt Active bit Register            */
   unsigned int RESERVED4[56];
   volatile unsigned char IP[240];                      /*!< Interrupt Priority Register, 8Bit wide   */
   unsigned int RESERVED5[644];
   volatile unsigned int STIR;                         /*!< Software Trigger Interrupt Register      */
}NVIC_Type;

/* memory mapping struct for System Control Block */

typedef struct
{
  volatile unsigned int CPUID;                        /*!< CPU ID Base Register                                     */
  volatile unsigned int ICSR;                         /*!< Interrupt Control State Register                         */
  volatile unsigned int VTOR;                         /*!< Vector Table Offset Register                             */
  volatile unsigned int AIRCR;                        /*!< Application Interrupt / Reset Control Register           */
  volatile unsigned int SCR;                          /*!< System Control Register                                  */
  volatile unsigned int CCR;                          /*!< Configuration Control Register                           */
  volatile unsigned char SHP[12];                     /*!< System Handlers Priority Registers (4-7, 8-11, 12-15)    */
  volatile unsigned int SHCSR;                        /*!< System Handler Control and State Register                */
  volatile unsigned int CFSR;                         /*!< Configurable Fault Status Register                       */
  volatile unsigned int HFSR;                         /*!< Hard Fault Status Register                               */
  volatile unsigned int DFSR;                         /*!< Debug Fault Status Register                              */
  volatile unsigned int MMFAR;                        /*!< Mem Manage Address Register                              */
  volatile unsigned int BFAR;                         /*!< Bus Fault Address Register                               */
  volatile unsigned int AFSR;                         /*!< Auxiliary Fault Status Register                          */
  volatile unsigned int PFR[2];                       /*!< Processor Feature Register                               */
  volatile unsigned int DFR;                          /*!< Debug Feature Register                                   */
  volatile unsigned int ADR;                          /*!< Auxiliary Feature Register                               */
  volatile unsigned int MMFR[4];                      /*!< Memory Model Feature Register                            */
  volatile unsigned int ISAR[5];                      /*!< ISA Feature Register                                     */
}SCB_Type;


/* memory mapping struct for SysTick */

typedef struct
{
  volatile unsigned int CTRL;                         /*!< SysTick Control and Status Register */
  volatile unsigned int LOAD;                         /*!< SysTick Reload Value Register       */
  volatile unsigned int VAL;                          /*!< SysTick Current Value Register      */
  volatile unsigned int CALIB;                        /*!< SysTick Calibration Register        */
}SysTick_Type;


/* Memory mapping of Cortex-M3 Hardware */

#define SCS_BASE            (0xE000E000UL)                              /*!< System Control Space Base Address    */
#define ITM_BASE            (0xE0000000UL)                              /*!< ITM Base Address                     */
#define CoreDebug_BASE      (0xE000EDF0UL)                              /*!< Core Debug Base Address              */
#define SysTick_BASE        (SCS_BASE +  0x0010UL)                      /*!< SysTick Base Address                 */
#define NVIC_BASE           (SCS_BASE +  0x0100UL)                      /*!< NVIC Base Address                    */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                      /*!< System Control Block Base Address    */

#define InterruptType       ((InterruptType_Type *) SCS_BASE)         /*!< Interrupt Type Register              */
#define SCB                 ((SCB_Type *)           SCB_BASE)         /*!< SCB configuration struct             */
#define SysTick             ((SysTick_Type *)       SysTick_BASE)     /*!< SysTick configuration struct         */
#define NVIC                ((NVIC_Type *)          NVIC_BASE)        /*!< NVIC configuration struct            */
#define ITM                 ((ITM_Type *)           ITM_BASE)         /*!< ITM configuration struct             */
#define CoreDebug           ((CoreDebug_Type *)     CoreDebug_BASE)   /*!< Core Debug configuration struct      */

// System Reset

#define NVIC_VECTRESET                    0U      //-- Vector Reset Bit
#define NVIC_SYSRESETREQ                  2U      //-- System Reset Request
#define NVIC_AIRCR_VECTKEY     (0x5FAU << 16U)     //-- AIRCR Key for write access
#define NVIC_AIRCR_ENDIANESS             15U      //-- Endianess

// Core Debug

#define CoreDebug_DEMCR_TRCENA    (1U << 24U)      //-- DEMCR TRCENA enable
#define ITM_TCR_ITMENA                   1U       //-- ITM enable

//---- Flash

#define 	rMEMMAP   (*(volatile unsigned long *)  0x400FC040UL) //-- Memory Mapping Control register


    //---- lpc17xx_core.c

void NVIC_SetPriorityGrouping(unsigned int PriorityGroup);
unsigned int NVIC_GetPriorityGrouping(void);
void NVIC_EnableIRQ(IRQn_Type IRQn);
void NVIC_DisableIRQ(IRQn_Type IRQn);
unsigned int NVIC_GetPendingIRQ(IRQn_Type IRQn);
void NVIC_SetPendingIRQ(IRQn_Type IRQn);
void NVIC_ClearPendingIRQ(IRQn_Type IRQn);
unsigned int NVIC_GetActive(IRQn_Type IRQn);
void NVIC_SetPriority(IRQn_Type IRQn, unsigned int priority);
unsigned int NVIC_GetPriority(IRQn_Type IRQn);
unsigned int NVIC_EncodePriority (unsigned int PriorityGroup, unsigned int PreemptPriority, unsigned int SubPriority);
void NVIC_DecodePriority (unsigned int Priority, unsigned int PriorityGroup, unsigned int* pPreemptPriority, unsigned int* pSubPriority);
unsigned int SysTick_Config(unsigned int ticks);
void NVIC_SystemReset(void);

//-------------------------------
#endif /* #ifndef LPC176X_H */



