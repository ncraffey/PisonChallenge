// Nicholas Craffey
// Pison Coding Challenge

#include <bluefruit.h>

BLEUart bleuart; // uart over ble

unsigned short degrees = 0; // for sine wave
unsigned long timestamp;

void setup()
{
  Serial.begin(115200); // desired baud
  while (!Serial) { }; // while serial stream is not available do nothing

  Bluefruit.begin();
  Bluefruit.setName("Feather");
  
  // start ble uart service
  bleuart.begin();
 
  // start advertising for connection with hub
  Bluefruit.Advertising.start();
}

// easy way to map longs/ints to bytes

union longToBytes {
  unsigned long value;
  byte bytes[4];
};

union int32toBytes {
  int32_t value;
  byte bytes[4];
};

void loop()
{
    
    // 20 byte buffer
    byte buf[20];
    longToBytes timestamp;
    timestamp.value = millis(); // get the current time

    int32toBytes sine_wave;
    // need to convert degrees to radians, then multiply by 5000 for
    // a nicer looking plot
    sine_wave.value = 5000 * sin(degrees * PI / 180);

    // big endian representation of data
    
    // msec timestamp
    buf[0] = (byte) timestamp.bytes[3];
    buf[1] = (byte) timestamp.bytes[2];
    buf[2] = (byte) timestamp.bytes[1];
    buf[3] = (byte) timestamp.bytes[0];

    // imu data (n/a)
    buf[4] = (byte) 0;
    buf[5] = (byte) 0;
    buf[6] = (byte) 0;
    buf[7] = (byte) 0;
    buf[8] = (byte) 0;
    buf[9] = (byte) 0;
    buf[10] = (byte) 0;
    buf[11] = (byte) 0;

    // moving sin wave
    buf[12] = (byte) sine_wave.bytes[3];
    buf[13] = (byte) sine_wave.bytes[2];
    buf[14] = (byte) sine_wave.bytes[1];
    buf[15] = (byte) sine_wave.bytes[0];

    // adc channel 1 (n/a)
    buf[16] = (byte) 0;
    buf[17] = (byte) 0;
    buf[18] = (byte) 0;
    buf[19] = (byte) 0;

    // now that buffer is populated, write to the feather
    bleuart.write(buf, sizeof(buf));

  // for sine
  if (degrees < 359)
    degrees++;
  else degrees = 0;
  
  // enter low-power mode until something happens
  waitForEvent();
}

