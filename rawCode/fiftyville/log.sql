-- Keep a log of any SQL queries you execute as you solve the mystery.

--Who the thief is,
--What city the thief escaped to, and
--Who the thief’s accomplice is who helped them escape
--All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.

/*
CREATE TABLE crime_scene_reports (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    street TEXT,
    description TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE interviews (
    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE atm_transactions (
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE bank_accounts (
    account_number INTEGER,
    person_id INTEGER,
    creation_year INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);
CREATE TABLE airports (
    id INTEGER,
    abbreviation TEXT,
    full_name TEXT,
    city TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE flights (
    id INTEGER,
    origin_airport_id INTEGER,
    destination_airport_id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
    FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
);
CREATE TABLE passengers (
    flight_id INTEGER,
    passport_number INTEGER,
    seat TEXT,
    FOREIGN KEY(flight_id) REFERENCES flights(id)
);
CREATE TABLE phone_calls (
    id INTEGER,
    caller TEXT,
    receiver TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    duration INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE people (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE bakery_security_logs (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
);

*/


-- get the initial report of the crime
SELECT *
    FROM  crime_scene_reports
WHERE year = 2021 AND
    month = 7 AND
    day = 28 AND
    street = 'Humphrey Street';
/*
2 records
Desc:
1. Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
    Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts
    mentions the bakery.
2. Littering took place at 16:36. No known witnesses.

#1 is the crime we are interested in.
*/

-- Let's look at the interviews
SELECT *
    FROM  interviews
WHERE year = 2021 AND
    month = 7 AND
    day = 28;
/* Interviews that mention the crime
1. Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
    If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that
    time frame.

2. I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
    I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

3. As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call,
    I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
    The thief then asked the person on the other end of the phone to purchase the flight ticket.
*/

-- The crime occurred at the bakery, according to #1 interview,
--    look at the bakery_security_logs for July 28, 2021 anything from 10:15 to 10:25
SELECT *
    FROM bakery_security_logs
WHERE year = 2021 AND
    month = 7 AND
    day = 28 AND
    hour = 10 AND
    minute BETWEEN 15 AND 25;
/*
+-----+------+-------+-----+------+--------+----------+---------------+
| id  | year | month | day | hour | minute | activity | license_plate |
+-----+------+-------+-----+------+--------+----------+---------------+
| 260 | 2021 | 7     | 28  | 10   | 16     | exit     | 5P2BI95       |
| 261 | 2021 | 7     | 28  | 10   | 18     | exit     | 94KL13X       |
| 262 | 2021 | 7     | 28  | 10   | 18     | exit     | 6P58WS2       |
| 263 | 2021 | 7     | 28  | 10   | 19     | exit     | 4328GD8       |
| 264 | 2021 | 7     | 28  | 10   | 20     | exit     | G412CB7       |
| 265 | 2021 | 7     | 28  | 10   | 21     | exit     | L93JTIZ       |
| 266 | 2021 | 7     | 28  | 10   | 23     | exit     | 322W7JE       |
| 267 | 2021 | 7     | 28  | 10   | 23     | exit     | 0NTHK55       |
+-----+------+-------+-----+------+--------+----------+---------------+
*/

-- Based on interview #2,
--  I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
--      I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

SELECT c.name, c.phone_number, c.passport_number, c.license_plate
    FROM atm_transactions a
        JOIN bank_accounts b on b.account_number = a.account_number
        JOIN people c on c.id = b.person_id
WHERE a.year = 2021 AND
    a.month = 7 AND
    a.day = 28 AND
    a.atm_location LIKE 'Leggett%' AND
    a.transaction_type = 'withdraw';
/*
+---------+----------------+-----------------+---------------+
|  name   |  phone_number  | passport_number | license_plate |
+---------+----------------+-----------------+---------------+
| Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
| Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
| Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3       |
| Kenny   | (826) 555-1652 | 9878712108      | 30G67EN       |
| Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
| Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
| Taylor  | (286) 555-6063 | 1988161715      | 1106N58       |
| Benista | (338) 555-6650 | 9586786673      | 8X428L0       |
+---------+----------------+-----------------+---------------+
*/

