#include "epdframe.h"

#include <string>

// has to be allocated in BSS section. Too large for stack
static unsigned char _redBuffer[BUFFERSIZE];    // used in redCanvas
static unsigned char _blackBuffer[BUFFERSIZE];  // used in blackCanvas

EPDFrame::EPDFrame() : redCanvas(_redBuffer, EPD_WIDTH, EPD_HEIGHT), blackCanvas(_blackBuffer, EPD_WIDTH, EPD_HEIGHT), asleep(true) {
    clearRed();
    clearBlack();
    awaken();
}

/**
 * @brief Refreshes the entire e-paper display
 *
 * @param clear default: true clears the entire display SRAM
 */
void EPDFrame::refresh(bool clear) {
    if (clear) epd.ClearFrame();
    epd.DisplayFrame(_blackBuffer, _redBuffer);
}

/**
 * @brief Puts display into deep sleep. Can be woken using awaken()
 *
 */
void EPDFrame::sleep() {
    epd.Sleep();
    asleep = true;
}

/**
 * @brief Wakes up display from deep sleep. If display is awake does nothing
 *
 */
void EPDFrame::awaken() {
    if (asleep) {
        epd.Reset();
        if (epd.Init() != 0) {
            Serial.print("e-Paper init failed");
            return;
        }
        asleep = false;
    }
}

/**
 * @brief Check if the epaper module is in sleep mode. Only accurate if sleep mode is controlled by this class and none other.
 *
 * @return true
 * @return false
 */
bool EPDFrame::isAsleep() const { return asleep; }

/**
 * @brief Displays a horizontal and vertical line every 50 pixels
 * Vertical lines are black, horizontal lines are red
 */
void EPDFrame::displayGrid() {
    for (int x = 50; x < EPD_WIDTH; x += 50) {
        blackCanvas.DrawVerticalLine(x, 0, EPD_HEIGHT, COLORED);
    }
    for (int y = 50; y < EPD_HEIGHT; y += 50) {
        redCanvas.DrawHorizontalLine(0, y, EPD_WIDTH, COLORED);
    }
}

void EPDFrame::addGraphicObject(Graph& graph) {
    graphicObjects.push_back(new Graph(graph));
}

// void EPDFrame::addGraphicObject(GraphicObject* object){
//     graphicObjects.push_back(object);
// }

Paint& EPDFrame::getBlackCanvasRef() {
    return blackCanvas;
}

Paint& EPDFrame::getRedCanvasRef() {
    return redCanvas;
}

void EPDFrame::drawGraphicObjects() {
    for (GraphicObject* g : graphicObjects) {
        g->draw(*this);
    }
}

/**
 * @brief Clears the red canvas
 *
 * @param fill if set to true fills with red
 */
void EPDFrame::clearRed(bool fill) {
    if (fill)
        redCanvas.Clear(COLORED);
    else
        redCanvas.Clear(UNCOLORED);
}

/**
 * @brief Clears the black canvas
 *
 * @param fill if set to true fills with black
 */
void EPDFrame::clearBlack(bool fill) {
    if (fill)
        blackCanvas.Clear(COLORED);
    else
        blackCanvas.Clear(UNCOLORED);
}

