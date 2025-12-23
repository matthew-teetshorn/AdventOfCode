from collections import Counter


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

        decoded_strings = []
        line_str = " ".join(chunks[0:-1])

        if checksum == testcheck:
            dec_str_list = []
            total += int(sector_id)
            rotation = int(sector_id) % 26

            for c in line_str:
                tempc = ord(c) + rotation
                if tempc > ord("z"):
                    dec_str_list.append(chr(tempc - ord("z") + ord("`")))
                elif c == " ":
                    dec_str_list.append(" ")
                else:
                    dec_str_list.append(chr(tempc))

            decoded_strings.append("".join(list(dec_str_list)))
            if decoded_strings[-1] == target_room:
                target_sector_id = sector_id

    print(f"Total of valid room ids: {total}")
    print(f"Found target room: '{target_room}' at sector id: {target_sector_id}")


if __name__ == "__main__":
    main()
