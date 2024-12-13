import tkinter as tk
from tkinter import ttk

# Constants for tile states
COLORS = ["gray", "red", "green"]

class TileApp:
    def __init__(self, root, rows=10, cols=10, frames=2):
        self.root = root
        self.rows = rows
        self.cols = cols
        self.frames = frames
        self.frame_data = [{} for _ in range(frames)]  # Store tile states for each frame
        self.selection_start = None  # To store the first right-click position

        # Create a notebook (tab control)
        self.notebook = ttk.Notebook(root)
        self.notebook.pack(expand=True, fill="both")

        # Create tabs for each frame
        for frame in range(frames):
            tab = ttk.Frame(self.notebook)
            self.notebook.add(tab, text=f"Frame {frame + 1}")
            self.create_tab_grid(tab, frame)

        # Add an OK button to save/export all frames
        ok_button = tk.Button(root, text="OK", command=self.generate_output)
        ok_button.pack()

    def create_tab_grid(self, tab, frame):
        """Create a grid of tiles for a specific frame."""
        self.frame_data[frame]["tiles"] = {}
        for i in range(self.rows):
            for j in range(self.cols):
                btn = tk.Button(tab, bg="gray", width=3, height=1)
                btn.grid(row=i, column=j)
                self.frame_data[frame]["tiles"][(i, j)] = btn

                # Bind left and right click events
                btn.bind("<Button-1>", lambda event, x=i, y=j, f=frame: self.toggle_tile(x, y, f))
                btn.bind("<Button-3>", lambda event, x=i, y=j, f=frame: self.select_area(x, y, f))

    def toggle_tile(self, x, y, frame):
        """Toggle the color of a single tile on left click."""
        btn = self.frame_data[frame]["tiles"][(x, y)]
        current_color = btn["bg"]
        new_color = COLORS[(COLORS.index(current_color) + 1) % len(COLORS)]
        btn.config(bg=new_color)

    def select_area(self, x, y, frame):
        """Handle right-click area selection."""
        if not self.selection_start:
            # Store the starting point of the selection
            self.selection_start = (x, y)
        else:
            # Determine the area to select
            start_x, start_y = self.selection_start
            end_x, end_y = x, y

            # Normalize selection bounds
            top_left_x, bottom_right_x = sorted([start_x, end_x])
            top_left_y, bottom_right_y = sorted([start_y, end_y])

            # Get the color of the top-left tile in the selection
            top_left_tile = self.frame_data[frame]["tiles"][(top_left_x, top_left_y)]
            current_color = top_left_tile["bg"]

            # Determine the new color
            new_color = COLORS[(COLORS.index(current_color) + 1) % len(COLORS)]

            # Change the color of all tiles in the selected area
            for i in range(top_left_x, bottom_right_x + 1):
                for j in range(top_left_y, bottom_right_y + 1):
                    btn = self.frame_data[frame]["tiles"][(i, j)]
                    btn.config(bg=new_color)

            # Reset the selection start
            self.selection_start = None

    def generate_output(self):
        """Generate and print the output in a C++-compatible vector format."""
        red_tile_states = []
        green_tile_states = []

        for frame_data in self.frame_data:
            red_tiles = []
            green_tiles = []
            for (i, j), btn in frame_data["tiles"].items():
                color = btn["bg"]
                if color == "red":
                    red_tiles.append((i, j))
                elif color == "green":
                    green_tiles.append((i, j))
            red_tile_states.append(red_tiles)
            green_tile_states.append(green_tiles)

        # Format the output for C++
        def format_cpp_vector(vector_data):
            formatted = []
            for tiles in vector_data:
                formatted.append("{" + ", ".join(f"{{{i}, {j}}}" for i, j in tiles) + "}")
            return "{" + ", ".join(formatted) + "}"

        # Generate final output
        red_output = f"std::vector<std::vector<std::pair<int, int>>> redTileStates = {format_cpp_vector(red_tile_states)};"
        green_output = f"std::vector<std::vector<std::pair<int, int>>> greenTileStates = {format_cpp_vector(green_tile_states)};"

        print(red_output)
        print(green_output)


# Main application
if __name__ == "__main__":
    root = tk.Tk()
    app = TileApp(root)
    root.mainloop()
