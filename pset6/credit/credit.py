from cs50 import get_int

# gets user input and assing it to an str
number = get_int("Number: ")
card = str(number)

# defines variable to sum the numbers
numbersSum = 0

# sum the numbers acording to Luhn’s Algorithm
for i in range(len(card)):
    if (i % 2 != 0):
        num = int(card[::-1][i]) * 2
        for j in str(num):
            numbersSum += int(j)
    else:
        numbersSum += int(card[::-1][i])

# prints the results

if (numbersSum % 10 != 0):
    print('INVALID')

if ((len(card) == 15) & ((card[0:2] == '34') | (card[0:2] == '37'))):
    print('AMEX')

if ((len(card) == 16) & ((int(card[0:2]) >= 51) | (int(card[0:2]) <= 55))):
    print('MASTERCARD')

if (((len(card) == 16) | (len(card) == 13)) & (card[0] == '4')):
    print('VISA')

