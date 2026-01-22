def main():
    MAX_INT = 4294967295
    with open("ex20.input", "r") as f:
        input = sorted([[int(item) for item in line.strip().split("-")] for line in f])

    new_ranges = []
    min_start, max_end = input[0]
    # Condense adjacent and overlapping ranges into disjoint ranges
    for i in range(1, len(input)):
        curr_start, curr_end = input[i]

        if curr_end < max_end:  # Range is fully contained
            continue
        elif curr_start <= max_end + 1 and curr_end > max_end:  # Ranges overlap / join
            max_end = curr_end
            continue
        elif curr_start > max_end:  # Gap between ranges
            new_ranges.append([min_start, max_end])
            min_start = curr_start
            max_end = curr_end

    # Add the last found range
    new_ranges.append([min_start, max_end])

    total_ips = 0
    first_open = True
    for i in range(len(new_ranges) - 1):
        gap_start = new_ranges[i][1] + 1
        gap_end = new_ranges[i + 1][0]
        if first_open:
            print(f"First available IP: {gap_start}")
            first_open = False
        total_ips += gap_end - gap_start

    # If final range ends less than MAX_INT
    total_ips += MAX_INT - new_ranges[-1][1]

    print(f"Total open ips: {total_ips}")


if __name__ == "__main__":
    main()
