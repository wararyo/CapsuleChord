#ifndef _BLEMIDI_H_
#define _BLEMIDI_H_

#include <string>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define MIDI_SERVICE_UUID        "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define MIDI_CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"

class BLEMidi {
public:
    BLEServer *server;
    BLEAdvertising *advertising;
    BLECharacteristic *characteristic;

    std::string deviceName = "BLE MIDI Device";
    std::string midiServiceUUID = MIDI_SERVICE_UUID;
    std::string midiCharacteristicUUID = MIDI_CHARACTERISTIC_UUID;

    bool isConnected = false;

    void begin(std::string deviceName,
        BLEServerCallbacks *serverCallback,
        BLECharacteristicCallbacks *chatacteristicCallback);

    void sendNote(uint8_t status, uint8_t noteNo, uint8_t vel);

    void sendCC(uint8_t ccNo, uint8_t value);
};

extern BLEMidi Midi;

class BLEMidiServerCallbacks: public BLEServerCallbacks {
public:
    virtual void onConnect(BLEServer* pServer) {
        Midi.isConnected = true;
    };

    virtual void onDisconnect(BLEServer* pServer) {
        Midi.isConnected = false;
    }
};

#endif