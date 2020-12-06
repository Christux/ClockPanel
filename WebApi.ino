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

int setConfig(String data)
{ 
  // Init Json buffer
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data);

  if (root.success())
  {
    JsonVariant animation = root["animation"];
    if (animation.success()) 
    {
      int anim = root["animation"].as<int>();

      if (anim >= 0 && anim < clockPanelAnimator.animCount()) {
        config.saveMainAnimation(anim);
        clockPanelAnimator.setAnimation(anim);
      }
      else return -1;
    }

    JsonVariant separator = root["separator"];
    if (separator.success()) 
    {
      int anim = root["separator"].as<int>();

      if (anim >= 0 && anim < clockPanelAnimator.getSeparatorCount()) {
        config.saveSeparatorAnimation(anim);
        clockPanelAnimator.setSeparatorAnimation(anim);
      }
      else return -1;
    }

    JsonVariant mirror = root["mirror"];
    if (mirror.success()) 
    {
        int anim = root["mirror"].as<bool>();
        config.saveMirror(anim);
        clockPanel.setMirror(anim);
    }

    JsonVariant color = root["color"];
    if (color.success())
    {
      ChristuxAnimation::RgbColor rgb(0,0,0);

      JsonVariant red = color["red"];
      if(red.success())
      {
        rgb.R = red.as<char>();
      }
      else return -1;

      JsonVariant green = color["green"];
      if(green.success())
      {
        rgb.G = green.as<char>();
      }
      else return -1;

      JsonVariant blue = color["blue"];
      if(blue.success())
      {
        rgb.B = blue.as<char>();
      }
      else return -1;
      
      config.saveColor(rgb);
      clockPanelAnimator.setColor(rgb);
    }
  }
  else return -1;
  
  return 0;
}

void getConfig(char* buffer, int bufferSize)
{
  int animation = clockPanelAnimator.getAnimation();
  int separator = clockPanelAnimator.getSeparatorAnimation();
  bool mirror = clockPanel.getMirror();
  ChristuxAnimation::RgbColor rgb = clockPanelAnimator.getColor();
  
  // Init Json buffer
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonObject& color = root.createNestedObject("color");

  root["animation"] = animation;
  root["separator"] = separator;
  root["mirror"] = mirror;
  color["red"] = rgb.R;
  color["green"] = rgb.G;
  color["blue"] = rgb.B;

  // Prints Json object to string buffer
  root.printTo(buffer, bufferSize);
}

void getInfo(char* buffer, int bufferSize)
{
 // Init Json buffer
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["vendor"] = "Christux";
  root["model"] = "ClockPanel01";
  root["version"] = 1;
  root["serial"] = "PROTO01";
  root["firmware_build_date"] = __DATE__ " " __TIME__;
  root["ip_adress"] = WiFi.localIP().toString();
  root["mac_adress"] = WiFi.macAddress();
  root["Free_Heap"] = ESP.getFreeHeap();
  root["Chip_Id"] = ESP.getChipId();
  root["Flash_Chip_Size"] = ESP.getFlashChipSize();
  root["Flash_Chip_Frequency"] = ESP.getFlashChipSpeed();

   // Prints Json object to string buffer
  root.printTo(buffer, bufferSize);
}
