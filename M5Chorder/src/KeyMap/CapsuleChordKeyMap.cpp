#include "CapsuleChordKeyMap.h"
#include "Modifier.h"

void CapsuleChordKeyMap::update() {
  while(Keypad.hasEvent()){
    char event = Keypad.getEvent();
    switch(event >> 7 & 0b1) {
      case Key_State_Pressed:
        switch(event >> 4 & 0b111) {
          case 0: {// Numbers Pressed
            uint8_t number = (event & 0b1111) - 1;
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
