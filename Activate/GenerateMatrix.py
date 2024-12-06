import tkinter as tk

# Constants for button states
COLORS = ["gray", "red", "green"]

class TileApp:
    def __init__(self, root, rows=20, cols=20):
        self.root = root
        self.rows = rows
        self.cols = cols
        self.tiles = {}
        self.red_tiles = []
        self.green_tiles = []
        self.current_color_index = 1

        # Create a grid of buttons
        for i in range(rows):
            for j in range(cols):
                btn = tk.Button(root, bg="gray", width=3, height=1, command=lambda x=i, y=j: self.toggle_tile(x, y))
                btn.grid(row=i, column=j)
                self.tiles[(i, j)] = btn

        # Add an OK button
        ok_button = tk.Button(root, text="OK", command=self.generate_output)
        ok_button.grid(row=rows, columnspan=cols)

    def toggle_tile(self, x, y):
        """Toggle the color of a tile on click."""
        btn = self.tiles[(x, y)]
        self.current_color_index = (COLORS.index(btn["bg"]) + 1) % len(COLORS)
        new_color = COLORS[self.current_color_index]
        btn.configure(bg=new_color)

    def generate_output(self):
        """Generate and print the output for C++."""
        self.red_tiles = []
        self.green_tiles = []

        # Group tiles by color
        for (i, j), btn in self.tiles.items():
            if btn["bg"] == "red":
                self.red_tiles.append((i, j))
            elif btn["bg"] == "green":
                self.green_tiles.append((i, j))

        # Format the output for C++
        def format_cpp_output(tile_states):
            formatted_output = "{"
            grouped_tiles = []

            # Simulate grouping for the demo
            group_size = 3
            for i in range(0, len(tile_states), group_size):
                group = tile_states[i:i + group_size]
                group_str = "{" + ", ".join(f"{{{x}, {y}}}" for x, y in group) + "}"
                grouped_tiles.append(group_str)

            formatted_output += ", ".join(grouped_tiles)
            formatted_output += "}"
            return formatted_output

        red_cpp_output = f"std::vector<std::vector<std::pair<int, int>>> redTileStates = {format_cpp_output(self.red_tiles)};"
        green_cpp_output = f"std::vector<std::vector<std::pair<int, int>>> greenTileStates = {format_cpp_output(self.green_tiles)};"

        print(red_cpp_output)
        print(green_cpp_output)

# Run the application
root = tk.Tk()
app = TileApp(root)
root.mainloop()
