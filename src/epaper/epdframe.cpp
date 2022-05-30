#include "epdframe.h"

// has to be allocated in BSS section. Too large for stack
static unsigned char _redBuffer[BUFFERSIZE];    // used in redCanvas
static unsigned char _blackBuffer[BUFFERSIZE];  // used in blackCanvas

EPDFrame::EPDFrame() : redCanvas(_redBuffer, EPD_WIDTH, EPD_HEIGHT), blackCanvas(_blackBuffer, EPD_WIDTH, EPD_HEIGHT) {
    clearRed();
    clearBlack();
}

void EPDFrame::refresh() {
    epd.Reset();
    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
    epd.ClearFrame();
    epd.DisplayFrame(_blackBuffer, _redBuffer);
    epd.Sleep();
}

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
            redCanvas.DrawPixel(x, y, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawPixel(x, y, COLORED);
            break;
        default:
            blackCanvas.DrawPixel(x, y, COLORED);
    }
}

void EPDFrame::drawLine(int x0, int y0, int x1, int y1, Color color) {
    switch (color) {
        case WHITE:
            redCanvas.DrawLine(x0, y0, x1, y1, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawLine(x0, y0, x1, y1, COLORED);
            break;
        default:
            blackCanvas.DrawLine(x0, y0, x1, y1, COLORED);
    }
}

void EPDFrame::drawHorizontalLine(int x, int y, int width, Color color) {
    switch (color) {
        case WHITE:
            redCanvas.DrawHorizontalLine(x, y, width, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawHorizontalLine(x, y, width, COLORED);
            break;
        default:
            blackCanvas.DrawHorizontalLine(x, y, width, COLORED);
    }
}

void EPDFrame::drawVerticalLine(int x, int y, int height, Color color) {
    switch (color) {
        case WHITE:
            redCanvas.DrawVerticalLine(x, y, height, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawVerticalLine(x, y, height, COLORED);
            break;
        default:
            blackCanvas.DrawVerticalLine(x, y, height, COLORED);
    }
}

void EPDFrame::drawCharAt(int x, int y, char ascii_char, sFONT* font, Color color) {
    switch (color) {
        case WHITE:
            redCanvas.DrawCharAt(x, y, ascii_char, font, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawCharAt(x, y, ascii_char, font, COLORED);
            break;
        default:
            blackCanvas.DrawCharAt(x, y, ascii_char, font, COLORED);
    }
}

void EPDFrame::drawStringAt(int x, int y, const char* text, sFONT* font, Color color) {
    switch (color) {
        case WHITE:
            redCanvas.DrawStringAt(x, y, text, font, UNCOLORED);
            break;
        case RED:
            redCanvas.DrawStringAt(x, y, text, font, COLORED);
            break;
        default:
            blackCanvas.DrawStringAt(x, y, text, font, COLORED);
    }
}

void EPDFrame::drawRectangle(int x0, int y0, int x1, int y1, Color color, bool filled) {
    Paint& canvas = blackCanvas;
    int colored = 0;
    if (color == WHITE)
        colored = UNCOLORED;
    else if (color == RED)
        canvas = redCanvas;

    if (filled)
        canvas.DrawFilledRectangle(x0, y0, x1, y1, colored);
    else
        canvas.DrawRectangle(x0, y0, x1, y1, colored);
}

void EPDFrame::drawCircle(int x, int y, int radius, Color color, bool filled) {
    Paint& canvas = blackCanvas;
    int colored = 0;
    if (color == WHITE)
        colored = UNCOLORED;
    else if (color == RED)
        canvas = redCanvas;

    if (filled)
        canvas.DrawFilledCircle(x, y, radius, colored);
    else
        canvas.DrawCircle(x, y, radius, colored);
}

void EPDFrame::drawGraph(int x, int y, const Graph& graph){
    int h = graph.getHeight();
    int w = graph.getWidth();
    blackCanvas.DrawVerticalLine(x, y, h, COLORED);
    blackCanvas.DrawHorizontalLine(x, y-h/2, w, COLORED);
}