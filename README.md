# AI Motivation Box

![AI Motivation Box](/images/final_presentation.jpg "Picture of the AI Motivation Box")

## Table of Contents

- [Introduction](#introduction)
- [Requirements](#requirements)
  - [Software Requirements](#software-requirements)
  - [Arduino Library Requirements](#arduino-library-requirements)
  - [Hardware Requirements](#hardware-requirements)
  - [Tool Requirements](#tool-requirements)
- [Instructions](#instructions)
  - [Step 1: Electrical Setup](#step-1-electrical-setup)
  - [Step 2: Code Install](#step-2-code-install)
  - [Step 3: 3D-Print](#step-3-3d-print-and-assemble)
  - [Step 4: Get Motivated I Guess](#step-4-get-motivated-i-guess)
 
## Introduction

This project utilizes the OpenAI API to pull down 5 motivational messages in the style of a random celebrity (that you specify in an array). This was a fun project to play around with the OpenAI API and finally use my I2C LCD display for something. 

The prompt can be see best in [this](\code\prompt_engineering\test_motivation.py) file.
 
## Requirements

### Software Requirements

| Software     | Version   | 
|--------------|-----------|
| Arduino IDE  |  1.8.19 or compatible    |
| [ESP32 Configuration of Arduino IDE](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) | 2.0.11 or compatible |
| Ultimaker Cura | 5.1.0 or higher |

### Arduino Library Requirements 

| Library     |
|--------------|
| ESP32 Arduino       |
| ArduinoJSON      |
| WiFi      |
| WiFiMulti      |
| HTTPClient    | 
| WifiClientSecure      |
| LiquidCrystal_I2C      | 

### Hardware Requirements

- [1 ESP 32](https://www.amazon.com/gp/product/B09GK74F7N/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)
- [1 20X4 LCD Module with I2C Shield](https://www.amazon.com/SunFounder-Serial-Module-Arduino-Mega2560/dp/B01GPUMP9C/ref=sr_1_5?keywords=sainsmart%2Blcd%2Bshield&th=1)
- [1 9V Power Supply](https://www.amazon.com/gp/product/B092V92YLW/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1)
- [2 4.7kohm Resistors](https://www.amazon.com/gp/product/B098BKR447/ref=ppx_yo_dt_b_asin_title_o08_s00?ie=UTF8&psc=1)
- [3.3V 5V MB102 Power Supply Module (or Substitute)](https://www.amazon.com/gp/product/B08JYPMCZY/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1)
- Assorted Wire
- Prototyping Board (or just wiring)
- PLA (or any other filament of choice)

### Tool Requirements

- Glue Gun
- Soldering Iron
- 3D Printer (mine is a GEEETECH A10)

## Instructions
 
### Step 1: Electrical Setup

The wiring diagram for this project can be seen in the below image.

![Wiring Diagram for the AI Motivation Box](/images/high_level_ai_motivation_diagram.jpg "Wiring Diagram for the AI Motivation Box")

### Step 2: Code Install

For the code install, clone this repo. Then make sure all libraries needed are installed. The code is in code/ai_motivation_box_working_code. Change the secrets.h file then use the Arduino IDE to compile and upload the code to an ESP32. You should set the SSID/password of your wireless network as well as your [OpenAI API key](https://www.howtogeek.com/885918/how-to-get-an-openai-api-key/).

If you want to add to the celebs list, go to "prompt.h" and change the "openai_celebs" variable as well as the "celeb_num" definition.

### Step 3: 3D-Print and Assemble

For this print, I flipped the AI Motivation Box on its face. I also used a support cube to increase the infill density of the cylindars for the button piece to about 90% (as shown below).

![Cura Layout Pic Showing Infill Cube](/images/cura_infill_increase.png "Cura Layout Pic Showing Infill Cube over the box's push button component that increases infill to 90% for that part alone")

I printed it at about 20% infill for most of it with 0.15mm resolution. You do need supports as there are 90 degree overhangs.

For the assembly, hot glue the push button to the top of the housing (as shown below). You will either need to solder leads onto the button (as I did) or attach leads some other way. 

![Glue the button on](/images/place_button.jpg "Glue the button in place with the leads to connect it to the ESP32")

Then, glue the LCD display into the front of the casing (as shown below).

Finally, place the button on the top, plug it in and get ready for some motivation.

![Place button on top](/images/putting_on_button.jpg "Place the 3d-printed button in place")

### Step 4: Get Motivated I Guess

Down here it's a 1920s theme.

![1920s Theme AI Motivation Box](/images/spacey_presentation.jpg "1920s themed AI motivation box")
