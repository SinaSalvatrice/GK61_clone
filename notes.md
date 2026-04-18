# gk61_clone Notes

- Matrix only, no RGB, encoder, or OLED.
- RP2040 Zero microcontroller.
- Matrix rows: 0, 1, 2, 3, 4
- Matrix columns: 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 26, 27, 28
- Matrix is fully routed on the RP2040, no expander required.
- Layout matches GK61 (see reference image for mapping).
- No advanced features enabled in rules.mk.
- Build output: UF2 file for RP2040 bootloader.
