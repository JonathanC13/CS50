from cs50 import SQL;   # cs50.readthedocs.io/libraries/cs50/python/#cs50.SQL
import csv;

with open("students.csv") as file:
    reader = csv.DictReader(file)

    db = SQL("sqlite:///roster_refact.db")

    for row in reader:
        #print(row)
        db.execute("BEGIN TRANSACTION")
        # if house exists get the house_id, else insert into house table
        house_rows = db.execute("SELECT house_id FROM houses WHERE house_name = ? AND house_head = ?", row["house"], row["head"])
        house_id = None
        if house_rows:
            house_id = house_rows[0]["house_id"]
        else:
            house_id = db.execute("INSERT INTO houses (house_name, house_head) VALUES (?, ?)", row["house"], row["head"])

        # insert into student table
        student_id = db.execute("INSERT INTO students (student_id, student_name) VALUES (?, ?)", row["id"], row["student_name"])

        # insert into relation table, rel_student_house, if both ids were created
        if student_id is not None and house_id is not None:
            rel_id = db.execute("INSERT INTO rel_student_house (student_id, house_id) VALUES (?, ?)", student_id, house_id)
        else:
            db.execute("ROLLBACK")

        db.execute("COMMIT")


# select * from students a JOIN rel_student_house b on b.student_id = a.student_id JOIN houses c ON c.house_id = b.house_id;