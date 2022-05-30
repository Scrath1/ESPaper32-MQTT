#ifndef GRAPH_H
#define GRAPH_H
#include <array>
#include <queue>

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
        if(data[data.size()-1]!=NO_VALUE) index = (newestIndex+data.size()+1+i)%data.size();
        else index=i;
        return data[index];
    }
    int size() const;
};

class Graph {
   public:
    Graph(unsigned int w, unsigned int h);
    int getMaxValue() const;
    int getMinValue() const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    void setAxisThickness(unsigned int t);
    unsigned int getAxisThickness() const;
    std::array<Linedata, 2> getLineDataArray() const;
    void pushData(int index, int data);

    Linedata& operator[](int i){
        return data[i];
    }

   private:
    std::array<Linedata, 2> data;  // for now only 2 lines are allowed
    unsigned int width;
    unsigned int height;
    unsigned int axisThickness;
};

#endif