-- Based on interview #3
/*
As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call,
    I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
    The thief then asked the person on the other end of the phone to purchase the flight ticket.

call duration < 1 min
earliest flight out of Fiftyville on July 29
asked someone else to buy the ticket
*/
-- get all calls lasting less than 1 minute that was made after 10:15 am
SELECT *
    FROM phone_calls
WHERE year = 2021 AND
    month = 7 AND
    day = 28 AND
    duration < 60;
/*
+-----+----------------+----------------+------+-------+-----+----------+
| id  |     caller     |    receiver    | year | month | day | duration |
+-----+----------------+----------------+------+-------+-----+----------+
| 221 | (130) 555-0289 | (996) 555-8899 | 2021 | 7     | 28  | 51       |
| 224 | (499) 555-9472 | (892) 555-8872 | 2021 | 7     | 28  | 36       |
| 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       |
| 251 | (499) 555-9472 | (717) 555-1342 | 2021 | 7     | 28  | 50       |
| 254 | (286) 555-6063 | (676) 555-6554 | 2021 | 7     | 28  | 43       |
| 255 | (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49       |
| 261 | (031) 555-6622 | (910) 555-3251 | 2021 | 7     | 28  | 38       |
| 279 | (826) 555-1652 | (066) 555-9701 | 2021 | 7     | 28  | 55       |
| 281 | (338) 555-6650 | (704) 555-2131 | 2021 | 7     | 28  | 54       |
+-----+----------------+----------------+------+-------+-----+----------+
*/

-- before continuing with the flight, cross reference the other 2 interviews information
-- Of the license plates that left within 10 min of the theft, get the names, phone, and passport of people that made withdraws
--  before the crime
SELECT c.name, c.phone_number, c.passport_number, c.license_plate
    FROM atm_transactions a
        JOIN bank_accounts b on b.account_number = a.account_number
        JOIN people c on c.id = b.person_id
WHERE a.year = 2021 AND
    a.month = 7 AND
    a.day = 28 AND
    a.atm_location LIKE 'Leggett%' AND
    a.transaction_type = 'withdraw' AND
    c.license_plate IN (SELECT license_plate
        FROM bakery_security_logs
    WHERE year = 2021 AND
        month = 7 AND
        day = 28 AND
        hour = 10 AND
        minute BETWEEN 15 AND 25);
/*
+-------+----------------+-----------------+---------------+
| name  |  phone_number  | passport_number | license_plate |
+-------+----------------+-----------------+---------------+
| Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
| Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
| Iman  | (829) 555-5269 | 7049073643      | L93JTIZ       |
| Luca  | (389) 555-5198 | 8496433585      | 4328GD8       |
+-------+----------------+-----------------+---------------+
*/

-- cross ref result with caller after theft
SELECT c.name, c.phone_number, c.passport_number, c.license_plate
    FROM atm_transactions a
        JOIN bank_accounts b on b.account_number = a.account_number
        JOIN people c on c.id = b.person_id
WHERE a.year = 2021 AND
    a.month = 7 AND
    a.day = 28 AND
    a.atm_location LIKE 'Leggett%' AND
    a.transaction_type = 'withdraw' AND
    c.phone_number IN (SELECT caller
        FROM phone_calls
    WHERE year = 2021 AND
        month = 7 AND
        day = 28 AND
        duration < 60) AND
    c.license_plate IN (SELECT license_plate
        FROM bakery_security_logs
    WHERE year = 2021 AND
        month = 7 AND
        day = 28 AND
        hour = 10 AND
        minute BETWEEN 15 AND 25);
/*
+-------+----------------+-----------------+---------------+
| name  |  phone_number  | passport_number | license_plate |
+-------+----------------+-----------------+---------------+
| Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
| Diana | (770) 555-1861 | 3592750733      | 322W7JE       |
+-------+----------------+-----------------+---------------+
*/

