/* Author: whe024
 * Partner(s) Name: Wentao He
 * Lab Section: A21
 * Assignment: Lab #10  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B 	= 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect)
{
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

enum ThreeLEDsSM {BitZero, BitOne, BitTwo} threeLEDs;
enum BlinkingLEDSM {BitThree, Zero} blinkingLED;

unsigned char LEDsBehavior = 0x00;
unsigned char BlinkingBehavior = 0x00;

void L_Tick();
void B_Tick();

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF;
    PORTB = 0x00;
    /* Insert your solution below */
    unsigned long L_elapsedTime = 0;
    unsigned long B_elapsedTime = 0;
    TimerSet(1); 
    TimerOn();  
    TimerFlag = 0;
    threeLEDs = BitZero;
    blinkingLED = BitThree;
    while (1) {
	if(L_elapsedTime >= 300)
	{
		L_Tick();
		L_elapsedTime = 0;
	}
	if(B_elapsedTime >= 1000)
	{
		B_Tick();
		B_elapsedTime = 0;
	}
	PORTB = (LEDsBehavior | BlinkingBehavior);
	while(!TimerFlag){};
	TimerFlag = 0;
	L_elapsedTime += 8;
	B_elapsedTime += 8;
    }
    return 1;
}

void L_Tick(){
	switch(threeLEDs){
                case BitZero:
                        LEDsBehavior = 0x01;
                        threeLEDs = BitOne;
                        break;
                case BitOne:
                        LEDsBehavior = 0x02;
                        threeLEDs = BitTwo;
                        break;
                case BitTwo:
                        LEDsBehavior = 0x04;
                        threeLEDs = BitZero;
                        break;
                default:
                        threeLEDs = BitZero;
        }
}

void B_Tick(){		
	switch(blinkingLED){
		case BitThree:
			BlinkingBehavior = 0x00;
			blinkingLED = Zero;
			break;
		case Zero:
			BlinkingBehavior = 0x08;
			blinkingLED = BitThree;
			break;
		default:
			blinkingLED = BitThree;
	}
}
