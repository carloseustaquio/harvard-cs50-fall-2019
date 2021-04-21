import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # !! Update this route, use new user_stocks table to get a better result !!
    stocks = db.execute("SELECT * FROM user_stocks WHERE user_id = ? ORDER BY id DESC", session["user_id"])
    total_amount = 0
    for stock in stocks:
        stock_details = lookup(stock["stock_symbol"])
        stock["name"] = stock_details["name"]
        stock["current_price"] = stock_details["price"]
        stock["total"] = round(float (stock_details["price"] * stock["shares"]), 2)
        total_amount += stock["total"]

    print(stocks)
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    user_cash = user_cash[0]["cash"]
    total_cash = total_amount + user_cash
    return render_template("index.html", usd=usd, stocks=stocks, cash=user_cash, total_cash=total_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        print("shares", shares)

        # checks if user sent fields
        if not symbol or not shares:
            return render_template("buy.html", apology="You must send the symbol and the number of shares.")
        quote = lookup(symbol)
        print("quote", quote)

        # check if the quote exists
        if quote == None:
            return render_template("buy.html", apology="Sorry, quote not found!")

        purchaseValue = (quote["price"] * float (shares))
        queryUserCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        userCash = round(float (queryUserCash[0]["cash"]), 2)

        print('1', purchaseValue)
        print('2', userCash)

        # checks if the user can afford the purchase
        if purchaseValue > userCash:
            return render_template("buy.html", apology="You don't have enough money!")

        # concrete the purchase
        else:
            # saves purchase in transactions table
            db.execute("INSERT INTO transactions (user_id, stock_symbol, shares, price, operation, created_at) VALUES (?, ?, ?, ?, ?, CURRENT_TIMESTAMP)", session["user_id"], quote["symbol"], shares, purchaseValue, 'buy')
            db.execute("UPDATE users SET cash = ? WHERE id = ?", (userCash - purchaseValue), session["user_id"])

            # query to see if user already have the stock in the user_stocks table
            user_stock_info = db.execute("SELECT id, shares FROM user_stocks WHERE user_id = ? AND stock_symbol = ?", session["user_id"], quote["symbol"])

            if len(user_stock_info) > 0:
                # if user already has the stock, update it number
                updated_shares = (int (user_stock_info[0]["shares"]) + int (shares))
                db.execute("UPDATE user_stocks SET shares = ? WHERE id = ?", updated_shares, user_stock_info[0]["id"])

            else:
                # otherwise, create a new row that tracks user/stock
                db.execute("INSERT INTO user_stocks (user_id, stock_symbol, shares) VALUES (?, ?, ?)", session["user_id"], quote["symbol"], shares)

            return render_template("buy.html", success="Bought!")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():

    user_history = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY created_at DESC", session["user_id"])

    return render_template("history.html", usd=usd, history=user_history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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
        quote = lookup(request.form.get("symbol"))
        if quote == None:
            return render_template("quote.html", apology="Stock not found!")
        else:
            print(quote)
            return render_template("quoted.html", quote=quote, usd=usd)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        password = request.form.get("password")
        username = request.form.get("username")

        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif password != request.form.get("password_confirm"):
            return apology("The passwords should match", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(rows) > 0:
            return apology("The user already exists")
        else:
            hashed_password = generate_password_hash(password)
            new_user_id = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hashed_password)
            # start session
            session["user_id"] = new_user_id
            return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    user_stocks = db.execute("SELECT * FROM user_stocks WHERE user_id = ?", session["user_id"],)

    if request.method == "POST":
        stock_symbol = request.form.get("stock")
        shares_to_sell = request.form.get("shares")

        user_stock = db.execute("SELECT * FROM user_stocks WHERE user_id = ? AND stock_symbol = ?", session["user_id"], stock_symbol)
        user_stock = user_stock[0]

        # checks if the user has the stock
        if user_stock == {}:
            return render_template("sell.html", stocks=user_stocks, apology="You don't have this stock!")

        # checks if the user has enough shares to sell
        if user_stock["shares"] < int (shares_to_sell):
            return render_template("sell.html", stocks=user_stock, apology=f"Not enough shares to complete this sell! You have { user_stock['shares'] } shares!")

        quote = lookup(stock_symbol)

        # register in transactions table
        db.execute("INSERT INTO transactions (user_id, stock_symbol, shares, price, operation, created_at) VALUES (?, ?, ?, ?, ?, CURRENT_TIMESTAMP)", session["user_id"], stock_symbol, shares_to_sell, quote["price"], 'sell')

        # subtract from amount from users_table
        updated_shares =  user_stock["shares"] - int (shares_to_sell)
        if updated_shares == 0:
            db.execute("DELETE FROM user_stocks WHERE id = ?", user_stock["id"])
        else:
            db.execute("UPDATE user_stocks SET shares = ? WHERE id = ?", updated_shares, user_stock["id"])

        sell_total = (quote["price"] * int(shares_to_sell))

        # update users amount of money
        user_cash = db.execute("SELECT cash FROM users WHERE id = ? ", session["user_id"])
        updated_cash = user_cash[0]["cash"] + sell_total
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, session["user_id"])

        # render with success message
        return render_template("sell.html", stocks=user_stocks, success="Sold!")
    else:
        return render_template("sell.html", stocks=user_stocks)

@app.route("/add_money", methods=["GET", "POST"])
@login_required
def add_money():
    if request.method == "POST":
        money = float (request.form.get("money"))
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", money, session["user_id"])
        db.execute("INSERT INTO transactions (user_id, stock_symbol, shares, price, operation, created_at) VALUES (?, ?, ?, ?, ?, CURRENT_TIMESTAMP)",
            session["user_id"], " - ", " - ", money, 'add money')
        print(money)
        return redirect("/history")
        # return render_template("add_money.html", success="Done!")
    else:
        return render_template("add_money.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
