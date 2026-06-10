#include "spi_driver.h"
#include <stdlib.h>
#define SPI1_TXE (1U<<1)
#define SPI1_RXE (1U<<0)
#define SPI1_BSY (1U<<7)

void cmd(uint8_t comand);
void data(uint8_t data);
void raw_data_nocs(uint8_t data);
void raw_cmd_nocs(uint8_t comand);
void plot_circle_points(int xc, int yc, int x, int y, uint16_t color);
void fill_circle_line(int xc, int yc, int x, int y, uint16_t color);




void display_init(void)
{
    rst_of();
    delay_ms(50);

    rst_on();
    delay_ms(150);

    cmd(0x11);              // SLPOUT
    delay_ms(120);

    cmd(0x3A);              // COLMOD
    data(0x55);             // RGB565

    cmd(0xB2);              // PORCTRL
    data(0x0C);
    data(0x0C);
    data(0x00);
    data(0x33);
    data(0x33);

    cmd(0xB7);              // GCTRL
    data(0x35);

    cmd(0xBB);              // VCOMS
    data(0x19);

    cmd(0xC0);              // LCMCTRL
    data(0x2C);

    cmd(0xC2);              // VDVVRHEN
    data(0x01);

    cmd(0xC3);              // VRHS
    data(0x12);

    cmd(0xC4);              // VDVS
    data(0x20);

    cmd(0xC6);              // FRCTRL2
    data(0x0F);

    cmd(0xD0);              // PWCTRL1
    data(0xA4);
    data(0xA1);

    cmd(0x36);              // MADCTL
    data(0x00);

    cmd(0xE0);              // Positive Gamma
    data(0xD0);
    data(0x04);
    data(0x0D);
    data(0x11);
    data(0x13);
    data(0x2B);
    data(0x3F);
    data(0x54);
    data(0x4C);
    data(0x18);
    data(0x0D);
    data(0x0B);
    data(0x1F);
    data(0x23);

    cmd(0xE1);              // Negative Gamma
    data(0xD0);
    data(0x04);
    data(0x0C);
    data(0x11);
    data(0x13);
    data(0x2C);
    data(0x3F);
    data(0x44);
    data(0x51);
    data(0x2F);
    data(0x1F);
    data(0x1F);
    data(0x20);
    data(0x23);

    cmd(0x20);              // INVON
    cmd(0x13);              // NORON
    cmd(0x29);              // DISPON

    delay_ms(20);
}
void cmd(uint8_t comand)
{
	cs_en();
	dc_of();
	*((volatile uint8_t*)&SPI1->DR)=comand;
	while((SPI1->SR&SPI1_TXE)==0){}
	while(SPI1->SR&SPI1_BSY){}
	cs_dis();
}
void data(uint8_t data)
{
	cs_en();
	dc_on();
	*((volatile uint8_t*)&SPI1->DR)=data;
	while((SPI1->SR&SPI1_TXE)==0){}
	while(SPI1->SR&SPI1_BSY){}
	cs_dis();
}
void set_window(uint16_t x1,uint16_t y1 ,uint16_t x2,uint16_t y2)
{
	cs_en();
	raw_cmd_nocs(0x2A);
	raw_data_nocs(x1>>8);
	raw_data_nocs(x1&0xFF);
	raw_data_nocs(x2>>8);
	raw_data_nocs(x2&0xFF);
	raw_cmd_nocs(0x2B);
	raw_data_nocs(y1>>8);
	raw_data_nocs(y1&0xFF);
	raw_data_nocs(y2>>8);
	raw_data_nocs(y2&0xFF);
	raw_cmd_nocs(0x2C);
	cs_dis();

}
void send_pixel(uint8_t data)
{

	*((volatile uint8_t*)&SPI1->DR)=data;
	while((SPI1->SR&SPI1_TXE)==0){}
	while(SPI1->SR&SPI1_BSY){}

}
void fill_rectangle(uint16_t color,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	set_window(x1,y1,x2,y2);
		uint8_t high=color>>8;
		uint8_t low =color&0xFF;
	    cs_en();
	    dc_on();
	    for (int i=0;i<((uint32_t)(x2-x1+1)*(y2-y1+1));i++)
	    {
	    send_pixel(high);
	    send_pixel(low);
	    }
	    cs_dis();
}
void fill_screen(uint16_t color)
{
    fill_rectangle(color, 0, 0, 239, 319);
}
void draw_pixel(uint16_t x,
                uint16_t y,
                uint16_t color)
{
	uint8_t high=color>>8;
	uint8_t low =color&0xFF;
	set_window( x, y, x, y);
	dc_on();
	cs_en();
	send_pixel(high);
	send_pixel(low);
	cs_dis();
}
void draw_hline(uint16_t x1,
                uint16_t x2,
                uint16_t y,
                uint16_t color)
{
    fill_rectangle(color, x1, y, x2, y);
}

