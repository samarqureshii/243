// make microphone input come out of the speaker, but make it echo
// sound that was earlier comes back, some delay time later 
// N = how many sample periods ago we want to replicate, bring back
// N = 0.4s
// when it comes back, it is quieter it is damped by damping factor (less than 1)
// current sound is also coming out even while the echo comes and fades away 

#define AUDIO_BASE			0xFF203040

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

int main(void){

}