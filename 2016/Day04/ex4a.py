from collections import Counter


def main():
    with open("ex4.input", "r") as f:
        lines = [line.rstrip() for line in f]

    total = 0
    for line in lines:
        chunks = line.split("-")
        line_str = "".join(chunks[0:-1])
        letter_counts = sorted(
            Counter(line_str).items(), key=lambda item: (-item[1], item[0])
        )
        sector_id, checksum = chunks[-1].split("[")
        checksum = checksum.rstrip("]")
        testcheck = "".join([item[0] for item in letter_counts[0:5]])

        if checksum == testcheck:
            total += int(sector_id)

    print(f"Total of valid room ids: {total}")


if __name__ == "__main__":
    main()
