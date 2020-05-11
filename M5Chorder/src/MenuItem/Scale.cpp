#include "MenuItem/Scale.h"
#include "Scale.h"

String MenuItemScale::getStringOfItem(int value) {
  return Scale::getAvailableScales()[value]->name();
}