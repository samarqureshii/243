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

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


void plot_pixel(int x, int y, short int line_color)
{
    volatile short int *one_pixel_address;

        one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

        *one_pixel_address = line_color;
}

