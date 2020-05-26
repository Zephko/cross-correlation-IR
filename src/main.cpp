#define ADC_IN PB0 
#define NUM_SAMPLES 100
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <array>
#include <chrono>
#include <thread>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET   -1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

std::array<double, NUM_SAMPLES * 2> x_correlation(std::array<double, NUM_SAMPLES>raw_data,  std::array<double, NUM_SAMPLES>ref) ;
std::array<double, NUM_SAMPLES> reference_wave(int amplitude, int frequency, int sampleRate, int numSamples) ;
void printToDisplay(String s);

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
   // display.println("Hello world!");
  display.display();
}

void loop() 
{
  display.clearDisplay();
  int sampling_period = 0.1;
  int sampling_rate_Hz = (int) (NUM_SAMPLES/ sampling_period);

  std::array<double, NUM_SAMPLES> raw;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    raw[i] = analogRead(ADC_IN);
    //std::this_thread::sleep_for(std::chrono::milliseconds(100)) ;
    delay(100);
  };
  std::array<double, NUM_SAMPLES> ref = reference_wave(1, 1000, sampling_rate_Hz, NUM_SAMPLES);
  std::array<double, NUM_SAMPLES * 2> x_corr = x_correlation(raw, ref);
  double maxSignal = *std::max_element(x_corr.begin(), x_corr.end());
  printToDisplay((String) maxSignal);
}

std::array<double, NUM_SAMPLES * 2> x_correlation(std::array<double, NUM_SAMPLES>raw_data,  std::array<double, NUM_SAMPLES>ref) {
  std::array<double, raw_data.size() + ref.size()> output = {0};

  int i = - 1;
  for (int raw_index = 0; raw_index < (int) raw_data.size(); ++raw_index) {
    i++;
    int j = -1;
    for (int ref_index = 0; ref_index < (int) ref.size(); ++ref_index) {
      j++;
      output[i + j] += raw_data[raw_index] * ref[ref_index];
    }
  }
  return output;
}

std::array<double, NUM_SAMPLES> reference_wave(int amplitude, int frequency, int sampleRate, int numSamples) {
  std::array<double, NUM_SAMPLES> wave;

  for (int n = 0; n < numSamples; ++n) {
    wave[n] = amplitude * sin(2 * PI * frequency * n / sampleRate);
  }

  return wave;
}

void printToDisplay(String s) 
{
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(s);
}