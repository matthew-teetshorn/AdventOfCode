def main():
    num_elves = 3014387
    elves_left = 3014387
    elves = [1 for _ in range(num_elves)]

    winner = 0
    start = 0

    # the "split" (location of first unlucky elf) advances by 1.5 elves on each iteration
    # with fractional elves not being possible we advance by 1 then 2 or 2 then 1 elves
    # to find the next unlucky elf depending on whether the initial "split" is odd/even
    unlucky_elf = elves_left // 2
    split_odd_even = elves_left % 2

    if split_odd_even:
        unlucky_step, next_unlucky_step = 2, 1
    else:
        unlucky_step, next_unlucky_step = 1, 2

    while True:
        elves[start] = elves[unlucky_elf]
        elves[unlucky_elf] = 0
        elves_left -= 1

        if elves_left == 1:
            winner = start + 1
            break

        # find next non-zero start elf
        start += 1
        if start == num_elves:
            start = 0

        while not elves[start]:
            start += 1
            if start >= num_elves:
                start = 0

        # find next non-zero unlucky_elf
        for _ in range(unlucky_step):
            unlucky_elf += 1
            if unlucky_elf >= num_elves:
                unlucky_elf = 0
            while not elves[unlucky_elf]:
                unlucky_elf += 1
                if unlucky_elf >= num_elves:
                    unlucky_elf = 0
        unlucky_step, next_unlucky_step = next_unlucky_step, unlucky_step

    print(winner)


if __name__ == "__main__":
    main()
