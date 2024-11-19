import tkinter as tk

# Create the main window
root = tk.Tk()
root.title("12x12 Matrix")

# Define the size of the matrix
ROWS = 12
COLS = 12

# List to store the indices of the clicked buttons
clicked_buttons = []

# Define a callback function to handle button clicks
def button_click(row, col):
    # Toggle the button color between red and default
    if (row, col) in clicked_buttons:
        buttons[row][col].config(bg="SystemButtonFace")
        clicked_buttons.remove((row, col))
    else:
        buttons[row][col].config(bg="red")
        clicked_buttons.append((row, col))
    # Print the list of all clicked button indices
    print_clicked_buttons()

# Function to print the clicked button indices
def print_clicked_buttons():
    for btn in clicked_buttons:
        print(f"[{btn[0]}, {btn[1]}]", end=' ')
    print()

# Create a 2D list to store button references
buttons = [[None for _ in range(COLS)] for _ in range(ROWS)]

# Create and place buttons in the grid
for row in range(ROWS):
    for col in range(COLS):
        # Create a button
        button = tk.Button(root, text=f"{row},{col}", width=5, height=2,
                           command=lambda r=row, c=col: button_click(r, c))
        # Place the button in the grid
        button.grid(row=row, column=col)
        # Store the button reference
        buttons[row][col] = button

# Start the Tkinter event loop
root.mainloop()
