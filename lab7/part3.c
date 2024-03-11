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
short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512] ;
int pixelInfoBuf1[8][2] = {0};
int pixelInfoBuf2[8][2] = {0};
int pixelInfo[8][4] = {0};
int idx = 0;

short int fullColourArray[] = {
    0x7f42, 0x7f41, 0x7f41, 0x7741, 0x7721, 0x7721, 0x7721, 0x7721,
    0x6f01, 0x6f01, 0x6f01, 0x6f01, 0x6ee1, 0x66e0, 0x66e0, 0x66e0,
    0x66c0, 0x66c0, 0x5ec0, 0x5ec0, 0x5ea0, 0x5ea0, 0x5ea0, 0x5680,
    0x5680, 0x5680, 0x5660, 0x5660, 0x4e60, 0x4e60, 0x4e40, 0x4e40,
    0x4e40, 0x4e20, 0x4620, 0x4620, 0x4600, 0x4600, 0x45e0, 0x3de0,
    0x3de0, 0x3dc0, 0x3dc0, 0x3dc0, 0x3da0, 0x35a0, 0x35a0, 0x3580,
    0x3580, 0x3560, 0x3560, 0x2d60, 0x2d40, 0x2d40, 0x2d40, 0x2d20,
    0x2d20, 0x2500, 0x2500, 0x2500, 0x24e0, 0x24e0, 0x24c0, 0x24c0,
    0x1cc0, 0x1ca0, 0x1ca0, 0x1c80, 0x1c81, 0x1c81, 0x1c61, 0x1c61,
    0x1441, 0x1441, 0x1441, 0x1421, 0x1421, 0x1401, 0x1401, 0x13e1,
    0x13e2, 0xbe2, 0xbc2, 0xbc2, 0xba2, 0xba2, 0xba2, 0xb82,
    0xb82, 0xb63, 0xb63, 0xb63, 0xb43, 0x343, 0x323, 0x323,
    0x323, 0x304, 0x304, 0x2e4, 0x2e4, 0x2e4, 0x2c4, 0x2c4,
    0x2a5, 0x2a5, 0x2a5, 0x285, 0x285, 0x265, 0x266, 0x266,
    0x246, 0x246, 0x246, 0x226, 0x227, 0x207, 0x207, 0x207,
    0x1e7, 0x1e7, 0x1e8, 0x1c8, 0x1c8, 0x1c8, 0x1a8, 0x1a8,
    0x1a9, 0x189, 0x189, 0x189, 0x169, 0x16a, 0x16a, 0x14a,
    0x14a, 0x14a, 0x12b, 0x12b, 0x12b, 0x12b, 0x10b, 0x10c,
    0x10c, 0xec, 0xec, 0xec, 0x8ed, 0x8cd, 0x8cd, 0x8cd,
    0x8cd, 0x8ae, 0x8ae, 0x8ae, 0x8ae, 0x88e, 0x88f, 0x88f,
    0x108f, 0x108f, 0x106f, 0x1070, 0x1070, 0x1070, 0x1070, 0x1050,
    0x1051, 0x1851, 0x1851, 0x1851, 0x1851, 0x1832, 0x1832, 0x1832,
    0x1832, 0x2032, 0x2033, 0x2033, 0x2033, 0x2033, 0x2013, 0x2014,
    0x2814, 0x2814, 0x2814, 0x2814, 0x2814, 0x2815, 0x3015, 0x3015,
    0x3015, 0x3015, 0x3016, 0x3016, 0x3016, 0x3816, 0x3816, 0x3817,
    0x3817, 0x3817, 0x4017, 0x4017, 0x4017, 0x4018, 0x4018, 0x4018,
    0x4818, 0x4818, 0x4818, 0x4819, 0x4819, 0x5019, 0x5019, 0x5019,
    0x5019, 0x5019, 0x581a, 0x581a, 0x583a, 0x583a, 0x583a, 0x583a,
    0x603a, 0x603b, 0x603b, 0x603b, 0x605b, 0x685b, 0x685b, 0x685b,
    0x685b, 0x685c, 0x705c, 0x707c, 0x707c, 0x707c, 0x707c, 0x787c,
    0x789c, 0x789c, 0x789d, 0x809d, 0x809d, 0x80bd, 0x80bd, 0x80bd,
    0x88bd, 0x88dd, 0x88dd, 0x88dd, 0x88dd, 0x90fe, 0x90fe, 0x90fe,
    0x90fe, 0x911e, 0x991e, 0x991e, 0x993e, 0x993e, 0x993e, 0x993e,
    0xa15e, 0xa15e, 0xa15e, 0xa17e, 0xa17e, 0xa97e, 0xa99e, 0xa99e,
    0xa99e, 0xa9be, 0xb1be, 0xb1be, 0xb1de, 0xb1de, 0xb1de, 0xb1fe,
    0xb9fe, 0xb9fe, 0xba1e, 0xba1e, 0xba1e, 0xc23e, 0xc23e, 0xc25e,
    0xc25e, 0xc25e, 0xc27e, 0xc27e, 0xca7e, 0xca9e, 0xca9e, 0xcabe,
    0xcabe, 0xcabe, 0xd2de, 0xd2de, 0xd2fe, 0xd2fe, 0xd2fe, 0xd31e,
    0xd31e, 0xdb3e, 0xdb3e, 0xdb3e, 0xdb5e, 0xdb5d, 0xdb7d, 0xdb7d,
    0xdb7d, 0xe39d, 0xe39d, 0xe3bd, 0xe3bd, 0xe3bd, 0xe3dd, 0xe3dd,
    0xe3fc, 0xe3fc, 0xec1c, 0xec1c, 0xec1c, 0xec3c, 0xec3c, 0xec5c,
    0xec5c, 0xec5c, 0xec7b, 0xec7b, 0xec9b, 0xec9b, 0xf49b, 0xf4bb,
    0xf4bb, 0xf4da, 0xf4da, 0xf4da, 0xf4fa, 0xf4fa, 0xf51a, 0xf51a,
    0xf519, 0xf539, 0xf539, 0xf559, 0xf559, 0xf559, 0xf579, 0xf578,
    0xf598, 0xf598, 0xf598, 0xf5b8, 0xf5b8, 0xf5b7, 0xf5d7, 0xf5d7,
    0xf5d7, 0xf5f7, 0xf5f7, 0xf616, 0xf616, 0xf616, 0xf636, 0xf636,
    0xf635, 0xf655, 0xf655, 0xf655, 0xf675, 0xf675, 0xf674, 0xf694,
    0xf694, 0xf694, 0xf694, 0xf6b3, 0xf6b3, 0xf6b3, 0xf6d3, 0xf6d3,
    0xf6d2, 0xf6d2, 0xf6f2, 0xf6f2, 0xf6f2, 0xef11, 0xef11, 0xef11,
    0xef11, 0xef31, 0xef30, 0xef30, 0xef30, 0xef30, 0xef50, 0xef4f,
    0xef4f, 0xe74f, 0xe76f, 0xe76f, 0xe76e, 0xe76e, 0xe76e, 0xe76e,
    0xe78e, 0xe78d, 0xdf8d, 0xdf8d, 0xdf8d, 0xdf8d, 0xdfac, 0xdfac,
    0xdfac, 0xdfac, 0xd7ac, 0xd7ab, 0xd7ab, 0xd7ab, 0xd7cb, 0xd7cb,
    0xd7ca, 0xcfca, 0xcfca, 0xcfca, 0xcfca, 0xcfc9, 0xcfc9, 0xc7c9,
    0xc7c9, 0xc7c9, 0xc7c8, 0xc7c8, 0xc7c8, 0xbfc8, 0xbfc8, 0xbfc8,
    0xbfc7, 0xbfc7, 0xbfc7, 0xb7c7, 0xb7c7, 0xb7c7, 0xb7c6, 0xb7c6,
    0xafc6, 0xafc6, 0xafc6, 0xafc6, 0xafc5, 0xafc5, 0xa7c5, 0xa7c5,
    0xa7c5, 0xa7c5, 0xa7c4, 0x9fc4, 0x9fc4, 0x9fa4, 0x9fa4, 0x9fa4,
    0x97a4, 0x97a3, 0x97a3, 0x97a3, 0x97a3, 0x8f83, 0x8f83, 0x8f83,
    0x8f83, 0x8f82, 0x8782, 0x8762, 0x8762, 0x8762, 0x8762, 0x7f62,
    0x7f42
};





