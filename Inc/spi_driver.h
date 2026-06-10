#ifndef __SPI_DRIVER_H__
#define __SPI_DRIVER_H__
#include "stm32f4xx.h"
#include "delay_ms.h"
void spi_config(void);
void cs_en(void);
void cs_dis(void);
void dc_on(void);
void dc_of(void);
void rst_on(void);
void rst_of(void);


#endif
