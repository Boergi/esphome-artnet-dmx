#include "artnet_dmx.h"

namespace esphome
{
    namespace artnet_dmx
    {

        void ArtNetDMXOutput::setup()
        {
            pinMode(dere_pin_, OUTPUT);
            digitalWrite(dere_pin_, HIGH); // TX aktiv

            udp_.listen(6454);
            udp_.onPacket([this](AsyncUDPPacket packet)
                          {
    if (packet.length() > 18 &&
        memcmp(packet.data(), "Art-Net", 7) == 0 &&
        packet.data()[8] == 0x00 && packet.data()[9] == 0x50) { // OpCode ArtDMX
      uint16_t incomingUniverse = packet.data()[14] | (packet.data()[15] << 8);
      if (incomingUniverse == universe_) {
        uint16_t length = (packet.data()[17] << 8) | packet.data()[16];
        memcpy(dmx_data_, packet.data() + 18, length);
        send_dmx(length);
      }
    } });
        }

        void ArtNetDMXOutput::send_dmx(uint16_t length)
        {
            if (!uart_)
                return;
            uart_->write_byte(0); // Startcode
            for (int i = 0; i < length; i++)
            {
                uart_->write_byte(dmx_data_[i]);
            }
        }

    } // namespace artnet_dmx
} // namespace esphome