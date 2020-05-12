#include <M5Stack.h>
#include <M5TreeView.h>
#include <vector>
#include <MenuItemToggle.h>
#include <MenuItemNumeric.h>
#include <Preferences.h>
#include "M5StackUpdater.h"
#include "MenuItemKey.h"
#include "MenuItemScale.h"
#include "BLEMidi.h"
#include "Chord.h"
#include "Scale.h"
#include "Keypad.h"

#define DEVICE_NAME "BLEChorder"

std::vector<uint8_t> playingNotes;
Scale scale = Scale(0);
bool seventh = true;
uint8_t centerNoteNo = 64;

M5TreeView tv;
typedef std::vector<MenuItem*> vmi;
M5ButtonDrawer buttonDrawer;

//画面
enum Scene : uint8_t {
  Connection,
  Play,
  Menu,
  length
};

Scene currentScene = Scene::length; //Specify a value other than Connection in order to draw correctly at the first changeScene()
Scene requiredToChangeScene;

void changeScene(Scene scene) {
  //To draw at the end of loop(), set flag
  requiredToChangeScene = scene;
}

//Used only in setup() and the end of loop() Do not touch it anywhere else!
void _changeScene_raw() {
  M5.Lcd.clear();

  //終了処理
  switch(currentScene) {
  }

  //開始処理
  switch(requiredToChangeScene) {
    case Scene::Connection:
      M5.Lcd.setCursor(0, 48);
      M5.Lcd.setTextSize(4);
      M5.Lcd.println("CapsuleChord");
      M5.Lcd.setTextSize(2);
      M5.Lcd.println("BLE MIDI Chordpad Device\n");
      M5.Lcd.setTextSize(1);
      M5.Lcd.println("Advertising...");
      M5.Lcd.println("Press the button A to power off.");
    break;
    case Scene::Play:
      M5.Lcd.setCursor(0,0);
      M5.Lcd.setTextSize(2);
      M5.Lcd.println(scale.toString());
      buttonDrawer.setText(String("Fn"),String("I"),String("Menu"));//TODO:Change it to "Fn, ,Menu"
      buttonDrawer.draw(true);
    break;
    case Scene::Menu:
      tv.begin();
    break;
  }
  currentScene = requiredToChangeScene;
}

void sendNotes(bool isNoteOn, std::vector<uint8_t> notes, int vel) {
  if(isNoteOn) {
    for(uint8_t n : notes) {
      Midi.sendNote(0x90, n, vel);
    }
    playingNotes.insert(playingNotes.end(),notes.begin(),notes.end());
  }
  else {
    for(uint8_t n : playingNotes) {
      Midi.sendNote(0x80, n, 0);
    }
    playingNotes.clear();
  }
}

void playChord(Chord chord) {
  sendNotes(true,chord.toMidiNoteNumbers(centerNoteNo,16),120);
  M5.Lcd.setTextSize(4);
  M5.Lcd.fillRect(0,60,320,120,BLACK);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextSize(5);
  M5.Lcd.drawString(chord.toString(), 160, 120, 2);
  M5.Lcd.setTextDatum(TL_DATUM);
}

class ServerCallbacks: public BLEMidiServerCallbacks {
    void onConnect(BLEServer* pServer) {
      BLEMidiServerCallbacks::onConnect(pServer);
      changeScene(Scene::Play);
    };

    void onDisconnect(BLEServer* pServer) {
      BLEMidiServerCallbacks::onDisconnect(pServer);
      changeScene(Scene::Connection);
    }
};

void callBackKey(MenuItem* sender) {
  MenuItemKey* mi((MenuItemKey*)sender);
  scale.key = mi->value;
}

void callBackScale(MenuItem* sender) {
  MenuItemKey* mi((MenuItemKey*)sender);
  scale.currentScale = Scale::getAvailableScales()[mi->value].get();
}

void callBackSeventh(MenuItem* sender) {
  MenuItemToggle* mi((MenuItemToggle*)sender);
  seventh = mi->value;
}

void callBackCenterNoteNo(MenuItem* sender) {
  MenuItemNumeric* mi((MenuItemNumeric*)sender);
  centerNoteNo = mi->value;
}

void setup() {
  M5.begin();
  Keypad.begin();
  Serial.begin(9600);
  // Decline speaker noise
  M5.Speaker.begin();
  M5.Speaker.mute();

  //SD Updater
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }

  changeScene(Scene::Connection);
  _changeScene_raw();

  Midi.begin(DEVICE_NAME, new ServerCallbacks(), NULL);

  //MenuMenu
  M5ButtonDrawer::width = 106;
  tv.clientRect.x = 60;
  tv.clientRect.y = 16;
  tv.clientRect.w = 240;
  tv.clientRect.h = 200;
  tv.itemWidth = 192;
  tv.itemHeight = 24;
  tv.setTextFont(2);
  tv.setItems(vmi{
    new MenuItemKey("Key", scale.key, callBackKey),
    new MenuItemScale("Scale", 0, callBackScale),
    new MenuItemToggle("Seventh",seventh,callBackSeventh),
    new MenuItemNumeric("CenterNoteNo",24,81,centerNoteNo,callBackCenterNoteNo)
  });  
}

void loop() {
  switch(currentScene) {
    case Scene::Connection:
      M5.update();
      if(M5.BtnA.wasPressed()) M5.Power.deepSleep();
    break;
    case Scene::Play:
      M5.update();
      if(M5.BtnC.pressedFor(100)) { //Go to Menu Scene
        sendNotes(false,std::vector<uint8_t>(),120);
        changeScene(Scene::Menu);
        break;
      }
      if(M5.BtnA.wasPressed())  playChord(scale.getDiatonic(2,seventh)); //For testing TODO:delete it
      if(M5.BtnA.wasReleased()) sendNotes(false,std::vector<uint8_t>(),120);
      Keypad.update();
      while(Keypad.hasEvent()){
        char event = Keypad.getEvent();
        switch(event >> 7 & 0b1) {
          case Key_State_Pressed:
            if((event & 0b1110000) == 0) {
              uint8_t number = (event & 0b1111) - 1;
              if(0 <= number && number <= 6) playChord(scale.getDiatonic(number,seventh));
            }
          break;
          case Key_State_Released:
            if((event & 0b1110000) == 0) sendNotes(false,std::vector<uint8_t>(),120);
          break;
        }
      }
      buttonDrawer.draw();
    break;
    case Scene::Menu:
      tv.update();//Contains M5.update(); (probably...)
      // Press A at root to back to play scene
      if(M5.BtnA.wasPressed() && (M5TreeView::getFocusItem()->parentItem() == &tv)) changeScene(Scene::Play);
    break;
  }
  if(currentScene != requiredToChangeScene) _changeScene_raw();
}