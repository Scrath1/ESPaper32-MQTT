#include "view.h"

/**
 * @brief Construct a new View:: View object
 *
 * @param frame The frame that is used to draw this view
 * @param instructionsfile A file in SPIFFS that will be attempted to be read for drawing instructions
 */
View::View(EPDFrame& frame, const char* instructionsFile) : frame(frame), instructionsFile(instructionsFile) {}

/**
 * @brief Add a graphical object (e.g. Graph) to the GraphicObject List. Use with a derived class from GraphicObject.
 *
 * @param object Object pointer of class derived from GraphicObject
 */
void View::addGraphicObject(GraphicObject* object) {
    graphicObjects.push_back(object);
}

void View::drawGraphicObjects() {
    for (GraphicObject* g : graphicObjects) {
        g->draw(frame);
    }
}

void View::drawView(bool clearScreen) {
    if (clearScreen) {
        frame.clearBlack();
        frame.clearRed();
    }
    drawGraphicObjects();
    parseFile();
}

/**
 * @brief reads file defined by View Parameter instructionsFile and returns contents in dynamically allocated char array
 *
 * @return char* contents of File. Remember to call delete after using it.
 */
char* View::readFile() {
    File file = SPIFFS.open(instructionsFile, FILE_READ);
    size_t filesize = file.size();
    char* buffer = new char[filesize + 1];
    file.read((uint8_t*)buffer, filesize);
    file.close();
    buffer[filesize] = '\0';
    return buffer;
}

void View::parseFile() {
    char* filecontents = readFile();
    const char delim[] = "\n";
    char* line = strtok(filecontents, delim);
    std::vector<char*> lines;
    while (line != nullptr) {
        lines.push_back(line);
        line = strtok(nullptr, delim);
    }
    for (char* l : lines) {  // can not be in the tokenization loop due to strtok
        parseLine(l);
    }
    delete filecontents;
}

void View::parseLine(char* line) {
    const char delim[] = ",";
    std::vector<const char*> tokens;
    const char* word = strtok(line, delim);
    while (word != nullptr) {
        tokens.push_back(word);
        word = strtok(nullptr, delim);
    }
    if (strncmp(tokens[0], "Line", 4) == 0) {
        lineHandler(tokens);
    } else if (strncmp(tokens[0], "Rectangle", 9) == 0) {
        rectangleHandler(tokens);
    } else if (strncmp(tokens[0], "Circle", 6) == 0) {
        circleHandler(tokens);
    }
}

Color stringToColor(const char* str) {
    if (strncmp(str, "BLACK", 5) == 0) {
        return BLACK;
    } else if (strncmp(str, "RED", 3) == 0) {
        return RED;
    } else if (strncmp(str, "DARK_RED", 8) == 0) {
        return DARK_RED;
    } else {
        return WHITE;
    }
}

void View::lineHandler(const std::vector<const char*>& tokens) {
    if (tokens.size() < 6) return;
    errno = 0;
    int x0 = strtol(tokens[1], (char**)nullptr, 10);
    int y0 = strtol(tokens[2], (char**)nullptr, 10);
    int x1 = strtol(tokens[3], (char**)nullptr, 10);
    int y1 = strtol(tokens[4], (char**)nullptr, 10);
    if (errno != 0) return;  // error occurred while converting integers
    Color color = stringToColor(tokens[5]);
    frame.drawLine(x0, y0, x1, y1, color);
}

void View::rectangleHandler(const std::vector<const char*>& tokens) {
    if (tokens.size() < 7) return;
    errno = 0;
    int x0 = strtol(tokens[1], (char**)nullptr, 10);
    int y0 = strtol(tokens[2], (char**)nullptr, 10);
    int x1 = strtol(tokens[3], (char**)nullptr, 10);
    int y1 = strtol(tokens[4], (char**)nullptr, 10);
    if (errno != 0) return;  // error occurred while converting integers
    bool filled = false;
    if (strncmp(tokens[5], "true", 4) == 0) filled = true;
    Color color = stringToColor(tokens[6]);
    frame.drawRectangle(x0, y0, x1, y1, color, filled);
}

void View::circleHandler(const std::vector<const char*>& tokens) {
    if (tokens.size() < 6) return;
    errno = 0;
    int x = strtol(tokens[1], (char**)nullptr, 10);
    int y = strtol(tokens[2], (char**)nullptr, 10);
    int r = strtol(tokens[3], (char**)nullptr, 10);
    bool filled = false;
    if (strncmp(tokens[4], "true", 4) == 0) filled = true;
    Color color = stringToColor(tokens[5]);
    frame.drawCircle(x, y, r, color, filled);
}