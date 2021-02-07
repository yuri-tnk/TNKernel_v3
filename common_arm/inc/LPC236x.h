//----------------------------------------------------------------------------
//   LPC23xx.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
//   The header file is the super set of all hardware definition of the
//   peripherals for the LPC23xx/24xx family microprocessor.
//
//   Copyright(C) 2006, NXP Semiconductor
//   All rights reserved.
//
//   History
//   2005.10.01  ver 1.00    Prelimnary version, first Release
//
//----------------------------------------------------------------------------

#ifndef LPC236x_H_
#define LPC236x_H_


//-- Pin Connect Block ---

#define PINSEL_BASE_ADDR        0xE002C000

#define rPINSEL0        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x00))
#define rPINSEL1        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x04))
#define rPINSEL2        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x08))
#define rPINSEL3        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x0C))
#define rPINSEL4        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x10))
#define rPINSEL5        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x14))
#define rPINSEL6        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x18))
#define rPINSEL7        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x1C))
#define rPINSEL8        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x20))
#define rPINSEL9        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x24))
#define rPINSEL10       (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x28))

#define rPINMODE0        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x40))
#define rPINMODE1        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x44))
#define rPINMODE2        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x48))
#define rPINMODE3        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x4C))
#define rPINMODE4        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x50))
#define rPINMODE5        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x54))
#define rPINMODE6        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x58))
#define rPINMODE7        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x5C))
#define rPINMODE8        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x60))
#define rPINMODE9        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x64))

//-- General Purpose Input/Output (GPIO)

#define GPIO_BASE_ADDR                0xE0028000

#define rIOPIN0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x00))
#define rIOSET0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x04))
#define rIODIR0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x08))
#define rIOCLR0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x0C))
#define rIOPIN1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x10))
#define rIOSET1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x14))
#define rIODIR1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x18))
#define rIOCLR1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x1C))



//--- Vectored Interrupt Controller (VIC) ---

#define VIC_BASE_ADDR        0xFFFFF000

#define rVICIRQStatus   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x000))
#define rVICFIQStatus   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x004))
#define rVICRawIntr     (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x008))
#define rVICIntSelect   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x00C))
#define rVICIntEnable   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x010))
#define rVICIntEnClr    (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x014))
#define rVICSoftInt     (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x018))
#define rVICSoftIntClr  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x01C))
#define rVICProtection  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x020))
#define rVICSWPrioMask  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x024))

#define rVICVectAddr0   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x100))
#define rVICVectAddr1   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x104))
#define rVICVectAddr2   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x108))
#define rVICVectAddr3   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x10C))
#define rVICVectAddr4   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x110))
#define rVICVectAddr5   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x114))
#define rVICVectAddr6   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x118))
#define rVICVectAddr7   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x11C))
#define rVICVectAddr8   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x120))
#define rVICVectAddr9   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x124))
#define rVICVectAddr10  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x128))
#define rVICVectAddr11  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x12C))
#define rVICVectAddr12  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x130))
#define rVICVectAddr13  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x134))
#define rVICVectAddr14  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x138))
#define rVICVectAddr15  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x13C))
#define rVICVectAddr16  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x140))
#define rVICVectAddr17  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x144))
#define rVICVectAddr18  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x148))
#define rVICVectAddr19  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x14C))
#define rVICVectAddr20  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x150))
#define rVICVectAddr21  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x154))
#define rVICVectAddr22  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x158))
#define rVICVectAddr23  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x15C))
#define rVICVectAddr24  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x160))
#define rVICVectAddr25  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x164))
#define rVICVectAddr26  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x168))
#define rVICVectAddr27  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x16C))
#define rVICVectAddr28  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x170))
#define rVICVectAddr29  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x174))
#define rVICVectAddr30  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x178))
#define rVICVectAddr31  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x17C))

//--  The name convention below is from previous LPC2000 family MCUs,
//--  in LPC23xx/24xx, these registers are known as "VICVectPriority(x)".

#define rVICVectCntl0   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x200))
#define rVICVectCntl1   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x204))
#define rVICVectCntl2   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x208))
#define rVICVectCntl3   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x20C))
#define rVICVectCntl4   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x210))
#define rVICVectCntl5   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x214))
#define rVICVectCntl6   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x218))
#define rVICVectCntl7   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x21C))
#define rVICVectCntl8   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x220))
#define rVICVectCntl9   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x224))
#define rVICVectCntl10  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x228))
#define rVICVectCntl11  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x22C))
#define rVICVectCntl12  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x230))
#define rVICVectCntl13  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x234))
#define rVICVectCntl14  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x238))
#define rVICVectCntl15  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x23C))
#define rVICVectCntl16  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x240))
#define rVICVectCntl17  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x244))
#define rVICVectCntl18  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x248))
#define rVICVectCntl19  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x24C))
#define rVICVectCntl20  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x250))
#define rVICVectCntl21  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x254))
#define rVICVectCntl22  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x258))
#define rVICVectCntl23  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x25C))
#define rVICVectCntl24  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x260))
#define rVICVectCntl25  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x264))
#define rVICVectCntl26  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x268))
#define rVICVectCntl27  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x26C))
#define rVICVectCntl28  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x270))
#define rVICVectCntl29  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x274))
#define rVICVectCntl30  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x278))
#define rVICVectCntl31  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x27C))

#define rVICVectAddr    (*(volatile unsigned long *)(VIC_BASE_ADDR + 0xF00))

//-- GPIO Interrupt Registers

#define rIO0_INT_EN_R    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x90))
#define rIO0_INT_EN_F    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x94))
#define rIO0_INT_STAT_R  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x84))
#define rIO0_INT_STAT_F  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x88))
#define rIO0_INT_CLR     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x8C))

#define rIO2_INT_EN_R    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xB0))
#define rIO2_INT_EN_F    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xB4))
#define rIO2_INT_STAT_R  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xA4))
#define rIO2_INT_STAT_F  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xA8))
#define rIO2_INT_CLR     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xAC))

