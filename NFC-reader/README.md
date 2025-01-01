# NFC Reader

The goal of this project is to implemented smart NFC reader than communicated with PC via USB and could be controlled with build-in LCD display.

## Scripts

```sh
cmake --build .
cmake --build . && make clean && make # universal rebuild snippet
```

## Components

### LCD controlling

Using DSI and LTDC component. DSI Host can works in modes:
- adapted command mode (**DBI**) - using LTDC interface - *using in my project*
- video mode (**DPI**) - using LTDC interface
- APB slave interface - can operate concurrently with either LTDC interface in video or adapted command mode
- Video mode pattern generator - using for testing

> [!NOTE]
> The display serial interface (**DSI**) is part of a group of communication protocols defined by the MIPI® Alliance. 
> Detailed described in the [reference manual](/assets/reference_manual.pdf) in chapt 34 **DSI Host**.

> [!NOTE]
> The LCD-TFT (liquid crystal display - thin film transistor) display controller (**LTDC**) provides a parallel digital RGB (red, green, blue) and signals for horizontal, vertical synchronization, pixel clock and data enable as output to interface directly to a variety of LCD and TFT panels.
> Detailed described in the [reference manual](/assets/reference_manual.pdf) in chapt 33.

#### Adapted command mode on LTDC interface

The adapted command mode, enables the system to input a stream of pixel from the LTDC that is conveyed by DSI Host using the command mode transmission (using the DCS packets). The adapted command mode also supports pixel input control rate signaling and **tearing effect** report mechanism.

> [!NOTE]
> Screen tearing is a visual artifact in video display where a display device shows information from multiple frames in a single screen draw. The artifact occurs when the video feed to the device is not synchronized with the display's refresh rate. (*Source: [Wikipedia](https://en.wikipedia.org/wiki/Screen_tearing)*)

Adapted command mode of operation supports: 16 bpp, 18 bpp, and 24 bpp RGB. In my project I choose 24 bpp.

### OTM8009A

The LCD (`B-LCD40-DSI1`) is controlled by `otm8009a` integrated driver.



### BSP components

BSP components are imported as a submodule. To initialize them use command

```sh
git submodule update --recursive
```

> [!WARNING]
> Regeneration code with using STM32CubeMX remove BSP components from CMake list files. So, do not forget return origin BSP source files imports. Relative to [CM7/mx-generated.cmake](CM7/mx-generated.cmake)

#### List of BSP components

| name        | source                                                  | description      |
| ----------- | ------------------------------------------------------- | ---------------- |
| Common      | https://github.com/STMicroelectronics/stm32-bsp-common  | Common component |
| ft6x06      | https://github.com/STMicroelectronics/stm32-ft6x06      | Touch screen     |
| is42s32800j | https://github.com/STMicroelectronics/stm32-is42s32800j | SDRAM            |
| otm8009a    | https://github.com/STMicroelectronics/stm32-otm8009a    | LCD              |