//function declarations
void wait_for_vsync(void);
void clear_screen(void);
void draw_box(int x1, int y1, short int color);
void draw_line(int x1, int y1, int x2, int y2, short int color);
void plot_pixel(int x, int y, short int line_color);
void checkNewDirection(int pixelInfo[8][4]);
void draw(int pixelInfo[8][4], short int colour);
void erasePrev(int pixelInfoBuf[][2]);
void storePrev(int pixelInfo[][4], int pixelInfoBuf[][2]);
void swap(int *a, int *b);

int main(void){
    srand(time(NULL)); //seed the start locations
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
    pixel_buffer_start = (int)*pixel_ctrl_ptr;
    clear_screen();
     // pixel_buffer_start points to the pixel buffer
    //printf("Initial clear of the screen\n");

    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    //printf("Second clear of the screen\n");

    while (1){  
        short int colour = fullColourArray[idx];   
         // draw boxes lines, erase, and update directions if necessary
        draw(pixelInfo, colour);

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer, gets the address of the current back buffer 

        idx = (idx+1) % 481; //loop around 
    }
}

void draw(int pixelInfo[8][4], short int colour){ //draw the box and the line 
    /* Erase any boxes and lines that were drawn in the last iteration */
    if (*(pixel_ctrl_ptr + 1) == (int)&Buffer1) {
        erasePrev(pixelInfoBuf1); 
    } 
    else {
        erasePrev(pixelInfoBuf2); 
    }

    // code for generating a new direction IF AND ONLY IF a pixel hits the edge of the screen 
    checkNewDirection(pixelInfo);

    //draw 8 boxes and draw 8 lines connecting them
    for(int i = 0; i < 8; i++){
        draw_box(pixelInfo[i][0], pixelInfo[i][1], colour);
    
        if(i == 7){ //connect the first box to the last one 
            draw_line(pixelInfo[0][0]+1, pixelInfo[0][1]+1, pixelInfo[i][0]+1, pixelInfo[i][1]+1, colour);
        } 

        else{
            draw_line(pixelInfo[i][0]+1, pixelInfo[i][1]+1, pixelInfo[i+1][0]+1, pixelInfo[i+1][1]+1, colour);
        }

    }

    //store the previous locations
    if(*(pixel_ctrl_ptr + 1) == (int) &Buffer1){ //wrote to buffer 1
        storePrev(pixelInfo, pixelInfoBuf1);
    }

    else{
        storePrev(pixelInfo, pixelInfoBuf2);
    }

    
    //update the box position for the next frame (modify the pixelInfo array)
    for(int i = 0; i < 8; i++){
        pixelInfo[i][0] += pixelInfo[i][2]; //update the x direction
        pixelInfo[i][1] += pixelInfo[i][3]; //update the y direction
    }

}

