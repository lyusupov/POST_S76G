/*
 * POST_S76G(.ino) firmware
 * Copyright (C) 2019-2021 Linar Yusupov
 *
 * Author: Linar Yusupov, linar.r.yusupov@gmail.com
 *
 * Web: http://github.com/lyusupov/SoftRF
 *
 * Credits:
 *   Arduino Core for STM32 is developed by Frederic Pillon
 *   U8g2 monochrome LCD, OLED and eInk library is developed by Oliver Kraus
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

#include <Wire.h>
#include <SPI.h>
#include <U8x8lib.h>

#define SX1276_RegVersion     0x42
#define SSD1306_OLED_I2C_ADDR 0x3C
#define BMP085_I2CADDR        0x77
#define BMP280_ADDRESS        0x77
#define BMP280_ADDRESS_ALT    0x76 /* GY-91, SA0 is NC */

#define BMP280_REGISTER_CHIPID 0xD0

#define BMP280_CHIPID         (0x58)
#define BME280_CHIPID         (0x60)

#if !defined(USBD_USE_CDC) || defined(DISABLE_GENERIC_SERIALUSB)
#define Serial                Serial1
#endif

/* Peripherals */
#define SOC_GPIO_PIN_CONS_RX  PA10
#define SOC_GPIO_PIN_CONS_TX  PA9

#define SOC_GPIO_PIN_GNSS_RX  PC11
#define SOC_GPIO_PIN_GNSS_TX  PC10

#define SOC_GPIO_PIN_GNSS_RST PB2
#define SOC_GPIO_PIN_GNSS_LS  PC6
#define SOC_GPIO_PIN_GNSS_PPS PB5

/* SPI0 */
#define SOC_GPIO_PIN_MOSI     PB15
#define SOC_GPIO_PIN_MISO     PB14
#define SOC_GPIO_PIN_SCK      PB13
#define SOC_GPIO_PIN_SS       PB12

/* SX1276 */
#define SOC_GPIO_PIN_RST      PB10
#define SOC_GPIO_PIN_DIO0     PB11
#define SOC_GPIO_PIN_DIO1     PC13
#define SOC_GPIO_PIN_DIO2     PB9
#define SOC_GPIO_PIN_DIO3     PB4
#define SOC_GPIO_PIN_DIO4     PB3
#define SOC_GPIO_PIN_DIO5     PA15

/* RF antenna switch */
#define SOC_GPIO_PIN_ANT_RXTX PA1 // 1:Rx, 0:Tx

/* I2C */
#define SOC_GPIO_PIN_SDA      PB7
#define SOC_GPIO_PIN_SCL      PB6

#define GNSS_BAUD_RATE        115200

HardwareSerial Serial1(SOC_GPIO_PIN_CONS_RX, SOC_GPIO_PIN_CONS_TX);
HardwareSerial Serial3(SOC_GPIO_PIN_GNSS_RX, SOC_GPIO_PIN_GNSS_TX);

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8_i2c(U8X8_PIN_NONE);

static U8X8_SSD1306_128X64_NONAME_HW_I2C *u8x8 = NULL;

static boolean GNSS_probe() {

  unsigned long startTime = millis();
  char c1, c2;
  c1 = c2 = 0;

  // clean any leftovers
  Serial3.flush();

    // Timeout if no valid response in 3 seconds
  while (millis() - startTime < 3000) {

    if (Serial3.available() > 0) {
      c1 = Serial3.read();
      if ((c1 == '$') && (c2 == 0)) { c2 = c1; continue; }
      if ((c2 == '$') && (c1 == 'G')) {
        /* got $G */

        /* leave the function with GNSS port opened */
        return true;
      } else {
        c2 = 0;
      }
    }

    delay(1);
  }

  return false;
}

static bool bmp_probe()
{
#if 1

  Wire.beginTransmission(BMP280_ADDRESS);
  if (Wire.endTransmission() == 0) return true;
  Wire.beginTransmission(BMP280_ADDRESS_ALT);
  if (Wire.endTransmission() == 0) return true;

#else

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_CHIPID);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, (byte)1);
  if (Wire.read() == BMP280_CHIPID) return true;

  Wire.beginTransmission(BMP280_ADDRESS_ALT);
  Wire.write(BMP280_REGISTER_CHIPID);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS_ALT, (byte)1);
  if (Wire.read() == BMP280_CHIPID) return true;

  Wire.beginTransmission(BMP280_ADDRESS);
  Wire.write(BMP280_REGISTER_CHIPID);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS, (byte)1);
  if (Wire.read() == BME280_CHIPID) return true;

  Wire.beginTransmission(BMP280_ADDRESS_ALT);
  Wire.write(BMP280_REGISTER_CHIPID);
  Wire.endTransmission();
  Wire.requestFrom(BMP280_ADDRESS_ALT, (byte)1);
  if (Wire.read() == BME280_CHIPID) return true;

#endif

  return false;
}

