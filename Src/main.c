#include "spi_driver.h"
#include "spi_write.h"
#include "spi_read.h"
#include "st7789_setup.h"
#include "delay_ms.h"
#define SPI1_EN  (1U<<6)
#define SPI1_TXE (1U<<1)
#define SPI1_RXE (1U<<0)
#define SPI1_BSY (1U<<7)
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define YELLOW  0xFFE0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
// centre of 240x320 screen
#define CX 120
#define CY 160

// 4 corners to centre

#define CELL_SIZE 10
#define GRID_W 24
#define GRID_H 32
#define SNAKE_LEN 12

typedef struct
{
    int x;
    int y;
} Segment;

Segment snake[SNAKE_LEN];

enum
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

void snake_demo(void)
{
    fill_screen(BLACK);

    // Initial snake
    for(int i = 0; i < SNAKE_LEN; i++)
    {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }

    int dir = RIGHT;

    while(1)
    {
        // Erase tail
        fill_rectangle(
            BLACK,
            snake[SNAKE_LEN - 1].x * CELL_SIZE,
            snake[SNAKE_LEN - 1].y * CELL_SIZE,
            snake[SNAKE_LEN - 1].x * CELL_SIZE + CELL_SIZE - 1,
            snake[SNAKE_LEN - 1].y * CELL_SIZE + CELL_SIZE - 1
        );

        // Shift body
        for(int i = SNAKE_LEN - 1; i > 0; i--)
        {
            snake[i] = snake[i - 1];
        }

        // Move head
        switch(dir)
        {
            case RIGHT:
                snake[0].x++;
                break;

            case DOWN:
                snake[0].y++;
                break;

            case LEFT:
                snake[0].x--;
                break;

            case UP:
                snake[0].y--;
                break;
        }

        // Follow screen border forever
        if(dir == RIGHT && snake[0].x >= GRID_W - 2)
            dir = DOWN;

        else if(dir == DOWN && snake[0].y >= GRID_H - 2)
            dir = LEFT;

        else if(dir == LEFT && snake[0].x <= 1)
            dir = UP;

        else if(dir == UP && snake[0].y <= 1)
            dir = RIGHT;

        // Draw new head
        fill_rectangle(
            GREEN,
            snake[0].x * CELL_SIZE,
            snake[0].y * CELL_SIZE,
            snake[0].x * CELL_SIZE + CELL_SIZE - 1,
            snake[0].y * CELL_SIZE + CELL_SIZE - 1
        );

        // Draw body
        for(int i = 1; i < SNAKE_LEN; i++)
        {
            fill_rectangle(
                0x07E0, // green
                snake[i].x * CELL_SIZE,
                snake[i].y * CELL_SIZE,
                snake[i].x * CELL_SIZE + CELL_SIZE - 1,
                snake[i].y * CELL_SIZE + CELL_SIZE - 1
            );
        }

        delay_ms(80);
    }
}
#include <stdint.h>
#include "stm32f4xx.h"
#include "spi_driver.h"

#define BLACK 0x0000

extern void snake_demo(void);

int main(void)
{
    spi_config();      // SPI1 setup

    display_init();      // ST7789 init

    fill_screen(BLACK);

    delay_ms(500);

    snake_demo();

    while(1)
    {
    }
}
/*
int main(void)



{

    spi_config();

    display_init();

    fill_screen(BLACK);

for(int i=0;i<10;i++){

    draw_rect(0+i,0+i,239-i,319-i,WHITE);}


	fill_circle(120,160,100,WHITE);

    draw_hline(0,239,160,GREEN);

    draw_vline(120,0,319,BLUE);

    draw_pixel(100,100,WHITE);

    while(1){}



}
*/


