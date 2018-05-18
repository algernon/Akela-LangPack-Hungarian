/* -*- mode: c++ -*-
 * Kaleidoscope-LangPack-Hungarian -- Hungarian language support
 * Copyright (C) 2016, 2017  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Kaleidoscope-LangPack-Hungarian.h>
#include <Kaleidoscope-OneShot.h>

typedef enum {
  AA,
  OA,
  OU,
  ODA,
  EA,
  UA,
  UU,
  UDA,
  IA,
} HungarianSymbol;

namespace kaleidoscope {
namespace language {

static void tap_key(Key key) {
  handleKeyswitchEvent(key, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
  hid::sendKeyboardReport();
  handleKeyswitchEvent(key, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
  hid::sendKeyboardReport();
}

EventHandlerResult Hungarian::onKeyswitchEvent(Key &mapped_key, byte row, byte col, uint8_t keyState) {
  if (mapped_key.raw < HUNGARIAN_FIRST || mapped_key.raw > HUNGARIAN_LAST)
    return EventHandlerResult::OK;

  if (!keyToggledOn(keyState)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  bool need_shift = hid::isModifierKeyActive(Key_LeftShift) ||
                    ::OneShot.isModifierActive(Key_LeftShift);

  tap_key(Key_RightAlt);

  HungarianSymbol symbol = (HungarianSymbol)(mapped_key.raw - HUNGARIAN_FIRST);
  Key accent;
  uint8_t kc = 0;

  accent.flags = KEY_FLAGS;
  accent.keyCode = Key_Quote.raw;

  switch (symbol) {
  case AA:
    kc = Key_A.keyCode;
    break;
  case OA:
    kc = Key_O.keyCode;
    break;
  case OU:
    kc = Key_O.keyCode;
    accent.flags |= SHIFT_HELD;
    break;
  case ODA:
    kc = Key_O.keyCode;
    accent.raw = Key_Equals.raw;
    break;
  case EA:
    kc = Key_E.keyCode;
    break;
  case UA:
    kc = Key_U.keyCode;
    break;
  case UU:
    kc = Key_U.keyCode;
    accent.flags |= SHIFT_HELD;
    break;
  case UDA:
    kc = Key_U.keyCode;
    accent.raw = Key_Equals.raw;
    break;
  case IA:
    kc = Key_I.keyCode;
    break;
  }

  if (accent.flags & SHIFT_HELD)
    handleKeyswitchEvent(Key_LeftShift, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
  else
    handleKeyswitchEvent(Key_LeftShift, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);
  hid::sendKeyboardReport();

  tap_key(accent);

  if (need_shift)
    handleKeyswitchEvent(Key_LeftShift, UNKNOWN_KEYSWITCH_LOCATION, IS_PRESSED | INJECTED);
  else
    handleKeyswitchEvent(Key_LeftShift, UNKNOWN_KEYSWITCH_LOCATION, WAS_PRESSED | INJECTED);

  mapped_key = kc;

  hid::sendKeyboardReport();

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::language::Hungarian Hungarian;
