#include <stdint.h>
#include "tm4c123gh6pm.h"

// ----------------- WS2812 TIMING (Your Format) -----------------
void send_data_one(void){
    GPIO_PORTB_DATA_R = 0x40;   // PB6 HIGH
    int j = 12;
    while(j--);

    GPIO_PORTB_DATA_R = 0x00;   // PB6 LOW
    j = 5;
    while(j--);
}

void send_data_zero(void){
    GPIO_PORTB_DATA_R = 0x40;
    GPIO_PORTB_DATA_R = 0x00;

    int j = 13;
    while(j--);
}

// ----------------- SEND ONE BYTE -----------------
void send_byte(uint8_t b){
    int i;
    for(i=7; i>=0; i--){
        if((b>>i)&1) send_data_one();
        else         send_data_zero();
    }
}

// ----------------- SEND A PIXEL ------------------
void send_pixel(uint8_t r, uint8_t g, uint8_t b){
    send_byte(g);
    send_byte(r);
    send_byte(b);
}

// ----------------- WS2812 RESET ------------------
void ws_reset(void){
    int j = 6000;
    while(j--);
}

// ----------------- DELAY -------------------------
void delay_ms(int ms){
    volatile int i,k;
    for(i=0;i<ms;i++){
        for(k=0;k<16000;k++);
    }
}

// ----------------- COLOR LIST --------------------
uint8_t colors[7][3] = {
    {255,0,0},    // RED
    {0,255,0},    // GREEN
    {0,0,255},    // BLUE
    {255,255,0},  // YELLOW
    {0,255,255},  // CYAN
    {255,0,255},  // MAGENTA
    {255,255,255} // WHITE
};

// --------------------------- MAIN ----------------------------
int main(void){

    // Enable PB6 clock
    SYSCTL_RCGCGPIO_R |= (1<<1);
    while(!(SYSCTL_PRGPIO_R & (1<<1)));

    // Configure PB6
    GPIO_PORTB_DIR_R  |= 0x40;
    GPIO_PORTB_DEN_R  |= 0x40;
    GPIO_PORTB_AFSEL_R &= ~0x40;
    GPIO_PORTB_AMSEL_R &= ~0x40;
    GPIO_PORTB_PCTL_R &= ~(0xF << 24);

    int i;
    int pos = 0;      // LED position rotating
    int color_id = 0; // which color is active

    while(1){

        // Send one full frame
        for(i=0; i<16; i++){
            if(i == pos){
                send_pixel(colors[color_id][0], colors[color_id][1], colors[color_id][2]);
            } else {
                send_pixel(0,0,0);
            }
        }

        ws_reset();
        delay_ms(5);         // speed of rotation

        // Rotate LED
        pos++;
        if(pos >= 16){
            pos = 0;

            // Change color after every full cycle
            color_id++;
            if(color_id >= 7)
                color_id = 0;
        }
    }
}
//understood
