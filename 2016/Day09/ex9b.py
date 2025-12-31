import re


# Decompress the first found marker
# return: length of leading non-marker string and remaining string
# This runs pretty slowly but does produce the right answer without running out of memory
def process_chunk(string: str) -> tuple[int, str]:
    working_list = re.split(r"\(([0-9]+)x([0-9]+)\)", string, maxsplit=1)
    new_string = ""
    pretext = working_list[0]
    length = len(pretext)

    if len(working_list) == 1:
        return (length, new_string)

    num_chars = int(working_list[1])
    num_times = int(working_list[2])
    posttext = working_list[3]
    for _ in range(num_times):
        new_string += posttext[:num_chars]
    new_string += posttext[num_chars:]

    return (length, new_string)


def main():
    with open("ex9.input", "r") as f:
        input = f.read().strip()

    next_string = input
    total_length = 0
    while True:
        length, next_string = process_chunk(next_string)
        total_length += length
        if next_string == "":
            break

    print(f"Total length: {total_length}")


if __name__ == "__main__":
    main()
