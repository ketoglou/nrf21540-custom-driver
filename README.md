# NRF21540 Custom Driver for Nordic Connect SDK
This is a simple custom driver for NRF21540 for use with Nordic Connect SDK. Because of the confusion I had to use the official
Nordic FEM driver, I created a simpler version for me. You can use it with Nordic Connect SDK, but you must follow some configuring
steps before.
\\
1. Add the following to device tree (or overlay) and change the pins if needed.
```
&pinctrl {

	spi0_default: spi0_default {
		group1 {
			psels = <NRF_PSEL(SPIM_SCK, 0, 15)>,
				<NRF_PSEL(SPIM_MOSI, 0, 13)>,
				<NRF_PSEL(SPIM_MISO, 0, 14)>;
		};
	};

	spi0_sleep: spi0_sleep {
		group1 {
			psels = <NRF_PSEL(SPIM_SCK, 0, 15)>,
				<NRF_PSEL(SPIM_MOSI, 0, 13)>,
				<NRF_PSEL(SPIM_MISO, 0, 14)>;
			low-power-enable;
		};
	};

};
```
```
fem_spi: &spi0 {
	status = "okay";
	cs-gpios = <&gpio0 12 GPIO_ACTIVE_LOW>;

	pinctrl-0 = <&spi0_default>;
	pinctrl-1 = <&spi0_sleep>;
	pinctrl-names = "default", "sleep";

	nrf_radio_fem_spi: nrf21540-fem-spi@0 {
		compatible = "nordic,nrf21540-fem-spi";
		status = "okay";
		reg = <0>;
		spi-max-frequency = <8000000>;
	};
};
```
```
&gpio0 {
	status = "okay";
};
```
2. Add the following configs.
```
CONFIG_GPIO=y
CONFIG_SPI=y
CONFIG_PINCTRL=y
```
3. Create a inc folder in project folder and add .h file there, add the .c file to src folder. Add the following to CMakeLists.txt
```
target_include_directories(app PRIVATE inc)

target_sources(app PRIVATE src/main.c src/fem_nrf21540.c inc/fem_nrf21540.h)
```

## Example use
```
...
//Init FEM
fem_nrf21540_init();
fem_nrf21540_power_on(true);
fem_nrf21540_set_power(31); //Max gain
fem_nrf21540_tx_enable();
...
```


