/*********************************************************************************
* By Rahmat Saeedi & Kevin Cui
* Feb 20, 2017
*
* This is written for Atmel-AVR ATTiny2313 MCU to output square waveforms, using CTC
* mode, to play up to two melodies simultaneously. The melody starts at a push of a
* button. Timer0 and Timer1 are used to generate frequencies that are adjusted
* accordingly.
*********************************************************************************/
#define			voice0Play		1
#define			voice1Play		1
#define			F_CPU			8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/pgmspace.h>

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////Music Note>>OCRnx//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//Notes and OCR0x values for Timer0 using n=64, 8MHz setting
#define         _G8          9
#define         _C8_         13
#define         _C8          14
#define         _G7          19
#define         _F7_         20
#define         _D7_         24
#define         _C7_         27
#define         _C7          29
#define         _A6          35
#define         _G6          39
#define         _F6_         41
#define         _F6          44
#define         _E6          46
#define         _D6_         49
#define         _D6          52
#define         _C6_         55
#define         _C6          59
#define         _B5          62
#define         _A5_         66
#define         _A5          70
#define         _G5_         74
#define         _G5          79
#define         _F5_         84
#define         _F5          89
#define         _E5          94
#define         _D5_         100
#define         _D5          106
#define         _C5_         112
#define         _C5          119
#define         _B4          126
#define         _A4_         133
#define         _A4          141
#define         _G4_         150
#define         _G4          159
#define         _F4_         168
#define         _F4          178
#define         _E4          189
#define         _D4_         200
#define         _D4          212
#define         _C4_         225
#define         _C4          238
#define         _B3          252

//Notes and OCR1x values for Timer1 using n=1, 8MHz setting
#define         C0_         2144
#define         A6          2272
#define         A6_         2407
#define         G6          2550
#define         G6_         2702
#define         F6          2862
#define         E6          3033
#define         F6_         3213
#define         D6          3404
#define         D6_         3607
#define         C6          3821
#define         B5          4049
#define         C6_         4289
#define         A5          4544
#define         A5_         4815
#define         G5          5101
#define         G5_         5404
#define         F5          5726
#define         E5          6067
#define         F5_         6427
#define         D5          6809
#define         D5_         7214
#define         C5          7644
#define         B4          8098
#define         C5_         8580
#define         A4          9090
#define         A4_         9631
#define         G4          10203
#define         G4_         10810
#define         F4          11453
#define         E4          12134
#define         F4_         12855
#define         D4          13620
#define         D4_         14430
#define         C4          15288
#define         B3          16197
#define         C4_         17160
#define         A3          18181
#define         A3_         19262
#define         G3          20407
#define         G3_         21621
#define         F3          22907
#define         E3          24269
#define         F3_         25713
#define         D3          27241
#define         D3_         28861
#define         C3          30578
#define         B2          32396
#define         C3_         34322
#define         A2          36363
#define         A2_         38524
#define         G2          40815
#define         G2_         43242
#define         F2          45813
#define         E2          48537
#define         F2_         51426
#define         D2          54480
#define         D2_         57719
#define         C2          61152
#define         B1          64787


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////Melodies///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define		VOICE_0_NOTES						42
#define		VOICE_1_NOTES						47



/*
//42 Notes, Twinkle Twinkle Little Star;   500ms delays
#define		MUSIC_NOTES						42
const uint8_t melody_voice0[] PROGMEM = {_C4,_C4,_G5,_G5,_A5,_A5,_G5,_F4,_F4,_E4,
	_E4,_D4,_D4,_C4,_G5,_G5,_F4,_F4,_E4,_E4,_D4,_G4,_G5,_F5,_F4,_E4,_E4,_D4,_C4,
	_C4,_G5,_G5,_A5,_A5,_G5,_F4,_F4,_E4,_E4,_D4,_D4,_C4};
const uint16_t melody_voice0Delay[] PROGMEM = {50,50,50,50,50,50,100,50,50,50,50,50,
	50,100,50,50,50,50,50,50,100,50,50,50,50,50,50,100,50,50,50,50,50,50,100,50,50,
	50,50,50,50,100};
const uint16_t melody_voice1[] PROGMEM = {C5,C5,G6,G6,A6,A6,G6,F5,F5,E5,
	E5,D5,D5,C5,G6,G6,F5,F5,E5,E5,D5,G5,G6,F6,F5,E5,E5,D5,C5,
	C5,G6,G6,A6,A6,G6,F5,F5,E5,E5,D5,D5,C5};
const uint16_t melody_voice1Delay[] PROGMEM = {50,50,50,50,50,50,100,50,50,50,50,50,
	50,100,50,50,50,50,50,50,100,50,50,50,50,50,50,100,50,50,50,50,50,50,100,50,50,
50,50,50,50,100};
/*/


