#ifndef GRAPH_H
#define GRAPH_H
#include <array>

#include "graphic_object.h"

// equivalent to no data in array
#define NO_VALUE -255

struct Linedata {
    Linedata();
    explicit Linedata(const char* label);
    std::array<int, 10> data;
    const char* label;
    // maybe add color information later
    unsigned int newestIndex;  // keeps track of current oldest record
    void pushBack(int d);

    int& operator[](int i) {
        int index;
        if (data[data.size() - 1] != NO_VALUE)
            index = (newestIndex + data.size() + 1 + i) % data.size();
        else
            index = i;
        return data[index];
    }
    int size() const;
};

class Graph : public GraphicObject {
   public:
    Graph(int x, int y, unsigned int w, unsigned int h);
    int getMaxValue() const;
    int getMinValue() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    void setAxisThickness(unsigned int t);
    unsigned int getAxisThickness() const;
    std::array<Linedata, 2> getLineDataArray() const;
    void pushData(int index, int data);
    unsigned int getYIntervalDistance() const;
    void setYIntervalDistance(unsigned int d);
    unsigned int getLabelFrequency() const;
    void setLabelFrequency(unsigned int f);
    void draw(EPDFrame& frame) override;
    

    Linedata& operator[](int i) {
        return data[i];
    }

   private:
    void drawGraphXIntervalbars(Paint& blackCanvas, int xAxPos, unsigned int numIntervals);
    void drawGraphYIntervalbars(Paint& blackCanvas, int maxVal, int numIntervals);
    void drawGraphYLabels(Paint& blackCanvas, int maxVal, unsigned int numIntervals);
    std::array<Linedata, 2> data;  // for now only 2 lines are allowed
    unsigned int width;
    unsigned int height;
    unsigned int labelFrequency;
    unsigned int yIntervalDistance;  // how large the intervals on the y axis between results are
    unsigned int axisThickness;
};

#endif