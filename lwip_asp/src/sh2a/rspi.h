#ifndef RSPI
#define RSPI		RSPI0
//#define RSPI		RSPI1
#endif

void spi_init(void);
#define spi_data RSPI.SPDR.BYTE
void spi_assert(void);
void spi_deassert(void);
void spi_wait(void);
