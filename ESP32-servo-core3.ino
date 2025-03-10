#include <Arduino.h>
#include <ServoESP32.h>

MyServo servo;
unsigned long lastSwitch = 0;
const unsigned long switchInterval = 2000; // interval 2 sekundy

void setup() {
  Serial.begin(115200);
  // Připojení serva na pin 18 s výchozím nastavením (kanál 0, timer 0, 50 Hz, 16-bit, 1–2 ms pulz)
  servo.attach(18);
  servo.write(0);  // startujeme s úhlem 0°
  lastSwitch = millis();
}

void loop() {
  servo.update();  // aktualizuje plynulý přechod k cílovému úhlu

  // Neblokující přepínání cílového úhlu každých 2 sekundy
  if (millis() - lastSwitch >= switchInterval) {
    if (servo.read() == 0) {
      servo.moveTo(180, 2); // plynulý přechod na 180° se skokem 2°
    } 
    else if (servo.read() == 180) {
      servo.moveTo(0, 2);   // plynulý přechod zpět na 0°
    }
    lastSwitch = millis();
  }
}
