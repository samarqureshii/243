// turns on all red LEDs when KEY0 is pressed and released, and then turns them off with  KEY1 when pressed and released
// use edge capture register to determine which of the keys was pressed 

#define LED_BASE			0xFF200000
#define KEY_BASE			0xFF200050


int main(void){
    int on = 0; // global variable for if the LEDs are on or off
    volatile int *KEYs = KEY_BASE; //load memory address of KEYs into pointer register 
    volatile int *LEDs = LED_BASE;
    int edgeCapture = *(KEYs + 3); //3 * 4 bytes
    
    while(1){
        //load new value into EC register
        edgeCapture = *(KEYs + 3);

        if(edgeCapture & 0x1){ // if KEY0 is pressed, then turn LEDs on
            on = 0x1;
            //turn all the LEDs on 
            *LEDs = 0xff;
            // reset edge capture register 
            *(KEYs + 3) = 0x1;
        }

        else if(on == 0x1 && (edgeCapture & 0x2)){ // if KEY1 is pressed and the LEDs are currently on
            on = 0;
            //turn all LEDs off
            *LEDs = 0;
            // reset edge capture register 
            *(KEYs + 3) = 0x2;
        }

    }
}