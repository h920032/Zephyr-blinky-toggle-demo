# Zephyr PWM and GPIO LED Example on Custom F303 Board

This project demonstrates two basic functionalities on a custom board based on the ST Nucleo F303K8 using the Zephyr RTOS:

1.  **PWM LED Fading:** Fades an LED connected to a PWM pin (PB3, TIM2_CH2).
2.  **GPIO LED Toggling:** Toggles an LED connected to a GPIO pin (PB4) every 500ms.

![](./img/demo.gif)

## Hardware Requirements

*   A custom board based on the ST Nucleo F303K8 (or the Nucleo F303K8 itself).
*   An LED connected to pin PB3 (PWM output, `pwm-led0` alias).
*   An LED connected to pin PB4 (GPIO output, `led1` alias).
*   ST-Link debugger/programmer (on-board for Nucleo boards).

## Building

You can build the application using the Zephyr build system (`west`):

```bash
west build -b custom_board
```

## Flashing

Flash the application to the board using `west`:

```bash
west flash
```

This command assumes you have the necessary flashing tools (like STM32CubeProgrammer or OpenOCD) installed and configured. Refer to the Zephyr documentation for details on flashing specific boards.
