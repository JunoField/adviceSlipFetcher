#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#include "arduino_secrets.h"

//define LCD output
LiquidCrystal_I2C lcd(0x27, 16,2);

//LCD parameters
#define LINE_LENGTH 16
#define SCROLL_DELAY 150

//Button input
#define BUTTON_PIN 5

//WiFi and server details
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* api = "https://api.adviceslip.com/advice";


//Connection vars
String receivedData;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;




//overloaded function: print a line or 2 lines to LCD
void printToLCD(String line1, String line2){
    if ((line1.length() > 16) || (line2.length() > 16)){
        printToLCD("Runtime Error", "String too long"); //if strings are too long, show an error
        return;
    }
    //if strings are of correct length, print them centred:
    lcd.clear();
    lcd.setCursor(((16 - line1.length()) / 2), 0);
    lcd.print(line1);
    lcd.setCursor(((16 - line2.length()) / 2), 1);
    lcd.print(line2);
}
void printToLCD(String text){
    if (text.length() < 16){
        printToLCD(text, "");
    }
    else{
        printToLCD(text.substring(0, 16), text.substring(16));
    }
}

//function: print a long scrolling string
void printScrollingString(String inputText){
    String remainingText = inputText;
    while (remainingText.length() > LINE_LENGTH){
        printToLCD(remainingText.substring(0, LINE_LENGTH), "----------------");
        remainingText = remainingText.substring(1);
        delay(SCROLL_DELAY);
    }
}


//function: GET the quote from the server and return it as a string
String getNewQuote(const char* api){
    HTTPClient http;
    http.begin(api);

    //send request
    int httpResponse = http.GET();

    String payload = "";

    if (httpResponse > 0){
        payload = http.getString();
    }

    else{
        printToLCD("Server error", String(httpResponse));
    }
    http.end();
    JSONVar receivedJson = JSON.parse(payload);
    String quote = receivedJson["slip"]["advice"];
    return quote;
}


//SETUP function
void setup(){
    //init lcd and pins
    lcd.init();
    lcd.backlight();
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    //Connect to WiFi
    printToLCD("Connecting to", "WiFi");
    delay(500);
    printToLCD(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){ //while waiting for connection display this
        delay(500);
        printToLCD("Connecting", "Please wait");
    }
    printToLCD("Connected", WiFi.localIP().toString()); //confirm successful connection and display IP
    delay(500);

    //display intro msg
    printToLCD("Quote generator", "Press button");
    lcd.noBacklight();
    
}

void loop(){
    if (digitalRead(BUTTON_PIN) == LOW){
        lcd.backlight();
        if (WiFi.status() == WL_CONNECTED){
            String quote = getNewQuote(api);
            printScrollingString(quote);
        } else{
            printToLCD("Network error", "Try again later");
        }

        
        delay(500);
        printToLCD("Quote Generator", "Press button");
        lcd.noBacklight();
    }
}
