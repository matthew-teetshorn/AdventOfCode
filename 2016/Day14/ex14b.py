import hashlib
from functools import lru_cache


# Room for many optimizations in this code :P
@lru_cache()
def gen_hash(key: str) -> str:
    hash = hashlib.md5(key.encode()).hexdigest()
    for _ in range(2016):
        hash = hashlib.md5(hash.encode()).hexdigest()
    return hash


def scan_ahead(char: str, salt: str, start: int) -> bool:
    for index in range(start, start + 1000):
        hash = gen_hash(salt + str(index))
        for a, b, c, d, e in zip(hash, hash[1:], hash[2:], hash[3:], hash[4:]):
            if char == a == b == c == d == e:
                return True

    return False


def main():
    with open("ex14.input", "r") as f:
        input = f.read().strip()

    key_count = 0
    index = 0
    while key_count < 64:
        index += 1
        curr_key = input + str(index)
        hash = gen_hash(curr_key)
        for x, y, z in zip(hash, hash[1:], hash[2:]):
            if x == y == z:
                if scan_ahead(x, input, index + 1):
                    key_count += 1
                break

    print(f"Index of final key found: {index}")


if __name__ == "__main__":
    main()
