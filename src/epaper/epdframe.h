#ifndef EPDFRAME_H
#define EPDFRAME_H
#include "constants.h"
#include "epd4in2b_V2.h"
#include "epdpaint.h"
#include "graphic_objects/graphic_object.h"
// #include "graphic_objects/graph.h"


class Graph;
class GraphicObject;

enum Color {
    WHITE = 0,
    BLACK = 1,
    RED = 2,
    DARK_RED = 3  // currently only supported for lines and filled rectangles
};

class EPDFrame {
   public:
    EPDFrame();
    void refresh(bool clear = true);
    void sleep();
    void awaken();
    bool isAsleep() const;
    void displayGrid();
    Paint& getBlackCanvasRef();
    Paint& getRedCanvasRef();
    void drawGraphicObjects();
    // void addGraphicObject(Graph& graph);
    void addGraphicObject(GraphicObject* object);

    void clearRed(bool fill = false);
    void clearBlack(bool fill = false);
    void drawPixel(int x, int y, Color color);
    void drawLine(int x0, int y0, int x1, int y1, Color color);
    void drawHorizontalLine(int x, int y, int width, Color color);
    void drawVerticalLine(int x, int y, int height, Color color);
    void drawCharAt(int x, int y, char ascii_char, sFONT* font, Color color);
    void drawStringAt(int x, int y, const char* text, sFONT* font, Color color);
    void drawRectangle(int x0, int y0, int x1, int y1, Color color, bool filled = false);
    void drawCircle(int x, int y, int radius, Color color, bool filled = false);

   private:
    void drawDarkRedLine(int x0, int y0, int x1, int y1);
    void drawDarkRedHorizontalLine(int x, int y, int line_width);
    void drawDarkRedVerticalLine(int x, int y, int line_height);
    void drawDarkRedFilledRectangle(int x0, int y0, int x1, int y1);
    void drawGraphXIntervalbars(int x, int xAxPos, unsigned int h, unsigned int w, unsigned int aT, unsigned int numIntervals);
    void drawGraphYIntervalbars(int x, int y, unsigned int h, unsigned int aT, int maxVal, int minVal);
    void drawGraphYLabels(int x, int y, unsigned int h, int maxVal, unsigned int numIntervals, unsigned int yIntervalDistance, unsigned int labelFrequency);
    Epd epd;  // display
    Paint redCanvas;
    Paint blackCanvas;
    bool asleep;
    std::vector<GraphicObject*> graphicObjects;
};
#endif