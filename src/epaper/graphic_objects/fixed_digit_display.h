#ifndef FIXED_DIGIT_DISPLAY_H
#define FIXED_DIGIT_DISPLAY_H
#include "../fonts.h"
#include "graphic_object.h"

class FixedDigitDisplay : public GraphicObject {
   public:
    FixedDigitDisplay(int x, int y, sFONT* font, unsigned int noOfDigits);
    FixedDigitDisplay(int x, int y, sFONT* font, unsigned int noOfDigits, Color textcolor);
    ~FixedDigitDisplay();
    void draw(EPDFrame& frame) override;

    unsigned int getNoOfDigits() const;
    void setContent(const char* newContent);
    const char* getContent() const;
    
    enum Alignment{
        LEFT, RIGHT
    };
    void setAlignment(Alignment a);
    void setColor(Color color);


   private:
    Color color{BLACK};
    unsigned int getTotalPixelWidth();
    unsigned int noOfDigits; // amount of digits in content (not including null terminator)
    char* content;
    sFONT* font;
    Alignment alignment{LEFT};
};

#endif