void checkNewDirection(int pixelInfo[8][4]){
    for(int i = 0; i < 8; i++){
        if(pixelInfo[i][0] <= 0 || pixelInfo[i][0] >= 317) { //change x direction
            pixelInfo[i][2] = -pixelInfo[i][2];
        }
        if(pixelInfo[i][1] <= 0 || pixelInfo[i][1] >= 237) { //change y direction 
            pixelInfo[i][3] = -pixelInfo[i][3];
        }
    }
}

// void erasePrev(int pixelInfoBuf1[][2], int pixelInfoBuf2[][2]){ //erase the previous iteration of lines and boxes
//     //determine which buffer we drew to to figure out the locations of what we want to erase
//     if(*(pixel_ctrl_ptr + 1) == (int) &Buffer1) {
//         for(int i = 0; i < 8; i++){
//             draw_box(pixelInfoBuf1[i][0], pixelInfoBuf1[i][1], 0xFFFF);
//             if(i == 7){ //connect the first box to the last one 
//                 draw_line(pixelInfoBuf1[0][0]+1, pixelInfoBuf1[0][1]+1, pixelInfoBuf1[i][0]+1, pixelInfoBuf1[i][1]+1, 0xFFFF);
//             } 
//             else {
//                 draw_line(pixelInfoBuf1[i][0]+1, pixelInfoBuf1[i][1]+1, pixelInfoBuf1[i+1][0]+1, pixelInfoBuf1[i+1][1]+1, 0xFFFF);
//             }
//         }
//     } else { 
//         for(int i = 0; i < 8; i++){
//             draw_box(pixelInfoBuf2[i][0], pixelInfoBuf2[i][1], 0xFFFF);
//             if(i == 7){ //connect the first box to the last one 
//                 draw_line(pixelInfoBuf2[0][0]+1, pixelInfoBuf2[0][1]+1, pixelInfoBuf2[i][0]+1, pixelInfoBuf2[i][1]+1, 0xFFFF);
//             } else {
//                 draw_line(pixelInfoBuf2[i][0]+1, pixelInfoBuf2[i][1]+1, pixelInfoBuf2[i+1][0]+1, pixelInfoBuf2[i+1][1]+1, 0xFFFF);
//             }
//         }
//     }
// }

