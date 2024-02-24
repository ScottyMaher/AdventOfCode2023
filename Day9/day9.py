import re

with open("./day9.txt", 'r') as file:
    puzzle_input = file.read().split("\n")


def part1(puzzle_input):
    accumulator = 0
    for row in puzzle_input:
        row = [int(number) for number in row.split()]
        row_ends = []
        not_final_row = True
        while not_final_row:
            new_row = []
            prev_num = None
            not_final_row = False
            for num in row:
                if prev_num is not None:
                    new_row.append(num - prev_num)
                    if num - prev_num != 0:
                        not_final_row = True
                prev_num = num
            row_ends.append(prev_num)
            row = new_row

        accumulator += sum(row_ends)

    return accumulator


print("Part 1 answer: ", part1(puzzle_input))