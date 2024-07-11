import serial
import threading
import sys
import time
import ttkbootstrap as ttk
from ttkbootstrap.constants import *

# Global flag to signal the listener thread to stop
exit_flag = threading.Event()

# Global variable to track fault state
is_fault_active = False

# Initialize oops_label as None initially
oops_label = None

# Function to handle incoming serial data
def listen_serial(ser):
    try:
        while not exit_flag.is_set():
            if ser.in_waiting > 0:
                # Read and decode incoming data
                incoming_data = ser.readline().decode('utf-8').strip()
                if incoming_data:
                    print(f"\nReceived: {incoming_data}")
                    update_display(incoming_data)
                    print("Enter command to send to Teensy (or 'exit' to quit): ", end='', flush=True)
            else:
                update_display("NO_DATA")
            time.sleep(0.1)  # Add a small delay to reduce CPU usage
    except Exception as e:
        if not exit_flag.is_set():
            print(f"Error reading from serial port: {e}")

# Function to update the display
def update_display(data):
    global is_fault_active
    global oops_label

    if oops_label is None:
        return  # Return early if oops_label has not been initialized

    if "LEVEL" in data:
        parts = data.split(':')
        if len(parts) == 2 and parts[0].strip() == "LEVEL":
            current_level.set(parts[1].strip())
    elif "SCORE" in data:
        parts = data.split(':')
        if len(parts) == 2 and parts[0].strip() == "SCORE":
            current_score.set(parts[1].strip())
    elif "FAULT" in data:

        oops_label.set("OOPS!!!")  # Update using set() method of the StringVar
    elif "NO_DATA" in data:
        oops_label.set("")
    else:
        # Clear fault state if no FAULT message is received
        if is_fault_active and oops_label:
            is_fault_active = False
            oops_label.set("")  # Update to empty string using set() method

# Function to send commands to the Teensy
def send_command(ser, command):
    try:
        # Send the command to Teensy
        ser.write(command.encode('utf-8') + b'\n')
        print(f"Command '{command}' sent to the Teensy.")
    except Exception as e:
        print(f"Error sending command to serial port: {e}")

# Function to create the GUI
def create_gui():
    global oops_label

    root = ttk.Window(themename="darkly")
    root.title("Game Status")

    global current_level
    global current_score

    current_level = ttk.StringVar(value="1")
    current_score = ttk.StringVar(value="0")
    oops_label = ttk.StringVar(value="")

    style = ttk.Style()
    style.configure('TLabel', font=('Comic Sans MS', 24), foreground='orange')
    style.configure('Title.TLabel', font=('Comic Sans MS', 48, 'bold'), foreground='blue')

    frame = ttk.Frame(root, padding=20, style='TFrame')
    frame.pack(fill=BOTH, expand=True)

    ttk.Label(frame, text="Hi Jelle, good luck!", style='Title.TLabel').pack(pady=20)

    ttk.Label(frame, text="Current Level:", style='TLabel').pack(pady=5)
    ttk.Label(frame, textvariable=current_level, style='TLabel').pack(pady=5)

    ttk.Label(frame, text="Current Score:", style='TLabel').pack(pady=5)
    ttk.Label(frame, textvariable=current_score, style='TLabel').pack(pady=5)

    ttk.Label(frame, text="Mistake:", style='TLabel').pack(pady=5)
    ttk.Label(frame, textvariable=oops_label, style='TLabel').pack(pady=5)

    return root

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

    # Create and start the GUI
    root = create_gui()

    # Get screen width and height
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()

    # Calculate window dimensions
    window_width = screen_width - 100  # Adjust as needed
    window_height = screen_height - 100  # Adjust as needed

    # Center the window
    window_x = (screen_width - window_width) // 2
    window_y = (screen_height - window_height) // 2

    # Set window size and position
    root.geometry(f"{window_width}x{window_height}+{window_x}+{window_y}")

    # Run the GUI in the main thread
    try:
        root.mainloop()
    except KeyboardInterrupt:
        exit_flag.set()
        listener_thread.join()

    print("Exiting program.")
    ser.close()