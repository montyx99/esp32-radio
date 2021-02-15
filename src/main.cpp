#include <Arduino.h>
#include <WiFi.h>
#include <NewEncoder.h>
#include <Audio.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const uint8_t Signal816[16] PROGMEM = //mobie signal
{
    0xFE,0x02,0x92,0x0A,0x54,0x2A,0x38,0xAA,0x12,0xAA,0x12,0xAA,0x12,0xAA,0x12,0xAA
};

const uint8_t Msg816[16] PROGMEM =  //message
{
    0x1F,0xF8,0x10,0x08,0x18,0x18,0x14,0x28,0x13,0xC8,0x10,0x08,0x10,0x08,0x1F,0xF8
};

const uint8_t Bat816_100[16] PROGMEM = //batery
{
    0b00001111,0b11111110, //     ########### 
    0b00110000,0b00000010, //   ##          #  
    0b00100110,0b11011010, //   #  ## ## ## # 
    0b00100110,0b11011010, //   #  ## ## ## # 
    0b00100110,0b11011010, //   #  ## ## ## # 
    0b00100110,0b11011010, //   #  ## ## ## # 
    0b00110000,0b00000010, //   ##          # 
    0b00001111,0b11111110  //     ########### 
};

const uint8_t Bat816_66[16] PROGMEM = //batery
{
    0b00001111,0b11111110, //     ########### 
    0b00110000,0b00000010, //   ##          #  
    0b00100000,0b11011010, //   #     ## ## # 
    0b00100000,0b11011010, //   #     ## ## # 
    0b00100000,0b11011010, //   #     ## ## # 
    0b00100000,0b11011010, //   #     ## ## # 
    0b00110000,0b00000010, //   ##          # 
    0b00001111,0b11111110  //     ########### 
};

const uint8_t Bat816_33[16] PROGMEM = //batery
{
    0b00001111,0b11111110, //     ########### 
    0b00110000,0b00000010, //   ##          #  
    0b00100000,0b00011010, //   #        ## # 
    0b00100000,0b00011010, //   #        ## # 
    0b00100000,0b00011010, //   #        ## # 
    0b00100000,0b00011010, //   #        ## # 
    0b00110000,0b00000010, //   ##          # 
    0b00001111,0b11111110  //     ########### 
};

const uint8_t Bat816_0[16] PROGMEM = //batery
{
    0b00001111,0b11111110, //     ########### 
    0b00110000,0b00000010, //   ##          #  
    0b00100000,0b00000010, //   #           # 
    0b00100000,0b00000010, //   #           # 
    0b00100000,0b00000010, //   #           # 
    0b00100000,0b00000010, //   #           # 
    0b00110000,0b00000010, //   ##          # 
    0b00001111,0b11111110  //     ########### 
};

const uint8_t Bat816_charge[16] PROGMEM = //batery
{
    0b00001111,0b11111110, //     ########### 
    0b00110000,0b11000010, //   ##    ##    #  
    0b00100001,0b11100010, //   #    ####   # 
    0b00100011,0b11110010, //   #   ######  # 
    0b00100000,0b11000010, //   #     ##    # 
    0b00100000,0b11000010, //   #     ##    # 
    0b00110000,0b11000010, //   ##    ##    # 
    0b00001111,0b11111110  //     ########### 
};

const uint8_t Bluetooth88[8] PROGMEM = // bluetooth
{
    0x18,0x54,0x32,0x1C,0x1C,0x32,0x54,0x18
};

const uint8_t GPRS88[8] PROGMEM = //GPRS
{
    0xC3,0x99,0x24,0x20,0x2C,0x24,0x99,0xC3
};

const uint8_t Alarm88[8] PROGMEM = //alram
{
    0xC3,0xBD,0x42,0x52,0x4E,0x42,0x3C,0xC3
};

const uint8_t Volume816_21[16] PROGMEM =
{
	0b00000000, 0b01000000, //          #      
	0b00000000, 0b11000000, //         ##   
	0b00000001, 0b11000000, //        ###  
	0b00000101, 0b11000000, //      # ###   
	0b00001101, 0b11000000, //     ## ###       
	0b00011101, 0b11000000, //    ### ###  
	0b01011101, 0b11000000, //  # ### ###      
	0b11011101, 0b11000000, // ## ### ###      
};

const uint8_t Volume816_14[16] PROGMEM =
{
	0b00000000, 0b00000000, //                
	0b00000000, 0b00000000, //            
	0b00000000, 0b00000000, //          
	0b00000100, 0b00000000, //      #    
	0b00001100, 0b00000000, //     ##        
	0b00011100, 0b00000000, //    ###   
	0b01011100, 0b00000000, //  # ###       
	0b11011100, 0b00000000, // ## ###       
};

const uint8_t Volume816_7[16] PROGMEM =
{
	0b00000000, 0b00000000, //                
	0b00000000, 0b00000000, //            
	0b00000000, 0b00000000, //          
	0b00000000, 0b00000000, //        
	0b00000000, 0b00000000, //            
	0b00000000, 0b00000000, //       
	0b01000000, 0b00000000, //  #        
	0b11000000, 0b00000000, // ##        
};

