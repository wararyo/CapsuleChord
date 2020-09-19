#include <M5Stack.h>
#include <vector>
#include <Preferences.h>
#include "M5StackUpdater.h"
#include "BLEMidi.h"
#include "Chord.h"
#include "Scale.h"
#include "Keypad.h"
#include "Menu.h"
#include "KeyMap/KeyMap.h"
#include "Context.h"

#define DEVICE_NAME "BLEChorder"

std::vector<uint8_t> playingNotes;
bool seventh = false;
int option = 0;

// Initialize at setup()
Scale *scale;
int *centerNoteNo;
Context context;

M5ButtonDrawer buttonDrawer;

typedef std::vector<SettingItem*> si;
typedef std::vector<const char *> strs;
Settings settings(si{
  new SettingItemEnum("Mode",{"CapsuleChord","CAmDion","Presets"},0),
  new SettingItemScale("Scale",Scale(0)),
  new SettingItemEnum("Bass",{"None","C1","C2"},0),
  new SettingItemEnum("Voicing",{"Open","Closed"},0),
  new SettingItemNumeric("CenterNoteNo",24,81,60),
  new SettingItemDegreeChord("Custom 1", DegreeChord(4,Chord::Minor|Chord::MajorSeventh)),
  new SettingItemDegreeChord("Custom 2", DegreeChord(5,Chord::Minor|Chord::Seventh)),
  new SettingItem("Keymap",si{
    new SettingItemEnum("Fuction 1",{"Gyro","Sustain","Note","CC"},0),
    new SettingItemEnum("Fuction 2",{"Gyro","Sustain","Note","CC"},1)
  }),
  new SettingItemEnum("SustainBehavior",{"Normal","Trigger"},0)
});

enum Scene : uint8_t {
  Connection,
  Play,
  FunctionMenu,
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
      M5.Lcd.setTextFont(1);
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
      M5.Lcd.println(scale->toString());
      buttonDrawer.setText(String("Fn"),String("I"),String("Menu"));//TODO:Change it to "Fn, ,Menu"
      buttonDrawer.draw(true);
    break;
    case Scene::FunctionMenu:
      Menu.update(true);
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
  sendNotes(true,chord.toMidiNoteNumbers(*centerNoteNo,16),120);
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

void setup() {
  M5.begin();
  Serial.begin(9600);

  //SD Updater
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }

  Keypad.begin();
  // Decline speaker noise
  M5.Speaker.begin();
  M5.Speaker.mute();

  // Load settings
  if(!settings.load()){
    Serial.println("settings.json is not found in SD, so I'll try to create it.");
    if(!settings.save()) Serial.println("Setting file creation failed.");
  }

  // Get setting items
  scale = &((SettingItemScale*)settings.findSettingByKey(String("Scale")))->content;
  centerNoteNo = &((SettingItemNumeric*)settings.findSettingByKey(String("CenterNoteNo")))->number;

  // Make Context
  context = Context(&settings);
  context.playChord = playChord;
  context.sendNotes = sendNotes;

  //Menu initialization
  M5ButtonDrawer::width = 106;
  Menu.begin(&settings);

  // Scene initialization
  changeScene(Scene::Connection);
  _changeScene_raw();

  Midi.begin(DEVICE_NAME, new ServerCallbacks(), NULL);
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
        changeScene(Scene::FunctionMenu);
        break;
      }
      
      // if(M5.BtnB.wasPressed()) {
      //   Chord c = scale->getDiatonic(0,Keypad[Key_Seventh].isPressed());
      //   if(Keypad[Key_ThirdInvert].isPressed())   thirdInvert(&c);
      //   if(Keypad[Key_FlatFive].isPressed())     fifthFlat(&c);
      //   if(Keypad[Key_Augment].isPressed())       augment(&c);
      //   if(Keypad[Key_Sus4].isPressed())          sus4(&c);
      //   // if(Keypad[Key_Seventh].isPressed()) thirdInvert(&c);
      //   if(Keypad[Key_SeventhInvert].isPressed()) seventhInvert(&c);
      //   if(Keypad[Key_Ninth].isPressed())         ninth(&c);
      //   if(Keypad[Key_Thirteenth].isPressed())    thirteenth(&c);
      //   playChord(c);
      // }
      if(M5.BtnB.wasReleased()) sendNotes(false,std::vector<uint8_t>(),120);

      Keypad.update();
      KeyMap::getAvailableKeyMaps()[0].get()->update();
      
      buttonDrawer.draw();
    break;
    case Scene::FunctionMenu:
      Menu.update();//Contains M5.update()
      // When requried, back to play scene
      if(Menu.isExitRequired()) {
        Menu.save();
        changeScene(Scene::Play);
      }
    break;
  }
  if(currentScene != requiredToChangeScene) _changeScene_raw();
}