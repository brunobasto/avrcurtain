#define F_CPU 1000000

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "bit.h"

// P0 - Direction Left
// P1 - Direction Right
// P2 - End-stop Left
// P3 - End-stop Right
// P4 - Activation switch

void goLeft() {
	setBit(PORTB, PB0);
    unsetBit(PORTB, PB1);
}

void goRight() {
    unsetBit(PORTB, PB0);
    setBit(PORTB, PB1);
}

unsigned int isGoingLeft() {
    return (readBit(PORTB, PB0) != 0) && (readBit(PORTB, PB1) == 0);
}

unsigned int isGoingRight() {
    return (readBit(PORTB, PB0) == 0) && (readBit(PORTB, PB1) != 0);
}

unsigned int isAtEndStopLeft() {
    return readBit(PINB, PINB2) == 0;
}

unsigned int isAtEndStopRight() {
    return readBit(PINB, PINB3) == 0;
}

unsigned int isMoving() {
    return isGoingLeft() || isGoingRight();
}

void stop() {
    // Move away from end-stop
    if (isGoingLeft() && isAtEndStopLeft()) {
        goRight();
    }
    else if (isGoingRight() && isAtEndStopRight()) {
        goLeft();
    }

    _delay_ms(20);

    unsetBit(PORTB, PB0);
    unsetBit(PORTB, PB1);
}

unsigned int getActivationSwitchState() {
    unsigned int state = readBit(PINB, PINB4);
	
	uint8_t c = 0;

	while (readBit(PINB, PINB4) == state && state == 0)  {
		_delay_ms(10);

		c++;
	}

	return state == 0 && c >= 50;
}

// initial direction;
static int direction = 1;

void toggleMotorState() {
    if (isMoving()) {
        stop();
    }
    else if (isAtEndStopLeft()) {
        goRight();

        direction = -1;
    }
    else if (isAtEndStopRight()) {
        goLeft();

        direction = 1;
    }
    else {
        if (direction == -1) {
            goLeft();

            direction = 1;
        }
        else {
            goRight();

            direction = -1;
        }
    }
}

int main(void) {
	/*
	 * Direction
	 */

	// Set PB1 and PB2 as outputs and disable pull-up resistors
	setBit(DDRB, PB0);
	setBit(DDRB, PB1);

	stop();

	/*
	 * End-stops and Activation Switch
	 */

	// End-stops

	// Set PB3 and PB4 as inputs and enable pull-up resistors
	unsetBit(DDRB, PB2);
	unsetBit(DDRB, PB3);

	setBit(PORTB, PB2);
	setBit(PORTB, PB3);

	// Activation switch

	// Set PB5 as input and enable pull-up resistor
	unsetBit(DDRB, DDB4);
	setBit(PORTB, PB4);

	for (;;) {
		 if (isGoingLeft() && isAtEndStopLeft()) {
            stop();
        }
        else if (isGoingRight() && isAtEndStopRight()) {
            stop();
        }
        else if (getActivationSwitchState()) {
            toggleMotorState();
        }
	}

	return 0;
}