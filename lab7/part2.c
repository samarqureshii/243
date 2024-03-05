/*
Write program that moves a horizontal line up and down on the screen and "bounces" the line off the top and bottom edges of the display
Program should first clear the screen and draw the line at a starting row on the screen
In an endless loop, erase the line (by drawing over it in black), and redraw it one row above or below the last one 
When the line reaches the top or bottom of the screen, it should start moving in the opposite direction 
*/

#include <stdbool.h>
int pixel_buffer_start;
#define FPGA_PIXEL_BUF_BASE		0x08000000
#define FPGA_PIXEL_BUF_END		0x08000000

int main(void){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;

    int direction = 1; //start moving up 
    int y = 238;

    //clear the screen
    clearScreen();

    while(1){
        //erase the old line 
        eraseLine(y);

        //increment position
        if ((direction == 1 && y > 10)) {
            y--;
        } 
        
        else if ((direction == -1 && y < 230)) {
            y++;
        } 

        else if(y <= 10 || y >= 230){
            direction=-direction;
        }
        
        // need for an else state here?

        //draw the line
        drawLine(y);

        volatile int * pixel_ctrl_ptr = (int *) 0xff203030; // base address

        int status;
        *pixel_ctrl_ptr = 1; // start the synchronization process
        // - write 1 into front buffer address register
        status = *(pixel_ctrl_ptr + 3); // read the status register
        
        while ((status & 0x01) != 0){ // polling loop waiting for S bit to go to 0
            //poll the status register so we know when we are finished writing- after 1/60 of a second 
            status = *(pixel_ctrl_ptr + 3);
        }

        

    }
}

void drawLine(int y){
    //x positions always stay the same, it is just the y position that is changing
    //start at x = 79, end at x = 240
    int colour = 0xFFFF;
    for(int x = 79; x <=240; x++){
        plot_pixel(x, y, colour); 
    }
}

void eraseLine(int y){
    int colour = 0x0000;
    //x positions always stay the same, it is just the y position that is changing
    for(int x = 79; x <=240; x++){
        plot_pixel(x, y, colour); 
    }
}

void plot_pixel(int x, int y, short int line_color){
    volatile short int *one_pixel_address;
        
        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
        
        *one_pixel_address = line_color;
}

void clearScreen(){ //make the screen entirely black
    for(int y = 0; y <= 239; y++){
        for(int x = 0; x <= 319; x++){
            plot_pixel(x,y,0x0000);
        }
    }

}