/*

//30+17 Notes, In the Jungle;(+17 of my notes)  250ms delays
#define		MUSIC_NOTES						47
const int8_t melody_voice0[] PROGMEM = {_G6,_A4,_B4,_A4,_B4,_C5,_B4,_A4,_G4,_A4,_B4,_A4,_G4,
	_B4,_A4,_D5,_B4,_A4,_B4,_D5,_C5,_B4,_A4,_G4,_A4,_B4,_A4,_B4,_A4,_G4, _F4, _E4, _D4, _D4,
	 _E4, _D4, _C4, _C4, _D4, _C4, _B4, _A4, _G4, _G4, _D4, _D4, _G4};
const uint16_t melody_voice0Delay[] PROGMEM = {46,46,46,22,22,45,45,45,22,22,45,45,
	45,45,180,45,45,45,22,22,45,45,45,22,22,45,45,45,45,180,25,25,10,10,25,25,25,25,10,10,25,25,26,27,12,12,200};
const int16_t melody_voice1[] PROGMEM = {G5,A5,B5,A5,B5,C6,B5,A5,G5,A5,B5,A5,G5,
	B5,A5,D6,B5,A5,B5,D6,C6,B5,A5,G5,A5,B5,A5,B5,A5,G5, F5, E5, D5, D5, E5, D5, C5, C5, D5, C5, B5, A5, G5, G5, D5, D5, G5};
const uint16_t melody_voice1Delay[] PROGMEM = {46,46,46,22,22,45,45,45,22,22,45,45,
	45,45,180,45,45,45,22,22,45,45,45,22,22,45,45,45,45,180,25,25,10,10,25,25,25,25,10,10,25,25,26,27,12,12,200};

*/
const uint8_t melody_voice0[] PROGMEM = {_C4,_C4,_G5,_G5,_A5,_A5,_G5,_F4,_F4,_E4,
	_E4,_D4,_D4,_C4,_G5,_G5,_F4,_F4,_E4,_E4,_D4,_G4,_G5,_F5,_F4,_E4,_E4,_D4,_C4,
_C4,_G5,_G5,_A5,_A5,_G5,_F4,_F4,_E4,_E4,_D4,_D4,_C4};
const uint16_t melody_voice0Delay[] PROGMEM = {50,50,50,50,50,50,100,50,50,50,50,50,
	50,100,50,50,50,50,50,50,100,50,50,50,50,50,50,100,50,50,50,50,50,50,100,50,50,
50,50,50,50,100};

const int16_t melody_voice1[] PROGMEM = {G5,A5,B5,A5,B5,C6,B5,A5,G5,A5,B5,A5,G5,
B5,A5,D6,B5,A5,B5,D6,C6,B5,A5,G5,A5,B5,A5,B5,A5,G5, F5, E5, D5, D5, E5, D5, C5, C5, D5, C5, B5, A5, G5, G5, D5, D5, G5};
const uint16_t melody_voice1Delay[] PROGMEM = {46,46,46,22,22,45,45,45,22,22,45,45,
45,45,180,45,45,45,22,22,45,45,45,22,22,45,45,45,45,180,25,25,10,10,25,25,25,25,10,10,25,25,26,27,12,12,200};



/*
//Pink Panther.....The Failure.....
#define		MUSIC_NOTES						39
const int8_t melody_voice0[] PROGMEM = {_C4_, _D4_, _E4, _F4_,_G4, _D4,_E4,_F4_,_G4,_B5,_G4_,_A4_,_B4,_C4_,_D4,_D4_,_E4,_E4,_B4,_E4,_G4,_B4,_A4_,_A4,_G4,_E4,_D4,_E4,_A4_,_A4,_G4_,_G4,_D4_,_D4,_C4_,_C4,_C4,_G4};
const uint16_t melody_voice0Delay[] PROGMEM = {110,10,90,10,90,10,30,10,50,30,10,30,10,390,10,30,10,230,220,270,10,30,10,130,10,10,10,10,170,10,30,10,390,10,30,10,230,350,270};
const int16_t melody_voice1[] PROGMEM = {C5_, D5_, E5, F5_,G5, D5,E5,F5_,G5,B5,G5_,A5_,B5,C5_,D5,D5_,E5,E5,B5,E5,G5,B5,A5_,A5,G5,E5,D5,E5,A5_,A5,G5_,G5,D5_,D5,C5_,C5,C5,G5};
const uint16_t melody_voice1Delay[] PROGMEM = {110,10,90,10,90,10,30,10,50,30,10,30,10,390,10,30,10,230,220,270,10,30,10,130,10,10,10,10,170,10,30,10,390,10,30,10,230,350,270};
*/


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////Data Direction/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
inline void setUpPushButton()
{
	MCUCR  &=~(1<<PUD);			//PullUP resistor is enabled
	DDRB   &=~(1<<PB0);
	PORTB  |=1<<PB0;			//PB0 is input for push button
}


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////Timers Setup///////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
inline void setUpTimers(){
	//Clock Select Bits for 8 and 16bit Timers
	//Mode		CS02:0		Prescaler (N)
	//0		000			No Clock
	//1		001			1
	//2		010			8
	//3		011			64
	//4		100			256
	//5		101			1024
	TCCR0B |= (0<<CS02)|(1<<CS01)|(1<<CS00); //ClockSelect of 8Bit Timer with N=64
	TCCR1B |=(0<<CS12)|(0<<CS11)|(1<<CS10); //ClockSelect of 16Bit Timer with N=1

	//Enabling CTC modes
	TCCR0A |= (1<<WGM01);	//8Bit timer
	TCCR1B |= (1<<WGM12);	//16Bit timer

	//Enabling Toggle On modes
	TCCR0A |= (1<<COM0A0);	//8Bit timer
	TCCR1A |= (1<<COM1A0);	//16Bit timer

	//Wake Up MCU 
	//Put MCU to 8MHz frequency mode
	//CLKPR = 1<<CLKPCE;
	//CLKPR = 1<<CLKPS0;
	
}



