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

void wifiSetup() 
{
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(WiFi.SSID());

  // WiFiManager
  WiFiManager wm;

  wm.setAPCallback([](WiFiManager *myWiFiManager){
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    //if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
  });

  if (!wm.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

  Serial.println("IP number assigned by DHCP is ");
  Serial.println(WiFi.localIP());
}

void animationSetup() 
{  
  // Start of ledstrip
  clockPanel.begin();

  // Setup of main animations
  clockPanelAnimator.add(&unicolor);
  clockPanelAnimator.add(&rainbowAll);
  clockPanelAnimator.add(&rainbow);
  clockPanelAnimator.add(&fire);
  clockPanelAnimator.add(&glitter);

  // Setup of animations of separator
  clockPanelAnimator.addSeparator(&unicolorS);
  clockPanelAnimator.addSeparator(&blinkS);
  clockPanelAnimator.addSeparator(&breathS);
  clockPanelAnimator.addSeparator(&knightRiderS);
  clockPanelAnimator.addSeparator(&bounceS);

  // Read config from EEPROM
  int animId = config.readMainAnimation();
  if(animId < 0 || animId > clockPanelAnimator.animCount())
  {
    animId = 0;
  }

  int separatorId = config.readSeparatorAnimation();
  if(separatorId < 0 || separatorId > clockPanelAnimator.getSeparatorCount())
  {
    separatorId = 0;
  }

  bool mirror = config.readMirror();
  clockPanel.setMirror(mirror);
  
  clockPanelAnimator.setAnimation(animId);
  clockPanelAnimator.setSeparatorAnimation(separatorId);
  clockPanelAnimator.setColor(config.readColor());
}

void MDNSSetup() 
{
  while (!MDNS.begin(myHostname,WiFi.localIP(),30))
  {
    delay(100);
    bl.handle();
    Serial.print(".");
    yield();
  }
  
  MDNS.addService(type, proto, port);
  MDNS.addServiceTxt(type, proto, "vendor", "Christux");
  MDNS.addServiceTxt(type, proto, "model", "ClockPanel01");
  MDNS.addServiceTxt(type, proto, "version", "1");
  MDNS.addServiceTxt(type, proto, "serial", "PROTO01");
  MDNS.addServiceTxt(type, proto, "firmware_build_date", __DATE__ " " __TIME__);
  MDNS.addServiceTxt(type, proto, "mac_adress", WiFi.macAddress());
}

void OTASetup() 
{

  ArduinoOTA.setHostname(myHostname);
  
  ArduinoOTA.onStart([]() {
    clockPanel.setAllPixels(ChristuxAnimation::RgbColor::blank);
    clockPanel.showWithoutMask();
    clockPanel.commit();
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    ChristuxAnimation::RgbColor color(64,0,0);
    for(int i = 0, l = (pixelCount * progress) / total; i < l; i++)
    {
      clockPanel.setPixelColor(i,color);
    }
    clockPanel.showWithoutMask();
    clockPanel.commit();
  });
  
  ArduinoOTA.onEnd([]() {
   clockPanel.setAllPixels(ChristuxAnimation::RgbColor::blank);
   clockPanel.showWithoutMask();
   clockPanel.commit();
  });
  
  ArduinoOTA.onError([](ota_error_t) {
    ESP.restart();
  });
  
  ArduinoOTA.begin();
}

void ntpSetup()
{
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  
  Serial.println("Waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  Serial.print("The Time is: ");
  Serial.print(hour());
  Serial.print(" : ");
  Serial.print(minute());
  Serial.print(" : ");
  Serial.println(second());
  Serial.print("The Date is: ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.println(year());
}
