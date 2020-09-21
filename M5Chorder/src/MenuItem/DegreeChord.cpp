#include "MenuItem/DegreeChord.h"
#include <Rect16.h>
#include "Keypad.h"
#include "Scale.h"

void MenuItemDegreeChord::onAfterDraw(){
  drawChord(value, 0);
}

void MenuItemDegreeChord::setValue(DegreeChord value)
{
  this->value = value;
  if (callback) callback(this);
}

void MenuItemDegreeChord::onEnter() {
  draw();
  M5ButtonDrawer btnDrawer;
  btnDrawer.setText("Cancel","OK","Reset");
  btnDrawer.draw(true);
  drawChord(value, 1);
  DegreeChord pv = DegreeChord();
  DegreeChord temp = DegreeChord(value.root,value.option);
  Keypad.update();
  Keypad.disposeEvents();
  for (;;) {
    M5.update();
    if(M5.BtnA.wasReleased()) break;
    if(M5.BtnB.wasReleased()) {setValue(temp); break;}
    if(M5.BtnC.wasReleased()) {temp.root = 0; temp.option = 0;}

    // Input Chord
    Keypad.update();
    while(Keypad.hasEvent()){
      char event = Keypad.getEvent();
      switch(event >> 7 & 0b1) {
        case Key_State_Pressed:
          if((event & 0b1110000) == 0) {// Root
            uint8_t number = (event & 0b1111) - 1;
            if(0 <= number && number <= 6) temp.root = MajorScale::pitch[number];
          }
        break;
        case Key_State_Released:
        break;
      }
    }

    // Draw
    btnDrawer.draw();
    if (!pv.equals(temp)) {
      drawChord(temp,1);
      pv.root = temp.root;
      pv.option = temp.option;
    } else {
      delay(10);
    }
  }
  M5.update();//Let BtnA.wasReleased return false
}

void MenuItemDegreeChord::drawChord(DegreeChord value, int flg)
{
  applyFont();
  String str(value.toString());
  int w = std::max(M5.Lcd.textWidth(str)+4, 64);
  Rect16 r ( rect.right() - (w + 8)
           , rect.y + 2
           , w
           , rect.h - 4);
  M5.Lcd.drawRect(r.x+1, r.y, r.w-2, r.h, frameColor[flg]);
  M5.Lcd.drawRect(r.x, r.y+1, r.w, r.h-2, frameColor[flg]);
  r.inflate(-2);

  M5.Lcd.setTextColor(fontColor[flg]);
  M5.Lcd.fillRect(r.x, r.y, r.w, r.h, backColor[flg]);
  M5.Lcd.drawRightString(str,r.right(), rect.y + (rect.h - fontHeight()) / 2, font);
}

void MenuItemDegreeChord::drawWindow(DegreeChord value){

}