#define rIO_INT_STAT     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x80))

#define PARTCFG_BASE_ADDR                0x3FFF8000

#define rPARTCFG        (*(volatile unsigned long *)(PARTCFG_BASE_ADDR + 0x00))

//-- Fast I/O setup --

#define FIO_BASE_ADDR                0x3FFFC000

#define rFIO0DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x00))
#define rFIO0MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x10))
#define rFIO0PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x14))
#define rFIO0SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x18))
#define rFIO0CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x1C))

#define rFIO1DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x20))
#define rFIO1MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x30))
#define rFIO1PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x34))
#define rFIO1SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x38))
#define rFIO1CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x3C))

#define rFIO2DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x40))
#define rFIO2MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x50))
#define rFIO2PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x54))
#define rFIO2SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x58))
#define rFIO2CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x5C))

#define rFIO3DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x60))
#define rFIO3MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x70))
#define rFIO3PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x74))
#define rFIO3SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x78))
#define rFIO3CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x7C))

#define rFIO4DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x80))
#define rFIO4MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x90))
#define rFIO4PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x94))
#define rFIO4SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x98))
#define rFIO4CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x9C))

//-- FIOs can be accessed through WORD, HALF-WORD or BYTE. --

#define rFIO0DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x00))
#define rFIO1DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x20))
#define rFIO2DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x40))
#define rFIO3DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x60))
#define rFIO4DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x80))

#define rFIO0DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x01))
#define rFIO1DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x21))
#define rFIO2DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x41))
#define rFIO3DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x61))
#define rFIO4DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x81))

#define rFIO0DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x02))
#define rFIO1DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x22))
#define rFIO2DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x42))
#define rFIO3DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x62))
#define rFIO4DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x82))

#define rFIO0DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x03))
#define rFIO1DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x23))
#define rFIO2DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x43))
#define rFIO3DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x63))
#define rFIO4DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x83))

#define rFIO0DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x00))
#define rFIO1DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x20))
#define rFIO2DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x40))
#define rFIO3DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x60))
#define rFIO4DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x80))

#define rFIO0DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x02))
#define rFIO1DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x22))
#define rFIO2DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x42))
#define rFIO3DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x62))
#define rFIO4DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x82))

#define rFIO0MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x10))
#define rFIO1MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x30))
#define rFIO2MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x50))
#define rFIO3MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x70))
#define rFIO4MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x90))

#define rFIO0MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x11))
#define rFIO1MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x21))
#define rFIO2MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x51))
#define rFIO3MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x71))
#define rFIO4MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x91))

#define rFIO0MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x12))
#define rFIO1MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x32))
#define rFIO2MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x52))
#define rFIO3MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x72))
#define rFIO4MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x92))

#define rFIO0MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x13))
#define rFIO1MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x33))
#define rFIO2MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x53))
#define rFIO3MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x73))
#define rFIO4MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x93))

#define rFIO0MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x10))
#define rFIO1MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x30))
#define rFIO2MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x50))
#define rFIO3MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x70))
#define rFIO4MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x90))

#define rFIO0MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x12))
#define rFIO1MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x32))
#define rFIO2MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x52))
#define rFIO3MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x72))
#define rFIO4MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x92))

#define rFIO0PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x14))
#define rFIO1PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x34))
#define rFIO2PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x54))
#define rFIO3PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x74))
#define rFIO4PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x94))

#define rFIO0PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x15))
#define rFIO1PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x25))
#define rFIO2PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x55))
#define rFIO3PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x75))
#define rFIO4PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x95))

#define rFIO0PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x16))
#define rFIO1PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x36))
#define rFIO2PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x56))
#define rFIO3PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x76))
#define rFIO4PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x96))

#define rFIO0PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x17))
#define rFIO1PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x37))
#define rFIO2PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x57))
#define rFIO3PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x77))
#define rFIO4PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x97))

#define rFIO0PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x14))
#define rFIO1PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x34))
#define rFIO2PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x54))
#define rFIO3PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x74))
#define rFIO4PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x94))

#define rFIO0PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x16))
#define rFIO1PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x36))
#define rFIO2PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x56))
#define rFIO3PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x76))
#define rFIO4PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x96))

#define rFIO0SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x18))
#define rFIO1SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x38))
#define rFIO2SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x58))
#define rFIO3SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x78))
#define rFIO4SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x98))

#define rFIO0SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x19))
#define rFIO1SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x29))
#define rFIO2SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x59))
#define rFIO3SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x79))
#define rFIO4SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x99))

#define rFIO0SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1A))
#define rFIO1SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3A))
#define rFIO2SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5A))
#define rFIO3SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7A))
#define rFIO4SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9A))

#define rFIO0SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1B))
#define rFIO1SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3B))
#define rFIO2SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5B))
#define rFIO3SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7B))
#define rFIO4SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9B))

#define rFIO0SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x18))
#define rFIO1SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x38))
#define rFIO2SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x58))
#define rFIO3SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x78))
#define rFIO4SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x98))

#define rFIO0SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1A))
#define rFIO1SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3A))
#define rFIO2SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5A))
#define rFIO3SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7A))
#define rFIO4SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9A))

#define rFIO0CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1C))
#define rFIO1CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3C))
#define rFIO2CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5C))
#define rFIO3CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7C))
#define rFIO4CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9C))

#define rFIO0CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1D))
#define rFIO1CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x2D))
#define rFIO2CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5D))
#define rFIO3CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7D))
#define rFIO4CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9D))

#define rFIO0CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1E))
#define rFIO1CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3E))
#define rFIO2CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5E))
#define rFIO3CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7E))
#define rFIO4CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9E))

#define rFIO0CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1F))
#define rFIO1CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3F))
#define rFIO2CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5F))
#define rFIO3CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7F))
#define rFIO4CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9F))