inline void deProgramTimers(){
	//Turn off Toggle mode
	TCCR0A &= ~(1<<COM0A0);	//8Bit timer
	TCCR1A &= ~(1<<COM1A0);	//16Bit Timer

	//Disable CTC
	TCCR0A &= ~(1<<WGM01);	//8Bit timer
	TCCR1B &= ~(1<<WGM12);	//16Bit Timer

	//Remove Clock Sources
	TCCR0B &=0; //ClockSelect of 8Bit Timer
	TCCR1B &=0; //ClockSelect of 16Bit Timer

	//Put MCU to sleep
	//Put MCU to power saving mode
	//CLKPR = 1<<CLKPCE;
	//CLKPR = 1<<CLKPS3;
}
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////Main///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	//Initial setup of the pins and timers and declare variables
	setUpPushButton();
	uint8_t i=0, j=0;
	int16_t note1Delay=0, note0Delay=0;

	_delay_us(3);	//To avoid a know bug caused by
	while (1)
	{
		//IF: wait for push button
		if((PINB & (1<<PB0))){	
		
		}
		//ELSE: Push button is activated and play the music
		else{

			//Initialize the counting variables
			i=0; j=0, note1Delay=0, note0Delay=0;


			// Set up timers by enabling toggle and CTC mode
			setUpTimers();

			// Set up th OCR pins as outputs
			if(voice1Play){
				DDRB |=(1<<PB3);
			}
			if(voice0Play){
				DDRB |=(1<<PB2);
			}


			//Play the melodies
			while(1){
				//If Possible, play VOICE_1
				if(voice1Play && (i<VOICE_1_NOTES)){
					if(note1Delay<=0){				//Only get the next note if the previous note has been delayed sufficiently
						note1Delay= pgm_read_byte(&melody_voice1Delay[i]);
						OCR1A = pgm_read_word(&melody_voice1[i++]);
					}
					}else if(note1Delay<=0){
						//Change the data direction for VOICE_1 to input
						DDRB &= ~(1<<PB3);	//OC1A=PB3
				}


				//If Possible, play VOICE_0
				if (voice0Play && (j<VOICE_0_NOTES)){
					//Only get the next note if the previous note has been delayed sufficiently
					if(note0Delay<=0){				
						DDRB &=~(1<<PB2);			//To space out consecutive-musical notes and add a "kick" between notes

						note0Delay= pgm_read_byte(&melody_voice0Delay[j]);
						OCR0A = pgm_read_byte(&melody_voice0[j++]);
						_delay_ms(10);				//To space out consecutive-musical notes
						DDRB |=(1<<PB2);			//To space out consecutive-musical notes
						//Whenever there is a delay, we should keep track of it
						note0Delay--;
						if(voice1Play){
							note1Delay--;
						}
					}
					//If the notes are all played then set OCR0A to an input to avoid any more note
					}else if(note0Delay<=0){
						//Change the data direction for VOICE_0 to input
						DDRB &= ~(1<<PB2);	//OC0A=PB2
				}

				//Delaying in increments; to reduce overheads delays should be reasonably
				// high and sufficiently low to avoid interfere with the timings of both voices
				_delay_ms(10);
				note0Delay-=1;
				note1Delay-=1;
				if((note0Delay<=0 || !voice0Play) &&(note1Delay<=0 || !voice1Play) && (i>=VOICE_1_NOTES || !voice1Play) && (j>=VOICE_0_NOTES || !voice0Play)){
					break;
				}
			}
			//Once the melodies are done:
			// 1. Turn off timers
			// 2. Turn the OCRnX into inputs
			deProgramTimers();
			if(voice1Play){
				DDRB &=~(1<<PB3);	//OC1A=PB3
			}
			if(voice0Play){
				DDRB &=~(1<<PB2);	//OC0A=PB2
			}



			// 1s wait-time before accepting the next push-button
			_delay_ms(1000);
		}
	}
	return 0;
}

