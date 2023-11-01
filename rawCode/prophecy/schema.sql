CREATE TABLE students (
    student_id INTEGER NOT NULL,
    student_name TEXT,
    PRIMARY KEY(student_id)
);

CREATE TABLE houses (
    house_id INTEGER NOT NULL,
    house_name TEXT,
    house_head TEXT,
    PRIMARY KEY(house_id)
);

CREATE TABLE rel_student_house (
    id INTEGER NOT NULL,
    student_id INTEGER NOT NULL,
    house_id INTEGER NOT NULL,
    PRIMARY KEY(id),
    FOREIGN KEY(student_id) REFERENCES students(student_id),
    FOREIGN KEY(house_id) REFERENCES houses(house_id)
);
