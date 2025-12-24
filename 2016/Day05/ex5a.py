import hashlib


def main():
    with open("ex5.input", "r") as f:
        door_id = f.read().strip()

    password = ""
    salt = 0
    while len(password) < 8:
        to_hash = door_id + str(salt)
        md5_hash = hashlib.md5(to_hash.encode("utf-8")).hexdigest()

        if md5_hash[0:5] == "00000":
            print(md5_hash)
            password += md5_hash[5]

        salt += 1

    print(password)


if __name__ == "__main__":
    main()
