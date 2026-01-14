import hashlib

DIMENSION = 4


class Room:
    def __init__(self, x: int, y: int, path: str):
        self.x = x
        self.y = y
        self.path = path

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Room):
            return NotImplemented

        if self.x == other.x and self.y == other.y:
            return True
        return False


def main():
    with open("ex17.input", "r") as f:
        passcode = f.read().strip()

    start = Room(0, 0, "")
    end = Room(DIMENSION - 1, DIMENSION - 1, "")
    toVisit = [start]
    open_syms = {"b", "c", "d", "e", "f"}

    current: Room = Room(0, 0, "")
    while len(toVisit):
        current = toVisit.pop(0)
        print(f"At: {current.x}, {current.y} --- path: {current.path}")
        if current == end:
            break

        key = passcode + current.path
        up, down, left, right = (
            True if c in open_syms else False
            for c in hashlib.md5(key.encode()).hexdigest()[0:4]
        )
        if up and current.y > 0:
            toVisit.append(Room(current.x, current.y - 1, current.path + "U"))
        if down and current.y < DIMENSION - 1:
            toVisit.append(Room(current.x, current.y + 1, current.path + "D"))
        if left and current.x > 0:
            toVisit.append(Room(current.x - 1, current.y, current.path + "L"))
        if right and current.x < DIMENSION - 1:
            toVisit.append(Room(current.x + 1, current.y, current.path + "R"))

    print(f"Path to end: {current.path}")


if __name__ == "__main__":
    main()
