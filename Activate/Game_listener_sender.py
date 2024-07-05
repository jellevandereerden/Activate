import serial
import threading
import sys
import time

# Global flag to signal the listener thread to stop
exit_flag = threading.Event()

def listen_serial(ser):
    try:
        while not exit_flag.is_set():
            if ser.in_waiting > 0:
                # Read and decode incoming data
                incoming_data = ser.readline().decode('utf-8').strip()
                if incoming_data:
                    print(f"\nReceived: {incoming_data}")
                    print("Enter command to send to Teensy (or 'exit' to quit): ", end='', flush=True)
            time.sleep(0.1)  # Add a small delay to reduce CPU usage
    except Exception as e:
        if not exit_flag.is_set():
            print(f"Error reading from serial port: {e}")

def send_command(ser, command):
    try:
        # Send the command to Teensy
        ser.write(command.encode('utf-8') + b'\n')
        print(f"Command '{command}' sent to the Teensy.")
    except Exception as e:
        print(f"Error sending command to serial port: {e}")

if __name__ == "__main__":
    serial_port = 'COM4'  # Specify your desired serial port here

    # Initialize serial connection
    try:
        ser = serial.Serial(serial_port, 9600, timeout=1)
        print(f"Listening to serial port '{serial_port}'...")
    except Exception as e:
        print(f"Error opening serial port: {e}")
        sys.exit(1)

    # Start listening to serial port in a separate thread
    listener_thread = threading.Thread(target=listen_serial, args=(ser,), daemon=True)
    listener_thread.start()

    # Main loop for sending commands via prompt
    while True:
        command = input("Enter command to send to Teensy (or 'exit' to quit): ")
        
        if command.lower() == 'exit':
            exit_flag.set()  # Signal the listener thread to stop
            listener_thread.join()  # Wait for the listener thread to finish
            break
        
        send_command(ser, command)

    print("Exiting program.")
    ser.close()
