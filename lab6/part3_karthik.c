#include <stdbool.h>
#define AUDIO_BASE		0xff203040;
#define SW_BASE 		0xff200040;


int main(void){
	
	volatile int* audio_ptr = (int*) AUDIO_BASE;
	volatile int* sw_ptr = (int*) SW_BASE;
	int freq_Multiplier;
	int count_str = 80;
	int count = count_str;
	int fifospace;
	bool high = true;
	
	
	while(1){
		fifospace = *(audio_ptr + 1);
		freq_Multiplier = *sw_ptr;
		count_str = 80 - freq_Multiplier; 
		
		if((fifospace & 0x00ff0000) > 0){
			if(count > 0){
				if(high){
					*(audio_ptr + 2) = 3556769792;
					*(audio_ptr + 3) = 3556769792;	
					count = count - 1;
				}
				else{
					*(audio_ptr + 2) = 0; 
					*(audio_ptr + 3) = 0;
					count = count - 1;
				}
			}
			else{
				count = count_str;
				high = !high;
			}
		}
	}
		
}