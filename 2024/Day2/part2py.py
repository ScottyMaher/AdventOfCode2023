asc = "ASC"
desc = "DESC"
tolerance = 3


def get_lines(test=False):
    with open(("test" if test else "input") + ".txt") as file:
        for ln in file:
            yield ln.strip()


def check_numbers(type, first, second):
    """checks if number ascend or descend and are within a tolerance range"""
    global asc, desc
    type = type or (asc if first < second else desc)
    return (type, adheres_to_rules(type, first, second))


def adheres_to_rules(type, first, second):
    """ensures tolerance and asc / desc rules are followed"""
    global asc, desc, tolerance
    return not (
        (type == asc and (second <= first or second > first + tolerance))
        or (type == desc and (second >= first or second < first - tolerance))
    )


def is_safe(numbers, problem_dampener_disabled=True, problem_dampener_used=False):
    """
    recursive
    checks if a set of numbers is safe according to the puzzle rules
    """
    type = None
    for ii, num in enumerate(numbers):
        if ii >= len(numbers) - 1:
            print("Safe numbers:", numbers)
            return True
        (type, success) = check_numbers(type, num, numbers[ii + 1])
        if not success:
            break

    return (
        False
        if problem_dampener_disabled or problem_dampener_used
        else check_safe_with_elements_removed(numbers)
    )


def check_safe_with_elements_removed(numbers):
    """tries to find a safe solution with one of the numbers missing"""
    for ii in range(len(numbers)):
        if is_safe(
            numbers[:ii] + numbers[ii + 1 :],
            problem_dampener_disabled=False,
            problem_dampener_used=True,
        ):
            print("Safe numbers:", numbers)
            return True
    return False


def solution_1():
    return sum(
        [1 if is_safe(list(map(int, line.split()))) else 0 for line in get_lines()]
    )


def solution_2():
    return sum(
        [
            (
                1
                if is_safe(
                    list(map(int, line.split())), problem_dampener_disabled=False
                )
                else 0
            )
            for line in get_lines()
        ]
    )


print("1:", solution_1())
print("2:", solution_2())