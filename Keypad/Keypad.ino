#include <Wire.h>
#include "Queue.h"
#include "Set.h"

#define I2C_ADDR 0x09

#define sbi(val, bitn)    (val |=(1<<(bitn)))
#define cbi(val, bitn)    (val&=~(1<<(bitn)))
#define tbi(addr,bitn)    addr ^=  (1<<bitn)
#define Get_Bit(val, bitn)    (val &(1<<(bitn)))

/*** TWI Structure ***/
// State(1bit) | Button(7bit)
// State: 1 is pressed, 2 is released
// Button: see the following

/*** Buttons ***/
// Group(3bit) | Button(4bit)

// Numbers (Group 1)
#define I 1
#define IIm 2
#define IIIm 3
#define IV 4
#define V 5
#define VIm 6
#define VIIdim 7
#define Custom1 8
#define Custom2 9
// Modifiers (Group 2)
#define Minor        0x10 | 1
#define Seventh      0x10 | 2
#define SeventhMinor 0x10 | 3
#define Ninth        0x10 | 4
// Sliders (Group 3)
#define PitchUp       0x20 | 1
#define PitchDown     0x20 | 2
#define InversionUp   0x20 | 3
#define InversionDown 0x20 | 4
// Function Keys (Group 4)
#define Sustain 0x30 | 1
#define Gyro    0x30 | 2

/*** PIN ***/
// Board1
// X1 PD0 OUTPUT
// X2 PD1 OUTPUT
// X3 PD2 OUTPUT
// Y1 PD3 INPUT_PULLUP
// Y2 PD4 INPUT_PULLUP
// Y3 PD5 INPUT_PULLUP
// State: 1 is pressed, 2 is released
// Button: see the following

//IRQ PB0
#define IRQ_1 sbi(PORTB,0)
#define IRQ_0 cbi(PORTB,0)

char temp = 0b10000000;

class ButtonSet : public Set<char> {
  private:
    static char initializer();
    static char validator(char item);
    static char comparator(char a,char b);
  public:
    // using Set<char>::Set
    ButtonSet(int size = 256) : Set(size,initializer,validator,comparator){}
};

char ButtonSet::initializer() {
  return 0;
}
char ButtonSet::validator(char item){
  return ((item & 0b01111111) != 0) ? 1 : 0;
}
char ButtonSet::comparator(char a,char b) {
  if(a == NULL || b == NULL) return 0;
  return (a == b) ? 1 : 0;
}

void setup()
{
  sbi(DDRB, 1);
  sbi(PORTB,1);
  IRQ_1;
  Wire.begin(I2C_ADDR);
  Wire.onRequest(requestEvent);
  matrixInit();
}

void matrixInit() {
  DDRD &= ~(0b00111000); //cbi INPUT
  DDRD |=  (0b00000111); //sbi OUTPUT
  PORTD &= ~(0b00000000); //cbi
  PORTD |=  (0b00111111); //sbi
}

Queue<char> events = Queue<char>();

void requestEvent()
{
  Wire.write(temp);//if(events.count() != 0) Wire.write(events.pop());
  return;
}

void setKey(char state, char button) {
  //events.push((state & 1) << 7 | button & 0b01111111);
}

void loop()
{
  PORTD &= ~(0b001); //cbi
  PORTD |=  (0b110); //sbi
  delay(1);
  setKey(bit_is_clear(PIND,3),Custom1);
  setKey(bit_is_clear(PIND,4),VIIdim);
  setKey(bit_is_clear(PIND,5),Custom2);
  delay(1);

  PORTD &= ~(0b010); //cbi
  PORTD |=  (0b101); //sbi
  delay(1);
  setKey(bit_is_clear(PIND,3),IV);
  setKey(bit_is_clear(PIND,4),I);
  setKey(bit_is_clear(PIND,5),V);
  delay(1);

  PORTD &= ~(0b100); //cbi
  PORTD |=  (0b011); //sbi
  delay(1);
  setKey(bit_is_clear(PIND,3),IIm);
  setKey(bit_is_clear(PIND,4),VIm);
  setKey(bit_is_clear(PIND,5),IIIm);
  delay(1);
}