#define rFIO0CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1C))
#define rFIO1CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3C))
#define rFIO2CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5C))
#define rFIO3CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7C))
#define rFIO4CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9C))

#define rFIO0CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1E))
#define rFIO1CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3E))
#define rFIO2CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5E))
#define rFIO3CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7E))
#define rFIO4CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9E))


//-- System Control Block(SCB) modules include Memory Accelerator Module,
//-- Phase Locked Loop, VPB divider, Power Control, External Interrupt,
//-- Reset, and Code Security/Debugging

#define SCB_BASE_ADDR        0xE01FC000

//-- Memory Accelerator Module (MAM) --

#define rMAMCR          (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x000))
#define rMAMTIM         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x004))
#define rMEMMAP         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x040))

//-- Phase Locked Loop (PLL) --

#define rPLLCON         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x080))
#define rPLLCFG         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x084))
#define rPLLSTAT        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x088))
#define rPLLFEED        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x08C))

//-- Power Control --

#define rPCON           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x0C0))
#define rPCONP          (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x0C4))

//-- Clock Divider --

#define rCCLKCFG        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x104))
#define rUSBCLKCFG      (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x108))
#define rCLKSRCSEL      (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x10C))
#define rPCLKSEL0       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1A8))
#define rPCLKSEL1       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1AC))

//-- External Interrupts --

#define rEXTINT         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x140))
#define rINTWAKE        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x144))
#define rEXTMODE        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x148))
#define rEXTPOLAR       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x14C))

//-- Reset, reset source identification --

#define rRSIR           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x180))

//-- RSID, code security protection --

#define rCSPR           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x184))

//-- AHB configuration --

#define rAHBCFG1        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x188))
#define rAHBCFG2        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x18C))

//-- System Controls and Status --

#define rSCS            (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1A0))

//-- MPMC(EMC) registers, note: all the external memory controller(EMC)
//-- registers are for LPC24xx only.

#define rSTATIC_MEM0_BASE                0x80000000
#define rSTATIC_MEM1_BASE                0x81000000
#define rSTATIC_MEM2_BASE                0x82000000
#define rSTATIC_MEM3_BASE                0x83000000

#define rDYNAMIC_MEM0_BASE                0xA0000000
#define rDYNAMIC_MEM1_BASE                0xB0000000
#define rDYNAMIC_MEM2_BASE                0xC0000000
#define rDYNAMIC_MEM3_BASE                0xD0000000

//-- External Memory Controller (EMC) --

#define EMC_BASE_ADDR                0xFFE08000

#define rEMC_CTRL       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x000))
#define rEMC_STAT       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x004))
#define rEMC_CONFIG     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x008))

//-- Dynamic RAM access registers --

#define rEMC_DYN_CTRL     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x020))
#define rEMC_DYN_RFSH     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x024))
#define rEMC_DYN_RD_CFG   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x028))
#define rEMC_DYN_RP       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x030))
#define rEMC_DYN_RAS      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x034))
#define rEMC_DYN_SREX     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x038))
#define rEMC_DYN_APR      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x03C))
#define rEMC_DYN_DAL      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x040))
#define rEMC_DYN_WR       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x044))
#define rEMC_DYN_RC       (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x048))
#define rEMC_DYN_RFC      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x04C))
#define rEMC_DYN_XSR      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x050))
#define rEMC_DYN_RRD      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x054))
#define rEMC_DYN_MRD      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x058))

#define rEMC_DYN_CFG0     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x100))
#define rEMC_DYN_RASCAS0  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x104))
#define rEMC_DYN_CFG1     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x140))
#define rEMC_DYN_RASCAS1  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x144))
#define rEMC_DYN_CFG2     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x160))
#define rEMC_DYN_RASCAS2  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x164))
#define rEMC_DYN_CFG3     (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x180))
#define rEMC_DYN_RASCAS3  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x184))

//-- static RAM access registers --

#define rEMC_STA_CFG0      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x200))
#define rEMC_STA_WAITWEN0  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x204))
#define rEMC_STA_WAITOEN0  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x208))
#define rEMC_STA_WAITRD0   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x20C))
#define rEMC_STA_WAITPAGE0 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x210))
#define rEMC_STA_WAITWR0   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x214))
#define rEMC_STA_WAITTURN0 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x218))

#define rEMC_STA_CFG1      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x220))
#define rEMC_STA_WAITWEN1  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x224))
#define rEMC_STA_WAITOEN1  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x228))
#define rEMC_STA_WAITRD1   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x22C))
#define rEMC_STA_WAITPAGE1 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x230))
#define rEMC_STA_WAITWR1   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x234))
#define rEMC_STA_WAITTURN1 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x238))

#define rEMC_STA_CFG2      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x240))
#define rEMC_STA_WAITWEN2  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x244))
#define rEMC_STA_WAITOEN2  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x248))
#define rEMC_STA_WAITRD2   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x24C))
#define rEMC_STA_WAITPAGE2 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x250))
#define rEMC_STA_WAITWR2   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x254))
#define rEMC_STA_WAITTURN2 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x258))

#define rEMC_STA_CFG3      (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x260))
#define rEMC_STA_WAITWEN3  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x264))
#define rEMC_STA_WAITOEN3  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x268))
#define rEMC_STA_WAITRD3   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x26C))
#define rEMC_STA_WAITPAGE3 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x270))
#define rEMC_STA_WAITWR3   (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x274))
#define rEMC_STA_WAITTURN3 (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x278))

#define rEMC_STA_EXT_WAIT  (*(volatile unsigned long *)(EMC_BASE_ADDR + 0x880))

//-- Timer 0 --

#define TMR0_BASE_ADDR                0xE0004000

