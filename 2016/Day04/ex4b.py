from collections import Counter
import string


def make_translation(n: int):
    alpha_lower = string.ascii_lowercase
    shift = n % len(alpha_lower)
    return str.maketrans(alpha_lower, alpha_lower[shift:] + alpha_lower[:shift])


def main():
    with open("ex4.input", "r") as f:
        lines = [line.rstrip() for line in f]

    total = 0
    target_room = "northpole object storage"
    target_sector_id = 0

    for line in lines:
        chunks = line.split("-")
        line_str = "".join(chunks[0:-1])
        letter_counts = sorted(
            Counter(line_str).items(), key=lambda item: (-item[1], item[0])
        )
        sector_id, checksum = chunks[-1].split("[")
        checksum = checksum.rstrip("]")
        testcheck = "".join([item[0] for item in letter_counts[0:5]])

        line_str = " ".join(chunks[0:-1])

        if checksum == testcheck:
            total += int(sector_id)

            decoded_string = line_str.translate(make_translation(int(sector_id)))

            if decoded_string == target_room:
                target_sector_id = sector_id

    print(f"Total of valid room ids: {total}")
    print(f"Found target room: '{target_room}' at sector id: {target_sector_id}")


if __name__ == "__main__":
    main()
