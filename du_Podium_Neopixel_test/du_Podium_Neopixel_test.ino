#define DEBUG 1
//
//#define PIN_LED_STRIP   7    // Define the pin to which your Neopixel strip is connected
//#define BUTTON_PIN      6    // Define the pin for the push button
//#define RELAY           5
//#define NUM_LEDS        52    // Number of LEDs in your Neopixel strip

#define PIN_LED_STRIP   7    // Define the pin to which your Neopixel strip is connected
#define BUTTON_PIN      6    // Define the pin for the push button
#define RELAY           5
#define NUM_LEDS        52    // Number of LEDs in your Neopixel strip

#define LONG_PRESS_TIME 1000
#define RUN_PIXEL 26

#define PRINT_FLAG Serial.println(flag);
#define RELAY_OFF digitalWrite(RELAY,HIGH);
#define RELAY_ON digitalWrite(RELAY,LOW);


#include <Adafruit_NeoPixel.h>


long buttonTimer = 0;

boolean buttonActive = false;
boolean longPressActive = false;
boolean flag = false; // Add the flag and initialize it to false

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);

// Define the colors
uint32_t color1 = strip.Color(28, 155, 220);  // RGB color for the first 30 LEDs
uint32_t color2 = strip.Color(105, 20, 90);  // RGB color for the second 30 LEDs


void startAnimation() {
  // Run the animation in a loop
  for (int offset = 0; offset < NUM_LEDS; offset++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if ((i >= offset && i < offset + 25) || (i >= offset - NUM_LEDS && i < offset - NUM_LEDS + 25)) {
        strip.setPixelColor(i, color1);
      } else {
        strip.setPixelColor(i, color2);
      }
    }
    strip.show();
    delay(25+-);  // Adjust the delay time as needed
  }
}

void stopAnimation() {
  // Turn off all LEDs
  strip.fill(strip.Color(0, 0, 0));
  strip.show();
}
void buttonCheck() {
  if (digitalRead(BUTTON_PIN) == !HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }

    if ((millis() - buttonTimer > LONG_PRESS_TIME) && (longPressActive == false)) {
      longPressActive = true;
      if (DEBUG) {
        PRINT_FLAG
      }
      flag = false; // Set the flag to 0 when a long press is detected
      if (DEBUG) {
        PRINT_FLAG
      }
      stopAnimation();
      RELAY_OFF
      delay(2000);

    }
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        if (!flag) { // Only execute if the flag is still 0
          if (DEBUG) {
            PRINT_FLAG
          }
          flag = true; // Set the flag to 1 when a short press is detected
          if (DEBUG) {
            PRINT_FLAG
          }
        }
      }
      buttonActive = false;
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  RELAY_OFF
  strip.begin();
  strip.show();
}

void loop() {
  buttonCheck();

  if (flag == 1) {
    RELAY_ON
    startAnimation();
  } else if (flag == 0) {
    RELAY_OFF
    stopAnimation();
  }
}
