class Disc:
    def __init__(self, num_pos: int, start_pos: int):
        self.num_pos = num_pos
        self.start_pos = start_pos

    def get_pos(self, time: int) -> int:
        return (self.start_pos + time) % self.num_pos


def main():
    with open("ex15.input", "r") as f:
        input = [line.strip().split() for line in f]

    discs = []
    for line in input:
        num_pos = int(line[3])
        start_pos = int(line[-1])
        discs.append(Disc(num_pos, start_pos))

    # Define this to do part 2
    part2 = True
    if part2:
        discs.append(Disc(11, 0))

    button_time = -1
    ball_through = False
    while not ball_through:
        button_time += 1
        ball_through = True
        dt = 0
        for disc in discs:
            dt += 1
            if disc.get_pos(button_time + dt) != 0:
                ball_through = False
                break

    print(f"Ball through at press time: {button_time}")


if __name__ == "__main__":
    main()
