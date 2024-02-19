/*******************************************************************************
 * This program performs the following:
 *  1. checks if there is a sample ready to be read.
 * 	2. reads that sample from microphone channels.
 * 	3. writes that sample to the audio output channels.
 ******************************************************************************/

#define AUDIO_BASE			0xFF203040
int main(void) {
    // Audio port structure 

struct audio_t {
	volatile unsigned int control;  // The control/status register
	volatile unsigned char rarc;	// the 8 bit RARC register
	volatile unsigned char ralc;	// the 8 bit RALC register
	volatile unsigned char wsrc;	// the 8 bit WSRC register
	volatile unsigned char wslc;	// the 8 bit WSLC register
    volatile unsigned int ldata;	// the 32 bit (really 24) left data register
	volatile unsigned int rdata;	// the 32 bit (really 24) right data register
};

/* we don't need to 'reserve memory' for this, it is already there
     so we just need a pointer to this structure  */

struct audio_t *const audiop = ((struct audio_t *) AUDIO_BASE);

    // to hold values of samples
    int left, right;
	
	// infinite loop checking the RARC to see if there is at least a single
	// entry in the input fifos.   If there is, just copy it over to the output fifo.
	// The timing of the input fifo controls the timing of the output

    while (1) {
        
        if ( audiop->rarc > 0) // check RARC to see if there is data to read
        {
            // load both input microphone channels - just get one sample from each
	

	    left = audiop->ldata; // load the left input fifo
	    right = audiop->rdata; // load the right input fifo

	    // Did not check if output FIFO has space - we know it does,
	    // because the rate is the same as the input .
	    //  but should really have check if (audiop->wsrc > 0) - i.e. that there is an empty output FIFO slot
	    // available.  You'll need to do that in part 3. 

	    audiop->ldata = left;  // store to the left output fifo
	    audiop->rdata = right;  // store to the right output fifo
        }
    }
}
