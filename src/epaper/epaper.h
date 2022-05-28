#include <SPI.h>
#include <SPIFFS.h>

#include "epd4in2b_V2.h"
#include "epdpaint.h"
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1

void displayTestImage();