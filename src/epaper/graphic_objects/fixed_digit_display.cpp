#include "fixed_digit_display.h"

FixedDigitDisplay::FixedDigitDisplay(int x, int y, sFONT* font, unsigned int noOfDigits) : GraphicObject(x, y), noOfDigits(noOfDigits), font(font) {}

unsigned int FixedDigitDisplay::getPixelWidth() {
    return noOfDigits * font->Width;
}

void FixedDigitDisplay::draw(EPDFrame& frame){}