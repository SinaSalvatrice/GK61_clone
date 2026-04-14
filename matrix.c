#include QMK_KEYBOARD_H
#include "i2c_master.h"

#define GPIO_COL_COUNT 13
#define MATRIX_IO_DELAY_US 30
#define PCF8575_ADDRESS (0x20 << 1)
#define PCF8575_ALL_HIGH 0xFFFF
#define PCF8575_P10_MASK 0x0100
#define I2C_TIMEOUT_MS 20

static const pin_t row_pins[MATRIX_ROWS] = {GP2, GP3, GP4, GP5, GP6};
static const pin_t gpio_col_pins[GPIO_COL_COUNT] = {GP7, GP8, GP9, GP10, GP11, GP12, GP13, GP14, GP15, GP26, GP27, GP28, GP29};

static bool     pcf8575_present = false;
static uint16_t pcf8575_state   = PCF8575_ALL_HIGH;

static void set_matrix_inputs_pullup(void) {
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        gpio_set_pin_input_high(row_pins[row]);
    }

    for (uint8_t col = 0; col < GPIO_COL_COUNT; col++) {
        gpio_set_pin_input_high(gpio_col_pins[col]);
    }
}

static bool pcf8575_write_state(uint16_t state) {
    uint8_t data[2] = {
        (uint8_t)(state & 0xFF),
        (uint8_t)((state >> 8) & 0xFF),
    };

    if (i2c_transmit(PCF8575_ADDRESS, data, sizeof(data), I2C_TIMEOUT_MS) != I2C_STATUS_SUCCESS) {
        return false;
    }

    pcf8575_state = state;
    return true;
}

static bool pcf8575_release_p10(void) {
    return pcf8575_write_state(pcf8575_state | PCF8575_P10_MASK);
}

static bool pcf8575_drive_p10_low(void) {
    return pcf8575_write_state(pcf8575_state & ~PCF8575_P10_MASK);
}

void matrix_init_custom(void) {
    set_matrix_inputs_pullup();
    i2c_init();

    pcf8575_present = pcf8575_write_state(PCF8575_ALL_HIGH) && pcf8575_release_p10();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool changed = false;
    matrix_row_t new_matrix[MATRIX_ROWS] = {0};

    set_matrix_inputs_pullup();

    for (uint8_t col = 0; col < GPIO_COL_COUNT; col++) {
        gpio_set_pin_output(gpio_col_pins[col]);
        gpio_write_pin_low(gpio_col_pins[col]);
        wait_us(MATRIX_IO_DELAY_US);

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            if (!gpio_read_pin(row_pins[row])) {
                new_matrix[row] |= ((matrix_row_t)1U << col);
            }
        }

        gpio_set_pin_input_high(gpio_col_pins[col]);
    }

    if (pcf8575_present) {
        if (pcf8575_drive_p10_low()) {
            wait_us(MATRIX_IO_DELAY_US);

            for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
                if (!gpio_read_pin(row_pins[row])) {
                    new_matrix[row] |= ((matrix_row_t)1U << 13);
                }
            }

            if (!pcf8575_release_p10()) {
                pcf8575_present = false;
            }
        } else {
            pcf8575_present = false;
        }
    }

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        if (current_matrix[row] != new_matrix[row]) {
            current_matrix[row] = new_matrix[row];
            changed             = true;
        }
    }

    return changed;
}
