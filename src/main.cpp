#define ADC_IN PB0 
#define NUM_SAMPLES 100
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET   -1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void corr();
void sample();
void generate_ref(int sampling_frequency);

void setup() {
  pinMode(ADC_IN, INPUT_ANALOG);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display.display();
  delay(2000);
// Displays "Hello world!" on the screen 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Hello world!");
  display.display();
}

double raw[NUM_SAMPLES];
double reference[NUM_SAMPLES];
double output[NUM_SAMPLES * 2];
double ref_freq = 1000; 

void loop() 
{
  
  int start = micros();
  sample();
  double sample_time = (micros() - start) / pow(10, 6);
  double sampling_frequency = NUM_SAMPLES / sample_time;
  //generate_ref(sampling_frequency);
  // corr();

  for (int n = 0; n < NUM_SAMPLES; ++n) {
    reference[n] = sin(2*PI*ref_freq*n/sampling_frequency);
  }

  double output[NUM_SAMPLES * 2] = {};

  int i = -1;
  for(int raw_index = 0; raw_index < NUM_SAMPLES; ++raw_index) {
    i++;
    int j = -1 ;
    for(int ref_index = 0; ref_index < NUM_SAMPLES; ++ref_index) { 
      j++;
      output[i+j] += raw[raw_index] * reference[ref_index];
    }
  } 

  double highest = -10000000;
  for (double x : output) {
    if (x > highest) {
      highest = x; 
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Signal Strength:");
  display.println(highest);
//   display.println("Analog Input:");
//   display.println(analogRead(ADC_IN));
  display.display();
}

/*
void generate_ref(int sampling_frequency) {
  for (int n = 0; n < NUM_SAMPLES; ++n) {
    reference[n] = sin(2*PI*ref_freq*n/sampling_frequency);
  }
}
*/

void sample() {
  for (int i=0; i < NUM_SAMPLES; ++i){
    raw[i] = analogRead(ADC_IN);
  }
}

/*
void corr() {
  int i = -1;
  for(int raw_index = 0; raw_index < NUM_SAMPLES; ++raw_index) {
    i++;
    int j = -1 ;
    for(int ref_index = 0; ref_index < NUM_SAMPLES; ++ref_index) { 
      j++;
      output[i+j] += raw[raw_index] * reference[ref_index];
    }
  } 
}
*/