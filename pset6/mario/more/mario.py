from cs50 import get_int

rows = 0
while ((rows < 1) | (rows > 8)):
    rows = get_int("Enter the number of rows: ")
for i in range(rows):
    print((" " * (rows - (i + 1))) + ("#" * (i + 1)), end="")
    print("  ", end="")
    print("#" * (i + 1))
