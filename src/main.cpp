#include <Arduino.h>
#include <Preferences.h>

#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "epaper/epdframe.h"
#include "epaper/graphic_objects/graph.h"
#include "epaper/graphic_objects/fixed_digit_display.h"
#include "webserver.h"

const char* apSSID = "ESP-Epaper";
const char* apPassword = "123456789";
Preferences preferences;

AsyncWebServer server(80);

void displayTestImage() {
    Epd epd;
    epd.Reset();
    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }

    /* This clears the SRAM of the e-paper display */
    epd.ClearFrame();

    /**
     * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
     * In this case, a smaller image buffer is allocated and you have to
     * update a partial display several times.
     * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
     */
    unsigned char image[1500];    // image can store 1500*8 bit (12000bit)
    Paint paint(image, 400, 28);  // width should be the multiple of 8 (canvas size is 400*28 pixels => 11200bit)
    // paint.SetRotate(ROTATE_180);
    // Paint paint(_redBuffer, EPD_WIDTH, EPD_HEIGHT);
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 0, "e-Paper Demo", &Font24, COLORED);
    epd.SetPartialWindowBlack(paint.GetImage(), 100, 40, paint.GetWidth(), paint.GetHeight());

    paint.Clear(COLORED);
    paint.DrawStringAt(100, 2, "Hello world", &Font24, UNCOLORED);
    epd.SetPartialWindowRed(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());

    paint.SetWidth(64);
    paint.SetHeight(64);

    paint.Clear(UNCOLORED);
    paint.DrawRectangle(0, 0, 40, 50, COLORED);
    paint.DrawLine(0, 0, 40, 50, COLORED);
    paint.DrawLine(40, 0, 0, 50, COLORED);
    epd.SetPartialWindowBlack(paint.GetImage(), 72, 120, paint.GetWidth(), paint.GetHeight());

    paint.Clear(UNCOLORED);
    paint.DrawCircle(32, 32, 30, COLORED);
    epd.SetPartialWindowBlack(paint.GetImage(), 200, 120, paint.GetWidth(), paint.GetHeight());

    paint.Clear(UNCOLORED);
    paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
    epd.SetPartialWindowRed(paint.GetImage(), 72, 200, paint.GetWidth(), paint.GetHeight());

    paint.Clear(UNCOLORED);
    paint.DrawFilledCircle(32, 32, 30, COLORED);
    epd.SetPartialWindowRed(paint.GetImage(), 200, 200, paint.GetWidth(), paint.GetHeight());

    /* This displays the data from the SRAM in e-Paper module */
    epd.DisplayFrame();

    /* This displays an image */
    // epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

    /* Deep sleep */
    epd.Sleep();
}

void setup() {
    Serial.begin(9600);
    preferences.begin("pref");
    if (!preferences.isKey("ssid")) {  // initializes keys during first run
        preferences.putString("ssid", "");
        preferences.putString("password", "");
    }

    WiFi.disconnect();
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    Serial.print("Stored SSID is: ");
    Serial.println(ssid);

    if (ssid != "") {  // try to connect to known Access Point
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
        for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; ++i) {
            Serial.print('.');
            delay(1000);
        }
    }
    if (!WiFi.isConnected()) {  // Setup Hotspot if no WiFi connection is possible
        Serial.println("Failed to connect to WiFi. Starting Access Point");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(apSSID, apPassword);
    } else {
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    }
    setupWebServer(server);
    EPDFrame frame;
    frame.awaken();
    Graph* g = new Graph(40,10,200,250);
    g->setAxisThickness(3);
    g->pushData(0, 15);
    g->pushData(0, 7);
    g->pushData(0, 9);
    g->pushData(0, -5);
    g->pushData(0, 10);
    g->pushData(0, 0);
    g->pushData(0, 5);
    g->pushData(0, 10);
    g->pushData(0, -10);
    g->pushData(0, 0);
    frame.addGraphicObject(g);

    FixedDigitDisplay* d = new FixedDigitDisplay(300, 200, &Font24, 3);
    d->setAlignment(FixedDigitDisplay::RIGHT);
    d->setColor(RED);
    d->setContent("1234");
    frame.addGraphicObject(d);

    frame.drawGraphicObjects();
    
    int x0 = 50;
    int x1 = 250;
    int y0 = 50;
    int y1 = 100;
    int offset = 10;
    frame.drawLine(x0, y0 - offset, x1, y1 - offset, BLACK);
    frame.drawLine(x0, y0, x1, y1, DARK_RED);
    frame.drawLine(x0, y0 + offset, x1, y1 + offset, RED);
    frame.drawLine(x0, y0 + 2 * offset, x1, y1 + 2 * offset, DARK_RED);
    frame.drawLine(x0, y0 + 1 + 2 * offset, x1, y1 + 1 + 2 * offset, DARK_RED);
    frame.drawLine(x0, y0 + 3 * offset, x1, y1 + 3 * offset, BLACK);
    frame.drawLine(x0, y0 + 1 + 3 * offset, x1, y1 + 1 + 3 * offset, BLACK);
    frame.drawLine(x0, y0 + 4 * offset, x1, y1 + 4 * offset, RED);
    frame.drawLine(x0, y0 + 1 + 4 * offset, x1, y1 + 1 + 4 * offset, BLACK);
    frame.drawLine(x0, y0 + 2 + 4 * offset, x1, y1 + 2 + 4 * offset, RED);
    frame.drawHorizontalLine(50, 250, 50, DARK_RED);
    frame.drawRectangle(300, 25, 400, 75, DARK_RED, true);
    frame.drawRectangle(300, 75, 400, 125, RED, true);
    frame.drawRectangle(300, 125, 400, 175, BLACK, true);
    frame.drawRectangle(325, 50, 375, 150, WHITE, true);
    frame.refresh();
    // displayTestImage();
}

void loop() {
    // put your main code here, to run repeatedly:
}