void EPDFrame::drawPixel(int x, int y, Color color) {
    switch (color) {
        case WHITE:
            blackCanvas.DrawPixel(x, y, UNCOLORED);
            redCanvas.DrawPixel(x, y, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawPixel(x, y, COLORED);
            break;
        default:
            redCanvas.DrawPixel(x, y, UNCOLORED);
            blackCanvas.DrawPixel(x, y, COLORED);
    }
}

void EPDFrame::drawLine(int x0, int y0, int x1, int y1, Color color) {
    switch (color) {
        case WHITE:
            blackCanvas.DrawLine(x0, y0, x1, y1, UNCOLORED);
            redCanvas.DrawLine(x0, y0, x1, y1, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawLine(x0, y0, x1, y1, COLORED);
            break;
        case DARK_RED:
            redCanvas.DrawLine(x0, y0, x1, y1, UNCOLORED);
            drawDarkRedLine(x0, y0, x1, y1);
            break;
        default:
            redCanvas.DrawLine(x0, y0, x1, y1, UNCOLORED);
            blackCanvas.DrawLine(x0, y0, x1, y1, COLORED);
    }
}

void EPDFrame::drawHorizontalLine(int x, int y, int width, Color color) {
    switch (color) {
        case WHITE:
            blackCanvas.DrawHorizontalLine(x, y, width, UNCOLORED);
            redCanvas.DrawHorizontalLine(x, y, width, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawHorizontalLine(x, y, width, COLORED);
            break;
        case DARK_RED:
            redCanvas.DrawHorizontalLine(x, y, width, UNCOLORED);
            drawDarkRedHorizontalLine(x, y, width);
            break;
        default:
            redCanvas.DrawHorizontalLine(x, y, width, UNCOLORED);
            blackCanvas.DrawHorizontalLine(x, y, width, COLORED);
    }
}

void EPDFrame::drawVerticalLine(int x, int y, int height, Color color) {
    switch (color) {
        case WHITE:
            blackCanvas.DrawVerticalLine(x, y, height, UNCOLORED);
            redCanvas.DrawVerticalLine(x, y, height, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawVerticalLine(x, y, height, COLORED);
            break;
        case DARK_RED:
            redCanvas.DrawVerticalLine(x, y, height, UNCOLORED);
            drawDarkRedVerticalLine(x, y, height);
            break;
        default:
            redCanvas.DrawVerticalLine(x, y, height, UNCOLORED);
            blackCanvas.DrawVerticalLine(x, y, height, COLORED);
    }
}

void EPDFrame::drawCharAt(int x, int y, char ascii_char, sFONT* font, Color color) {
    switch (color) {
        case WHITE:
            blackCanvas.DrawCharAt(x, y, ascii_char, font, UNCOLORED);
            redCanvas.DrawCharAt(x, y, ascii_char, font, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawCharAt(x, y, ascii_char, font, COLORED);
            break;
        default:
            redCanvas.DrawCharAt(x, y, ascii_char, font, COLORED);
            blackCanvas.DrawCharAt(x, y, ascii_char, font, COLORED);
    }
}

void EPDFrame::drawStringAt(int x, int y, const char* text, sFONT* font, Color color) {
    switch (color) {
        case WHITE:
            blackCanvas.DrawStringAt(x, y, text, font, UNCOLORED);
            redCanvas.DrawStringAt(x, y, text, font, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawStringAt(x, y, text, font, COLORED);
            break;
        default:
            redCanvas.DrawStringAt(x, y, text, font, UNCOLORED);
            blackCanvas.DrawStringAt(x, y, text, font, COLORED);
    }
}

void EPDFrame::drawRectangle(int x0, int y0, int x1, int y1, Color color, bool filled) {
    switch (color) {
        case WHITE:
            if (filled) {
                blackCanvas.DrawFilledRectangle(x0, y0, x1, y1, UNCOLORED);
                redCanvas.DrawFilledRectangle(x0, y0, x1, y1, UNCOLORED);
            } else {
                blackCanvas.DrawRectangle(x0, y0, x1, y1, UNCOLORED);
                redCanvas.DrawRectangle(x0, y0, x1, y1, UNCOLORED);
            }
            break;
        case RED:  // red is the foremost layer and doesn't need to erase other layers to be visible
            if (filled)
                redCanvas.DrawFilledRectangle(x0, y0, x1, y1, COLORED);
            else
                redCanvas.DrawRectangle(x0, y0, x1, y1, COLORED);
        case DARK_RED:
            if (filled) drawDarkRedFilledRectangle(x0, y0, x1, y1);
            break;
        case BLACK:  // black is default
        default:
            if (filled) {
                blackCanvas.DrawFilledRectangle(x0, y0, x1, y1, COLORED);
                redCanvas.DrawFilledRectangle(x0, y0, x1, y1, UNCOLORED);
            } else {
                blackCanvas.DrawRectangle(x0, y0, x1, y1, COLORED);
                redCanvas.DrawRectangle(x0, y0, x1, y1, UNCOLORED);
            }
    }
}

void EPDFrame::drawCircle(int x, int y, int radius, Color color, bool filled) {
    switch (color) {
        case WHITE:
            if (filled) {
                redCanvas.DrawFilledCircle(x, y, radius, UNCOLORED);
                blackCanvas.DrawFilledCircle(x, y, radius, UNCOLORED);
            } else {
                redCanvas.DrawCircle(x, y, radius, UNCOLORED);
                blackCanvas.DrawCircle(x, y, radius, UNCOLORED);
            }
            break;
        case RED:
            if (filled)
                redCanvas.DrawFilledCircle(x, y, radius, COLORED);
            else
                redCanvas.DrawCircle(x, y, radius, COLORED);
            break;
        case BLACK:
        default:
            if (filled) {
                redCanvas.DrawFilledCircle(x, y, radius, UNCOLORED);
                blackCanvas.DrawFilledCircle(x, y, radius, COLORED);
            } else {
                redCanvas.DrawCircle(x, y, radius, UNCOLORED);
                blackCanvas.DrawCircle(x, y, radius, COLORED);
            }
    }
}

/**
 * @brief basically DrawLine from the Paint class but with alternating pixel colors
 *
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 */
void EPDFrame::drawDarkRedLine(int x0, int y0, int x1, int y1) {
    /* Bresenham algorithm */
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int counter = 0;
    while ((x0 != x1) && (y0 != y1)) {
        // draw every second pixel red or black
        if (counter % 2)
            redCanvas.DrawPixel(x0, y0, COLORED);
        else
            blackCanvas.DrawPixel(x0, y0, COLORED);
        if (2 * err >= dy) {
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx) {
            err += dx;
            y0 += sy;
        }
        counter++;
    }
}

void EPDFrame::drawDarkRedHorizontalLine(int x, int y, int line_width) {
    for (int i = x; i < x + line_width; i++) {
        // draw every second pixel red or black
        if (i % 2)
            redCanvas.DrawPixel(i, y, COLORED);
        else
            blackCanvas.DrawPixel(i, y, COLORED);
    }
}

void EPDFrame::drawDarkRedVerticalLine(int x, int y, int line_height) {
    for (int i = y; i < y + line_height; i++) {
        if (i % 2)
            redCanvas.DrawPixel(x, i, COLORED);
        else
            blackCanvas.DrawPixel(x, i, COLORED);
    }
}

void EPDFrame::drawDarkRedFilledRectangle(int x0, int y0, int x1, int y1) {
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    int counter = 0;
    for (int i = min_x; i <= max_x; i++) {
        for (int j = min_y; j <= max_y; j++) {
            if (counter % 2)
                redCanvas.DrawPixel(i, j, COLORED);
            else
                blackCanvas.DrawPixel(i, j, COLORED);
            counter++;
        }
    }
}