# This script is just result of thinking:
# During hiking to Monte Santo di Lussari there was
# a calvary with roman numbers and I was thinking 
# how to make a simple calculator from dec to rome
# (no validation, first valid solution)

LETTERS = ['I', 'V', 'X', 'L', 'C', 'D', 'M', '', '']

def insert_letter(number, letter_one, letter_five, letter_ten):
    if (number == 0): return ''

    if (number == 9): return letter_one + letter_ten

    if (number == 5): return letter_five

    result = ''

    if (number > 5):
        number %= 5
        result += letter_five

    if (number == 4): return result + letter_one + letter_five

    return result + letter_one * number

def dec_to_rome(n):
    result = ''

    # only valid numbers
    if (n > 3999): return result

    for i in range(3, -1, -1):
        position = 10 ** i
        base = n // position
        n %= position

        letter_one = LETTERS[i * 2]
        letter_five = LETTERS[i * 2 + 1]
        letter_ten = LETTERS[(i + 1) * 2]

        result += insert_letter(base, letter_one, letter_five, letter_ten)

    return result
