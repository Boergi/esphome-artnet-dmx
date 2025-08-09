# registers the "artnet_dmx:" YAML block for ESPHome 2025.x
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome import pins

CONF_DERE_PIN = "dere_pin"
CONF_UNIVERSE = "universe"
CONF_UART_ID = "uart_id"

artnet_ns = cg.esphome_ns.namespace("artnet_dmx")
ArtNetDMXOutput = artnet_ns.class_("ArtNetDMXOutput", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ArtNetDMXOutput),
    cv.Required(CONF_DERE_PIN): pins.gpio_output_pin_schema,
    cv.Required(CONF_UNIVERSE): cv.int_range(min=0, max=32767),
    cv.GenerateID(CONF_UART_ID): cv.use_id(uart.UARTComponent),
}).extend(cv.COMPONENT_SCHEMA)

DEPENDENCIES = ["uart"]

async def to_code(config):
    var = cg.new_Pvariable(config[cg.CONF_ID])
    dere = await cg.gpio_pin_expression(config[CONF_DERE_PIN])
    cg.add(var.set_dere_pin(dere))
    uart_comp = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart(uart_comp))
    cg.add(var.set_universe(config[CONF_UNIVERSE]))
    await cg.register_component(var, config)