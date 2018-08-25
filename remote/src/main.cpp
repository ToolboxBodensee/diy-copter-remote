//#include <SPI.h>
#include <Arduino.h>

void setup() {
  /* while(Serial) { */
  /*   delay(500); */
  /* } */
  Serial.begin(115200);
  Serial.println("Remote ON!");

  //SPI.begin();
  //CC2500.init();

}

void loop() {
  // put your main code here, to run repeatedly:

}
