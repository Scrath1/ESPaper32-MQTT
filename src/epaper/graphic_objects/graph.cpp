#include "graph.h"

Linedata::Linedata() : offsetIndex(0) {
    for (int i = 0; i < data.size(); ++i) {
        data[i] = NO_VALUE;
    }
}

Linedata::Linedata(const char* l) : label(l) {
    Linedata();
}

void Linedata::pushBack(int d) {
    offsetIndex = offsetIndex % data.size();  // wraps around to first element after last value
    data[offsetIndex] = d;
    offsetIndex++;
}

Graph::Graph(int x, int y, unsigned int w, unsigned int h)
    : width(w), height(h) {
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
            if (min != NO_VALUE && v < min) min = v;
        }
    }
    return min;
}

int Graph::getWidth() const { return width; }

int Graph::getHeight() const { return height; }