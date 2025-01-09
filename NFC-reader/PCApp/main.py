import serial
import time

def main():
    # Nastavení sériového portu
   #  port = "COM6"  # Změňte podle vašeho zařízení (např. /dev/ttyUSB0 na Linuxu)
    port = "/dev/ttyUSB0"  # Změňte podle vašeho zařízení (např. /dev/ttyUSB0 na Linuxu)
    baudrate = 115200  # Rychlost přenosu musí odpovídat nastavení zařízení STM

    try:
        # Otevření sériového portu
        ser = serial.Serial(port, baudrate, timeout=1)
        print(f"Connected to {port} at {baudrate} baud.")

        # Posílání dat do zařízení
        while True:
            message = input("Enter message to send (or 'exit' to quit): ")
            if message.lower() == "exit":
                break

            # Poslat zprávu
            ser.write(message.encode('utf-8'))
            print(f"Sent: {message}")

            # Čtení odpovědi ze zařízení
            time.sleep(0.1)  # Malá prodleva, aby zařízení mohlo odpovědět
            if ser.in_waiting > 0:
                response = ser.read(ser.in_waiting).decode('utf-8').strip()
                print(f"Received: {response}")

    except serial.SerialException as e:
        print(f"Error: {e}")
    finally:
        # Uzavření sériového portu
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial port closed.")

if __name__ == "__main__":
    main()