const uint8_t Volume816_0[16] PROGMEM =
{
	0b00000000, 0b00000000, //                
	0b00000000, 0b00000000, //            
	0b00000000, 0b00000000, //          
	0b11000110, 0b00000000, // ##   ##  
	0b01101100, 0b00000000, //  ## ##         
	0b00111000, 0b00000000, //   ###    
	0b01101100, 0b00000000, //  ## ##       
	0b11000110, 0b00000000, // ##   ##     
};

const uint8_t Wifi816[16] PROGMEM =
{
	0b00011111, 0b11111000, //   ##########
	0b00111111, 0b11111100, //  ############
	0b01100000, 0b00001110, // ###        ###
	0b00000011, 0b11000000, //      ####
	0b00000111, 0b11100000, //     ######
	0b00000000, 0b00000000, //            
	0b00000001, 0b10000000, //       ##
	0b00000001, 0b10000000, //       ##     
};

void btnClick();

/* initialise encoders */
char CLK = 34;
char DT = 35;
char BTTN = 32;

// Initialize audio
char I2S_BCLK = 27;
char I2S_LRC = 26;
char I2S_DOUT = 25;

void ESP_ISR callBack(NewEncoder &enc);
NewEncoder encoder(CLK, DT, 0, 21, 1, FULL_PULSE);

volatile int16_t encoderValue;
volatile bool newValue;

int b; // button state

Audio audio;

String ssid =     "xxxxx";
String password = "xxxxx";

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void setVolumeGui(uint8_t volume);

void setup() {
    int16_t value;
    Serial.begin(115200);
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED) delay(1500);
    delay(2000);

// // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    display.clearDisplay();
    display.drawBitmap(
        0,
        0,
        Wifi816, 16, 8, 1);

    display.display();

    if (!encoder.begin()) {

        Serial.println(F("Encoder Failed to Start. Check pin assignments and available interrupts. Aborting."));
        while (1) {
        yield();
        }
    }
    else {
#if defined(__AVR__)
        noInterrupts();  // 16-bit access not atomic on 8-bit processor
#endif
        encoderValue = encoder;
        value = encoderValue;
#if defined(__AVR__)
        interrupts();
#endif
        Serial.print(F("Encoder Successfully Started at value = "));
        Serial.println(value);
    }
    encoder.attachCallback(callBack);


    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(encoder.getValue()); // 0...21
    
    setVolumeGui(encoder.getValue());

    if(audio.connecttohost("http://stream2.radio1.hu:80/radio1_aac_320.m4a")) {
        Serial.println("Stream successfully connected!");
        display.setTextSize(2);
        display.setCursor((SCREEN_WIDTH - 60) / 2, 16);
        display.write("Radio1");

        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(10, 30 + 5);
        display.println(F("Test text to scroll!"));
        display.startscrollleft(0x04, 0x06);
        display.display();
    }
    // if(audio.connecttohost("http://www.wdr.de/wdrlive/media/einslive.m3u")) {
    //     Serial.println("Stream successfully connected!");
    // }
//    audio.connecttoFS(SD, "/320k_test.mp3");
//    audio.connecttoFS(SD, "test.wav");
//      audio.connecttohost("http://mp3.ffh.de/radioffh/hqlivestream.mp3"); //  128k mp3
//    audio.connecttospeech("Wenn die Hunde schlafen, kann der Wolf gut Schafe stehlen.", "de");

    attachInterrupt(digitalPinToInterrupt(BTTN), btnClick, FAIL);

    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.cp437(true);

    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setCursor((display.width() - 20 - 16), 0);     // Start at top-left corner
    display.write("16%");

    display.drawBitmap(
        (display.width()  - 16),
        0,
        Bat816_66, 16, 8, 1);

    display.display();
    delay(1000);
    Serial.println("Setup Ready!");
}

void loop()
{
    audio.loop();

    int16_t currentEncoderValue;

    if (newValue) {
        noInterrupts();
        newValue = false;
        currentEncoderValue = encoderValue;
        interrupts();
        Serial.print("Encoder: ");
        Serial.println(currentEncoderValue);
        audio.setVolume(currentEncoderValue);
        setVolumeGui(currentEncoderValue);
    }
}

void ESP_ISR callBack(NewEncoder &enc) {
  int16_t e = enc;
  if (e != encoderValue) {
    encoderValue = e;
    newValue = true;
  }
}

void btnClick() {
    Serial.println("Clicked!");
}

void setVolumeGui(uint8_t volume) {
    const uint8_t* icon;
    if(volume >14) {
        icon = Volume816_21;
    }
    else if (volume > 7) {
        icon = Volume816_14;
    }
    else if (volume > 0) {
        icon = Volume816_7;
    }
    else{
        icon = Volume816_0;
    }

    String volumeTxt;
    char tmp[5];
    if (volume < 10) {
        volumeTxt += '0';
    }
    volumeTxt += volume;
    volumeTxt += "/21";

    volumeTxt.toCharArray(tmp, 6);

    display.fillRect(0,
        display.height() - 8,
        10, 8, SSD1306_BLACK);
    display.drawBitmap(
        0,
        display.height() - 8,
        icon, 10, 8, 1);

    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setCursor(10 + 3, display.height() - 8);     // Start at top-left corner
    display.write(tmp);

    display.display();
}