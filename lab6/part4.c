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
int bufferL[3200]; //8000*N = 3200 where N = 0.4
int bufferR[3200]; //8000*N = 3200 where N = 0.4
int idx = 0; // echo index in the echo buffer

// Store the output from N cycles ago, which recursively includes the fading versions of what happened in 2N, 3N, 4N... time ago
// output(t) = Input(t) + D*Output(t-N), where D is the damping coefficient < 1
// keep that last N samples that were output, and get infinite fade by doing so 
// do we need to store something in a matrix?

void echo(int inputL, int inputR){

    int outputL = (int)(bufferL[idx]*0.6) + inputL;  
    int outputR = (int)(bufferR[idx]*0.6) + inputR;  

    //output = (output > 0x7FFFFF) ? 0x7FFFFF : ((output < -0x800000) ? -0x800000 : output);

    //update the buffer
    bufferL[idx] = outputL;
    bufferR[idx] = outputR;

    audio_p->ldata = outputL;
    audio_p->rdata = outputR;

    // write the output to output FIFO if space available
    // if(audio_p->wsrc > 0 || audio_p->wslc > 0){
    //     audio_p->ldata = output;
    //     audio_p->rdata = output;
    // }

    //increment the index counter
    idx = (idx+1) % 3200;

}

int main(void){
    //init the buffer
    for(int i = 0; i < 3200; i++){
        bufferL[i] = 0;
        bufferR[i] = 0;
    }

    while(1){
        if((audio_p->rarc > 0 || audio_p->ralc > 0)) { 
            int inputL = audio_p->ldata;
            int inputR = audio_p->rdata;
            echo(inputL, inputR);
        }
    }
}