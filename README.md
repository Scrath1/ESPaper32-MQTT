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

## Credit
ePaper Code is based on [this code example](https://github.com/waveshare/e-Paper/tree/master/Arduino/epd4in2b_V2) from waveshare.