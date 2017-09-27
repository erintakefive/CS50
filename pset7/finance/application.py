"""
This is the program that included most of the actually logic and programming work outside
of html templating with Jinja. 
"""

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    # get names of stocks held by user
    stocks = db.execute("SELECT stock, symbol, SUM(shares) FROM transactions WHERE user_id = :id GROUP BY symbol", id=session["user_id"])

    # prep variables
    x = len(stocks)
    current_price = []
    value = []
    cum_value = 0

    for i in range(0, x):
        current_price.append(lookup(stocks[i]["symbol"]))

    # calculate share values
    for i in range(0, x):
        value.append(current_price[i]["price"] * stocks[i]["SUM(shares)"])

    # find current cash balance
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

    # calculate cumulative values of portfolio
    for i in range(0, x):
        cum_value += value[i]

    # calculate total value of portfolio and cash
    total = cash[0]["cash"] + cum_value

    return render_template("index.html", stocks=stocks, price=current_price, value=value, cash=cash[0]["cash"], total=total, x=x)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    if request.method == "POST":

        # ensure symbol was submitted
        if not request.form.get("buy"):
            return apology("must provide stock symbol")

        # ensure shares amount was submitted
        if not request.form.get("shares"):
            return apology("must provide amount of shares")

        # lookup value of stock symbol submitted
        stock = lookup(request.form.get("buy"))

        # ensure stock exists
        if stock is None:
            return apology("symbol was not valid")

        # query database for cash
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        # make shares variable a float
        shares = int(request.form.get("shares"))

        # ensure shares is positive number
        if shares < 1:
            return apology("must input valid number of shares")

        # find total price of transaction
        total_price = stock["price"] * shares

        # ensure user has enough cash to make purchase
        if cash[0]["cash"] < stock["price"] * shares:
            return apology("not enough cash to make purchase")

        # query database for username
        username = db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])

        # db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=passhash)
        # complete purchase, add stock to portfolio
        db.execute("INSERT INTO transactions (user_id, username, stock, shares, share_price, total_price, symbol) VALUES (:user_id, :username, :stock, :shares, :share_price, :total_price, :symbol)",
            user_id=session["user_id"], username=username[0]["username"], stock=stock["name"], shares=shares, share_price=stock["price"], total_price=total_price, symbol=stock["symbol"])

        # update cash
        cash_change = cash[0]["cash"] - total_price
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash_change, id=session["user_id"])

        # redirect user to home page
        return redirect(url_for("index"))

    else:

        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # get history of user transactions
    history = db.execute("SELECT stock, shares, share_price, date, total_price, symbol FROM transactions WHERE user_id = :id ORDER BY transaction_id ASC", id=session["user_id"])

    # prep variables
    x = len(history)

    return render_template("history.html", history=history, x=x)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if reached via form submission, grab stock quote
    if request.method == "POST":

        # ensure symbol was submitted
        if not request.form.get("stock"):
            return apology("must provide stock symbol")

        # lookup value of stock symbol submitted
        quote = lookup(request.form.get("stock"))

        # ensure quote is valid
        if quote is None:
            return apology("symbol was not valid")

        session["quote"] = quote

        # go to quoted page
        return redirect(url_for("quoted"))

    # if reached via GET, render page
    else:

        return render_template("quote.html")

@app.route("/quoted")
@login_required
def quoted():
    """Display quote."""

    quote = session.get("quote", None)

    return render_template("quoted.html", quote=quote)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure username contains alphanumeric characters only
        if request.form.get("username").isalnum() != True:
            return apology("username must contain only alphanumeric characters")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # ensure password was confirmed
        elif not request.form.get("confirm"):
            return apology("must provide password and confirm")

        # ensure password matches confirmation
        if request.form.get("password") != request.form.get("confirm"):
            return apology("password did not match confirmation")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username does not exist
        if len(rows) > 0:
            return apology("username already exists")

        # if it's available, hash the password
        else:
            passhash = pwd_context.hash(request.form.get("password"))
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=passhash)

        # log new user in
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST":

        # ensure symbol was submitted
        if not request.form.get("sell"):
            return apology("must provide stock symbol")

        # ensure shares amount was submitted
        if not request.form.get("shares"):
            return apology("must provide amount of shares to sell")

        # lookup value of stock symbol submitted
        stock = lookup(request.form.get("sell"))

        # ensure stock exists
        if stock is None:
            return apology("symbol was not valid")

        # query database for cash
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        # query databse for username
        username = db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])

        # query database for shares of stock
        share_total = db.execute("SELECT SUM(shares) FROM transactions WHERE user_id = :id AND symbol = :symbol", id=session["user_id"], symbol=stock["symbol"])

        # make shares variable
        shares = int(request.form.get("shares"))

        # if shares is a negative number, return apology
        if shares < 1:
            return apology("number of shares must be at least one")

        # ensure number of shares requested lower than in portfolio
        if shares > share_total[0]["SUM(shares)"]:
            return apology("cannot sell more shares than in portfolio")

        # get total money of transaction
        total_price = shares * stock["price"]

        # make transaction of negative shares amount
        db.execute("INSERT INTO transactions (user_id, username, stock, shares, share_price, total_price, symbol) VALUES (:user_id, :username, :stock, :shares, :share_price, :total_price, :symbol)",
            user_id=session["user_id"], username=username[0]["username"], stock=stock["name"], shares=-shares, share_price=stock["price"], total_price=-total_price, symbol=stock["symbol"])

        # update cash
        cash_change = cash[0]["cash"] + total_price
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash_change, id=session["user_id"])

        # redirect user to home page
        return redirect(url_for("index"))

    else:

        return render_template("sell.html")

@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """Allow user to change their password"""

    if request.method == "POST":

        # ensure correct term submitted
        if not request.form.get("old"):
            return apology("must input current password")

        # ensure correct term submitted
        if not request.form.get("new"):
            return apology("must input new password")

        # ensure correct term submitted
        if not request.form.get("confirm"):
            return apology("must confirm new password")

        # ensure new password is confirmed
        if request.form.get("new") != request.form.get("confirm"):
            return apology("new password does not match confirmed password")

        # get hash of old password
        oldhash = db.execute("SELECT hash FROM users WHERE id = :id", id=session["user_id"])

        # ensure correct term submitted
        if not pwd_context.verify(request.form.get("old"), oldhash[0]["hash"]):
            return apology("current password incorrect")

        # get hash of new password
        newhash = pwd_context.hash(request.form.get("new"))

        # update password in database
        db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=newhash, id=session["user_id"])

        # redirect user to password success page
        return redirect(url_for("passwordsuccess"))

    else:

        return render_template("password.html")

@app.route("/passwordsuccess", methods=["GET", "POST"])
@login_required
def passwordsuccess():
    """Let user know their password has been changed"""

    if request.method == "POST":

        # redirect user to home page
        return redirect(url_for("index"))

    else:

        return render_template("passwordsuccess.html")