-- now check the people leaving Fiftyville on the first flight on July 29
SELECT a.id, a.hour, a.minute, (SELECT city from airports where id = destination_airport_id) as 'dest'
-- select *
    FROM flights a
WHERE a.year = 2021 AND
    a.month = 7 AND
    a.day = 29 AND
    a.origin_airport_id IN (select id from airports where city = 'Fiftyville')
ORDER BY a.hour, a.minute asc;

/*
DESTINATION IS LaGuardia Airpor
+----+------+--------+---------------+
| id | hour | minute |     dest      |
+----+------+--------+---------------+
| 36 | 8    | 20     | New York City |
| 43 | 9    | 30     | Chicago       |
| 23 | 12   | 15     | San Francisco |
| 53 | 15   | 20     | Tokyo         |
| 18 | 16   | 0      | Boston        |
+----+------+--------+---------------+

look at the passengers on flight id 36
*/
SELECT *
    FROM passengers
WHERE flight_id = (SELECT a.id
    FROM flights a
        JOIN airports b on b.id = a.origin_airport_id
    WHERE a.year = 2021 AND
        a.month = 7 AND
        a.day = 29 AND
        b.full_name LIKE 'Fifty%'
    ORDER BY a.hour, a.minute asc LIMIT 1);
/*
+-----------+-----------------+------+
| flight_id | passport_number | seat |
+-----------+-----------------+------+
| 36        | 7214083635      | 2A   |
| 36        | 1695452385      | 3B   |
| 36        | 5773159633      | 4A   |
| 36        | 1540955065      | 5C   |
| 36        | 8294398571      | 6C   |
| 36        | 1988161715      | 6D   |
| 36        | 9878712108      | 7A   |
| 36        | 8496433585      | 7B   |
+-----------+-----------------+------+
*/

-- Cross ref all the interview information
SELECT c.name, c.phone_number, c.passport_number, c.license_plate
    FROM atm_transactions a
        JOIN bank_accounts b on b.account_number = a.account_number
        JOIN people c on c.id = b.person_id
WHERE a.year = 2021 AND
    a.month = 7 AND
    a.day = 28 AND
    a.atm_location LIKE 'Leggett%' AND
    a.transaction_type = 'withdraw' AND
    c.phone_number IN (SELECT caller
        FROM phone_calls
    WHERE year = 2021 AND
        month = 7 AND
        day = 28 AND
        duration < 60) AND
    c.license_plate IN (SELECT license_plate
        FROM bakery_security_logs
    WHERE year = 2021 AND
        month = 7 AND
        day = 28 AND
        hour = 10 AND
        minute BETWEEN 15 AND 25) AND
    c.passport_number IN (
        SELECT passport_number
            FROM passengers
        WHERE flight_id = (SELECT a.id
            FROM flights a
                JOIN airports b on b.id = a.origin_airport_id
            WHERE a.year = 2021 AND
                a.month = 7 AND
                a.day = 29 AND
                b.full_name LIKE 'Fifty%'
            ORDER BY a.hour, a.minute asc LIMIT 1)
    );
/*
THEIF IS
+-------+----------------+-----------------+---------------+
| name  |  phone_number  | passport_number | license_plate |
+-------+----------------+-----------------+---------------+
| Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
+-------+----------------+-----------------+---------------+
*/


-- accomplice
select *
    from people
    where phone_number in (
        SELECT receiver
            FROM phone_calls
        WHERE year = 2021 AND
            month = 7 AND
            day = 28 AND
            duration < 60 AND
            caller = '(367) 555-5533'
    );
/*
ACCOMPLICE IS Robin
+--------+-------+----------------+-----------------+---------------+
|   id   | name  |  phone_number  | passport_number | license_plate |
+--------+-------+----------------+-----------------+---------------+
| 864400 | Robin | (375) 555-8161 | NULL            | 4V16VO0       |
+--------+-------+----------------+-----------------+---------------+
*/

-- check50 cs50/problems/2023/x/fiftyville