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

Graph::Graph(unsigned int w, unsigned int h)
    : width(w), height(h),labelFrequency(2), intervalDistance(5), axisThickness(1)  {
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

unsigned int Graph::getIntervalDistance() const { return intervalDistance; }
void Graph::setIntervalDistance(unsigned int d) { intervalDistance = d; }

unsigned int Graph::getLabelFrequency() const { return labelFrequency; }
void Graph::setLabelFrequency(unsigned int f) { labelFrequency = f; }