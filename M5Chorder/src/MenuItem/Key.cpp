#include "MenuItem/Key.h"

String MenuItemKey::getStringOfItem(int value) {
  return Chord::rootStrings[value];
}