#ifndef MY_SERVO_H
#define MY_SERVO_H

#include <Arduino.h>
#include "driver/ledc.h"

class MyServo {
public:
  MyServo();
  // Připojí servo na zadaný pin s možností nastavení kanálu, timeru, režimu, frekvence, rozlišení a pulzních šířek (v ms)
  bool attach(int pin, int channel = LEDC_CHANNEL_0, int timer = LEDC_TIMER_0,
              ledc_mode_t mode = LEDC_LOW_SPEED_MODE,
              int frequency = 50,
              int resolution = LEDC_TIMER_16_BIT,
              float minPulse = 1.0, // minimální pulz (v ms)
              float maxPulse = 2.0); // maximální pulz (v ms)
  // Odpojí servo – zastaví PWM signál
  void detach();
  // Okamžitě nastaví úhel serva (0 až 180°)
  void write(int angle);
  // Vrátí aktuální úhel
  int read();
  // Nastaví cílový úhel pro plynulý přechod (s daným krokem, výchozí 1°)
  void moveTo(int angle, int step = 1);
  // Volat pravidelně v loop() – zajistí plynulý přechod z aktuálního úhlu do cílového
  void update();
  
private:
  int _pin;
  int _channel;
  int _timer;
  ledc_mode_t _mode;
  int _frequency;
  int _resolution;
  int _maxDuty;   // maximální hodnota duty = (2^resolution - 1)
  int _dutyMin;   // vypočítaná hodnota duty pro minPulse
  int _dutyMax;   // vypočítaná hodnota duty pro maxPulse
  int _currentAngle; // aktuální úhel serva
  int _targetAngle;  // cílový úhel, ke kterému chceme plynule přejít
  int _step;         // velikost kroku při přechodu
  unsigned long _lastUpdate; // čas poslední aktualizace
};

#endif // MY_SERVO_H
