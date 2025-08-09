#pragma once
#include "esphome.h"
#include <ESPAsyncUDP.h>

namespace esphome
{
    namespace artnet_dmx
    {

        class ArtNetDMXOutput : public Component
        {
        public:
            void set_dere_pin(uint8_t pin) { dere_pin_ = pin; }
            void set_uart(UARTComponent *uart) { uart_ = uart; }
            void set_universe(uint16_t universe) { universe_ = universe; }

            void setup() override;
            void loop() override {}

        protected:
            void send_dmx(uint16_t length);

            uint8_t dere_pin_;
            UARTComponent *uart_;
            uint16_t universe_;
            AsyncUDP udp_;
            uint8_t dmx_data_[512] = {0};
        };

    } // namespace artnet_dmx
} // namespace esphome