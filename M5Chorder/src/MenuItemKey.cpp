#include "MenuItemKey.h"

String MenuItemKey::getStringOfItem(int value) {
  return Chord::rootStrings[value];
}