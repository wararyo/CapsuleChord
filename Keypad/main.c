#include "include/libraries/Wire/src/Wire.h"

#define I2C_ADDR 0x09

#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)    (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)))

/*** TWI Structure ***/
// State(1bit) | Button(7bit)
// State: 1 is pressed, 2 is released
// Button: see the following

/*** Buttons ***/
// Group(3bit) | Button(4bit)

// Numbers (Group 1)
#define I 1;
#define IIm 2;
#define IIIm 3;
#define IV 4;
#define V 5;
#define VIm 6;
#define VIIdim 7;
#define Custom1 8;
#define Custom2 9;
// Modifiers (Group 2)
#define Minor        0x10 | 1;
#define Seventh      0x10 | 2;
#define SeventhMinor 0x10 | 3;
#define Ninth        0x10 | 4;
// Sliders (Group 3)
#define PitchUp       0x20 | 1;
#define PitchDown     0x20 | 2;
#define InversionUp   0x20 | 3;
#define InversionDown 0x20 | 4;
// Function Keys (Group 4)
#define Sustain 0x30 | 1;
#define Gyro    0x30 | 2;

//L1 PD2
//L2 PD3
//L3 PD4
//L4 PD5
//L5 PD6

//C1 PC0
//C2 PC1
//C3 PC2
//C4 PC3

//IRQ PB0
#define IRQ_1 Set_Bit(PORTB,0)
#define IRQ_0 Clr_Bit(PORTB,0)

void setup()
{
	Set_Bit(DDRB, 0);
	IRQ_1;
	DDRC = 0x00;
	PORTC = 0xff;
	DDRD = 0xff;
	PORTD = 0x00;
	Wire.begin(I2C_ADDR);
	Wire.onRequest(requestEvent);
}


unsigned char i = 0;
unsigned char temp = 0b00100100,  hadPressed = 0;
void requestEvent()
{
	Wire.write(temp);
	return;
}

void loop()
{

}

void main() {
	setup();
	while(1) loop();
}