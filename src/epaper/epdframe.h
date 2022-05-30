#ifndef EPDFRAME_H
#define EPDFRAME_H
#include "constants.h"
#include "epd4in2b_V2.h"
#include "epdpaint.h"
#include "graphic_objects/graph.h"

enum Color {
    WHITE = 0,
    BLACK = 1,
    RED = 2
};

class EPDFrame {
   public:
    EPDFrame();
    void refresh();
    void displayGrid();

    void clearRed(bool fill= false);
    void clearBlack(bool fill= false);
    void drawPixel(int x, int y, Color color);
    void drawLine(int x0, int y0, int x1, int y1, Color color);
    void drawHorizontalLine(int x, int y, int width, Color color);
    void drawVerticalLine(int x, int y, int height, Color color);
    void drawCharAt(int x, int y, char ascii_char, sFONT* font, Color color);
    void drawStringAt(int x, int y, const char* text, sFONT* font, Color color);
    void drawRectangle(int x0, int y0, int x1, int y1, Color color, bool filled = false);
    void drawCircle(int x, int y, int radius, Color color, bool filled = false);
    void drawGraph(int x, int y, const Graph& graph);
   private:
    Epd epd; // display
    Paint redCanvas;
    Paint blackCanvas;
};
#endif