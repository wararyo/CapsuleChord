#include <Keypad.h>

void CapsuleChordKeypad::begin() {
    Wire.begin();
}

void CapsuleChordKeypad::update() {
    Wire.requestFrom(KEYPAD_I2C_ADDR,1);
    while(Wire.available()) {
        int val = Wire.read();
        if(val != 0) {
            _events.push((char)val);
        }
    }
}

bool CapsuleChordKeypad::hasEvent() {
    return !_events.empty();
}

char CapsuleChordKeypad::getEvent() {
    char event = _events.front();
    _events.pop();
    return event;
}

CapsuleChordKeypad Keypad;