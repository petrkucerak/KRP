# NFC Reader

The goal of this project is to implemented smart NFC reader than communicated with PC via USB and could be controlled with build-in LCD display.

## Scripts

```sh
cmake --build .
cmake --build . && make clean && make # universal rebuild snippet
```

## Components

### LCD controlling

Using DSI and LTDC component.

> [!NOTE]
> The display serial interface (**DSI**) is part of a group of communication protocols defined by the MIPI® Alliance. 
> Detailed described in the [reference manual](/assets/reference_manual.pdf) in chapt 34 **DSI Host**.

> [!NOTE]
> The LCD-TFT (liquid crystal display - thin film transistor) display controller (**LTDC**) provides a parallel digital RGB (red, green, blue) and signals for horizontal, vertical synchronization, pixel clock and data enable as output to interface directly to a variety of LCD and TFT panels.
> Detailed described in the [reference manual](/assets/reference_manual.pdf) in chapt 33.