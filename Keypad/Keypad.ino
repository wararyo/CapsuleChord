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
// State: 0 is pressed, 1 is released
// Button: see the following

/*** Buttons ***/
// Group(3bit) | Button(4bit)

#define PRESSED 0
#define RELEASED 1

// Numbers (Group 1)
#define Custom1 1
#define VIIdim 2
#define Custom2 3
#define IV 4
#define I 5
#define V 6
#define IIm 7
#define VIm 8
#define IIIm 9
// Modifiers (Group 2)
#define Thirteenth    0x10 | 1
#define Ninth         0x10 | 2
#define Sus4          0x10 | 3
#define Augment       0x10 | 4
#define SeventhInvert 0x10 | 5
#define Seventh       0x10 | 6
#define FlatFive      0x10 | 7
#define ThirdInvert   0x10 | 8
// Sliders (Group 3)
#define PitchUp       0x20 | 1
#define PitchDown     0x20 | 2
#define InversionUp   0x20 | 3
#define InversionDown 0x20 | 4
// Function Keys (Group 4)
#define Gyro       0x30 | 1
#define Sustain    0x30 | 2

//TODO: Board1もColumnを出力に、Rowを入力にする

/*** PIN ***/
/* I2C */
// SDA PC4
// SCL PC5
/* Board1 (Numbers) */
// Row1 PD0 OUTPUT
// Row2 PD1 OUTPUT
// Row3 PD2 OUTPUT
// Column1 PD3 INPUT_PULLUP
// Column2 PD4 INPUT_PULLUP
// Column3 PD5 INPUT_PULLUP
/* Board2 (Modifiers) */
// Row1 PB0 INPUT_PULLUP
// Row2 PB1 INPUT_PULLUP
// Row3 PB2 INPUT_PULLUP
// Row4 PB3 INPUT_PULLUP
// Column1 PB4 OUTPUT
// Column2 PB5 OUTPUT
/* Sliders */
// PitchUp PD6 INPUT_PULLUP
// PitchDown PD7 INPUT_PULLUP
// InversionUp PB6 INPUT_PULLUP
// InversionDown PB7 INPUT_PULLUP
/* Functions */
// Gyro PC0 INPUT_PULLUP
// Sustain PC1 INPUT_PULLUP

class ButtonSet : public Set<char> {
  private:
    static char initializer();
    static char validator(char item);
    static char comparator(char a,char b);
  public:
    ButtonSet(int size = 256) : Set(size,initializer,validator,comparator){}
};
char ButtonSet::initializer() {
  return 0;//Since there is no 0 in the button ID, 0 is used instead of NULL.
}
char ButtonSet::validator(char item){
  //0 is invalid because there is no 0 in the button ID.
  return ((item & 0b01111111) != 0) ? 1 : 0;
}
char ButtonSet::comparator(char a,char b) {
  if(a == NULL || b == NULL) return 0;
  return (a == b) ? 1 : 0;
}

void setup()
{
  Wire.begin(I2C_ADDR);
  Wire.onRequest(requestEvent);
  matrixInit();
  cbi(DDRD,6);//PitchUp
  cbi(DDRD,7);//PitchDown
  cbi(DDRB,6);//InversionUp
  cbi(DDRB,7);//InversionDown
  cbi(DDRC,0);//Gyro
  cbi(DDRC,1);//Sustain
  sbi(PORTD,6);//PitchUp
  sbi(PORTD,7);//PitchDown
  sbi(PORTB,6);//InversionUp
  sbi(PORTB,7);//InversionDown
  sbi(PORTC,0);//Gyro
  sbi(PORTC,1);//Sustain
}

void matrixInit() {
  DDRD &= ~(0b00111000); //cbi INPUT
  DDRD |=  (0b00000111); //sbi OUTPUT
  PORTD &= ~(0b00000000); //cbi
  PORTD |=  (0b00111111); //sbi

  DDRB &= ~(0b00001111); //cbi INPUT
  DDRB |=  (0b00110000); //sbi OUTPUT
  PORTB &= ~(0b00000000); //cbi
  PORTB |=  (0b00111111); //sbi
}

Queue<char> events = Queue<char>(); //Data to be sent to the host
ButtonSet pressing = ButtonSet(8); //The buttons currently pressed

void requestEvent()
{
  if(events.count() != 0) Wire.write(events.pop());
  return;
}

void setKey(char state, char button) {
  state = (state != 0) ? PRESSED : RELEASED;
  char event = (state & 1) << 7 | button & 0b01111111;
  switch(state){
    case PRESSED:
      if(pressing.Add(button)) events.push(event);
    break;
    case RELEASED:
      if(pressing.Remove(button)) events.push(event);
    break;
  }
}

void loop()
{
  /* Board1 */
  // Row 1
  PORTD &= ~(0b001); //cbi
  PORTD |=  (0b110); //sbi
  delay(1);
  setKey(bit_is_clear(PIND,3),Custom1);
  setKey(bit_is_clear(PIND,4),VIIdim);
  setKey(bit_is_clear(PIND,5),Custom2);
  delay(1);

  // Row 2
  PORTD &= ~(0b010); //cbi
  PORTD |=  (0b101); //sbi
  delay(1);
  setKey(bit_is_clear(PIND,3),IV);
  setKey(bit_is_clear(PIND,4),I);
  setKey(bit_is_clear(PIND,5),V);
  delay(1);

  // Row 3
  PORTD &= ~(0b100); //cbi
  PORTD |=  (0b011); //sbi
  delay(1);
  setKey(bit_is_clear(PIND,3),IIm);
  setKey(bit_is_clear(PIND,4),VIm);
  setKey(bit_is_clear(PIND,5),IIIm);
  delay(2);

  /* Board2 */
  // Column 1
  PORTB &= ~(0b010000); //cbi
  PORTB |=  (0b100000); //sbi
  delay(1);
  setKey(bit_is_clear(PINB,0),Ninth);
  setKey(bit_is_clear(PINB,1),SeventhInvert);
  setKey(bit_is_clear(PINB,2),Augment);
  setKey(bit_is_clear(PINB,3),FlatFive);
  delay(1);

  // Column 2
  PORTB &= ~(0b100000); //cbi
  PORTB |=  (0b010000); //sbi
  delay(1);
  setKey(bit_is_clear(PINB,0),Thirteenth);
  setKey(bit_is_clear(PINB,1),Seventh);
  setKey(bit_is_clear(PINB,2),Sus4);
  setKey(bit_is_clear(PINB,3),ThirdInvert);
  delay(2);

  /* Other */
  setKey(bit_is_set(PIND,6),PitchUp);
  setKey(bit_is_set(PIND,7),PitchDown);
  setKey(bit_is_set(PINB,6),InversionUp);
  setKey(bit_is_set(PINB,7),InversionDown);
  setKey(bit_is_set(PINC,0),Gyro);
  setKey(bit_is_set(PINC,1),Sustain);
  delay(2);
}
