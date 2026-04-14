#ifndef CONFIG_H
#define CONFIG_H

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6061
#define DEVICE_VER      0x0001
#define MANUFACTURER    SinaSalvatrice
#define PRODUCT         GK61_clone
#define DESCRIPTION     GK61 clone, RP2040 Zero, matrix only

#define MATRIX_ROWS 5
#define MATRIX_COLS 14
#define MATRIX_ROW_PINS { 2, 3, 4, 5, 6 }
#define MATRIX_COL_PINS { 7, 8, 9, 10, 11, 12, 13, 14, 15, 26, 27, 28, 29, P10 }
#define DIODE_DIRECTION COL2ROW

#endif // CONFIG_H
