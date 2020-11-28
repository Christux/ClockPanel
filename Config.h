/* 
 * Copyright (c) 2020 Christophe Rubeck.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as   
 * published by the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <EEPROM.h>
#include <ChristuxAnimation.h>

/*
 * Save data in eeprom
 * byte 0: main animation id
 * byte 1: separator animation id
 * byte 2: r
 * byte 3: g
 * byte 4: b
 * byte 5: mirror
 */

struct Config {

  void saveColor(ChristuxAnimation::RgbColor col) {
    EEPROM.begin(6);
    EEPROM.write(2, col.R);
    EEPROM.write(3, col.G);
    EEPROM.write(4, col.B);
    EEPROM.commit();
    EEPROM.end();
  }

  void saveMainAnimation(int id)
  {
    EEPROM.begin(6);
    EEPROM.write(0, id);
    EEPROM.commit();
    EEPROM.end();
  }

  void saveSeparatorAnimation(int id)
  {
    EEPROM.begin(6);
    EEPROM.write(1, id);
    EEPROM.commit();
    EEPROM.end();
  }

  void saveMirror(bool mirror)
  {
    EEPROM.begin(6);
    EEPROM.write(5, (int)mirror);
    EEPROM.commit();
    EEPROM.end();
  }

  int readMainAnimation() {
    EEPROM.begin(6);
    int id = EEPROM.read(0);
    EEPROM.end();
    return id;
  }

  int readSeparatorAnimation() {
    EEPROM.begin(6);
    int id = EEPROM.read(1);
    EEPROM.end();
    return id;
  }

  bool readMirror() {
    EEPROM.begin(6);
    int mirror = EEPROM.read(5);
    EEPROM.end();
    return (bool)mirror;
  }

  ChristuxAnimation::RgbColor readColor() {
    EEPROM.begin(6);
    ChristuxAnimation::RgbColor color(EEPROM.read(2),
                    EEPROM.read(3),
                    EEPROM.read(4));
    EEPROM.end();
    return color;
  }
};