#define rT0IR           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x00))
#define rT0TCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x04))
#define rT0TC           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x08))
#define rT0PR           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x0C))
#define rT0PC           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x10))
#define rT0MCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x14))
#define rT0MR0          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x18))
#define rT0MR1          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x1C))
#define rT0MR2          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x20))
#define rT0MR3          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x24))
#define rT0CCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x28))
#define rT0CR0          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x2C))
#define rT0CR1          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x30))
#define rT0CR2          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x34))
#define rT0CR3          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x38))
#define rT0EMR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x3C))
#define rT0CTCR         (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x70))

//-- Timer 1 --

#define TMR1_BASE_ADDR                0xE0008000

#define rT1IR           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x00))
#define rT1TCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x04))
#define rT1TC           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x08))
#define rT1PR           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x0C))
#define rT1PC           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x10))
#define rT1MCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x14))
#define rT1MR0          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x18))
#define rT1MR1          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x1C))
#define rT1MR2          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x20))
#define rT1MR3          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x24))
#define rT1CCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x28))
#define rT1CR0          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x2C))
#define rT1CR1          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x30))
#define rT1CR2          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x34))
#define rT1CR3          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x38))
#define rT1EMR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x3C))
#define rT1CTCR         (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x70))

//-- Timer 2 --

#define TMR2_BASE_ADDR                0xE0070000

#define rT2IR           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x00))
#define rT2TCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x04))
#define rT2TC           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x08))
#define rT2PR           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x0C))
#define rT2PC           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x10))
#define rT2MCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x14))
#define rT2MR0          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x18))
#define rT2MR1          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x1C))
#define rT2MR2          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x20))
#define rT2MR3          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x24))
#define rT2CCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x28))
#define rT2CR0          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x2C))
#define rT2CR1          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x30))
#define rT2CR2          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x34))
#define rT2CR3          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x38))
#define rT2EMR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x3C))
#define rT2CTCR         (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x70))

//-- Timer 3 --

#define TMR3_BASE_ADDR                0xE0074000

#define rT3IR           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x00))
#define rT3TCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x04))
#define rT3TC           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x08))
#define rT3PR           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x0C))
#define rT3PC           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x10))
#define rT3MCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x14))
#define rT3MR0          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x18))
#define rT3MR1          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x1C))
#define rT3MR2          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x20))
#define rT3MR3          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x24))
#define rT3CCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x28))
#define rT3CR0          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x2C))
#define rT3CR1          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x30))
#define rT3CR2          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x34))
#define rT3CR3          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x38))
#define rT3EMR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x3C))
#define rT3CTCR         (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x70))


//-- Pulse Width Modulator (PWM) --

#define PWM0_BASE_ADDR                0xE0014000

#define rPWM0IR          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x00))
#define rPWM0TCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x04))
#define rPWM0TC          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x08))
#define rPWM0PR          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x0C))
#define rPWM0PC          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x10))
#define rPWM0MCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x14))
#define rPWM0MR0         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x18))
#define rPWM0MR1         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x1C))
#define rPWM0MR2         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x20))
#define rPWM0MR3         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x24))
#define rPWM0CCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x28))
#define rPWM0CR0         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x2C))
#define rPWM0CR1         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x30))
#define rPWM0CR2         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x34))
#define rPWM0CR3         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x38))
#define rPWM0EMR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x3C))
#define rPWM0MR4         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x40))
#define rPWM0MR5         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x44))
#define rPWM0MR6         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x48))
#define rPWM0PCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x4C))
#define rPWM0LER         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x50))
#define rPWM0CTCR        (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x70))

#define PWM1_BASE_ADDR                0xE0018000

#define rPWM1IR          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x00))
#define rPWM1TCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x04))
#define rPWM1TC          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x08))
#define rPWM1PR          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x0C))
#define rPWM1PC          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x10))
#define rPWM1MCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x14))
#define rPWM1MR0         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x18))
#define rPWM1MR1         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x1C))
#define rPWM1MR2         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x20))
#define rPWM1MR3         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x24))
#define rPWM1CCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x28))
#define rPWM1CR0         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x2C))
#define rPWM1CR1         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x30))
#define rPWM1CR2         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x34))
#define rPWM1CR3         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x38))
#define rPWM1EMR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x3C))
#define rPWM1MR4         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x40))
#define rPWM1MR5         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x44))
#define rPWM1MR6         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x48))
#define rPWM1PCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x4C))
#define rPWM1LER         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x50))
#define rPWM1CTCR        (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x70))

//-- Universal Asynchronous Receiver Transmitter 0 (UART0) --

#define UART0_BASE_ADDR                0xE000C000

#define rU0RBR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define rU0THR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define rU0DLL          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define rU0DLM          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x04))
#define rU0IER          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x04))
#define rU0IIR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x08))
#define rU0FCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x08))
#define rU0LCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x0C))
#define rU0LSR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x14))
#define rU0SCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x1C))
#define rU0ACR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x20))
#define rU0ICR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x24))
#define rU0FDR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x28))
#define rU0TER          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x30))

//-- Universal Asynchronous Receiver Transmitter 1 (UART1) --

#define UART1_BASE_ADDR                0xE0010000

#define rU1RBR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define rU1THR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define rU1DLL          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define rU1DLM          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x04))
#define rU1IER          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x04))
#define rU1IIR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x08))
#define rU1FCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x08))
#define rU1LCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x0C))
#define rU1MCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x10))
#define rU1LSR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x14))
#define rU1MSR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x18))
#define rU1SCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x1C))
#define rU1ACR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x20))
#define rU1FDR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x28))
#define rU1TER          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x30))

//-- Universal Asynchronous Receiver Transmitter 2 (UART2) --

#define UART2_BASE_ADDR                0xE0078000

#define rU2RBR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define rU2THR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define rU2DLL          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define rU2DLM          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x04))
#define rU2IER          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x04))
#define rU2IIR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x08))
#define rU2FCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x08))
#define rU2LCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x0C))
#define rU2LSR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x14))
#define rU2SCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x1C))
#define rU2ACR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x20))
#define rU2ICR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x24))
#define rU2FDR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x28))
#define rU2TER          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x30))

