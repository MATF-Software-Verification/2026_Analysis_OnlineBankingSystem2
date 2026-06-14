class MockWindow:
    def __init__(self):
        self.chars = {}
        self.inputs = []
        self.timeout_value = None

    def addch(self, y, x, char):
        self.chars[(x, y)] = char

    def addstr(self, y, x, string):
        pass

    def inch(self, y, x):
        # prvo polje (3,3) je zauzeto — za testove koji testiraju else granu update_fruit_position
        if (x, y) == (3, 3):
            return ord("#")
        return ord(" ")

    def getch(self):
        return self.inputs.pop(0) if self.inputs else -1

    def border(self, *args):
        pass

    def timeout(self, value):
        self.timeout_value = value
