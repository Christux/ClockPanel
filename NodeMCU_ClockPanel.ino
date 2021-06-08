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

#include <TimeLib.h>
#include <NeoPixelBus.h>
#include <ChristuxUtils.h>
#include <ChristuxAnimation.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>

#include "Config.h"
#include "web/dist/data.h"

using namespace ChristuxAnimation;
using Color = ChristuxAnimation::RgbColor;

// Web server, myDNS
const char* myHostname = "ClockPanel";
const char port = 80;
const char* type = "http";
const char* proto = "tcp";

// NTP
//const char ntpServerName[] = "fr.pool.ntp.org";
const char ntpServerName[] = "192.168.1.42"; // my Syno

// LedStrip
const int pixelPin = 8;
const int pixelCount = 50;

// WS2812B ledstrip
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> neoPixelBus(pixelCount, pixelPin);

// Clock panel settings, ledstrip methods and leds mapping
ClockPanel clockPanel(
	[]() {
		neoPixelBus.Begin();
	},
	[]() {
		neoPixelBus.Show();
	},
	[](int i, Color color) {
		neoPixelBus.SetPixelColor(i, ::RgbColor(color.R, color.G, color.B));
	},
	(int[]){
		 7,8,
		5,6,9,
		 4,10,
		2,3,11,
		 1,0
	},
	(int[]){
		19,20,
	  17,18,21,
	  	16,22,
	  12,15,23,
	  	13,14
	},
	(int[]){
		25,
		24
	},
	(int[]){
		28,29,
	  26,27,30,
	  	32,31,
	  33,36,37,
	  	34,35
	},
	(int[]){
		48,49,
	  47,46,45,
	  	44,43,
	  38,41,42,
		39,40
	}
);

// Clock panel animator
// Manages animations and apply masks of digits
ClockPanelAnimator clockPanelAnimator(
	&clockPanel, 
	[](){
		return ClockTime(hour(), minute());
	});

// Digit animations
UniColor unicolor(pixelCount, &clockPanel);
RainbowLamp rainbowAll(pixelCount, &clockPanel);
Fire fire(pixelCount, &clockPanel);
Glitter glitter(pixelCount, &clockPanel);
RainbowClockPanel rainbow(&clockPanel);

// Separator animations
UniColor unicolorS(2, clockPanel.getSeparator());
Blink blinkS(2, clockPanel.getSeparator(), 1000);
Bounce bounceS(2, clockPanel.getSeparator(), 200);
Breathing breathS(2, clockPanel.getSeparator());
KnightRider knightRiderS(2, clockPanel.getSeparator(), 200);

// Blinking build-in led
BlinkLed bl(LED_BUILTIN,1000);

// Start webserver on port 80
ESP8266WebServer server(port);

// Read and write configuration from EEPROM
Config config;

// Udp for NTP sync
WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

// Setup, runs once
void setup()
{
  // Blinking led
  bl.setup();

  // Serial
  Serial.begin(9600);
  Serial.println("ClockPanel");
  
  // Start wifi
  wifiSetup();

  // Start mySDN
  MDNSSetup();

  // Sync time with NTP
  ntpSetup();

  // Web server
  serverSetup();

  // Setup of animations
	animationSetup();

  // Enable OTA
  OTASetup();
}

// Infinite loop
void loop()
{
  bl.handle();
	clockPanelAnimator.handle();
  ArduinoOTA.handle();
  MDNS.update();
  server.handleClient();
}
