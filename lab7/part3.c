/*
Animation of 8 filled boxes on the screen 
Boxes should appear to be moving and continuously "bouncing" off the edges of the screen
Boxes should be connected with lines to form a chain
To make the animation look slightly different, use the rand function 
Use double buffering 
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

#define FPGA_PIXEL_BUF_BASE		0x08000000
#define FPGA_PIXEL_BUF_END		0x08000000
volatile int pixel_buffer_start; // global variable
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];

//make the information about each pixel accessible globally 
volatile int pixelInfo[8][4];

int main(void){
    // global  and directions of each of the boxes (referenced by the top left corner)
// note that x is limited from 0 -> 316 (because of 3 by 3 box)
// note that y is limited from 0 -> 236(becaues of 3 b 3 box)
    int x1 = rand() % 316, y1 = rand() % 236, direction_x1 = (rand() % 2) * 2 - 1, direction_y1 = (rand() % 2) * 2 - 1;
    int x2 = rand() % 316, y2 = rand() % 236, direction_x2 = (rand() % 2) * 2 - 1, direction_y2 = (rand() % 2) * 2 - 1;
    int x3 = rand() % 316, y3 = rand() % 236, direction_x3 = (rand() % 2) * 2 - 1, direction_y3 = (rand() % 2) * 2 - 1;
    int x4 = rand() % 316, y4 = rand() % 236, direction_x4 = (rand() % 2) * 2 - 1, direction_y4 = (rand() % 2) * 2 - 1;
    int x5 = rand() % 316, y5 = rand() % 236, direction_x5 = (rand() % 2) * 2 - 1, direction_y5 = (rand() % 2) * 2 - 1;
    int x6 = rand() % 316, y6 = rand() % 236, direction_x6 = (rand() % 2) * 2 - 1, direction_y6 = (rand() % 2) * 2 - 1;
    int x7 = rand() % 316, y7 = rand() % 236, direction_x7 = (rand() % 2) * 2 - 1, direction_y7 = (rand() % 2) * 2 - 1;
    int x8 = rand() % 316, y8 = rand() % 236, direction_x8 = (rand() % 2) * 2 - 1, direction_y8 = (rand() % 2) * 2 - 1;
    //initialize each pixel with a random start location and random direction 

    //store this nicer in an array so we can traverse through it better 
    pixelInfo =   {{x1, y1, direction_x1, direction_y1}, // pixel 1
                        {x2, y2, direction_x2, direction_y2}, // pixel 2
                        {x3, y3, direction_x3, direction_y3}, // pixel 3
                        {x4, y4, direction_x4, direction_y4}, // pixel 4
                        {x5, y5, direction_x5, direction_y5}, // pixel 5
                        {x6, y6, direction_x6, direction_y6}, // pixel 6
                        {x7, y7, direction_x7, direction_y7}, // pixel 7
                        {x8, y8, direction_x8, direction_y8}  // pixel 8
                    };  

    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // initialize location and direction of rectangles

    /* set front pixel buffer to Buffer 1 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; // first store the address in the  back buffer

    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();

    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    while (1){
         // code for drawing the boxes and lines (not shown)
        drawBoxLine(pixelInfo);

        // code for generating a new direction IF a pixel hits the edge of the screen 
        generateNewDirection(pixelInfo);

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer, gets the address of the current back buffer 
        
        /* Erase any boxes and lines that were drawn in the last iteration */
        erasePrev(pixelInfo);

        // prep the boxes/lines for the next iteration 

    }
}

void generateNewDirection(int pixelInfo[][]){ //call this function when the x or y go out of bounds 
    //check if any pixel has gone out of bounds 
    for(){ //
        for(){
            if(){ //in the case where a pixel has gone out of bounds  
                //generate a new random direction for it
                // and update the direction in the pixelInfo array 

            }
        }

    }


}

void draw(){ //draw the box and the line 

}

void erasePrev(int pixelInfo[][]){ //erase the previous iteration of lines and boxes
    //traverse through all the pixels and make them white
    for(int i = 0; i < 8; i++){ //column
        for(int j = 0; j < 2; j++){ //row

        }

    }

    // traverse through each of the lines and make them white 
    for(int i = 0; i < 7; i++){

    }
}

void draw_box(int x1, int y1, short int colour){
    //draw a 3 by 3 box on the screen, where (x1, y1) is the top left corner 
    for(int i = y1; i < y1+3; i++){
        for(int j = x1; j < x1+3; j++){
            plot_pixel(i,j, 0xADD8E6);
        }
    }

}

void clearScreen(){ //make the screen entirely white 
    for(int y = 0; y <= 239; y++){
        for(int x = 0; x <= 319; x++){
            plot_pixel(x,y,0xFFFF);
        }
    }

}

void draw_line(int x1, int y1, int x2, int y2, short int colour){ //draw a line given (x1,y1) and (x2, y2) and the colour
    //figure out whether to traverse x or traverse y 
    bool isSteep = abs(y2-y1) > abs(x2-x1); 
    if (isSteep){ //swap x and y 
        int temp = y1;
        y1 = x1;
        x1 = temp;

        temp = y2;
        y2 = x2;
        x2 = temp;
    }

    if(x1 > x2){ //swap the points
        int temp = x1;
        x1 = x2;
        x2 = temp;

        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    // calculate the delta (error) to determine how often y should be incremented 
    int deltaX = x2-x1;
    int deltaY = y2-y1;
    int error = -(deltaX/2);
    int y = y1; //because we swapped points accordingly above, we can safely assume y = mx + b and y-y1 = m(x-x1) notation for now
    int yStep;

    yStep = y1 < y2 ? 1 : -1; 

    for(int x = x1; x <= x2; x++){
        if(isSteep){
            plot_pixel(y,x, colour);
        }

        else{
            plot_pixel(x,y, colour);
        }

        error+=deltaY;

        if(error > 0){
            y+=yStep;
            error-=deltaX;
        }
    }
}

void plot_pixel(int x, int y, short int line_color)
{
    volatile short int *one_pixel_address;
        
        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
        
        *one_pixel_address = line_color;
}

void wait_for_vsync(){ // to prevent tearing 
	volatile int * pixel_ctrl_ptr = (int *) 0xff203030; // base address
	
	int status;
	*pixel_ctrl_ptr = 1; // start the synchronization process
	// - write 1 into front buffer address register
	status = *(pixel_ctrl_ptr + 3); // read the status register
	
	while ((status & 0x01) != 0){ // polling loop waiting for S bit to go to 0
		status = *(pixel_ctrl_ptr + 3);
	}
}

