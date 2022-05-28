/**
 *  @filename   :   epd4in2_V2.h
 *  @brief      :   Header file for Dual-color e-paper library epd4in2.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     Nov 11 2020
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPD4IN2_V2_H
#define EPD4IN2_V2_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH 400
#define EPD_HEIGHT 300

/**
 * @brief Commands supported by e-Paper Display. For more information see: https://www.waveshare.net/w/upload/7/7f/4.2inch-e-paper-b-specification.pdf
 *
 */
enum EPD_Commands {
    PANEL_SETTING = 0x00,
    POWER_SETTING = 0x01,
    /**
     * After the Power OFF command, the driver will be powered OFF.
     * The sequence refers to POWER MANAGEMENT register section.
     * This command will turn off booster, controller, source driver, gate driver, VCOM, and temperature sensor,
     * but data will be kept until VDD turned OFF or Deep Sleep Mode.
     * Both source driver outputs and VCOM will be released to floating.
     */
    POWER_OFF = 0x02,
    POWER_OFF_SEQUENCE_SETTING = 0x03,
    /**
     * After the Power ON command, the driver will be powered ON The sequence refers to POWER MANAGEMENT section.
     * This command will turn on booster, controller, regulators, and temperature sensor will be activated for one-time sensing before enabling booster.
     * When all voltages are ready, the BUSY_N signal will return to high.
     */
    POWER_ON = 0x04,
    /**
     * This command enables the internal bandgap, which will be cleared by the next POF.
     */
    POWER_ON_MEASURE = 0x05,
    BOOSTER_SOFT_START = 0x06,
    DEEP_SLEEP = 0x07,
    /**
     * This command starts transmitting data and write them into SRAM.
     * In B/W mode, this command writes “OLD” data to SRAM. In B/W/Red mode, this command writes “B/W” data to SRAM.
     * In Program mode, this command writes “OTP” data to SRAM for programming.
     */
    DATA_START_TRANSMISSION_1 = 0x10,
    /**
     * Check the completeness of data. If data is complete, start to refresh display. 
     * Data_flag: Data flag of receiving user data. 
     * 0: Driver didn’t receive all the data. 
     * 1: Driver has already received all the one-frame data (DTM1 and DTM2). 
     * After “Data Start” (R10h) or “Data Stop” (R11h) commands and when data_flag=1, the 
     * refreshing of panel starts and BUSY_N signal will become “0”. 
     * The waiting interval form BUSY_N falling to the first FLG command must be > 200uS.
     */
    DATA_STOP = 0x11,
    /**
     * While user sent this command, driver will refresh display (data/VCOM) according to 
     * SRAM data and LUT. After Display Refresh command, BUSY_N signal will become “0” 
     * and the refreshing of panel starts. 
     * The waiting interval form BUSY_N falling to the first FLG command must be > 200uS.
     */
    DISPLAY_REFRESH = 0x12,
    /**
     * This command starts transmitting data and write them into SRAM. 
     * In B/W mode, this command writes “NEW” data to SRAM. 
     * In B/W/Red mode, this command writes “RED” data to SRAM.
     */
    DATA_START_TRANSMISSION_2 = 0x13,
    LUT_FOR_VCOM = 0x20,
    LUT_WHITE_TO_WHITE = 0x21,
    LUT_BLACK_TO_WHITE = 0x22,
    LUT_WHITE_TO_BLACK = 0x23,
    LUT_BLACK_TO_BLACK = 0x24,
    /**
     * The command controls the PLL clock frequency.
     */
    PLL_CONTROL = 0x30,
    TEMPERATURE_SENSOR_COMMAND = 0x40,
    /**
     * This command selects Internal or External temperature sensor.
     */
    TEMPERATURE_SENSOR_SELECTION = 0x41,
    /**
     * This command reads the temperature sensed by the temperature sensor.
     */
    TEMPERATURE_SENSOR_WRITE = 0x42,
    /**
     * This command reads the temperature sensed by the temperature sensor.
     */
    TEMPERATURE_SENSOR_READ = 0x43,
    /**
     * This command indicates the interval of VCOM and data output. When setting the 
     * vertical back porch, the total blanking will be kept (20 Hsync).
     */
    VCOM_AND_DATA_INTERVAL_SETTING = 0x50,
    /**
     * This command indicates the input power condition. Host can read this flag to learn 
     * the battery condition.
     */
    LOW_POWER_DETECTION = 0x51,
    /**
     * This command defines non-overlap period of Gate and Source.
     */
    TCON_SETTING = 0x60,
    /**
     * This command defines alternative resolution and this setting is of higher priority
     * than the RES[1:0] in R00H (PSR).
     */
    RESOLUTION_SETTING = 0x61,
    /**
     * This command defines the Fist Active Gate and First Active Source of active channels.
     */
    GSST_SETTING = 0x65,
    /**
     * This command reads the IC status.
     */
    GET_STATUS = 0x71,
    /**
     * This command reads the IC status.
     */
    AUTO_MEASUREMENT_VCOM = 0x80,
    /**
     * This command gets the VCOM value.
     */
    READ_VCOM_VALUE = 0x81,
    /**
     * This command sets VCOM_DC value
     */
    VCM_DC_SETTING = 0x82,
    /**
     * This command sets partial window.
     */
    PARTIAL_WINDOW = 0x90,
    /**
     * This command makes the display enter partial mode.
     */
    PARTIAL_IN = 0x91,
    /**
     * This command makes the display exit partial mode and enter normal mode.
     */
    PARTIAL_OUT = 0x92,
    /**
     * After this command is issued, the chip would enter the program mode. After the
     * programming procedure completed, a hardware reset is necessary for leaving
     * program mode.
     */
    PROGRAM_MODE = 0xA0,
    /**
     * After this command is transmitted, the programming state machine would be
     * activated. The BUSY flag would fall to 0 until the programming is completed.
     */
    ACTIVE_PROGRAMMING = 0xA1,
    /**
     * The command is used for reading the content of OTP for checking the data of
     * programming.
     */
    READ_OTP = 0xA2,
    /**
     * This command is set for saving power during fresh period. If the output voltage of
     * VCOM / Source is from negative to positive or from positive to negative, the power
     * saving mechanism will be activated.
     */
    POWER_SAVING = 0xE3
};

class Epd : EpdIf {
   public:
    unsigned int width;
    unsigned int height;

    Epd();
    ~Epd();
    int Init(void);
    void SendCommand(EPD_Commands command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);
    void SetPartialWindow(const unsigned char* buffer_black, const unsigned char* buffer_red, int x, int y, int w, int l);
    void SetPartialWindowBlack(const unsigned char* buffer_black, int x, int y, int w, int l);
    void SetPartialWindowRed(const unsigned char* buffer_red, int x, int y, int w, int l);
    void DisplayFrame(const unsigned char* frame_black, const unsigned char* frame_red);
    void DisplayFrame(void);
    void ClearFrame(void);
    void Sleep(void);

   private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
};

#endif /* EPD4IN2_H */

/* END OF FILE */