//-- Universal Asynchronous Receiver Transmitter 3 (UART3) --

#define UART3_BASE_ADDR                0xE007C000

#define rU3RBR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define rU3THR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define rU3DLL          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define rU3DLM          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x04))
#define rU3IER          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x04))
#define rU3IIR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x08))
#define rU3FCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x08))
#define rU3LCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x0C))
#define rU3LSR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x14))
#define rU3SCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x1C))
#define rU3ACR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x20))
#define rU3ICR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x24))
#define rU3FDR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x28))
#define rU3TER          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x30))

//-- I2C Interface 0 --

#define I2C0_BASE_ADDR                0xE001C000

#define rI20CONSET      (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x00))
#define rI20STAT        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x04))
#define rI20DAT         (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x08))
#define rI20ADR         (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x0C))
#define rI20SCLH        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x10))
#define rI20SCLL        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x14))
#define rI20CONCLR      (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x18))

//-- I2C Interface 1 --

#define I2C1_BASE_ADDR                0xE005C000

#define rI21CONSET      (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x00))
#define rI21STAT        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x04))
#define rI21DAT         (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x08))
#define rI21ADR         (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x0C))
#define rI21SCLH        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x10))
#define rI21SCLL        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x14))
#define rI21CONCLR      (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x18))

//-- I2C Interface 2 --

#define I2C2_BASE_ADDR                0xE0080000

#define rI22CONSET      (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x00))
#define rI22STAT        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x04))
#define rI22DAT         (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x08))
#define rI22ADR         (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x0C))
#define rI22SCLH        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x10))
#define rI22SCLL        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x14))
#define rI22CONCLR      (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x18))

//-- SPI0 (Serial Peripheral Interface 0) --

#define SPI0_BASE_ADDR                0xE0020000

#define rS0SPCR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x00))
#define rS0SPSR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x04))
#define rS0SPDR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x08))
#define rS0SPCCR        (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x0C))
#define rS0SPINT        (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x1C))

//-- SSP0 Controller --

#define SSP0_BASE_ADDR                0xE0068000

#define rSSP0CR0        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x00))
#define rSSP0CR1        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x04))
#define rSSP0DR         (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x08))
#define rSSP0SR         (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x0C))
#define rSSP0CPSR       (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x10))
#define rSSP0IMSC       (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x14))
#define rSSP0RIS        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x18))
#define rSSP0MIS        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x1C))
#define rSSP0ICR        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x20))
#define rSSP0DMACR      (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x24))

//-- SSP1 Controller --

#define SSP1_BASE_ADDR                0xE0030000

#define rSSP1CR0        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x00))
#define rSSP1CR1        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x04))
#define rSSP1DR         (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x08))
#define rSSP1SR         (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x0C))
#define rSSP1CPSR       (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x10))
#define rSSP1IMSC       (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x14))
#define rSSP1RIS        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x18))
#define rSSP1MIS        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x1C))
#define rSSP1ICR        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x20))
#define rSSP1DMACR      (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x24))

//-- Real Time Clock --

#define RTC_BASE_ADDR                0xE0024000

#define rRTC_ILR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x00))
#define rRTC_CTC         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x04))
#define rRTC_CCR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x08))
#define rRTC_CIIR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x0C))
#define rRTC_AMR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x10))
#define rRTC_CTIME0      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x14))
#define rRTC_CTIME1      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x18))
#define rRTC_CTIME2      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x1C))
#define rRTC_SEC         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x20))
#define rRTC_MIN         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x24))
#define rRTC_HOUR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x28))
#define rRTC_DOM         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x2C))
#define rRTC_DOW         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x30))
#define rRTC_DOY         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x34))
#define rRTC_MONTH       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x38))
#define rRTC_YEAR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x3C))
#define rRTC_CISS        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x40))
#define rRTC_ALSEC       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x60))
#define rRTC_ALMIN       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x64))
#define rRTC_ALHOUR      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x68))
#define rRTC_ALDOM       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x6C))
#define rRTC_ALDOW       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x70))
#define rRTC_ALDOY       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x74))
#define rRTC_ALMON       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x78))
#define rRTC_ALYEAR      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x7C))
#define rRTC_PREINT      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x80))
#define rRTC_PREFRAC     (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x84))

//-- A/D Converter 0 (AD0) --

#define AD0_BASE_ADDR                0xE0034000

#define rAD0CR          (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x00))
#define rAD0GDR         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x04))
#define rAD0INTEN       (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x0C))
#define rAD0DR0         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x10))
#define rAD0DR1         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x14))
#define rAD0DR2         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x18))
#define rAD0DR3         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x1C))
#define rAD0DR4         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x20))
#define rAD0DR5         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x24))
#define rAD0DR6         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x28))
#define rAD0DR7         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x2C))
#define rAD0STAT        (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x30))

//-- D/A Converter --

#define DAC_BASE_ADDR                0xE006C000
#define rDACR           (*(volatile unsigned long *)(DAC_BASE_ADDR + 0x00))

//-- Watchdog --

#define WDG_BASE_ADDR                0xE0000000

#define rWDMOD          (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x00))
#define rWDTC           (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x04))
#define rWDFEED         (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x08))
#define rWDTV           (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x0C))
#define rWDCLKSEL       (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x10))

//-- CAN CONTROLLERS AND ACCEPTANCE FILTER --

#define CAN_ACCEPT_BASE_ADDR                0xE003C000

#define rCAN_AFMR        (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x00))
#define rCAN_SFF_SA      (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x04))
#define rCAN_SFF_GRP_SA  (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x08))
#define rCAN_EFF_SA      (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x0C))
#define rCAN_EFF_GRP_SA  (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x10))
#define rCAN_EOT         (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x14))
#define rCAN_LUT_ERR_ADR (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x18))
#define rCAN_LUT_ERR     (*(volatile unsigned long *)(CAN_ACCEPT_BASE_ADDR + 0x1C))

