#include "CapsuleChordKeyMap.h"
#include "Modifier.h"

const uint8_t CapsuleChordKeyMap::numberKeyMap[] = {
    0, //Custom1
    6, //VII
    0, //Custom2
    3, //IV
    0, //I
    4, //V
    1, //II
    5, //VI
    2};

void CapsuleChordKeyMap::update() {
  while(Keypad.hasEvent()){
    char event = Keypad.getEvent();
    switch(event >> 7 & 0b1) {
      case Key_State_Pressed:
        switch(event >> 4 & 0b111) {
          case 0: {// Numbers Pressed
            uint8_t number = numberKeyMap[(event & 0b1111) - 1]; // Key number starts from 1
            if(0 <= number && number <= 6) {
              Chord c = context->scale->getDiatonic(number,Keypad[Key_Seventh].isPressed());
              if(Keypad[Key_ThirdInvert].isPressed())   thirdInvert(&c);
              if(Keypad[Key_FlatFive].isPressed())     fifthFlat(&c);
              if(Keypad[Key_Augment].isPressed())       augment(&c);
              if(Keypad[Key_Sus4].isPressed())          sus4(&c);
              // if(Keypad[Key_Seventh].isPressed()) thirdInvert(&c);
              if(Keypad[Key_SeventhInvert].isPressed()) seventhInvert(&c);
              if(Keypad[Key_Ninth].isPressed())         ninth(&c);
              if(Keypad[Key_Thirteenth].isPressed())    thirteenth(&c);
              if(Keypad[Key_PitchUp].isPressed()) pitchUp(&c);
              if(Keypad[Key_PitchDown].isPressed()) pitchDown(&c);
              context->playChord(c);
            }
          } break;
        }
      break;
      case Key_State_Released:
        if((event & 0b1110000) == 0) context->sendNotes(false,std::vector<uint8_t>(),120);
      break;
    }
  }
}
