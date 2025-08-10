#include "artnet_dmx.h"
#include <cstring>

namespace esphome
{
    namespace artnet_dmx
    {

        void ArtNetDMXOutput::setup()
        {
            if (this->dere_pin_)
            {
                this->dere_pin_->setup();
                this->dere_pin_->digital_write(true); // DE=HIGH, /RE=HIGH -> Sender aktiv
            }
            // Art-Net UDP Port
            udp_.begin(6454);
        }

        void ArtNetDMXOutput::loop()
        {
            int packet_size = udp_.parsePacket();
            if (packet_size <= 0)
                return;

            // Maximal sinnvolle Paketgröße abholen
            uint8_t buf[600];
            int n = udp_.read(buf, sizeof(buf));
            if (n <= 0)
                return;

            this->handle_packet_(buf, static_cast<size_t>(n));
        }

        void ArtNetDMXOutput::handle_packet_(const uint8_t *d, size_t n)
        {
            // Mindestlänge + Header prüfen: "Art-Net\0" und OpCode 0x5000 (ArtDMX)
            if (n <= 18)
                return;
            if (std::memcmp(d, "Art-Net", 7) != 0)
                return;
            if (!(d[8] == 0x00 && d[9] == 0x50))
                return; // little-endian 0x5000

            uint16_t incoming_universe = static_cast<uint16_t>(d[14] | (d[15] << 8));
            if (incoming_universe != this->universe_)
                return;

            uint16_t len = static_cast<uint16_t>((d[17] << 8) | d[16]);
            if (len > 512)
                len = 512;
            if (18 + len > n)
                return; // Paket unvollständig

            std::memcpy(this->dmx_data_, d + 18, len);
            this->send_dmx_(len);
        }

        void ArtNetDMXOutput::send_dmx_(uint16_t length)
        {
            if (!this->uart_)
                return;
            this->uart_->write_byte(0x00); // DMX Startcode
            for (uint16_t i = 0; i < length; i++)
            {
                this->uart_->write_byte(this->dmx_data_[i]);
            }
        }

    } // namespace artnet_dmx
} // namespace esphome