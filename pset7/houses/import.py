from sys import argv, exit
import csv
import cs50

if len(argv) != 2:
    print("You should pass an argument.")
    exit(1)

open(f"students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

with open(argv[1], "r") as student:

    reader = csv.DictReader(student)

    for row in reader:
        name = row["name"].split(" ")
        first = name[0]
        last = None
        middle = None

        if len(name) == 3:
            middle = name[1]
            last = name[2]

        else:
            last = name[1]

        print("Inserting in db: ", first, middle, last, row["house"], row["birth"])

        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
            first, middle, last, row["house"], row["birth"])