#define CAN_CENTRAL_BASE_ADDR                0xE0040000

#define rCAN_TX_SR       (*(volatile unsigned long *)(CAN_CENTRAL_BASE_ADDR + 0x00))
#define rCAN_RX_SR       (*(volatile unsigned long *)(CAN_CENTRAL_BASE_ADDR + 0x04))
#define rCAN_MSR         (*(volatile unsigned long *)(CAN_CENTRAL_BASE_ADDR + 0x08))

#define CAN1_BASE_ADDR                0xE0044000

#define rCAN1MOD         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x00))
#define rCAN1CMR         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x04))
#define rCAN1GSR         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x08))
#define rCAN1ICR         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x0C))
#define rCAN1IER         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x10))
#define rCAN1BTR         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x14))
#define rCAN1EWL         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x18))
#define rCAN1SR          (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x1C))
#define rCAN1RFS         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x20))
#define rCAN1RID         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x24))
#define rCAN1RDA         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x28))
#define rCAN1RDB         (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x2C))

#define rCAN1TFI1        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x30))
#define rCAN1TID1        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x34))
#define rCAN1TDA1        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x38))
#define rCAN1TDB1        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x3C))
#define rCAN1TFI2        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x40))
#define rCAN1TID2        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x44))
#define rCAN1TDA2        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x48))
#define rCAN1TDB2        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x4C))
#define rCAN1TFI3        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x50))
#define rCAN1TID3        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x54))
#define rCAN1TDA3        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x58))
#define rCAN1TDB3        (*(volatile unsigned long *)(CAN1_BASE_ADDR + 0x5C))

#define CAN2_BASE_ADDR                0xE0048000

#define rCAN2MOD         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x00))
#define rCAN2CMR         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x04))
#define rCAN2GSR         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x08))
#define rCAN2ICR         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x0C))
#define rCAN2IER         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x10))
#define rCAN2BTR         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x14))
#define rCAN2EWL         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x18))
#define rCAN2SR          (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x1C))
#define rCAN2RFS         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x20))
#define rCAN2RID         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x24))
#define rCAN2RDA         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x28))
#define rCAN2RDB         (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x2C))

#define rCAN2TFI1        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x30))
#define rCAN2TID1        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x34))
#define rCAN2TDA1        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x38))
#define rCAN2TDB1        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x3C))
#define rCAN2TFI2        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x40))
#define rCAN2TID2        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x44))
#define rCAN2TDA2        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x48))
#define rCAN2TDB2        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x4C))
#define rCAN2TFI3        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x50))
#define rCAN2TID3        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x54))
#define rCAN2TDA3        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x58))
#define rCAN2TDB3        (*(volatile unsigned long *)(CAN2_BASE_ADDR + 0x5C))

//-- MultiMedia Card Interface(MCI) Controller ---

#define MCI_BASE_ADDR                0xE008C000

#define rMCI_POWER       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x00))
#define rMCI_CLOCK       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x04))
#define rMCI_ARGUMENT    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x08))
#define rMCI_COMMAND     (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x0C))
#define rMCI_RESP_CMD    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x10))
#define rMCI_RESP0       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x14))
#define rMCI_RESP1       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x18))
#define rMCI_RESP2       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x1C))
#define rMCI_RESP3       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x20))
#define rMCI_DATA_TMR    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x24))
#define rMCI_DATA_LEN    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x28))
#define rMCI_DATA_CTRL   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x2C))
#define rMCI_DATA_CNT    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x30))
#define rMCI_STATUS      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x34))
#define rMCI_CLEAR       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x38))
#define rMCI_MASK0       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x3C))
#define rMCI_MASK1       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x40))
#define rMCI_FIFO_CNT    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x48))
#define rMCI_FIFO        (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x80))

//-- I2S Interface Controller (I2S)

#define I2S_BASE_ADDR                0xE0088000

#define rI2S_DAO        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x00))
#define rI2S_DAI        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x04))
#define rI2S_TX_FIFO    (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x08))
#define rI2S_RX_FIFO    (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x0C))
#define rI2S_STATE      (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x10))
#define rI2S_DMA1       (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x14))
#define rI2S_DMA2       (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x18))
#define rI2S_IRQ        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x1C))
#define rI2S_TXRATE     (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x20))
#define rI2S_RXRATE     (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x24))

//-- General-purpose DMA Controller --

#define DMA_BASE_ADDR                0xFFE04000

#define rGPDMA_INT_STAT         (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x000))
#define rGPDMA_INT_TCSTAT       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x004))
#define rGPDMA_INT_TCCLR        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x008))
#define rGPDMA_INT_ERR_STAT     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x00C))
#define rGPDMA_INT_ERR_CLR      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x010))
#define rGPDMA_RAW_INT_TCSTAT   (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x014))
#define rGPDMA_RAW_INT_ERR_STAT (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x018))
#define rGPDMA_ENABLED_CHNS     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x01C))
#define rGPDMA_SOFT_BREQ        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x020))
#define rGPDMA_SOFT_SREQ        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x024))
#define rGPDMA_SOFT_LBREQ       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x028))
#define rGPDMA_SOFT_LSREQ       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x02C))
#define rGPDMA_CONFIG           (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x030))
#define rGPDMA_SYNC             (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x034))

//-- DMA channel 0 registers --

#define rGPDMA_CH0_SRC      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x100))
#define rGPDMA_CH0_DEST     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x104))
#define rGPDMA_CH0_LLI      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x108))
#define rGPDMA_CH0_CTRL     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x10C))
#define rGPDMA_CH0_CFG      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x110))

//-- DMA channel 1 registers --

