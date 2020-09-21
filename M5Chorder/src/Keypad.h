#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include <Wire.h>
#undef min
#include <queue>
#include <map>

#define KEYPAD_I2C_ADDR 0x09

/*** TWI Structure ***/
// State(1bit) | Button(7bit)
// State: 0 is pressed, 1 is released
// Button: see the following

/*** Buttons ***/
// Group(3bit) | Button(4bit)

#define Key_State_Pressed 0
#define Key_State_Released 1

// Numbers (Group 1)
#define Key_Custom1 1
#define Key_VIIdim 2
#define Key_Custom2 3
#define Key_IV 4
#define Key_I 5
#define Key_V 6
#define Key_IIm 7
#define Key_VIm 8
#define Key_IIIm 9
// Modifiers (Group 2)
#define Key_Ninth         0x10 | 1
#define Key_Thirteenth    0x10 | 2
#define Key_Sus4          0x10 | 5
#define Key_Augment       0x10 | 6
#define Key_SeventhInvert 0x10 | 4
#define Key_Seventh       0x10 | 3
#define Key_FlatFive      0x10 | 7
#define Key_ThirdInvert   0x10 | 8
// Sliders (Group 3)
#define Key_PitchUp       0x20 | 1
#define Key_PitchDown     0x20 | 2
#define Key_InversionUp   0x20 | 3
#define Key_InversionDown 0x20 | 4
// Function Keys (Group 4)
#define Key_Gyro       0x30 | 1
#define Key_Sustain    0x30 | 2

class CapsuleChordKeypad {
public: class Key;
private:
    std::queue<char> _events;
    std::map<int,Key> keys;
public:
    void begin();
    void update();
    bool hasEvent();
    char getEvent();
    void disposeEvents();
    
    class Key {
        private:
            int id;
            bool mIsPressed;
        public:
            void press();
            void release();
            Key(){}
            Key(int keyId): id(keyId) {}
            bool isPressed();
    };

    Key operator [](int key){
        if(keys.find(key) == keys.end()) keys[key] = Key(key); // create if not exist
        return keys[key];
    }
};

extern CapsuleChordKeypad Keypad;

#endif