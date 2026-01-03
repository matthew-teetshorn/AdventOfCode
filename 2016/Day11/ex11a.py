import copy
import time

NUM_FLOORS = 4


def get_hash(elevator: int, state: list[list[int]] | None) -> str | None:
    if state is not None:
        return str(elevator) + str(
            [(len(state[i]), sum(x < 0 for x in state[i])) for i in range(4)]
        )
    return None


# Cannot leave generators alone with unmatching cards
def is_valid(floor: list[int]) -> bool:
    has_generator = sum(item < 0 for item in floor)
    unsafe_card = sum(item > 0 and -item not in floor for item in floor)

    return not (has_generator and unsafe_card)


# Move the elevator with the specified items in the specified direction
# Add the new state to the provided queue if the state is valid
def move_elevator(
    state: list[list[int]],
    states_seen: set[str],
    queue: list[tuple[list[list[int]], int, int, str]],
    curr_floor: int,
    direction: int,
    moves: int,
    index1: int,
    index2: int | None = None,
):
    new_state = copy.deepcopy(state)

    next_floor = curr_floor + direction
    if next_floor >= NUM_FLOORS or next_floor < 0:
        return

    # Pop item2 first as index2 is always > index1
    item2 = new_state[curr_floor].pop(index2) if index2 is not None else None
    item1 = new_state[curr_floor].pop(index1)

    new_state[next_floor].append(item1)
    if item2 is not None:
        new_state[next_floor].append(item2)

    for floor in new_state:
        if not is_valid(floor):
            return

    next_hash = get_hash(next_floor, new_state)
    if next_hash is not None and next_hash not in states_seen:
        queue.append((copy.deepcopy(new_state), next_floor, moves + 1, next_hash))
        queue.sort(key=lambda x: -x[2])


def main():
    # Start state for part 1
    start_state = [[-1, -2, 2, -3, -4, 4, -5, 5], [1, 3], [], []]
    # Start state for part 2
    # start_state = [[-1, -2, 2, -3, -4, 4, -5, 5, -6, 6, -7, 7], [1, 3], [], []]
    end_hash = get_hash(3, [[], [], [], sum(start_state, [])])
    start_hash = get_hash(0, start_state)
    queue = []
    states_seen = set()
    queue.append((start_state, 0, 0, start_hash))

    start_time = time.perf_counter()
    while True:
        curr_state, curr_floor, curr_moves, curr_hash = queue.pop()

        if curr_hash not in states_seen:
            states_seen.add(curr_hash)

            if curr_hash == end_hash:
                # Done
                end_time = time.perf_counter()
                print(f"Num moves: {curr_moves}")
                print(f"Elapsed time: {end_time - start_time:.4f} seconds")
                break

            # Move the elevator up and down with 2 and 1 items respectively
            length = len(curr_state[curr_floor])
            for i in range(length):
                for j in range(i + 1, length):
                    move_elevator(
                        curr_state, states_seen, queue, curr_floor, 1, curr_moves, i, j
                    )
                    move_elevator(
                        curr_state, states_seen, queue, curr_floor, -1, curr_moves, i, j
                    )

                move_elevator(
                    curr_state, states_seen, queue, curr_floor, 1, curr_moves, i
                )
                move_elevator(
                    curr_state, states_seen, queue, curr_floor, -1, curr_moves, i
                )


if __name__ == "__main__":
    main()
