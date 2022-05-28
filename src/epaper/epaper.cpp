#include "epaper.h"

Epd epd;

void displayTestImage() {
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
    unsigned char image[1500];
    Paint paint(image, 400, 28);  // width should be the multiple of 8
    paint.SetRotate(ROTATE_180);
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