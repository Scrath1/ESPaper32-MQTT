#ifndef VIEW_H
#define VIEW_H
#include <errno.h>

#include <vector>

#include "SPIFFS.h"
#include "epdframe.h"
#include "graphic_objects/graphic_object.h"

/**
 * @brief Stores commands to allow for quickly changing between multiple images on screen
 *
 */
class View {
   public:
    View(EPDFrame& frame, const char* instructionsFile = "");

    void addGraphicObject(GraphicObject* object);
    void drawView(bool clearScreen = true);

   private:
    void drawGraphicObjects();
    char* readFile();
    void parseFile();
    void parseLine(char* line);                                     // TODO
    void lineHandler(const std::vector<const char*>& tokens);       // TODO
    void rectangleHandler(const std::vector<const char*>& tokens);  // TODO
    EPDFrame frame;
    std::vector<GraphicObject*> graphicObjects;
    const char* instructionsFile;
};
#endif