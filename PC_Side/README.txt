On the PC side, a Python-based script defines the user menu and manages serial communication with the MSP430 microcontroller. This communication is handled using pyserial, a Python library that enables serial data exchange through the PyCharm environment.

The code architecture is built around multithreading: one thread is responsible for sending data from the user to the microcontroller, while a separate background thread continuously listens for incoming data from the MSP430. This separation ensures that incoming messages can be received and displayed in real time, even while the user is entering new commands.

