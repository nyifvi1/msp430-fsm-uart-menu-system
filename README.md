# MSP430 UART Menu-Controlled FSM System

This project implements a finite state machine (FSM) on an MSP430 microcontroller, controlled via UART communication from a PC. The system allows a user to select different actions through a terminal-based menu, such as displaying a counter on an LCD, playing tones on a buzzer, entering delay values, reading potentiometer values, and more.

## Project Structure

- `CCS/source/`
  - `main.c` – Main application logic and FSM controller
  - `api.c` – Application-level logic for each menu action
  - `bsp.c` – Board Support Package: initialization of peripherals and ports
  - `halGPIO.c` – Low-level hardware abstraction for GPIO
- `CCS/header/`
  - Header files matching each source module
- `PC_Side/`
  - `LAB4_DCS.py` – Python script for sending commands via UART to the MSP430
  - `README.txt` – Usage instructions for the PC-side script

## Menu Options (sent via UART)

The system accepts the following commands from the PC terminal:

1. Show counter on LCD with delay X (in milliseconds)
2. Play circular sequence of tones on buzzer with delay X
3. Input a new delay time X from the user (as a digit string ending with Enter)
4. Read 3-digit voltage value from potentiometer and display on LCD
5. On PB1 button press, send the string "I love my Negev" via UART
6. Clear the LCD screen
7. Show the menu again on the terminal
8. Enter sleep mode (idle state)

Default delay value: 500 ms  
Tone series: {1kHz, 1.25kHz, 1.5kHz, 1.75kHz, 2kHz, 2.25kHz, 2.5kHz}

## How to Run

### MSP430 Side

1. Open the `CCS/` project in Code Composer Studio.
2. Compile and flash to an MSP430 device (e.g., MSP430G2553).
3. Ensure proper UART connection (TX, RX, GND) between PC and MSP430.

### PC Side

1. Install Python and `pyserial`:
   ```
   pip install pyserial
   ```
2. Connect to the correct COM port.
3. Run the script:
   ```
   python PC_Side/LAB4_DCS.py
   ```
4. Choose options by typing the corresponding number and pressing Enter.

