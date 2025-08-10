#pragma once
#include "esphome.h"
#include <WiFi.h>
#include <WiFiUdp.h>

namespace esphome
{
    namespace artnet_dmx
    {

        class ArtNetDMXOutput : public Component
        {
        public:
            void set_dere_pin(GPIOPin *pin) { dere_pin_ = pin; }
            void set_uart(UARTComponent *uart) { uart_ = uart; }
            void set_universe(uint16_t universe) { universe_ = universe; }

            void setup() override;
            void loop() override;

        protected:
            void handle_packet_(const uint8_t *d, size_t n);
            void send_dmx_(uint16_t length);

            GPIOPin *dere_pin_{nullptr};
            UARTComponent *uart_{nullptr};
            uint16_t universe_{0};

            WiFiUDP udp_;
            uint8_t dmx_data_[512] = {0};
        };

    } // namespace artnet_dmx
} // namespace esphome