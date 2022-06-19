#ifndef FIXED_DIGIT_DISPLAY_H
#define FIXED_DIGIT_DISPLAY_H
#include "../fonts.h"
#include "graphic_object.h"

class FixedDigitDisplay : public GraphicObject {
   public:
    FixedDigitDisplay(int x, int y, sFONT* font, unsigned int noOfDigits);
    void draw(EPDFrame& frame) override;

   private:
    unsigned int getPixelWidth();
    unsigned int noOfDigits;
    sFONT* font;
};

#endif