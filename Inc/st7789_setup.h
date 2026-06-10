#ifndef  __ST7789_SETUP_H__
#define __ST7789_SETUP_H__
void display_init(void);
void cmd(uint8_t comand);
void data(uint8_t data);
void set_window(uint16_t x1,uint16_t y1 ,uint16_t x2,uint16_t y2);
void send_pixel(uint8_t data);
void fill_rectangle(uint16_t color,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void fill_screen(uint16_t color);
void draw_pixel(uint16_t x,
                uint16_t y,
                uint16_t color);
void draw_hline(uint16_t x1,
                uint16_t x2,
                uint16_t y,
                uint16_t color);


void draw_vline(uint16_t x,
                uint16_t y1,
                uint16_t y2,
                uint16_t color);
void draw_rect(uint16_t x1,
               uint16_t y1,
               uint16_t x2,
               uint16_t y2,
               uint16_t color);
void draw_slant_line(uint16_t x1,uint16_t y1,uint16_t x2, uint16_t y2,uint16_t color);
void draw_circle(uint16_t xc,uint16_t yc,uint16_t radius,uint16_t color);
void fill_circle(uint16_t xc,uint16_t yc,uint16_t radius,uint16_t color);

#endif
