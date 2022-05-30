#ifndef GRAPH_H
#define GRAPH_H
#include <array>
#include <queue>

// equivalent to no data in array
#define NO_VALUE -255

struct Linedata {
    Linedata();
    Linedata(const char* label);
    std::array<int, 10> data;
    const char* label;
    // maybe add color information later
    unsigned int offsetIndex;  // keeps track of current oldest record
    void pushBack(int d);
};

class Graph {
   public:
    Graph(int x, int y, unsigned int w, unsigned int h);
    int getMaxValue() const;
    int getMinValue() const;
    int getWidth() const ;
    int getHeight() const;

   private:
    std::array<Linedata, 2> data;  // for now only 2 lines are allowed
    unsigned int width;            // 200
    unsigned int height;           // 250
};

#endif