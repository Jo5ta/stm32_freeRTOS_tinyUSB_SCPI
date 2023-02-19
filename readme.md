# STM32 freeRTOS tinyUSB SCPI
## Steps to add tinyUBS to STM32CubeMx generated project
- enable in `STM32CubeMX` in `Pinout & Configuration` under `Connectivity` the periphery for `USB_OTG_FS` as `Device only`.
    - Make sure that usb global interrupt under `NVIC Settings` is enabled
- add `tinyUSB` as submodule to your project
- add the folder `tinyUSB/src` to your build sources    
    - exclude `tinyUSB/src/portable/st` from your build, instead use `tinyUSB/src/portable/synopsys`
- add the following lines to your `main.h`
    ```c
    #include "tusb_config.h"
    #include "tusb.h" 
    ```
- add `CFG_TUSB_MCU=OPT_MCU_STM32F7` as preprocessor macros to your project

- add `tinyUSB` interrupt handler with the following code to `OTG_FS_IRQHandler` in `stm32f7xx_it.c`, before `HAL_PCD_IRQHandler` is called
    ```c
    tud_int_handler(BOARD_TUD_RHPORT);
    return;
    ```
- add the following code after hal initalisation was called in `main` function
    ```c
    tud_init(BOARD_TUD_RHPORT);
    ```
- call `tud_task()` in each loop iteration in `main` function.
- add your own `tusb_config.h` and `usb_description.c` to your project