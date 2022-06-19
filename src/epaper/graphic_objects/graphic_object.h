#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H
#include "../epdframe.h"
class EPDFrame; //forward declaration to fix circular dependency with EPDFrame
class GraphicObject {
   public:
    GraphicObject(int x, int y) : x(x), y(y) {}
    virtual void draw(EPDFrame &frame) = 0;
    void setX(int x) { x = x; }
    void setY(int y) { y = y; }
    int getX() { return x; }
    int getY() { return y; }

   protected:
    int x{};
    int y{};
};

#endif