void setup() {

  bool has_SX1276 = false;
  bool has_GNSS   = false;
  bool has_OLED   = false;
  bool has_BMP280 = false;

  Serial.begin(115200);

#if defined(USBD_USE_CDC) && !defined(DISABLE_GENERIC_SERIALUSB)
  /* Let host's USB and console drivers to warm-up */
  delay(2000);
#else
  delay(500);
#endif

  Serial.println();
  Serial.println(F("TTGO T-Motion (S76G) Power-on Self Test"));
  Serial.println();
  Serial.flush();

  Wire.setSCL(SOC_GPIO_PIN_SCL);
  Wire.setSDA(SOC_GPIO_PIN_SDA);
  Wire.begin();

  /* SSD1306 I2C OLED probing */
  Wire.beginTransmission(SSD1306_OLED_I2C_ADDR);
  has_OLED = (Wire.endTransmission() == 0 ? true : false);

  if (has_OLED) {
    u8x8 = &u8x8_i2c;
  }

  if (u8x8) {
    u8x8->begin();
    u8x8->setFont(u8x8_font_chroma48medium8_r);
    u8x8->clear();
    u8x8->draw2x2String(2, 1, "SoftRF");
    u8x8->drawString   (6, 4, "and");
    u8x8->draw2x2String(2, 6, "LilyGO");
    delay(3000);
  }

  SPI.setMISO(SOC_GPIO_PIN_MISO);
  SPI.setMOSI(SOC_GPIO_PIN_MOSI);
  SPI.setSCLK(SOC_GPIO_PIN_SCK);

  SPI.begin();

  digitalWrite(SOC_GPIO_PIN_SS, HIGH);
  pinMode(SOC_GPIO_PIN_SS, OUTPUT);

  digitalWrite(SOC_GPIO_PIN_RST, HIGH);
  pinMode(SOC_GPIO_PIN_RST, OUTPUT);

  // manually reset radio
  digitalWrite(SOC_GPIO_PIN_RST, LOW); // drive RST pin low
  delay(5);
  digitalWrite(SOC_GPIO_PIN_RST, HIGH); // drive RST pin high
  delay(5);

  digitalWrite(SOC_GPIO_PIN_SS, LOW);

  SPI.transfer(SX1276_RegVersion & 0x7F);
  has_SX1276 = (SPI.transfer(0x00) == 0x12 ? true : false);

  digitalWrite(SOC_GPIO_PIN_SS, HIGH);

  SPI.end();
  pinMode(SOC_GPIO_PIN_SS, INPUT);
  pinMode(SOC_GPIO_PIN_RST, INPUT);
   
  Serial.println(F("Built-in components:"));

  Serial.print(F("RADIO  - ")); Serial.println(has_SX1276 ? F("PASS") : F("FAIL"));

  if (u8x8) {
    u8x8->clear();
    u8x8->draw2x2String(0, 0, "RADIO");
    u8x8->draw2x2String(14, 0, has_SX1276 ? "+" : "-");
  }

  Serial3.begin(GNSS_BAUD_RATE);

  /* drive GNSS RST pin low */
  pinMode(SOC_GPIO_PIN_GNSS_RST, OUTPUT);
  digitalWrite(SOC_GPIO_PIN_GNSS_RST, LOW);

  /* activate 1.8V<->3.3V level shifters */
  pinMode(SOC_GPIO_PIN_GNSS_LS,  OUTPUT);
  digitalWrite(SOC_GPIO_PIN_GNSS_LS,  HIGH);

  /* keep RST low to ensure proper IC reset */
  delay(200);

  /* release */
  digitalWrite(SOC_GPIO_PIN_GNSS_RST, HIGH);

  /* give Sony GNSS few ms to warm up */
  delay(100);

  /* hot start */
  Serial3.write("@GSR\r\n");       delay(250);

  has_GNSS = GNSS_probe();
  Serial.print(F("GNSS   - ")); Serial.println(has_GNSS ? F("PASS") : F("FAIL"));

  if (u8x8) {
    u8x8->draw2x2String(0, 2, "GNSS");
    u8x8->draw2x2String(14, 2, has_GNSS ? "+" : "-");
  }

  Serial.println();
  Serial.println(F("External components:"));
  Serial.print(F("OLED   - ")); Serial.println(has_OLED ? F("PASS") : F("FAIL"));

  if (u8x8) {
    u8x8->draw2x2String(0, 4, "OLED");
    u8x8->draw2x2String(14, 4, has_OLED ? "+" : "-");
  }

  has_BMP280 = bmp_probe();
  Serial.print(F("BMx280 - ")); Serial.println(has_BMP280 ? F("PASS") : F("FAIL"));

  if (u8x8) {
    u8x8->draw2x2String(0, 6, "BMx280");
    u8x8->draw2x2String(14, 6, has_BMP280 ? "+" : "-");
  }

  Wire.end();

  Serial.println();
  Serial.println(F("POST is completed."));
}

void loop() {

}
