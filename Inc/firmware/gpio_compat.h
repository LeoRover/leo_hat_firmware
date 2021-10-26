#pragma once

#include <cstdint>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

struct GPIO {
  GPIO_TypeDef* port;
  uint16_t pin;
};

inline void gpio_set(const GPIO& gpio) {
  HAL_GPIO_WritePin(gpio.port, gpio.pin, GPIO_PIN_SET);
}

inline void gpio_reset(const GPIO& gpio) {
  HAL_GPIO_WritePin(gpio.port, gpio.pin, GPIO_PIN_RESET);
}