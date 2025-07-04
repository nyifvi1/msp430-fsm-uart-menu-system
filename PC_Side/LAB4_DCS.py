import serial
import threading
import time

SERIAL_PORT = 'COM3'  #need to be edited
BAUD_RATE = 9600
lock = threading.Lock()

menu = """
=== MENU ===
1. Counting onto the LCD screen with a delay of X[ms]
2. Circular tone series via Buzzer with delay of X[ms]
3. Get delay time X[ms]:
4. Potentiometer 3-digit value [v] onto LCD
5. On each PB1 pressed, send a Massage “I love my Negev”
6. Clear LCD screen
7. Show menu
8. Sleep
"""


def read_from_serial(ser):
    global data
    while True:
        if ser.in_waiting:
            with lock:
                data = ser.read_until(expected=b'\n')
                print(data.decode('ascii', errors='ignore'), end="")



def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, bytesize=serial.EIGHTBITS,
                            parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=1)
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")

        thread = threading.Thread(target=read_from_serial, args=(ser,), daemon=True)
        thread.start()

        print(menu)

        while True:
            user_input = input("Enter state number: ")
            ser.write((user_input + '\n').encode('utf-8'))
            if user_input == "3":
                user_input = input("Enter X[ms]: ")
                ser.write((user_input + '\n').encode('utf-8'))
            #if user_input == "5":
             #   print(data.decode('utf-8', errors='ignore'), end="")
            if user_input == "7":
                print(menu)


    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()


if __name__ == '__main__':
    main()
