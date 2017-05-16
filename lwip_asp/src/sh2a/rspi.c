#include "iodefine.h"
#include "rspi.h"

#define BCLK		48000000UL			/* Clock frequency supplied for SPI module */
//#define CLK_FAST	24000000UL			/* Fast SCLK (working) */
#define CLK_FAST	3200000UL			/* Fast SCLK (working) */
#define	CLK_SLOW	400000UL			/* Fast SCLK (initialize) */
#define	DIV_F		(BCLK/CLK_FAST/2-1)
#define	DIV_S		(BCLK/CLK_SLOW/2-1)
#define FCLK_SLOW()	{RSPI0.SPCR.BIT.SPE = 0; RSPI0.SPBR.BYTE=DIV_S; RSPI0.SPCR.BIT.SPE = 1;}	/* Set slow clock (100k-400k) */
#define FCLK_FAST()	{RSPI0.SPCR.BIT.SPE = 0; RSPI0.SPBR.BYTE=DIV_F; RSPI0.SPCR.BIT.SPE = 1;}	/* Set fast clock (MMC:20MHz max, SD:25MHz max) */

#define CS_LOW()	PORT.PCDR0.BIT.PC7DR = 0
#define CS_HIGH()	PORT.PCDR0.BIT.PC7DR = 1

void spi_init(void)
{
	CS_HIGH();
	CPG.STBCR5.BIT.MSTP51 = 0;	/* SPI0モジュール有効化 */

	/* Attach SPI0 module to I/O pads */
	PORT.PFCR3.WORD = 0x0003;								/* MISO0 */
	PORT.PFCR2.WORD = (PORT.PFCR2.WORD & 0x0F0F) | 0x3030;	/* RSPCK0, MOSI0 */
	PORT.PCIOR0.WORD |= (1 << 7); PORT.PCCR1.WORD &= 0xCFFF;	/* MMC_CS */

	/* Enable SPI0 module */
	RSPI.SPPCR.BYTE = 0;			/* Fixed idle value, disable loop-back mode */
	RSPI.SPSCR.BYTE = 0;			/* Disable sequence control */
	RSPI.SPDCR.BYTE = 0x20;		/* FIFO access width: 8-bit */
	RSPI.SPBFCR.BYTE = 0xF0; RSPI.SPBFCR.BYTE = 0x30;	/* Initialize FIFO */
	RSPI.SPCMD0.WORD = 0x0700;		/* SPI mode: MODE0, 8bit, MSB first */
	RSPI.SPCR.BYTE = 0x48;			/* Start SPI in master mode */

//	FCLK_SLOW();						/* Set slow clock */
	FCLK_FAST();						/* Set slow clock */

	return;
}

void spi_assert(void)
{
	CS_LOW();
	return;
}

void spi_deassert(void)
{
	CS_HIGH();
	return;
}

void spi_wait(void)
{
	while(!RSPI.SPSR.BIT.SPRF);
	return;
}
