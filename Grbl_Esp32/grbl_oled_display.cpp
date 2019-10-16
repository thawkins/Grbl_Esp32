/*
  grbl_display.cpp - OLED Display code for GRBL.
  Part of Grbl

  Copyright (c) 2019 Tim Hawkins

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "grbl_oled_display.h"
#include "report.h"
#include <Preferences.h>


U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);

void grbl_oled_display_init(void){
    u8x8.begin();
    grbl_oled_display_status();
}

void grbl_oled_display_status(void){

    char currentIP[64];
    char currentAP[64];
    char currentSSID[64];
    String defV = "";
    Preferences prefs;

    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0, 0, "VER :" GRBL_VERSION);
    u8x8.drawString(0, 1, "BLD :" GRBL_VERSION_BUILD);

    int WifiMode = WiFi.getMode();

    WiFi.localIP().toString().toCharArray(currentIP,64);

    switch(WifiMode){
      case WIFI_MODE_STA:
        u8x8.drawString(0, 2, "WIFI: STATION");
        WiFi.localIP().toString().toCharArray(currentIP,64);
        u8x8.drawString(0, 3, "S");
        u8x8.drawString(0, 3, currentIP );
      break;

      case WIFI_MODE_APSTA:
        u8x8.drawString(0, 2, "WIFI: BOTH");
        WiFi.localIP().toString().toCharArray(currentIP,64);
        WiFi.softAPIP().toString().toCharArray(currentAP,64);
        u8x8.drawString(0, 3, "S");
        u8x8.drawString(1, 3, currentIP );
        u8x8.drawString(0, 4, "A");
        u8x8.drawString(1, 4, currentAP );
      break;

      case WIFI_MODE_AP:
        u8x8.drawString(0, 2, "WIFI: ACCESS");
        WiFi.softAPIP().toString().toCharArray(currentIP,64);
        u8x8.drawString(0, 4, "A");
        u8x8.drawString(1, 4,  currentIP );
      break;

      case WIFI_MODE_NULL:
         u8x8.drawString(0, 2, "WIFI: NONE");
      break;

    }
  
  
    u8x8.drawString(0, 5, (WiFi.status()==WL_CONNECTED)?"Connected":"Not connected" );
    u8x8.drawString(0, 6, "!");

    defV = DEFAULT_STA_SSID;
    String SSID = prefs.getString(STA_SSID_ENTRY, defV);
    SSID.toCharArray(currentSSID,64);
    u8x8.drawString(0, 6, "S:");
    u8x8.drawString(2, 6, currentSSID);

    defV = DEFAULT_AP_SSID;
    SSID = prefs.getString(AP_SSID_ENTRY, defV);
    SSID.toCharArray(currentSSID,64);
    u8x8.drawString(0, 7, "A:");
    u8x8.drawString(2, 7, currentSSID);
}