#define rGPDMA_CH1_SRC      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x120))
#define rGPDMA_CH1_DEST     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x124))
#define rGPDMA_CH1_LLI      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x128))
#define rGPDMA_CH1_CTRL     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x12C))
#define rGPDMA_CH1_CFG      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x130))


//-- USB Controller --

#define USB_INT_BASE_ADDR  0xE01FC1C0

#define USB_BASE_ADDR      0xFFE0C200                //-- USB Base Address

#define rUSB_INT_STAT    (*(volatile unsigned long *)(USB_INT_BASE_ADDR + 0x00))

//-- USB Device Interrupt Registers --

#define rDEV_INT_STAT    (*(volatile unsigned long *)(USB_BASE_ADDR + 0x00))
#define rDEV_INT_EN      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x04))
#define rDEV_INT_CLR     (*(volatile unsigned long *)(USB_BASE_ADDR + 0x08))
#define rDEV_INT_SET     (*(volatile unsigned long *)(USB_BASE_ADDR + 0x0C))
#define rDEV_INT_PRIO    (*(volatile unsigned long *)(USB_BASE_ADDR + 0x2C))

//-- USB Device Endpoint Interrupt Registers --

#define rEP_INT_STAT     (*(volatile unsigned long *)(USB_BASE_ADDR + 0x30))
#define rEP_INT_EN       (*(volatile unsigned long *)(USB_BASE_ADDR + 0x34))
#define rEP_INT_CLR      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x38))
#define rEP_INT_SET      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x3C))
#define rEP_INT_PRIO     (*(volatile unsigned long *)(USB_BASE_ADDR + 0x40))

//-- USB Device Endpoint Realization Registers --

#define rREALIZE_EP      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x44))
#define rEP_INDEX        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x48))
#define rMAXPACKET_SIZE  (*(volatile unsigned long *)(USB_BASE_ADDR + 0x4C))

//-- USB Device Command Reagisters --

#define rCMD_CODE        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x10))
#define rCMD_DATA        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x14))

//-- USB Device Data Transfer Registers --

#define rRX_DATA         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x18))
#define rTX_DATA         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x1C))
#define rRX_PLENGTH      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x20))
#define rTX_PLENGTH      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x24))
#define rUSB_CTRL        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x28))

//-- USB Device DMA Registers --

#define rDMA_REQ_STAT      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x50))
#define rDMA_REQ_CLR       (*(volatile unsigned long *)(USB_BASE_ADDR + 0x54))
#define rDMA_REQ_SET       (*(volatile unsigned long *)(USB_BASE_ADDR + 0x58))
#define rUDCA_HEAD         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x80))
#define rEP_DMA_STAT       (*(volatile unsigned long *)(USB_BASE_ADDR + 0x84))
#define rEP_DMA_EN         (*(volatile unsigned long *)(USB_BASE_ADDR + 0x88))
#define rEP_DMA_DIS        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x8C))
#define rDMA_INT_STAT      (*(volatile unsigned long *)(USB_BASE_ADDR + 0x90))
#define rDMA_INT_EN        (*(volatile unsigned long *)(USB_BASE_ADDR + 0x94))
#define rEOT_INT_STAT      (*(volatile unsigned long *)(USB_BASE_ADDR + 0xA0))
#define rEOT_INT_CLR       (*(volatile unsigned long *)(USB_BASE_ADDR + 0xA4))
#define rEOT_INT_SET       (*(volatile unsigned long *)(USB_BASE_ADDR + 0xA8))
#define rNDD_REQ_INT_STAT  (*(volatile unsigned long *)(USB_BASE_ADDR + 0xAC))
#define rNDD_REQ_INT_CLR   (*(volatile unsigned long *)(USB_BASE_ADDR + 0xB0))
#define rNDD_REQ_INT_SET   (*(volatile unsigned long *)(USB_BASE_ADDR + 0xB4))
#define rSYS_ERR_INT_STAT  (*(volatile unsigned long *)(USB_BASE_ADDR + 0xB8))
#define rSYS_ERR_INT_CLR   (*(volatile unsigned long *)(USB_BASE_ADDR + 0xBC))
#define rSYS_ERR_INT_SET   (*(volatile unsigned long *)(USB_BASE_ADDR + 0xC0))

//-- USB Host and OTG registers are for LPC24xx only --
//-- USB Host Controller --

#define USBHC_BASE_ADDR                0xFFE0C000

#define rHC_REVISION        (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x00))
#define rHC_CONTROL         (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x04))
#define rHC_CMD_STAT        (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x08))
#define rHC_INT_STAT        (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x0C))
#define rHC_INT_EN          (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x10))
#define rHC_INT_DIS         (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x14))
#define rHC_HCCA            (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x18))
#define rHC_PERIOD_CUR_ED   (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x1C))
#define rHC_CTRL_HEAD_ED    (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x20))
#define rHC_CTRL_CUR_ED     (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x24))
#define rHC_BULK_HEAD_ED    (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x28))
#define rHC_BULK_CUR_ED     (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x2C))
#define rHC_DONE_HEAD       (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x30))
#define rHC_FM_INTERVAL     (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x34))
#define rHC_FM_REMAINING    (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x38))
#define rHC_FM_NUMBER       (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x3C))
#define rHC_PERIOD_START    (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x40))
#define rHC_LS_THRHLD       (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x44))
#define rHC_RH_DESCA        (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x48))
#define rHC_RH_DESCB        (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x4C))
#define rHC_RH_STAT         (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x50))
#define rHC_RH_PORT_STAT1   (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x54))
#define rHC_RH_PORT_STAT2   (*(volatile unsigned long *)(USBHC_BASE_ADDR + 0x58))

//-- USB OTG Controller --

#define USBOTG_BASE_ADDR        0xFFE0C100

#define rOTG_INT_STAT       (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x00))
#define rOTG_INT_EN         (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x04))
#define rOTG_INT_SET        (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x08))
#define rOTG_INT_CLR        (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x0C))

