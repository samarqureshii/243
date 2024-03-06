/*

*/

#include <stdbool.h>
int pixel_buffer_start; // global variable (may cause CPUlator warning)

int main(void){
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();
    draw_line(0, 0, 150, 150, 0x001F);   // this line is blue
    draw_line(150, 150, 319, 0, 0x07E0); // this line is green
    draw_line(0, 239, 319, 239, 0xF800); // this line is red
    draw_line(319, 0, 0, 239, 0xF81F);   // this line is a pink color
}

void clear_screen(){ //make the screen entirely black
    for(int y = 0; y <= 239; y++){
        for(int x = 0; x <= 319; x++){
            plot_pixel(x,y,0x0000);
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

