#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ADXL345_U.h>
 
#define PIN 12
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, PIN, NEO_GRB + NEO_KHZ800);

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

struct vec3
{
  float x;
  float y;
  float z;
};

float length(float x, float y, float z)
{
  return sqrt(x * x + y * y + z * z);
}

float dot(vec3 a, vec3 b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 points[]={
  {0.2764f, -0.8506f, 0.4472f},
  {0.8944f,  0.0000f, 0.4472f},
  {0.2764f,  0.8506f, 0.4472f},
  {-0.7236f, 0.5257f, 0.4472f},
  {-0.7236f, -0.5257f, 0.4472f},
  
  {0.0f, 0.0f, 1.0f},  
};

void setup(void) 
{
  accel.begin();
  
  strip.begin();
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
  }
  strip.show();
  
  Serial.begin(9600);

  accel.setRange(ADXL345_RANGE_8_G);
  
  strip.show(); // Initialize all pixels to 'off'
}
 
void loop() {
  
  /* Get a new sensor event */ 
  sensors_event_t event; 
  accel.getEvent(&event);
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x / 8); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y / 8); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z / 8); Serial.print("  ");Serial.println("m/s^2 ");
  delay(50);

  float accelMag = length(event.acceleration.x / 8, event.acceleration.y / 8, event.acceleration.z / 8);

  vec3 dir;

  if (accelMag != 0) {
    dir.x = -(event.acceleration.x / 8) / accelMag;
    dir.y = -(event.acceleration.y / 8) / accelMag;
    dir.z = -(event.acceleration.z / 8) / accelMag;
  
    for (int i = 0; i < 6; i++) {
      if(dot(points[i], dir) > 0.7f) {
        strip.setPixelColor(i, strip.Color(255, 255, 255));
      }
      else{
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }
  }
  else  {
    for (int i = 0; i < 6; i++) {
        strip.setPixelColor(i, strip.Color(64, 0, 0));
    }    
  }
        strip.setPixelColor(0, strip.Color(255, 255, 255));
  
  strip.show();
  
  //colorWipe(strip.Color(x, y, z), 10); 
  //colorWipe(strip.Color(0, 0, 0), 10); 
  
  //rainbowCycle(20);
}
 
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
 
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
