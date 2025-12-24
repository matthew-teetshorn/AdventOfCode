import hashlib
import string
import random


def main():
    with open("ex5.input", "r") as f:
        door_id = f.read().strip()

    password = "--------"
    num_known = 0
    indices_known = []
    salt = 0
    print_delay = 0
    while num_known < 8:
        to_hash = door_id + str(salt)
        md5_hash = hashlib.md5(to_hash.encode("utf-8")).hexdigest()
        salt += 1

        # Dramatic Hollywood style decrypting animation 😄
        print_delay += 1
        if print_delay == 50000:
            print_delay = 0
            print("\r", end="", flush=True)
            for i in range(len(password)):
                if i in indices_known:
                    print(password[i], end="", flush=True)
                else:
                    print(random.choice(string.ascii_letters), end="", flush=True)

        if md5_hash[0:5] == "00000":
            if not md5_hash[5].isnumeric():
                continue
            index = int(md5_hash[5])
            if index >= 0 and index < 8 and index not in indices_known:
                num_known += 1
                indices_known.append(index)
                temp = password[0:index] + md5_hash[6] + password[index + 1 :]
                password = temp

    print(f"\r{password}")


if __name__ == "__main__":
    main()
