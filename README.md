# ESPaper32-MQTT
A project to build an e-paper display that can display data from subbing to MQTT topics and is configurable using a webinterface. This ia a work-in-progress project and as such there are many parts that are not implemented yet.

## Current functionality
- [x] WiFi setup from webinterface
- [ ] MQTT connection
- [ ] Display customization from webinterface
- [x] Graph support
- [ ] Image support
- [ ] CSS for the webinterface (low priority)

## Hardware
- ESP32
- [Waveshare 4.2inch 3 color ePaper](https://www.waveshare.com/wiki/4.2inch_e-Paper_Module_(B))

## ESP32 Connections
| EPaper | ESP32 | Description           |
|--------|-------|-----------------------|
| Busy   | 25    | any GPIO will work    |
| RST    | 26    | any GPIO will work    |
| DC     | 27    | any GPIO will work    |
| CS     | 5     | Chip Select (HSPICS0) |
| CLK    | 18    | SCLK (HSPCLK)         |
| DIN    | 23    | MOSI (HSPID)          |
| VCC    | 3.3V  |                       |
| GND    | GND   |                       |

![](https://www.etkilesimliogrenme.com/uploads/images/2021/05/image_750x_608ff0fdc67a9.jpg)

## Support for other screens
This project is written specifically with the screen mentioned above in mind. Support for other screens is currently not planned. If you want to adapt this project for your needs you could try just swapping out the epd4in2b_V2 files with those of your screen. [Here](https://github.com/waveshare/e-Paper/tree/master/Arduino) you can find a list of the other files for waveshare e-paper displays.

## Notes on drawing
The EPDFrame class is intended as a class that can utilize the whole screen at once for drawing and is supposed to give more abstraction over drawing than the Paint class provided by Waveshare. To achieve this abstraction I had to sacrifice support for partial screen updates though. If this is something you need you can use the EPDFrame class for drawing your initial image and provide updates using the Paint class.

### Image layers
The colors on the screen are layered. Red is the topmost layer so if you want to draw something black at a place where there is already red you will need to clear those red pixels, otherwise your drawing will not be visible. If you want to draw something in white, both black and red have to be cleared at this position.

The EPDFrame class will handle this for you automatically. The paint class will not.

## Credit
ePaper Code is based on [this code example](https://github.com/waveshare/e-Paper/tree/master/Arduino/epd4in2b_V2) from waveshare.