# TSMS -Typical Modern SPI, IIC and Other Custom Digital Interface Driver Library

## Description

This library is typically made for STM32 series MCU. Also, the TI MSP series are on the roadmap.

This library contains following parts:

- SPI driver (Both hardware and software SPI, only support master mode)
- IIC driver (Both hardware and software IIC, only support master mode)
- Custom digital interface (parallel-write, parallel-read, etc.)
- Printer driver (Used to connect UART, Bluetooth, etc.)
- GPIO driver (Interrupted is supported)
- Utility functions (list, etc.)

This library contains following examples:

- SN74HC595 8-bit shift register
- AD7606 16-bit ADC
- DAC8831 16-bit DAC
- ADS1115 16-bit ADC
- AD7190 24-bit ADC

## Usage

### 1. Clone this repository

```bash
git clone https://github.com/MidCoard/TSMS_Driver.git
```

### 2. Copy the library to your project

Copy the `Tsms` folder in the `TSMS_Driver` folder to your project.

### 3. Add the library to your project

Add the following code to your `CMakeLists.txt` file:

```cmake
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Tsms/Inc)
```
Add `Tsms/*.*` to the `file(GLOB_RECURSE SOURCES ...)` function.

### 4. Add the header file to your source file

Just include the header file you need.

### Note

If you want to use the example driver code, just add `example` folder to your project as `Tsms` folder.


## License
This library is licensed under the AGPL-3.0 license. See [LICENSE](LICENSE) for details.