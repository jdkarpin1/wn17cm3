// Based on @swoga work https://github.com/swoga/standingdesk

#include "wn17cm3.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace wn17cm3 {

static const char *const TAG = "wn17cm3.sensor";

void Wn17cm3Component::loop() {
  uint8_t data;
  while (this->available() > 0) {
    this->read_byte(&data);
    if (this->buffer_.empty() && (data != ':'))
      continue;
    buffer_.push_back(data);
    if (this->buffer_.size() == 8 || this->buffer_.size() == 9) {
        this->check_buffer_();
    }        
  }
}

void Wn17cm3Component::check_buffer_() {
  if (this->buffer_[0] == ':' && 
      this->buffer_[1] == 'D' && 
      this->buffer_[this->buffer_.size()-1] == ';') {
    std::string str;
    // 01234567
    // :D101D6;
    if (this->buffer_.size() == 8) {
        str = std::string(buffer_.begin()+2, buffer_.end()-3);
    }
    // 012345678
    // :D75.614;
    if (this->buffer_.size() == 9) {
        str = std::string(buffer_.begin()+2, buffer_.end()-2);
    }    
    float num_float = std::stof(str);
    this->publish_state(num_float);
    ESP_LOGV(TAG, "Height: %.2f", num_float);
  }
  if (buffer_.size() >= 9) {
    this->buffer_.clear();
  }
}

void Wn17cm3Component::dump_config() { LOG_SENSOR("", "wn17cm3 Sensor", this); }

}  // namespace wn17cm3
}  // namespace esphome