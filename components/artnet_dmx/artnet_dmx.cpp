#include "artnet_dmx.h"
#include <cstring>

namespace esphome {
namespace artnet_dmx {

void ArtNetDMXOutput::setup() {
  if (this->dere_pin_) {
    this->dere_pin_->setup();
    this->dere_pin_->digital_write(true);  // DE=HIGH, /RE=HIGH -> Sender aktiv
  }

  udp_.listen(6454);
  udp_.onPacket([this](AsyncUDPPacket packet) {
    const uint8_t *d = packet.data();
    const size_t n = packet.length();
    // "Art-Net" + OpCode 0x5000 (ArtDMX)
    if (n > 18 && std::memcmp(d, "Art-Net", 7) == 0 && d[8] == 0x00 && d[9] == 0x50) {
      uint16_t incoming_universe = static_cast<uint16_t>(d[14] | (d[15] << 8));
      if (incoming_universe == this->universe_) {
        uint16_t len = static_cast<uint16_t>((d[17] << 8) | d[16]);
        if (len > 512) len = 512;
        std::memcpy(this->dmx_data_, d + 18, len);
        this->send_dmx(len);
      }
    }
  });
}

void ArtNetDMXOutput::send_dmx(uint16_t length) {
  if (!this->uart_) return;
  this->uart_->write_byte(0x00);  // DMX Startcode
  for (uint16_t i = 0; i < length; i++) {
    this->uart_->write_byte(this->dmx_data_[i]);
  }
}

}  // namespace artnet_dmx
}  // namespace esphome