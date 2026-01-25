class Block:
    def __init__(self, x: int, y: int, size: int, used: int, avail: int, percent: int):
        self.x = x
        self.y = y
        self.size = size
        self.used = used
        self.avail = avail
        self.percent = percent

    def fits(self, other: "Block") -> bool:
        if other.used <= self.avail:
            return True
        return False

    def __repr__(self):
        return f"Data Block: at:({self.x}, {self.y}) S:{self.size} U:{self.used} A:{self.avail} {self.percent}%"


def main():
    grid = []
    with open("ex22.input", "r") as f:
        for line in f:
            x, y, s, u, a, p = map(int, line.strip().split())
            grid.append(Block(x, y, s, u, a, p))

    viables = []
    for a in range(len(grid)):
        for b in range(len(grid)):
            if a == b:
                continue
            if grid[a].used != 0 and grid[b].fits(grid[a]):
                viables.append((a, b))

    print(len(viables))


if __name__ == "__main__":
    main()
