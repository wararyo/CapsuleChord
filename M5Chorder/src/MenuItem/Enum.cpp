#include "MenuItem/Enum.h"

String MenuItemEnum::getStringOfItem(int value) {
  return String(member[value]);
}