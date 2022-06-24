#include "graph.h"

Linedata::Linedata() : newestIndex(data.size() - 1) {
    for (int i = 0; i < data.size(); ++i) {
        data[i] = NO_VALUE;
    }
}

Linedata::Linedata(const char* l) : label(l) {
    Linedata();
}

void Linedata::pushBack(int d) {
    newestIndex = ++newestIndex % data.size();
    data[newestIndex] = d;
}

int Linedata::size() const { return data.size(); }

Graph::Graph(int x, int y, unsigned int w, unsigned int h, sFONT* font)
    : GraphicObject(x, y), width(w), height(h), labelFrequency(2), yIntervalDistance(5), axisThickness(1), font(font) {
}

/**
 * @brief Returns largest y value in graph
 *
 * @return int largest y value or -255 if no data is available
 */
int Graph::getMaxValue() const {
    int max = NO_VALUE;
    for (Linedata a : data) {
        for (int v : a.data) {
            if (v > max) max = v;
        }
    }
    return max;
}

/**
 * @brief Returns smallest y value in graph
 *
 * @return int smallest y value or -255 if no data is available
 */
int Graph::getMinValue() const {
    int min = NO_VALUE;
    for (Linedata a : data) {
        for (int v : a.data) {
            if (v != NO_VALUE) {
                if (min == NO_VALUE)
                    min = v;
                else if (v != NO_VALUE && v < min)
                    min = v;
            }
        }
    }
    return min;
}

unsigned int Graph::getWidth() const { return width; }

unsigned int Graph::getHeight() const { return height; }

/**
 * @brief Sets the thickness of the x and y axis bars in pixels
 *
 * @param t thickness in pixels
 */
void Graph::setAxisThickness(unsigned int t) { axisThickness = t; }

unsigned int Graph::getAxisThickness() const { return axisThickness; }

std::array<Linedata, 2> Graph::getLineDataArray() const { return data; }

void Graph::pushData(int index, int d) {
    if (index > data.size() - 1) {
        return;
    }
    data[index].pushBack(d);
}

void Graph::setFont(sFONT* f) {
    font = f;
}

unsigned int Graph::getYIntervalDistance() const { return yIntervalDistance; }
void Graph::setYIntervalDistance(unsigned int d) { yIntervalDistance = d; }

unsigned int Graph::getLabelFrequency() const { return labelFrequency; }
void Graph::setLabelFrequency(unsigned int f) { labelFrequency = f; }

/**
 * @brief Overwritten function from GraphicObject that draws the graph on the given frame
 *
 * @param frame
 */
void Graph::draw(EPDFrame& frame) {
    Paint blackCanvas = frame.getBlackCanvasRef();
    int maxVal = getMaxValue();                                                                                // maximum data value (used for scaling)
    int minVal = getMinValue();                                                                                // minimum data value (used for scaling)                                   // y distance between two y labels, relative to data (NOT absolute pixel count)
    maxVal = maxVal % yIntervalDistance ? maxVal + yIntervalDistance - (maxVal % yIntervalDistance) : maxVal;  // scales to next largest number divisible by 5
    minVal = minVal % yIntervalDistance ? minVal - yIntervalDistance + (minVal % yIntervalDistance) : minVal;
    int absMaxVal = sqrt(maxVal * maxVal);
    int absMinVal = sqrt(minVal * minVal);

    int numYIntervals = (absMaxVal + absMinVal) / yIntervalDistance;  // number of intervals on y axis (y axis label bars -1)

    // y position of the x axis
    unsigned int xAxPos = y - axisThickness / 2 + ((static_cast<float>(height) / (absMaxVal + absMinVal)) * absMaxVal);
    blackCanvas.DrawFilledRectangle(x, y, x + axisThickness - 1, y + height, COLORED);           // y axis bar
    blackCanvas.DrawFilledRectangle(x, xAxPos, x + width, xAxPos + axisThickness - 1, COLORED);  // x axis bar

    int numXIntervals = data[0].size();
    int xOffset = width / (numXIntervals - 1);
    drawGraphXIntervalbars(blackCanvas, xAxPos, numXIntervals);
    drawGraphYIntervalbars(blackCanvas, maxVal, numYIntervals);
    drawGraphYLabels(blackCanvas, maxVal, numYIntervals);

    // draw data lines
    for (Linedata d : data) {
        for (int i = 1; i < numXIntervals; ++i) {
            if (d[i] == NO_VALUE) break;
            int y0 = height + y - map(d[i - 1], minVal, maxVal, 0, height);
            int y1 = height + y - map(d[i], minVal, maxVal, 0, height);
            int x0 = x + axisThickness + xOffset * (i - 1);
            int x1 = x + axisThickness + xOffset * (i);

            // draw line from point 0 to point 1
            blackCanvas.DrawLine(x0, y0, x1, y1, COLORED);
        }
    }
}

void Graph::drawGraphXIntervalbars(Paint& blackCanvas, int xAxPos, unsigned int numIntervals) {
    int xOffset = width / (numIntervals - 1);
    // x intervals
    for (int i = 1; i < numIntervals; ++i) {
        blackCanvas.DrawVerticalLine(x + axisThickness + xOffset * i, xAxPos - 5, 10 + axisThickness, COLORED);
    }
}
void Graph::drawGraphYIntervalbars(Paint& blackCanvas, int maxVal, int numIntervals) {
    int yOffset = height / numIntervals;
    for (int i = 0; i <= numIntervals; ++i) {
        blackCanvas.DrawHorizontalLine(x - 5, y + yOffset * i, 10 + axisThickness, COLORED);
    }
}

/**
 * @brief Helper function for drawGraphYLabels that counts the digits in an int
 *
 * @param number
 * @return int
 */
int digitCount(int number) {
    int count = 0;
    while (number != 0) {
        number /= 10;
        count++;
    }
    return count;
}

void Graph::drawGraphYLabels(Paint& blackCanvas, int maxVal, unsigned int numIntervals) {
    int yOffset = height / numIntervals;
    for (int i = 0; i <= numIntervals; ++i) {
        if (i % labelFrequency == 0) {
            int curVal = maxVal - i * yIntervalDistance;
            unsigned int bufferSize = digitCount(curVal);
            char buffer[bufferSize];
            if (curVal < 0) bufferSize++;   // only for drawing the information. Does not impact size of string
            sprintf(buffer, "%d", curVal);  //
            blackCanvas.DrawStringAt(x - 5 - font->Width * bufferSize, y + yOffset * i, buffer, font, COLORED);
        }
    }
}