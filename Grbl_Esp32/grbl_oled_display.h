/*
  grbl_display.h - OLED Display code for GRBL.
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
#ifndef grbl_oled_display_h
#define grbl_oled_display_h  

#include <U8g2lib.h>

#define OLED_SCREEN_WIDTH 128   // OLED display width, in pixels
#define OLED_SCREEN_HEIGHT 64   // OLED display height, in pixels
#define OLED_RESET_PIN -1       // Pin Used to reset
#define OLED_I2C_SDA 4              // Pin Used i2c SDA pin
#define OLED_I2C_SDD 15             // Pin Used i2c SDD pin
#define OLED_I2C_ADDR 0x3D      // Pin Used i2c Address

void grbl_oled_display_init(void);
void grbl_oled_display_status(void);

#endif
