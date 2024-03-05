/*
Write program that moves a horizontal line up and down on the screen and "bounces" the line off the top and bottom edges of the display
//program should first clear the screen and draw the line at a starting row on the screen
*/

#include <stdbool.h>
int pixel_buffer_start;

int main(void){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;
}