//-- On LPC23xx, the name is USBPortSel, on LPC24xx, the name is OTG_STAT_CTRL

#define rOTG_STAT_CTRL      (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x10))
#define rOTG_TIMER          (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x14))

#define USBOTG_I2C_BASE_ADDR        0xFFE0C300

#define rOTG_I2C_RX         (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x00))
#define rOTG_I2C_TX         (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x00))
#define rOTG_I2C_STS        (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x04))
#define rOTG_I2C_CTL        (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x08))
#define rOTG_I2C_CLKHI      (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x0C))
#define rOTG_I2C_CLKLO      (*(volatile unsigned long *)(USBOTG_I2C_BASE_ADDR + 0x10))

//-- On LPC23xx, the names are USBClkCtrl and USBClkSt; on LPC24xx, the names
//-- are OTG_CLK_CTRL and OTG_CLK_STAT respectively.

#define USBOTG_CLK_BASE_ADDR        0xFFE0CFF0

#define rOTG_CLK_CTRL        (*(volatile unsigned long *)(USBOTG_CLK_BASE_ADDR + 0x04))
#define rOTG_CLK_STAT        (*(volatile unsigned long *)(USBOTG_CLK_BASE_ADDR + 0x08))

//-- Note: below three register name convention is for LPC23xx USB device only,
//-- match with the spec. update in USB Device Section.

#define rUSBPortSel          (*(volatile unsigned long *)(USBOTG_BASE_ADDR + 0x10))
#define rUSBClkCtrl          (*(volatile unsigned long *)(USBOTG_CLK_BASE_ADDR + 0x04))
#define rUSBClkSt            (*(volatile unsigned long *)(USBOTG_CLK_BASE_ADDR + 0x08))

//-- Ethernet MAC (32 bit data bus) -- all registers are RW unless indicated in parentheses

#define MAC_BASE_ADDR                0xFFE00000 //-- AHB Peripheral # 0

#define rMAC_MAC1            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x000)) // MAC config reg 1
#define rMAC_MAC2            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x004)) // MAC config reg 2
#define rMAC_IPGT            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x008)) // b2b InterPacketGap reg
#define rMAC_IPGR            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x00C)) // non b2b InterPacketGap reg
#define rMAC_CLRT            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x010)) // CoLlision window/ReTry reg
#define rMAC_MAXF            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x014)) // MAXimum Frame reg
#define rMAC_SUPP            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x018)) // PHY SUPPort reg
#define rMAC_TEST            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x01C)) // TEST reg
#define rMAC_MCFG            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x020)) // MII Mgmt ConFiG reg
#define rMAC_MCMD            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x024)) // MII Mgmt CoMmanD reg
#define rMAC_MADR            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x028)) // MII Mgmt ADdRess reg
#define rMAC_MWTD            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x02C)) // MII Mgmt WriTe Data reg (WO)
#define rMAC_MRDD            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x030)) // MII Mgmt ReaD Data reg (RO)
#define rMAC_MIND            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x034)) // MII Mgmt INDicators reg (RO)

#define rMAC_SA0             (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x040)) // Station Address 0 reg
#define rMAC_SA1             (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x044)) // Station Address 1 reg
#define rMAC_SA2             (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x048)) // Station Address 2 reg

#define rMAC_COMMAND         (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x100)) // Command reg
#define rMAC_STATUS          (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x104)) // Status reg (RO)
#define rMAC_RXDESCRIPTOR    (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x108)) // Rx descriptor base address reg
#define rMAC_RXSTATUS        (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x10C)) // Rx status base address reg
#define rMAC_RXDESCRIPTORNUM (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x110)) // Rx number of descriptors reg
#define rMAC_RXPRODUCEINDEX  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x114)) // Rx produce index reg (RO)
#define rMAC_RXCONSUMEINDEX  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x118)) // Rx consume index reg
#define rMAC_TXDESCRIPTOR    (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x11C)) // Tx descriptor base address reg
#define rMAC_TXSTATUS        (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x120)) // Tx status base address reg
#define rMAC_TXDESCRIPTORNUM (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x124)) // Tx number of descriptors reg
#define rMAC_TXPRODUCEINDEX  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x128)) // Tx produce index reg
#define rMAC_TXCONSUMEINDEX  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x12C)) // Tx consume index reg (RO)

#define rMAC_TSV0            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x158)) // Tx status vector 0 reg (RO)
#define rMAC_TSV1            (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x15C)) // Tx status vector 1 reg (RO)
#define rMAC_RSV             (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x160)) // Rx status vector reg (RO)

#define rMAC_FLOWCONTROLCNT  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x170)) // Flow control counter reg
#define rMAC_FLOWCONTROLSTS  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x174)) // Flow control status reg

#define rMAC_RXFILTERCTRL    (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x200)) // Rx filter ctrl reg
#define rMAC_RXFILTERWOLSTS  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x204)) // Rx filter WoL status reg (RO)
#define rMAC_RXFILTERWOLCLR  (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x208)) // Rx filter WoL clear reg (WO)

#define rMAC_HASHFILTERL     (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x210)) // Hash filter LSBs reg
#define rMAC_HASHFILTERH     (*(volatile unsigned long *)(MAC_BASE_ADDR + 0x214)) // Hash filter MSBs reg

#define rMAC_INTSTATUS       (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFE0)) // Interrupt status reg (RO)
#define rMAC_INTENABLE       (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFE4)) // Interrupt enable reg
#define rMAC_INTCLEAR        (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFE8)) // Interrupt clear reg (WO)
#define rMAC_INTSET          (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFEC)) // Interrupt set reg (WO)

#define rMAC_POWERDOWN       (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFF4)) // Power-down reg
#define rMAC_MODULEID        (*(volatile unsigned long *)(MAC_BASE_ADDR + 0xFFC)) // Module ID reg (RO)

#endif




