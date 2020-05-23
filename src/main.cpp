#define ADC_IN PB0 
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET   -1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void handle_interrupt();
void handle_interrupt_schmitt();
//volatile int i = 0;
//volatile int j = 0;
//int loopcount = 0;

void setup() {
  pinMode(ADC_IN, INPUT_ANALOG);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display.display();
  delay(2000);
// Displays "Hello world!" on the screen display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Hello world!");
  display.display();
}

void loop() 
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Analog Reading:");
  display.setCursor(0, 15);
  int val = analogRead(ADC_IN);
  display.println(val);
  display.display();
  delay(300);
}  