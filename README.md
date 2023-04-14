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



## License
This library is licensed under the AGPL-3.0 license. See [LICENSE](LICENSE) for details.