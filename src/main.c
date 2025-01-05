/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

static const struct pwm_dt_spec pwm_led0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1) , gpios);

#define NUM_STEPS	256U
#define SLEEP_MSEC	5U

static K_THREAD_STACK_DEFINE(fade_stack, 512);
static K_THREAD_STACK_DEFINE(toggle_stack, 512);
static struct k_thread fade_thread_data;
static struct k_thread toggle_thread_data;

void fade_led(void *p1, void *p2, void *p3)
{
	uint32_t pulse_width = 0U;
	uint32_t step = pwm_led0.period / NUM_STEPS;
	uint8_t dir = 1U;
	int ret;

	while (1) {
		ret = pwm_set_pulse_dt(&pwm_led0, pulse_width);
		if (ret) {
			printk("Error %d: failed to set pulse width\n", ret);
			return;
		}
		printk("Using pulse width %d%%\n", 100 * pulse_width / pwm_led0.period);

		if (dir) {
			pulse_width += step;
			if (pulse_width >= pwm_led0.period) {
				pulse_width = pwm_led0.period - step;
				dir = 0U;
			}
		} else {
			if (pulse_width >= step) {
				pulse_width -= step;
			} else {
				pulse_width = step;
				dir = 1U;
			}
		}
		k_sleep(K_MSEC(SLEEP_MSEC));
	}
}

void toggle_led(void *p1, void *p2, void *p3)
{
	while (1) {
		gpio_pin_toggle_dt(&led1);
		k_sleep(K_MSEC(500));
	}
}

int main(void)
{
	int ret;

	printk("PWM-based LED fade\n");

	if (!pwm_is_ready_dt(&pwm_led0)) {
		printk("Error: PWM device %s is not ready\n",
		       pwm_led0.dev->name);
		return 0;
	}

	gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);

	k_thread_create(&fade_thread_data, fade_stack,
			K_THREAD_STACK_SIZEOF(fade_stack),
			fade_led, NULL, NULL, NULL,
			5, 0, K_NO_WAIT);

	k_thread_create(&toggle_thread_data, toggle_stack,
			K_THREAD_STACK_SIZEOF(toggle_stack),
			toggle_led, NULL, NULL, NULL,
			5, 0, K_NO_WAIT);

	return 0;
}

