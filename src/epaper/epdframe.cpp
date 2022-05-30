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
        case DARK_RED:
            drawDarkRedLine(x0, y0, x1, y1);
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
        case DARK_RED:
            drawDarkRedHorizontalLine(x, y, width);
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
        case DARK_RED:
            drawDarkRedVerticalLine(x, y, height);
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
    else if (color == DARK_RED && filled)
        drawDarkRedFilledRectangle(x0, y0, x1, y1);

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

/**
 * @brief Draws the interval bars for the graph
 *
 * @param x x position of the graph
 * @param xAxPos y position of the graphs x-axis
 * @param h graph height
 * @param w graph width
 * @param aT graph axisThickness
 * @param numIntervals number of intervals on the x-Axis
 */
void EPDFrame::drawGraphXIntervalbars(int x, int xAxPos, unsigned int h, unsigned int w, unsigned int aT, unsigned int numIntervals) {
    int xOffset = w / (numIntervals - 1);
    // x intervals
    for (int i = 1; i < numIntervals; ++i) {
        blackCanvas.DrawVerticalLine(x + aT + xOffset * i, xAxPos - 5, 10 + aT, COLORED);
    }
}

void EPDFrame::drawGraphYIntervalbars(int x, int y, unsigned int h, unsigned int aT, int minVal, int numIntervals) {
}

void EPDFrame::drawGraph(int x, int y, const Graph& graph) {
    unsigned int h = graph.getHeight();
    unsigned int w = graph.getWidth();
    unsigned int aT = graph.getAxisThickness();
    int maxVal = graph.getMaxValue();
    int minVal = graph.getMinValue();
    maxVal = maxVal % 5 ? maxVal + 5 - (maxVal % 5) : maxVal;  // scales to next largest number divisible by 5
    minVal = minVal % 5 ? minVal - 5 + (minVal % 5) : minVal;
    int absMaxVal = sqrt(maxVal * maxVal);
    int absMinVal = sqrt(minVal * minVal);

    int numYIntervals = (absMaxVal + absMinVal) / 5;

    // y position of the x axis
    unsigned int xAxPos = y + h - (static_cast<float>(h) * (static_cast<float>(absMinVal) / absMaxVal));
    Serial.print("xAxPos: ");
    Serial.println(xAxPos);
    blackCanvas.DrawFilledRectangle(x, y, x + aT, y + h, COLORED);            // y axis bar
    blackCanvas.DrawFilledRectangle(x, xAxPos, x + w, xAxPos + aT, COLORED);  // x axis bar

    int dSize = graph.getLineDataArray()[0].size();
    int xOffset = w / (dSize - 1);
    drawGraphXIntervalbars(x, xAxPos, h, w, aT, dSize);
    drawGraphYIntervalbars(x, y, h, aT, minVal, numYIntervals);

    // draw data lines
    for (Linedata d : graph.getLineDataArray()) {
        for (int i = 1; i < dSize; ++i) {
            if (d[i] == NO_VALUE) break;
            // TODO: scale data to axis;
            // int y0 = xAxPos-map(d[i-1], 0, h, minVal, maxVal);
            int y0 = xAxPos - d[i - 1];
            int y1 = xAxPos - d[i];
            // int y1 = xAxPos-map(d[i], 0, h, minVal, maxVal);
            int x0 = x + aT + xOffset * (i - 1);
            int x1 = x + aT + xOffset * (i);

            // draw line from point 0 to point 1
            blackCanvas.DrawLine(x0, y0, x1, y1, COLORED);
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
    Paint* paint;
    int counter = 0;
    while ((x0 != x1) && (y0 != y1)) {
        // draw every second pixel red or black
        if (counter % 2)
            paint = &redCanvas;
        else
            paint = &blackCanvas;
        paint->DrawPixel(x0, y0, COLORED);
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
    Paint* paint;
    for (int i = x; i < x + line_width; i++) {
        // draw every second pixel red or black
        if (i % 2)
            paint = &redCanvas;
        else
            paint = &blackCanvas;
        paint->DrawPixel(i, y, COLORED);
    }
}

void EPDFrame::drawDarkRedVerticalLine(int x, int y, int line_height) {
    Paint* paint;
    for (int i = y; i < y + line_height; i++) {
        if (i % 2)
            paint = &redCanvas;
        else
            paint = &blackCanvas;
        paint->DrawPixel(x, i, COLORED);
    }
}

void EPDFrame::drawDarkRedFilledRectangle(int x0, int y0, int x1, int y1) {
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    Paint* paint;
    for (int i = min_x; i <= max_x; i++) {
        for (int j = min_y; j <= max_y; j++) {
            if (i * j % 2)
                paint = &redCanvas;
            else
                paint = &blackCanvas;
            paint->DrawPixel(i, j, COLORED);
        }
    }
}