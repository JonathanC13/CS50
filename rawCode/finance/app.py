import os
import time
import decimal

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


BUY = 1
SELL = 2

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    total = 0
    listStocks = []

    rows = db.execute("""SELECT symbol, SUM(shares) as shares
                    FROM transactions
                    WHERE userID = ?
                    GROUP BY symbol
                        HAVING SUM(shares) > 0
                    ORDER BY SUM(shares) desc""", session["user_id"])   # get all current stock holdings in the user's portfolio

    for stock in rows:
        dictStock = {}

        dictLookUp = lookup(stock["symbol"].upper())
        symbol = dictLookUp["symbol"]
        name = dictLookUp["name"]

        dictStock["symbol"] = symbol
        dictStock["name"] = name

        shares = stock["shares"]
        dictStock["shares"] = shares

        price = dictLookUp["price"]
        dictStock["price"] = price

        stockTotal = price * shares
        total += stockTotal
        dictStock["total"] = stockTotal

        listStocks.append(dictStock)

    rows = db.execute("SELECT cash FROM users where id = ?", session["user_id"])

    if len(rows) != 1:
            return apology("user error", 409)
    else:
        cash = rows[0]["cash"]
        total += cash

    return render_template("portfolio.html", stocks=listStocks, cash=cash, total=total)

@app.route("/addCash", methods=["GET", "POST"])
@login_required
def addCash():
    if request.method == "POST":
        addCash = request.form.get("addCash")

        try:
            addCash = float(addCash)
        except ValueError:
            return apology("must provide a numerical value", 400)

        d = decimal.Decimal(str(addCash))

        if addCash <= 0:
            return apology("must provide a non-zero and positive value", 400)
        elif d.as_tuple().exponent < -2:
            return apology("must provide a numerical value with max 2 digits after the decimal", 400)

        retNumRowsUpdated = db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", addCash, session["user_id"])

        if retNumRowsUpdated != 1:
            return apology("update cash error", 409)

    return redirect("/")

def buyTransact(symbol):
    dictLookUp = lookup(symbol)
    if not symbol or dictLookUp is None:
        return apology("must provide symbol or symbol does not exist", 400)

    shares = request.form.get("shares")

    try:
        shares = int(shares)
    except (ValueError):
        return apology("must provide an integer value for shares", 400)

    if shares <= 0:
        return apology("must provide a non-zero and positive integer value for shares", 400)

    price = dictLookUp["price"]
    totalCheckOutCost = round(float(price * shares), 2)

    rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    if len(rows) != 1:
        return apology("user error", 409)

    cash = round(float(rows[0]["cash"]), 2)
    if cash < totalCheckOutCost:
        return apology("you can not afford that many shares", 400)
    else:
        timeStamp = time.time()

        retTransactionID = db.execute("""INSERT INTO transactions(userID, transactionDate, symbol, shares, price, total, transactionTypeID)
                                        VALUES (?, ?, ?, ?, ?, ?, ?)""", session["user_id"], timeStamp, symbol, shares, price, totalCheckOutCost, BUY)

        if retTransactionID is None:
            return apology("transaction error", 409)

        newCash = round(float(cash - totalCheckOutCost), 2)
        retNumRowsUpdated = db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, session["user_id"])

        if retNumRowsUpdated != 1:
            return apology("update cash error", 409)



    return redirect("/")

def sellTransact(symbol):
    dictLookUp = lookup(symbol)
    if not symbol or dictLookUp is None:
        return apology("must provide symbol or symbol does not exist", 400)

    shares = request.form.get("shares") # input is string
    try:
        shares = int(shares)    # this works due to input being a string. This would not work if input was float.
    except ValueError:
        return apology("must provide an integer value for shares", 400)

    if shares <= 0:
        return apology("must provide a non-zero and positive integer value for shares", 400)

    rows = db.execute("""SELECT symbol, SUM(shares) as shares
                    FROM transactions
                    WHERE userID = ? AND UPPER(symbol) = ?
                    GROUP BY symbol
                    HAVING SUM(shares) > 0
                    ORDER BY SUM(shares) desc""", session["user_id"], symbol)

    if len(rows) != 1:
        return apology("must provide a stock you own", 400)

    if rows[0]["shares"] < shares:
        return apology("cannot sell more shares than you own", 400)
    else:
        timeStamp = time.time()

        price = lookup(symbol)["price"]
        total = round(float(price * shares), 2)

        retTransactionID = db.execute("""INSERT INTO transactions(userID, transactionDate, symbol, shares, price, total, transactionTypeID)
                                    VALUES (?, ?, ?, ?, ?, ?, ?)""", session["user_id"], timeStamp, symbol, -1 * shares, price, total, SELL)

        if retTransactionID is None:
            return apology("transaction error", 409)

        rows = db.execute("SELECT cash FROM users where id = ?", session["user_id"])

        if len(rows) != 1:
            return apology("users error", 409)

        newCash = round(float(rows[0]["cash"] + total), 2)
        retNumRowsUpdated = db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, session["user_id"])

        if retNumRowsUpdated != 1:
            return apology("update cash error", 409)

        return redirect("/")


