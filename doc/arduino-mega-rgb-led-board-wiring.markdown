# Arduino Mega RGB LED board wiring

## Introduction

This wiring is for [Velleman VMA307 LED board](https://www.velleman.eu/products/view/?country=be&lang=en&id=435528).

For other boards wiring is similar.

Only RGB LED boards **with current limiting resistors** shall be used.

## Wiring illustration

![arduino-mega-rgb-led-board-wiring.png](arduino-mega-rgb-led-board-wiring.png)

**NB! current limiting resistors are not shown in illustration.**

## Wiring table

| Signal | Arduino Mega 2560 pin | Velleman WMA307 |
| --- | --- | --- |
| Red | Digital pin 23 | R (red anode via resistor) |
| Green | Digital pin 25 | G (green anode via resistor) |
| Blue | Digital pin 27 | B (blue anode via resistor) |
| Ground | GND | - (common cathode) |