void draw_vline(uint16_t x,
                uint16_t y1,
                uint16_t y2,
                uint16_t color)
{
    fill_rectangle(color, x, y1, x, y2);
}
void draw_rect(uint16_t x1,
               uint16_t y1,
               uint16_t x2,
               uint16_t y2,
               uint16_t color)
{
    draw_hline(x1, x2, y1, color);
    draw_hline(x1, x2, y2, color);

    draw_vline(x1, y1, y2, color);
    draw_vline(x2, y1, y2, color);
}
void draw_slant_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int dx = abs((int)x2 - (int)x1);
    int dy = abs((int)y2 - (int)y1);
    int sx = (x2 > x1) ? 1 : -1;  // left or right step
    int sy = (y2 > y1) ? 1 : -1;  // up or down step
    int x  = x1;
    int y  = y1;


    if (dy <= dx) {
        // x drives the loop
        int p = 2 * dy - dx;      // Correct initial p for x-drive
        while (1) {
            draw_pixel(x, y, color);
            if (x == x2) break;   // Standardized termination condition

            if (p < 0) {
                p = p + 2 * dy;
            } else {
                y += sy;          // Use sy instead of y++
                p = p + 2 * dy - 2 * dx;
            }
            x += sx;              // Use sx instead of x++
        }
    }
    else {
        // y drives the loop
        int p = 2 * dx - dy;      // Correct initial p for y-drive
        while (1) {
           draw_pixel(x, y, color);
            if (y == y2) break;   // Standardized termination condition

            if (p < 0) {
                p = p + 2 * dx;
            } else {
                x += sx;          // Use sx instead of x++
                p = p + 2 * dx - 2 * dy;
            }
            y += sy;              // Use sy instead of y++
        }
    }
}

void raw_cmd_nocs(uint8_t comand)
{
	dc_of();
	*((volatile uint8_t*)&SPI1->DR)=comand;
	while((SPI1->SR&SPI1_TXE)==0){}
	while(SPI1->SR&SPI1_BSY){}
}
void raw_data_nocs(uint8_t data)
{  	dc_on();
	*((volatile uint8_t*)&SPI1->DR)=data;
	while((SPI1->SR&SPI1_TXE)==0){}
	while(SPI1->SR&SPI1_BSY){}
}
void draw_circle(uint16_t xc,uint16_t yc,uint16_t radius,uint16_t color)
{
int p = 3-radius*2;
int x=0;
int y=radius;
while(x<=y)
{
if(p<0)

{
	plot_circle_points(xc,yc,x,y,color);
	x=x+1;
	p=p+4*x+6;
}
else
{
	plot_circle_points(xc,yc,x,y,color);
	x=x+1;
	y--;
	p=p+4*(x-y)+10;
}
}

}
void plot_circle_points(int xc, int yc, int x, int y, uint16_t color)
{
    draw_pixel(xc + x, yc + y, color);
    draw_pixel(xc - x, yc + y, color);
    draw_pixel(xc + x, yc - y, color);
    draw_pixel(xc - x, yc - y, color);

    draw_pixel(xc + y, yc + x, color);
    draw_pixel(xc - y, yc + x, color);
    draw_pixel(xc + y, yc - x, color);
    draw_pixel(xc - y, yc - x, color);
}

void fill_circle(uint16_t xc,
                 uint16_t yc,
                 uint16_t radius,
                 uint16_t color)
{
    int p = 3 - radius * 2;
    int x = 0;
    int y = radius;

    while (x <= y)
    {
        fill_circle_line(xc, yc, x, y, color);

        if (p < 0)
        {
            x++;
            p = p + 4 * x + 6;
        }
        else
        {
            x++;
            y--;
            p = p + 4 * (x - y) + 10;
        }
    }
}

void fill_circle_line(int xc,
                      int yc,
                      int x,
                      int y,
                      uint16_t color)
{
    draw_hline(xc - x, xc + x, yc + y, color);
    draw_hline(xc - x, xc + x, yc - y, color);

    draw_hline(xc - y, xc + y, yc + x, color);
    draw_hline(xc - y, xc + y, yc - x, color);
}