@app.route("/transact", methods=["GET", "POST"])
@login_required
def transact():

    """
    <input type="submit" name="submit_button" value="Do Something">
    <input type="submit" name="submit_button" value="Do Something Else">

    def contact():
    if request.method == 'POST':
        if request.form['submit_button'] == 'Do Something':
            pass # do something
        elif request.form['submit_button'] == 'Do Something Else':
            pass # do something else
        else:
            pass # unknown
    elif request.method == 'GET':
        return render_template('contact.html', form=form)

    """

    if request.method == "POST":

        if request.form["transact"] == "BUY":
            return buyTransact(request.form.get("symbol"))
        elif request.form["transact"] == "SELL":
            return sellTransact(request.form.get("symbol"))
        else:
            return apology("error", 409)

    return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        return buyTransact(symbol)


    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    rows = db.execute("""SELECT DATETIME(transactionDate, 'unixepoch', 'localtime') as 'transactionDate', symbol, shares, price, transactionTypeDesc as 'type'
                      FROM transactions a
                        INNER JOIN transactionType b on b.transactionTypeID = a.transactionTypeID
                      WHERE userID = ?
                      ORDER BY transactionDate desc""", session["user_id"])

    return render_template("history.html", transactions=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        symbol = request.form.get("symbol").upper()

        if not symbol:
            return apology("must provide a sybmol", 400)

        dictLookup = lookup(symbol)

        if dictLookup is None:
            return apology("symbol does not exist", 400)
        else:
            return render_template("quoted.html", name=dictLookup["name"], price=dictLookup["price"], symbol=dictLookup["symbol"])

    return render_template("quote.html")

@app.route("/changePW", methods=["GET", "POST"])
def changePW():
    if request.method == "POST":
        rows = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])

        if len(rows) != 1:
            return apology("users error", 409)

        if not check_password_hash(rows[0]["hash"], request.form.get("password_old")):
            return apology("old password is not correct", 400)

        newPW = request.form.get("password_new")

        hasLetter = False
        hasNumber = False
        for letter in newPW:
            if hasLetter and hasNumber:
                break
            else:
                if letter.isalpha():
                    hasLetter = True

                if letter.isdigit():
                    hasNumber = True

        if not hasLetter or not hasNumber:
            return apology("must provide password that contains letters AND numbers", 400)


        newPWConfirm = request.form.get("confirmation_new")

        if newPW != newPWConfirm:
            return apology("new password and confirm does not match", 400)

        password = generate_password_hash(newPW)

        db.execute("UPDATE users SET hash = ? WHERE id = ?", password, session["user_id"])

        return redirect("/")

    else:
        return render_template("changePW.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if not username or len(rows) > 0:
            return apology("must provide username or username already exists", 400)

        password = request.form.get("password")

        hasLetter = False
        hasNumber = False
        for letter in password:
            if hasLetter and hasNumber:
                break
            else:
                if letter.isalpha():
                    hasLetter = True

                if letter.isdigit():
                    hasNumber = True

        if not hasLetter or not hasNumber:
            return apology("must provide password that contains letters AND numbers", 400)

        confirmation = request.form.get("confirmation")
        if not password or not confirmation or password != confirmation:
            return apology("must provide password or password does not match", 400)

        password = generate_password_hash(password)

        db.execute("INSERT INTO users(username, hash) VALUES(?, ?)", username, password)

        return redirect("/")

    return render_template("register.html")




@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        return sellTransact(symbol)

    else:

        rows = db.execute("""SELECT symbol from transactions
                          WHERE userID = ?
                          GROUP BY symbol
                            HAVING SUM(shares) > 0
                          ORDER BY SUM(shares) desc""", session["user_id"])
        return render_template("sell.html", ownedShares=rows)



#check50 cs50/problems/2023/x/finance

#CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT NOT NULL, hash TEXT NOT NULL, cash NUMERIC NOT NULL DEFAULT 10000.00);
#CREATE TABLE sqlite_sequence(name,seq);
#CREATE UNIQUE INDEX username ON users (username);

"""
Apple
password123

CREATE TABLE transactionType (
    transactionTypeID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    transactionTypeDesc TEXT NOT NULL
);

INSERT INTO transactionType(transactionTypeDesc)
    VALUES ("BUY"), ("SELL");

CREATE TABLE transactions (
    transactionID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    userID INTEGER NOT NULL,
    transactionDate INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    price NUMERIC NOT NULL,
    total NUMERIC NOT NULL,
    transactionTypeID INTEGER NOT NULL,
    FOREIGN KEY(userID) REFERENCES users(id),
    FOREIGN KEY(transactionTypeID) REFERENCES transactionType(transactionTypeID)
);



----------

SELECT symbol, SUM(shares) as shares
    FROM transactions
    WHERE userID = 1
    GROUP BY symbol
        HAVING SUM(shares) > 0
    ORDER BY SUM(shares) desc;

SELECT symbol, shares
                      FROM transactions
                      WHERE userID = 1 AND symbol = "TD"
                      GROUP BY symbol
                        HAVING SUM(shares) > 0
                      ORDER BY SUM(shares) desc
"""
