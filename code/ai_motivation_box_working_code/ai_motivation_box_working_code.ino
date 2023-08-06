#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include "secrets.h"
#include "prompt.h"

#define button_pin 13

#define lcdCol 20
#define lcdRow 4
LiquidCrystal_I2C lcd(0x3f, lcdCol, lcdRow);  

int curr_msg_num = 1;
String curr_msg = "msg";
int curr_celeb_num = 1;

HTTPClient https;
StaticJsonDocument<2048> temp_json_buff;
char* buff = (char*)malloc(sizeof(char)*2048); //is really used as a buffer for all data types
char* api_bearer_buff = (char*)malloc(sizeof(char)*50);
DynamicJsonDocument openai_result_buff(2048);
DynamicJsonDocument openai_content_buff(2048);
WiFiClientSecure https_client;
WiFiMulti WiFiMulti;

void wifi_setup() {
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.println("Wifi connected!");
  https_client.setCACert(openai_rootCACert);
}

void fill_api_bearer(char* api_bearer_buffer, char* header_val_bearer) {
  sprintf(api_bearer_buffer, header_val_bearer, openai_key);
  Serial.println();
}

int fill_prompt() {
  int rand_celeb = random(celeb_num);
  char* temp_celeb_chrArr = (char*)malloc(50);
  openai_celebs[rand_celeb].toCharArray(temp_celeb_chrArr, 50);
  sprintf(buff+1024, openai_prompt, temp_celeb_chrArr);
  free(temp_celeb_chrArr);
  Serial.println("Prompt: ");
  Serial.println(buff+1024);
  Serial.println();
  return rand_celeb;
}

void fill_prompt_json() {
  sprintf(buff, openai_json, buff+1024);
}

//returns the random celeb num
int post_openai() {
  Serial.println("Starting to parse openai");
  if (https.begin(https_client, openai_api_endpoint)) {
    fill_api_bearer(api_bearer_buff, (char*)(&openai_header_auth[1]));
    
    https.addHeader(openai_header_auth[0], api_bearer_buff);
    char* temp_content_buff = (char*)malloc(openai_header_content_type[1].length()+1);
    openai_header_content_type[1].toCharArray(temp_content_buff, openai_header_content_type[1].length()+1);
    https.addHeader(openai_header_content_type[0], temp_content_buff);
    int rand_celeb_num = fill_prompt();
    fill_prompt_json();
    https.setTimeout(30000);
    Serial.println(buff);
    int https_code = https.POST(buff);
    free(temp_content_buff);
    if (https_code > 0) {
      Serial.print("HTTPS POST code: ");
      Serial.println(https_code);
      (https.getString()).toCharArray(buff, 2048);
      return rand_celeb_num;
    }
    else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(https_code).c_str());
    }
    https.end();
  }
  else {
    Serial.println("HTTPS Unable to Connect");
  }
}

int parse_openai() {
  DeserializationError err = deserializeJson(openai_result_buff, buff);
  if (err) {
    Serial.print("Deserialization failed");
    Serial.println(err.f_str());
    return 1;
  }
  else {
    //char curr_content = &openai_result_buff["choices"][0]["message"]["content"];
    DeserializationError err = deserializeJson(openai_content_buff, openai_result_buff["choices"][0]["message"]["content"]);
    if (err) {
      Serial.print("Deserialization failed for content openai");
      Serial.println(err.f_str());
      return 1;
    }
    return 0;
  }
}

void lcd_print_str_old(String* msg2print) {
  int curr_col = -1;
  int page_num = 0;
  for (int curr_chr = 0; curr_chr < msg2print->length(); curr_chr++){
    if ((curr_chr % lcdCol) == 0) {
      curr_col++;
      if (curr_col > 3) {
        curr_col = 0;
      }
      lcd.setCursor(0, curr_col);
    }
    lcd.print((*msg2print)[curr_chr]);
    delay(50);
    if ((curr_chr % (lcdCol*lcdRow)) == 0) {
      if (page_num > 0) {
        delay(1000);
        Serial.println("Next page...");
        lcd.clear();
      }
      else {
        page_num++;
      }
    }
  }
}

void lcd_print_line(char* curr_line, int sz) {
  for (int i = 0; i < sz; i++) {
    lcd.print(curr_line[i]); 
    Serial.print(curr_line[i]);
    delay(50);
  }
  Serial.println();
}

