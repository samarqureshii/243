/*
Write program that moves a horizontal line up and down on the screen and "bounces" the line off the top and bottom edges of the display
Program should first clear the screen and draw the line at a starting row on the screen
In an endless loop, erase the line (by drawing over it in black), and redraw it one row above or below the last one 
When the line reaches the top or bottom of the screen, it should start moving in the opposite direction 
*/

#include <stdbool.h>
int pixel_buffer_start;

int main(void){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;

    int direction = 1; //start moving up 
    int y = 238;

    //clear the screen
    for(int y = 0; y <= 239; y++){
        for(int x = 0; x <= 319; x++){
            plot_pixel(x,y,0x0000);
        }
    }

    while(1){
        //draw the line
        drawLine(y);
        //erase the line 
        eraseLine(y);

        //increment position
        if(direction == 1 && y > 0){ // if we are moving upwards, keep moving upwards
            y++;
        }

        else if (direction == -1 && y < 239){ //if we are moving downwards, keep moving downwards
            y--;
        }

        else if(y == 0 || y == 239){ //we have hit the edge of the screen, toggle the direction
            direction!=direction;
        }

    }
}

void drawLine(int y){
    //x positions always stay the same, it is just the y position that is changing
}

void eraseLine(int y){
    //x positions always stay the same, it is just the y position that is changing

}