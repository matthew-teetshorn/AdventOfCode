class Coord:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __eq__(self, other):
        if self.x == other.x and self.y == other.y:
            return True
        else:
            return False


def main():
    with open("ex1.input", "r") as f:
        content = f.read()

    instrs = [x.strip() for x in content.split(",")]

    directions = ("up", "right", "down", "left")
    dir_index = 0
    coord = Coord(0, 0)
    coords = []
    coords.append(Coord(0, 0))

    done = False
    for instr in instrs:
        if done:
            break
        if instr[0] == "L":
            dir_index = dir_index - 1 if dir_index > 0 else 3
        if instr[0] == "R":
            dir_index = dir_index + 1 if dir_index < 3 else 0

        steps = int(instr[1:])
        # print(f"Rotate: {instr[0]} Dir: {directions[dir_index]} Steps: {steps}")
        match directions[dir_index]:
            case "up":
                velocity = (0, 1)
            case "right":
                velocity = (1, 0)
            case "down":
                velocity = (0, -1)
            case "left":
                velocity = (-1, 0)
        for _ in range(steps):
            coord.x += velocity[0]
            coord.y += velocity[1]

            if Coord(coord.x, coord.y) in coords:
                done = True
                break
            else:
                coords.append(Coord(coord.x, coord.y))

    print(f"EB HQ at: ({coord.x}, {coord.y})")
    print(f"Distance from start: {abs(coord.x) + abs(coord.y)}")


if __name__ == "__main__":
    main()
