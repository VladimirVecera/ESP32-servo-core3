#include "ServoESP32.h"

MyServo::MyServo() : _pin(-1), _channel(-1), _timer(-1), _mode(LEDC_LOW_SPEED_MODE),
                     _frequency(50), _resolution(LEDC_TIMER_16_BIT), _maxDuty(0),
                     _dutyMin(0), _dutyMax(0), _currentAngle(0), _targetAngle(0),
                     _step(1), _lastUpdate(0)
{
}

bool MyServo::attach(int pin, int channel, int timer, ledc_mode_t mode, int frequency, int resolution, float minPulse, float maxPulse) {
  _pin = pin;
  _channel = channel;
  _timer = timer;
  _mode = mode;
  _frequency = frequency;
  _resolution = resolution;
  _maxDuty = (1 << _resolution) - 1;
  float period = 1000.0 / _frequency; // perioda v ms (např. 20 ms při 50 Hz)
  _dutyMin = (int)((minPulse / period) * _maxDuty);
  _dutyMax = (int)((maxPulse / period) * _maxDuty);

  // Konfigurace LEDC timeru – dle deklarace:
  // speed_mode, duty_resolution, timer_num, freq_hz, clk_cfg
  ledc_timer_config_t timer_conf = {
    .speed_mode      = _mode,
    .duty_resolution = (ledc_timer_bit_t)_resolution,
    .timer_num       = (ledc_timer_t)_timer,
    .freq_hz         = _frequency,
    .clk_cfg         = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer_conf);

  // Konfigurace LEDC kanálu – dle deklarace:
  // gpio_num, speed_mode, channel, intr_type, timer_sel, duty, hpoint
  ledc_channel_config_t channel_conf = {
    .gpio_num   = _pin,
    .speed_mode = _mode,
    .channel    = (ledc_channel_t)_channel,
    .intr_type  = LEDC_INTR_DISABLE,
    .timer_sel  = (ledc_timer_t)_timer,
    .duty       = 0,
    .hpoint     = 0
  };
  ledc_channel_config(&channel_conf);

  _currentAngle = 0;
  _targetAngle = 0;
  write(_currentAngle);
  return true;
}

void MyServo::detach() {
  ledc_stop(_mode, (ledc_channel_t)_channel, 0);
}

void MyServo::write(int angle) {
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  _currentAngle = angle;
  int duty = map(angle, 0, 180, _dutyMin, _dutyMax);
  ledc_set_duty(_mode, (ledc_channel_t)_channel, duty);
  ledc_update_duty(_mode, (ledc_channel_t)_channel);
}

int MyServo::read() {
  return _currentAngle;
}

void MyServo::moveTo(int angle, int step) {
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  _targetAngle = angle;
  _step = (step > 0) ? step : 1;
}

void MyServo::update() {
  unsigned long now = millis();
  // Aktualizace každých 20 ms (můžete změnit interval dle potřeby)
  if (now - _lastUpdate >= 20) {
    if (_currentAngle < _targetAngle) {
      _currentAngle += _step;
      if (_currentAngle > _targetAngle)
        _currentAngle = _targetAngle;
      write(_currentAngle);
    }
    else if (_currentAngle > _targetAngle) {
      _currentAngle -= _step;
      if (_currentAngle < _targetAngle)
        _currentAngle = _targetAngle;
      write(_currentAngle);
    }
    _lastUpdate = now;
  }
}
