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

//volatile short int *pixel_buffer_start; // global variable

int pixel_buffer_start; // global variable
volatile int * pixel_ctrl_ptr = (int *) 0xff203020; // base address
short int Buffer1[240][320]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][320];
int pixelInfoBuf1[8][2] = {0};
int pixelInfoBuf2[8][2] = {0};
int pixelInfo[8][4] = {0};

//function declarations
void wait_for_vsync(void);
void clear_screen(void);
void draw_box(int x1, int y1, short int color);
void draw_line(int x1, int y1, int x2, int y2, short int color);
void plot_pixel(int x, int y, short int line_color);
void checkNewDirection(int pixelInfo[8][4]);
void draw(int pixelInfo[8][4]);
void erasePrev(int pixelInfoBuf1[][2], int pixelInfoBuf2[][2]);
void storePrev(int pixelInfo[][4], int pixelInfoBuf[][2]);

int main(void){
    //srand(time(NULL)); //seed the start locations
    // global  and directions of each of the boxes (referenced by the top left corner)
    int x1 = rand() % 300, y1 = rand() % 200, direction_x1 = (rand() % 2) * 2 - 1, direction_y1 = (rand() % 2) * 2 - 1;
    int x2 = rand() % 300, y2 = rand() % 200, direction_x2 = (rand() % 2) * 2 - 1, direction_y2 = (rand() % 2) * 2 - 1;
    int x3 = rand() % 300, y3 = rand() % 200, direction_x3 = (rand() % 2) * 2 - 1, direction_y3 = (rand() % 2) * 2 - 1;
    int x4 = rand() % 300, y4 = rand() % 200, direction_x4 = (rand() % 2) * 2 - 1, direction_y4 = (rand() % 2) * 2 - 1;
    int x5 = rand() % 300, y5 = rand() % 200, direction_x5 = (rand() % 2) * 2 - 1, direction_y5 = (rand() % 2) * 2 - 1;
    int x6 = rand() % 300, y6 = rand() % 200, direction_x6 = (rand() % 2) * 2 - 1, direction_y6 = (rand() % 2) * 2 - 1;
    int x7 = rand() % 300, y7 = rand() % 200, direction_x7 = (rand() % 2) * 2 - 1, direction_y7 = (rand() % 2) * 2 - 1;
    int x8 = rand() % 300, y8 = rand() % 200, direction_x8 = (rand() % 2) * 2 - 1, direction_y8 = (rand() % 2) * 2 - 1;
    //initialize each pixel with a random start location and random direction 

    //store this nicer in an array so we can traverse through it better 
    int pixelInfo[8][4] =  {{x1, y1, direction_x1, direction_y1}, // pixel 1
                            {x2, y2, direction_x2, direction_y2}, // pixel 2
                            {x3, y3, direction_x3, direction_y3}, // pixel 3
                            {x4, y4, direction_x4, direction_y4}, // pixel 4
                            {x5, y5, direction_x5, direction_y5}, // pixel 5
                            {x6, y6, direction_x6, direction_y6}, // pixel 6
                            {x7, y7, direction_x7, direction_y7}, // pixel 7
                            {x8, y8, direction_x8, direction_y8}  // pixel 8
                            
                            };  

    /* set front pixel buffer to Buffer 1 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; // first store the address in the  back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen();
     // pixel_buffer_start points to the pixel buffer
    printf("Initial clear of the screen\n");

    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    printf("Second clear of the screen\n");

    while (1){     
         // draw boxes lines, erase, and update directions if necessary
        draw(pixelInfo);

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer, gets the address of the current back buffer 

        // prep the boxes/lines for the next iteration 

    }
}

void draw(int pixelInfo[8][4]){ //draw the box and the line 
    /* Erase any boxes and lines that were drawn in the last iteration */
    erasePrev(pixelInfoBuf1, pixelInfoBuf2);

    // code for generating a new direction IF AND ONLY IF a pixel hits the edge of the screen 
    checkNewDirection(pixelInfo);

    //draw 8 boxes and draw 8 lines connecting them
    for(int i = 0; i < 8; i++){
        draw_box(pixelInfo[i][0], pixelInfo[i][1], 0x07E0);

        if(i == 7){ //connect the first box to the last one 
            draw_line(pixelInfo[0][0], pixelInfo[0][1], pixelInfo[7][0], pixelInfo[7][1], 0x07E0);
        } 

        else{
            draw_line(pixelInfo[i][0], pixelInfo[i][1], pixelInfo[i+1][0], pixelInfo[i+1][1], 0x07E0);
        }

    }
    //store the previous locations
    // if(*(pixel_ctrl_ptr + 1) == (int) &Buffer1){ //wrote to buffer 1
    //     storePrev(pixelInfo, pixelInfoBuf1);
    // }

    // else{
    //     storePrev(pixelInfo, pixelInfoBuf2);
    // }
    storePrev(pixelInfo, (pixel_buffer_start == (int)&Buffer1) ? pixelInfoBuf1 : pixelInfoBuf2);

    
    //update the box position for the next frame (modify the pixelInfo array)
    for(int i = 0; i < 8; i++){
        pixelInfo[i][0] += pixelInfo[i][2]; //update the x direction
        pixelInfo[i][1] += pixelInfo[i][3]; //update the y direction
    }

}

