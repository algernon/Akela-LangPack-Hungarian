/* -*- mode: c++ -*-
 * Kaleidoscope-LangPack-Hungarian -- Hungarian language support
 * Copyright (C) 2016-2020  Gergely Nagy
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

#include <Kaleidoscope.h>
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
  handleKeyswitchEvent(key, UnknownKeyswitchLocation, IS_PRESSED | INJECTED);
  Kaleidoscope.hid().keyboard().sendReport();
  handleKeyswitchEvent(key, UnknownKeyswitchLocation, WAS_PRESSED | INJECTED);
  Kaleidoscope.hid().keyboard().sendReport();
}

EventHandlerResult Hungarian::onKeyswitchEvent(Key &mapped_key, KeyAddr key_addr, uint8_t keyState) {
  if (mapped_key.getRaw() < HUNGARIAN_FIRST || mapped_key.getRaw() > HUNGARIAN_LAST)
    return EventHandlerResult::OK;

  if (!keyToggledOn(keyState)) {
    return EventHandlerResult::EVENT_CONSUMED;
  }

  bool need_shift =
      Kaleidoscope.hid().keyboard().isModifierKeyActive(Key_LeftShift) ||
      ::OneShot.isModifierActive(Key_LeftShift);

  tap_key(compose_key);

  HungarianSymbol symbol = (HungarianSymbol)(mapped_key.getRaw() - HUNGARIAN_FIRST);
  Key accent;
  uint8_t kc = 0;

  accent.setFlags(KEY_FLAGS);
  accent.setKeyCode(Key_Quote.getKeyCode());

  switch (symbol) {
  case AA:
    kc = Key_A.getKeyCode();
    break;
  case OA:
    kc = Key_O.getKeyCode();
    break;
  case OU:
    kc = Key_O.getKeyCode();
    accent.setFlags(accent.getFlags() | SHIFT_HELD);
    break;
  case ODA:
    kc = Key_O.getKeyCode();
    accent.setRaw(Key_Equals.getRaw());
    break;
  case EA:
    kc = Key_E.getKeyCode();
    break;
  case UA:
    kc = Key_U.getKeyCode();
    break;
  case UU:
    kc = Key_U.getKeyCode();
    accent.setFlags(accent.getFlags() | SHIFT_HELD);
    break;
  case UDA:
    kc = Key_U.getKeyCode();
    accent.setRaw(Key_Equals.getRaw());
    break;
  case IA:
    kc = Key_I.getKeyCode();
    break;
  }

  if (accent.getFlags() & SHIFT_HELD)
    handleKeyswitchEvent(Key_LeftShift, UnknownKeyswitchLocation, IS_PRESSED | INJECTED);
  else
    handleKeyswitchEvent(Key_LeftShift, UnknownKeyswitchLocation, WAS_PRESSED | INJECTED);
  Kaleidoscope.hid().keyboard().sendReport();

  tap_key(accent);

  if (need_shift)
    handleKeyswitchEvent(Key_LeftShift, UnknownKeyswitchLocation, IS_PRESSED | INJECTED);
  else
    handleKeyswitchEvent(Key_LeftShift, UnknownKeyswitchLocation, WAS_PRESSED | INJECTED);

  mapped_key.setFlags(0);
  mapped_key.setKeyCode(kc);

  Kaleidoscope.hid().keyboard().sendReport();

  return EventHandlerResult::OK;
}

}
}

kaleidoscope::language::Hungarian Hungarian;
