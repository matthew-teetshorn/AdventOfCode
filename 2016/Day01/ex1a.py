class Coord:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y


def main():
    with open("ex1.input", "r") as f:
        content = f.read()

    instrs = [x.strip() for x in content.split(",")]

    directions = ("up", "right", "down", "left")
    dir_index = 0
    coord = Coord(0, 0)

    for instr in instrs:
        if instr[0] == "L":
            dir_index = dir_index - 1 if dir_index > 0 else 3
        if instr[0] == "R":
            dir_index = dir_index + 1 if dir_index < 3 else 0

        steps = int(instr[1:])
        match directions[dir_index]:
            case "up":
                coord.y += steps
            case "right":
                coord.x += steps
            case "down":
                coord.y -= steps
            case "left":
                coord.x -= steps

    distance = abs(coord.x) + abs(coord.y)
    print(f"Distance to end: {distance}")


if __name__ == "__main__":
    main()