void erasePrev(int pixelInfoBuf[][2]) {
    for (int i = 0; i < 8; i++) {
        draw_box(pixelInfoBuf[i][0], pixelInfoBuf[i][1], 0xFFFF);
        int nextIdx = (i == 7) ? 0 : i + 1;
        draw_line(pixelInfoBuf[i][0] + 1, pixelInfoBuf[i][1] + 1, pixelInfoBuf[nextIdx][0] + 1, pixelInfoBuf[nextIdx][1] + 1, 0xFFFF);
    }
}



void draw_box(int x1, int y1, short int colour){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            plot_pixel(x1+j, y1+i, colour);
        }
    }
}

void clear_screen(){ //make the screen entirely white 
    for(int y = 0; y <= 239; y++){
        for(int x = 0; x <= 319; x++){
            plot_pixel(x,y,0xFFFF);
        }
    }

}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void draw_line(int x1, int y1, int x2, int y2, short int colour){
    bool isSteep = abs(y2 - y1) > abs(x2 - x1);
    if (isSteep) {
        swap(&x1, &y1);
        swap(&x2, &y2);
    }

    if (x1 > x2) {
        // swap the start and end points if x2 is less than x1
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    int deltaX = x2 - x1;
    int deltaY = abs(y2 - y1);
    int error = -(deltaX / 2); // initialize error term to negative half delta X
    int y = y1;
    int yStep = (y1 < y2) ? 1 : -1;

    for (int x = x1; x <= x2; x++) {
        if (isSteep) {
            plot_pixel(y, x, colour); 
        } 
        
        else {
            plot_pixel(x, y, colour);
        }

        error += deltaY; 

        if (error >= 0) {
            y += yStep;
            error -= deltaX; 
        }
    }
}

void plot_pixel(int x, int y, short int line_color)
{
    short int *one_pixel_address;
    if(y < 240 && y >= 0 && x >= 0 && x < 320) {
       one_pixel_address = (short int *)((unsigned int)pixel_buffer_start + (y << 10) + (x << 1));
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
