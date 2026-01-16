def main():
    num_elves = 3014387
    elves = [1 for _ in range(num_elves)]

    curr_elf = 0
    winner = 0
    while True:
        next_elf = (curr_elf + 1) % num_elves
        while elves[next_elf] == 0:
            next_elf = (next_elf + 1) % num_elves

        elves[curr_elf] += elves[next_elf]
        elves[next_elf] = 0

        if elves[curr_elf] == num_elves:
            winner = curr_elf + 1
            break

        curr_elf = (curr_elf + 1) % num_elves

        while elves[curr_elf] == 0:
            curr_elf = (curr_elf + 1) % num_elves

    print(winner)


if __name__ == "__main__":
    main()
