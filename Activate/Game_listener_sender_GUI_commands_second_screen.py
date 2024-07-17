import serial
import threading
import sys
import time
import queue
import tkinter as tk
import ttkbootstrap as ttk
from ttkbootstrap.constants import *

# Global flag to signal the listener thread to stop
exit_flag = threading.Event()

# Queue for communication between GUI and serial thread
command_queue = queue.Queue()

# Global variable to track fault state
is_fault_active = False
oops_label = None

# Function to handle incoming serial data
def listen_serial(ser):
    global oops_label
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
    elif "LIVES" in data:
        parts = data.split(':')
        if len(parts) == 2 and parts[0].strip() == "LIVES":
            current_lives.set(parts[1].strip())
    elif "FAULT" in data:
        oops_label.set("OOPS!!!")  # Update using set() method of the StringVar
    elif "NO_DATA" in data:
        oops_label.set("")  # Clear any previous fault message
    else:
        # Clear fault state if no FAULT message is received
        if is_fault_active and oops_label:
            is_fault_active = False
            oops_label.set("")  # Update to empty string using set() method

# Function to send commands to the Teensy
def send_command(ser, command):
    try:
        # Put the command into the queue
        command_queue.put(command)
        print(f"Command '{command}' added to queue.")
    except Exception as e:
        print(f"Error sending command to queue: {e}")

# Function to periodically check and send commands from the queue
def check_queue_and_send(ser):
    try:
        while not exit_flag.is_set():
            try:
                command = command_queue.get(timeout=0.1)  # Timeout to prevent blocking
                ser.write(command.encode('utf-8') + b'\n')
                print(f"Command '{command}' sent to the Teensy.")
            except queue.Empty:
                pass
            time.sleep(0.1)
    except Exception as e:
        print(f"Error checking queue and sending command: {e}")

# Function to create the main GUI
def create_main_gui():
    global oops_label

    root = ttk.Window(themename="darkly")
    root.title("Game Status")

    global current_level
    global current_score
    global current_lives

    current_level = ttk.StringVar(value="1")
    current_score = ttk.StringVar(value="0")
    current_lives = ttk.StringVar(value="10")
    oops_label = ttk.StringVar(value="")

    style = ttk.Style()
    style.configure('TLabel', font=('Comic Sans MS', 24), foreground='orange')
    style.configure('Title.TLabel', font=('Comic Sans MS', 48, 'bold'), foreground='blue')

    frame = ttk.Frame(root, padding=20, style='TFrame')
    frame.pack(fill=tk.BOTH, expand=True)

    ttk.Label(frame, text="Hi Jelle, good luck!", style='Title.TLabel').pack(pady=20)

    ttk.Label(frame, text="Current Level:", style='TLabel').pack(pady=5)
    ttk.Label(frame, textvariable=current_level, style='TLabel').pack(pady=5)

    ttk.Label(frame, text="Current Score:", style='TLabel').pack(pady=5)
    ttk.Label(frame, textvariable=current_score, style='TLabel').pack(pady=5)

    ttk.Label(frame, text="Lives Left:", style='TLabel').pack(pady=5)
    ttk.Label(frame, textvariable=current_lives, style='TLabel').pack(pady=5)

    ttk.Label(frame, text="Mistake:", style='TLabel').pack(pady=5)
    ttk.Label(frame, textvariable=oops_label, style='TLabel').pack(pady=5)

    return root

# Function to create the secondary GUI window with buttons
def create_secondary_gui():
    second_window = tk.Toplevel()
    second_window.title("Level Selection")

    # Configure window dimensions and positioning
    screen_width = second_window.winfo_screenwidth()
    screen_height = second_window.winfo_screenheight()
    window_width = 300
    window_height = 200
    window_x = (screen_width - window_width) // 2
    window_y = (screen_height - window_height) // 2
    second_window.geometry(f"{window_width}x{window_height}+{window_x}+{window_y}")

    # Styling for buttons
    button_style = ttk.Style()
    button_style.configure('TButton', font=('Arial', 14))

    # Define command strings
    reset_command = "RESTART"
    level1_command = "GOTO LEVEL 1"
    level2_command = "GOTO LEVEL 2"
    level3_command = "GOTO LEVEL 3"
    level4_command = "GOTO LEVEL 4"
    turn_off_command = "TURN OFF"  # Add turn off command

    # Create buttons with callbacks
    ttk.Button(second_window, text="Restart", style='TButton', command=lambda: send_command(ser, reset_command)).pack(pady=10)
    ttk.Button(second_window, text="Level 1", style='TButton', command=lambda: send_command(ser, level1_command)).pack(pady=10)
    ttk.Button(second_window, text="Level 2", style='TButton', command=lambda: send_command(ser, level2_command)).pack(pady=10)
    ttk.Button(second_window, text="Level 3", style='TButton', command=lambda: send_command(ser, level3_command)).pack(pady=10)
    ttk.Button(second_window, text="Level 4", style='TButton', command=lambda: send_command(ser, level4_command)).pack(pady=10)
    ttk.Button(second_window, text="Turn Off", style='TButton', command=lambda: send_command(ser, turn_off_command)).pack(pady=10)  # Add turn off button

# Function to handle command line input
def command_line_input():
    while True:
        command = input("Enter command to send to Teensy (or 'exit' to quit): ")
        if command.lower() == 'exit':
            exit_flag.set()  # Signal the listener thread to stop
            break
        send_command(ser, command)

    print("Exiting command line input.")

if __name__ == "__main__":
    serial_port = 'COM4'  # 'COM4' for Windows PC, '/dev/ttyACM0' for Pi.
    # Specify your desired serial port here

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

    # Start thread to check the queue and send commands
    send_thread = threading.Thread(target=check_queue_and_send, args=(ser,), daemon=True)
    send_thread.start()

    # Create the main GUI
    root = create_main_gui()

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

    # Start command line input in a separate thread
    command_line_thread = threading.Thread(target=command_line_input, daemon=True)
    command_line_thread.start()

    # Create the secondary GUI window with buttons
    create_secondary_gui()

    # Run the main GUI in the main thread
    try:
        root.mainloop()
    except KeyboardInterrupt:
        exit_flag.set()
        listener_thread.join()
        send_thread.join()
        command_line_thread.join()

    print("Exiting program.")
    ser.close()
