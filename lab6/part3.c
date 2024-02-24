// generate square wave input (no input) to audio device 
//frequency of the audio output is changeable from 10Hz to 2KHz across the 10 switches to create a fine-grained selection of frequency


#define SW_BASE				0xFF200040
#define AUDIO_BASE			0xFF203040

int main(void) {
    struct audio_t {
        volatile unsigned int control;  // The control/status register
        volatile unsigned char rarc;	// the 8 bit RARC register
        volatile unsigned char ralc;	// the 8 bit RALC register
        volatile unsigned char wsrc;	// the 8 bit WSRC register
        volatile unsigned char wslc;	// the 8 bit WSLC register
        volatile unsigned int ldata;	// the 32 bit (really 24) left data register
        volatile unsigned int rdata;	// the 32 bit (really 24) right data register  
    };

    struct audio_t *const audio_p = ((struct audio_t *) AUDIO_BASE); //pointer to audio

    int state = 0;
    int count = 0;
    int half_period_samples = 4000; // start at 100 Hz

    volatile int *sw_p = (int *)SW_BASE;

    while(1) {
        int SW = *sw_p; // current switch state

        // map the switch state to frequency, every binary increase in 1 on the SW is increase in 2 Hz 
        int frequency = ((SW & 0x3FF) * 2) + 100;
        // adjust the half period samples based on new calculated switch frequency
        half_period_samples = 8000 / (2 * frequency);
        // nios ii plays 8000 * N samples
        // 8000 / f = samples per cycle
        // but we only care about the high part of the duty cycle so we multiply by 2
    
        // check if space in the FIFO
        if(audio_p->wsrc > 0 || audio_p->wslc > 0) {
            if(count >= half_period_samples) { //toggle the duty cycle 
                state = !state;
                count = 0;
            }

            audio_p->ldata = state ? 0x7FFFFF : 0; // use 0x7FFFFF for high value 24 bits
            audio_p->rdata = state ? 0x7FFFFF : 0;
            count++;
        }
    }

    return 0; 
}