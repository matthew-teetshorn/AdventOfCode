class Elf:
    def __init__(self, index: int):
        self.index = index
        self.num_presents = 1

    def steal(self, other):
        self.num_presents += other.num_presents
        other.num_presents = 0

    def __repr__(self) -> str:
        return f"Elf({self.index}): num_presents: {self.num_presents}"


def main():
    num_elves = 3014387
    num_elves = 100000
    elves = [Elf(index) for index in range(1, num_elves + 1)]

    c_index = 0
    while len(elves) != 1:
        n_index = c_index + 1 if c_index != len(elves) - 1 else 0
        curr_elf = elves[c_index]
        next_elf = elves[n_index]
        curr_elf.steal(next_elf)
        elves.remove(next_elf)
        c_index = (elves.index(curr_elf) + 1) % len(elves)

    print(elves)


if __name__ == "__main__":
    main()