void checkNewDirection(int pixelInfo[8][4]){ //call this function when the x or y go out of bounds 
    //check if any pixel has gone out of bounds 
    for(int i = 0; i < 8; i++){ // each row in the pixelInfo array (pixel number)
        //each column in the pixelInfo array
        if((pixelInfo[i][0] == 319 && pixelInfo[i][1] == 239)
            || (pixelInfo[i][0] == 319 || pixelInfo[i][1] == 0)
            || (pixelInfo[i][0] == 0 || pixelInfo[i][1] == 239) 
            || (pixelInfo[i][0] == 0 || pixelInfo[i][1] == 0)){
            //in the case where a pixel has gone out of bounds 
            //Flip the direction 
            // and update the direction in the pixelInfo array 
            pixelInfo[i][2] = -pixelInfo[i][2];
            pixelInfo[i][3] = -pixelInfo[i][3];
        }
        

    }

}

void erasePrev(int pixelInfoBuf1[][2], int pixelInfoBuf2[][2]){ //erase the previous iteration of lines and boxes
    //determine which buffer we drew to to figure out the locations of what we want to erase
    if(*(pixel_ctrl_ptr + 1) == (int) &Buffer1) {
        for(int i = 0; i < 8; i++){
            draw_box(pixelInfoBuf1[i][0], pixelInfoBuf1[i][1], 0);
            if(i == 7){ //connect the first box to the last one 
                draw_line(pixelInfoBuf1[0][0], pixelInfoBuf1[0][1], pixelInfoBuf1[7][0], pixelInfoBuf1[7][1], 0);
            } else {
                draw_line(pixelInfoBuf1[i][0], pixelInfoBuf1[i][1], pixelInfoBuf1[i+1][0], pixelInfoBuf1[i+1][1], 0);
            }
        }
    } else { //we drew to buffer 2
        for(int i = 0; i < 8; i++){
            draw_box(pixelInfoBuf2[i][0], pixelInfoBuf2[i][1], 0);
            if(i == 7){ //connect the first box to the last one 
                draw_line(pixelInfoBuf2[0][0], pixelInfoBuf2[0][1], pixelInfoBuf2[7][0], pixelInfoBuf2[7][1], 0);
            } else {
                draw_line(pixelInfoBuf2[i][0], pixelInfoBuf2[i][1], pixelInfoBuf2[i+1][0], pixelInfoBuf2[i+1][1], 0);
            }
        }
    }
}


void draw_box(int x1, int y1, short int colour){ //draw a singular box 
    //draw a 3 by 3 box on the screen, where (x1, y1) is the top left corner 
    // for(int i = y1; i < y1+3; i++){
    //     for(int j = x1; j < x1+3; j++){
    //         plot_pixel(x1,y1, 0x07E0);
    //     }
    // }


    plot_pixel(x1,y1, 0x07E0);

}

void clear_screen(){ //make the screen entirely white 
    for(int y = 0; y <= 239; y++){
        for(int x = 0; x <= 319; x++){
            plot_pixel(x,y,0);
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
    if(y < 240 && y >= 0 && x >= 0 && x < 320) {
        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
        *one_pixel_address = line_color;
    }
}

void wait_for_vsync(){ // to prevent tearing 

	int status;
	*pixel_ctrl_ptr = 1; // start the synchronization process
	// - write 1 into front buffer address register
	status = *(pixel_ctrl_ptr + 3); // read the status register
	
	while ((status & 0x01) != 0){ // polling loop waiting for S bit to go to 0
		status = *(pixel_ctrl_ptr + 3);
	}
}

void storePrev(int pixelInfo[][4], int pixelInfoBuf[][2]){ //save the values into pixelInfoBuf1 or pixelInfoBuf2
    for(int i = 0; i < 8; i++){
        pixelInfoBuf[i][0] = pixelInfo[i][0]; //save x location
        pixelInfoBuf[i][1] = pixelInfo[i][1]; //save y location
    }
}

/************************************************ ARCHIVE ************************************************************

*/