void lcd_print_str(String* msg2print) {
  Serial.println(ESP.getFreeHeap());
  int curr_lcd_line_num = 0;
  char* curr_lcd_line_buff = (char*)malloc(lcdCol + 2);
  Serial.print("Curr line buff pointer:");
  Serial.println(curr_lcd_line_buff);
  if (!curr_lcd_line_buff) {
    Serial.println("Malloc for buffer failed");
    return;
  }
  msg2print->toCharArray(buff, 2048);
  Serial.print("msg2print: ");
  Serial.println(msg2print->length());
  char* curr_lcd_buff = buff;
  int curr_page = 0;
  int curr_line = 0;
  bool end_of_buff = false;
  bool start_line = true;
  while (!end_of_buff) {
    int next_line_offset = lcdCol;
    int curr_line_size = lcdCol;
    if (curr_lcd_line_num >= ((int)msg2print->length() - lcdCol)) {
      end_of_buff = true;
      curr_line_size = (int)msg2print->length() - curr_lcd_line_num;
      if (curr_line_size < 0) {
        curr_line_size = msg2print->length();
      }
    }
    if (end_of_buff) {
      memcpy(curr_lcd_line_buff, curr_lcd_buff + curr_lcd_line_num, curr_line_size);
      if (curr_line >= lcdRow-1) {
        curr_line = 0;
        delay(700);
        lcd.clear();
        curr_page++;
      }
      else {
        curr_line+= (!start_line);
        start_line = false;
      }
      lcd.setCursor(0, curr_line);
      lcd_print_line(curr_lcd_line_buff, curr_line_size);
    }
    else {
      memcpy(curr_lcd_line_buff, curr_lcd_buff + curr_lcd_line_num, curr_line_size+1);
      if (curr_line >= lcdRow-1) {
        curr_line = 0;
        delay(700);
        lcd.clear();
        curr_page++;
      }
      else {
        curr_line+= (!start_line);
        start_line = false;
      }
      lcd.setCursor(0, curr_line);
      if (curr_lcd_line_buff[lcdCol-1] != ' ' && curr_lcd_line_buff[lcdCol] != ' ') {
        int curr_split_line_num = lcdCol-1;
        while (curr_split_line_num > 0 && curr_lcd_line_buff[curr_split_line_num] != ' ') {
          curr_split_line_num--;
        }
        if (curr_split_line_num == 0) {
          lcd_print_line(curr_lcd_line_buff, curr_line_size);
        }
        else {
          lcd_print_line(curr_lcd_line_buff, curr_split_line_num);
          next_line_offset = (curr_split_line_num+1);
        }
      }
      else {
        lcd_print_line(curr_lcd_line_buff, curr_line_size);
        if (curr_lcd_line_buff[lcdCol] == ' ') {
          next_line_offset++;
        }
      }
    }
    curr_lcd_line_num += next_line_offset;
  } 
  free(curr_lcd_line_buff); 
}

void setup() {
  pinMode(button_pin, INPUT_PULLUP);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  String loading_start = "Starting Up...";
  lcd_print_str(&loading_start);
  wifi_setup();
}

void loop() {
  if (curr_msg_num > 5) {
    curr_msg_num = 1;
  }
  if (curr_msg_num <= 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    String loading_start = "Loading New Celeb...";
    lcd_print_str(&loading_start);
    curr_celeb_num = post_openai();
    while (parse_openai() != 0) {
      curr_celeb_num = post_openai();
    }
  }
  lcd.clear();
  int lcd_on = 0;
  int button_state = digitalRead(button_pin);
  while (button_state != LOW) {
    if (lcd_on == 10) {
      lcd.clear();
    }
    else {
      if (lcd_on == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Message Ready");
      }
    }
    delay(50);
    button_state = digitalRead(button_pin);
    if (lcd_on > 20) {
      lcd_on = 0;
    }
    else {
      lcd_on++;
    }
  }
  lcd.clear();
  curr_msg = "msg";
  curr_msg += String(curr_msg_num);
  String curr_msg_str = openai_content_buff[curr_msg];
  Serial.print("Rand celeb: ");
  Serial.println(openai_celebs[curr_celeb_num]);
  Serial.println("Motivational Message:");
  Serial.println(curr_msg_str);
  lcd.clear();
  lcd.setCursor(0, 0);
  String curr_celeb_str = "";
  curr_celeb_str += openai_celebs[curr_celeb_num];
  //lcd.print(curr_celeb_str);
  lcd_print_str(&curr_celeb_str);
  Serial.println("Celeb done");
  lcd.setCursor(0, 2);
  lcd.print("GPT Motivation Ver1");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd_print_str(&curr_msg_str);
  delay(10*1000);
  lcd.clear();
  curr_msg_num++;
}
