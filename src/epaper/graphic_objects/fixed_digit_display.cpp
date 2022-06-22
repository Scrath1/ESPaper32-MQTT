#include "fixed_digit_display.h"

FixedDigitDisplay::FixedDigitDisplay(int x, int y, sFONT* font, unsigned int noOfDigits) : GraphicObject(x, y), noOfDigits(noOfDigits), font(font) {
    content = new char[noOfDigits + 1];
}

FixedDigitDisplay::FixedDigitDisplay(int x, int y, sFONT* font, unsigned int noOfDigits, Color textcolor) : FixedDigitDisplay(x, y, font, noOfDigits) {
    color = textcolor;
}

FixedDigitDisplay::~FixedDigitDisplay() {
    delete content;
}

void FixedDigitDisplay::draw(EPDFrame& frame) {
    frame.drawStringAt(x, y, content, font, color);
}

// returns size of content string not including null terminator
unsigned int FixedDigitDisplay::getNoOfDigits() const {
    return noOfDigits;
}

/**
 * @brief Updates the content of the FixedLengthDisplay.
 * Passed strings can be aligned left or right on the display.
 * If a string is too long to fit it is cut off depending on alignment.
 * Too short strings are filled with spaces.
 * Example (if noOfDigits is 3):
 *  Left Alignment:
 *      setContent("12") => content = "12 "
 *      setContent("1234") => content = "123"
 *  Right Alignment:
 *      setContent("12") => content = " 12"
 *      setContent("1234") => content = "234"
 * @param newContent
 */
void FixedDigitDisplay::setContent(const char* newContent) {
    if (alignment == LEFT) {
        memset(content, static_cast<unsigned char>(' '), noOfDigits);  // fill content string with spaces
        strncpy(content, newContent, noOfDigits);
    } else {
        memset(content, static_cast<unsigned char>(' '), noOfDigits);  // fill content string with spaces
        int newContentLenDiff = noOfDigits - strlen(newContent);
        unsigned int targetIndex = newContentLenDiff > 0 ? newContentLenDiff : 0;
        if (newContentLenDiff < 0) {
            unsigned int absContentLenDiff = static_cast<int>(sqrt(newContentLenDiff * newContentLenDiff));
            strncpy(content + sizeof(char) * targetIndex, newContent + sizeof(char) * absContentLenDiff, noOfDigits - newContentLenDiff);
        } else {
            strncpy(content + sizeof(char) * targetIndex, newContent, noOfDigits - newContentLenDiff);
        }
    }
    content[noOfDigits] = '\0';  // add string terminator just in case it got deleted
}

const char* FixedDigitDisplay::getContent() const {
    return content;
}

void FixedDigitDisplay::setAlignment(Alignment a) {
    alignment = a;
}

void FixedDigitDisplay::setColor(Color c) {
    color = c;
}

/**
 * @brief Calculates the total pixel width taken up by the FixedDigitDisplay
 * 
 * @return unsigned int 
 */
unsigned int FixedDigitDisplay::getTotalPixelWidth() {
    return noOfDigits * font->Width;
}