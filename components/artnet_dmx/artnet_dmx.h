#pragma once
#include "esphome.h"
#include <AsyncUDP.h>  // aus ESP32-Arduino-Core

namespace esphome {
namespace artnet_dmx {

class ArtNetDMXOutput : public Component {
 public:
  void set_dere_pin(GPIOPin *pin) { dere_pin_ = pin; }
  void set_uart(UARTComponent *uart) { uart_ = uart; }
  void set_universe(uint16_t universe) { universe_ = universe; }

  void setup() override;
  void loop() override {}

 protected:
  void send_dmx(uint16_t length);

  GPIOPin *dere_pin_{nullptr};
  UARTComponent *uart_{nullptr};
  uint16_t universe_{0};
  AsyncUDP udp_;
  uint8_t dmx_data_[512] = {0};
};

}  // namespace artnet_dmx
}  // namespace esphome