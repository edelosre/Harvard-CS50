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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio = db.execute("""SELECT a.symbol, (SELECT sum(quantity) from tradehistory where userid = :userid and tradetype = 'BUY' and symbol = a.symbol) - IFNULL((SELECT sum(quantity) from tradehistory where userid = :userid and tradetype = 'SELL' and symbol = a.symbol),0) as quantity
    from tradehistory a
    where userid = :userid
    and (SELECT sum(quantity) from tradehistory where userid = :userid and tradetype = 'BUY' and symbol = a.symbol) - IFNULL((SELECT sum(quantity) from tradehistory where userid = :userid and tradetype = 'SELL' and symbol = a.symbol),0) > 0
    group by symbol""", userid=session.get("user_id"))

    currentcash = db.execute("SELECT cash FROM users WHERE id = :userid",
          userid=session.get("user_id"))

    currentcash = round(currentcash[0]["cash"], 2)

    #print(portfolio)
    portfolio_list = []
    stock_total = 0

    for i in range(len(portfolio)):
        symbol = portfolio[i]["symbol"]
        current_price = lookup(portfolio[i]["symbol"])["price"]
        quantity = portfolio[i]["quantity"]
        total_value = round(current_price*quantity, 2)


        stock_total = stock_total + total_value

        portfolio_list.append((symbol, usd(current_price), quantity, usd(total_value)))

    history = db.execute("""SELECT symbol, price, quantity, date, tradetype
    from tradehistory
    where userid = :userid""", userid=session.get("user_id"))

    for i in range(len(history)):
        history[i]["price"] = usd(history[i]["price"])

    return render_template("index.html", portfolio_list = portfolio_list, currentcash = usd(currentcash), stock_total = usd(stock_total), grand_total = usd(currentcash + stock_total), history = history)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        try:
            quote_details = lookup(request.form["symbol"])
            name = quote_details["name"]
            price = quote_details["price"]
            symbol = quote_details["symbol"]
        except:
            return apology("Invalid ticker symbol", 400)

        try:
            quantity = float(request.form["shares"])
        except:
            return apology("Please input a positive whole number", 400)

        if quantity < 1 or quantity % 1 != 0:
            return apology("Please input a positive whole number", 400)

        totalprice = price * quantity

        users_cash = db.execute("SELECT cash FROM users WHERE id = :userid",
                          userid=session.get("user_id"))

        if totalprice > users_cash[0]["cash"]:
            return apology("You do not have enough cash for that transaction.")

        db.execute("UPDATE users SET cash = cash - :totalprice WHERE id = :userid",
                          userid=session.get("user_id"), totalprice = totalprice)

        db.execute("INSERT INTO tradehistory(userid, symbol, price, quantity, tradetype) VALUES (:userid, :symbol, :price, :quantity, 'BUY')",
                          userid=session.get("user_id"), symbol = symbol, price = price, quantity = quantity)

        return index()




@app.route("/check", methods=["GET"], defaults={'username': None})
def check(username):
    """Return true if username available, else false, in JSON format"""
    # Query database for username
    username = request.args.get('username')
    rows = db.execute("SELECT * FROM users WHERE username = :username",
                      username=username)

    # Ensure username exists and password is correct
    if len(rows) != 1 and username is not None:
        return jsonify(True)
    else:
        return jsonify(False)

@app.route("/history", methods=["GET"])
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("""SELECT symbol, price, quantity, date, tradetype
    from tradehistory
    where userid = :userid""", userid=session.get("user_id"))

    for i in range(len(history)):
        history[i]["price"] = usd(history[i]["price"])

    return render_template("history.html", history = history)


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
    if request.method == "GET":
        return render_template("quote.html")
    else:
        try:
            quote_details = lookup(request.form["symbol"])
            quote_details["symbol"]
            return render_template("quoted.html", name = quote_details["name"], price = usd(quote_details["price"]), symbol = quote_details["symbol"])
        except:
            return apology("Invalid ticker symbol", 400)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form["username"]
        password = request.form["password"]
        confirmation = request.form["confirmation"]

        user_exists = db.execute("SELECT username FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if username == "" or len(user_exists) != 0:
            return apology("That username is not available.")

        if password == "" or confirmation == "" or password != confirmation:
            return apology("Passwords do not match.")
        else:
            hashed_password = generate_password_hash(password)

        db.execute("INSERT INTO users (username, hash) VALUES(:username, :passhash)",
                          username=request.form.get("username"),
                          passhash=hashed_password)

        return render_template("register.html", registered = 1)

    if request.method == "GET":
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    portfolio = db.execute("""SELECT a.symbol, (SELECT sum(quantity) from tradehistory where userid = :userid and tradetype = 'BUY' and symbol = a.symbol) - IFNULL((SELECT sum(quantity) from tradehistory where userid = :userid and tradetype = 'SELL' and symbol = a.symbol),0) as quantity
    from tradehistory a
    where userid = :userid
    and (SELECT sum(quantity) from tradehistory where userid = :userid and tradetype = 'BUY' and symbol = a.symbol) - IFNULL((SELECT sum(quantity) from tradehistory where userid = :userid and tradetype = 'SELL' and symbol = a.symbol),0) > 0
    group by symbol""", userid=session.get("user_id"))

    if request.method == "GET":

        available_stocks = []

        for i in range(len(portfolio)):
            stock = portfolio[i]["symbol"]
            available_stocks.append(stock)

        return render_template("sell.html", available_stocks = available_stocks)

    else:
        quantity_owned = 0
        quote_details = lookup(request.form["symbol"])
        quantity = int(request.form["shares"])

        name = quote_details["name"]
        price = quote_details["price"]
        symbol = quote_details["symbol"]

        for i in range(len(portfolio)):
            if portfolio[i]["symbol"] == symbol:
                quantity_owned = portfolio[i]["quantity"]

        totalprice = price * quantity

        #print(quantity_owned)

        if quantity_owned < quantity:
            return apology("You do not have enough shares for that transaction.")
        else:
            db.execute("UPDATE users SET cash = cash + :totalprice WHERE id = :userid",
                        userid=session.get("user_id"), totalprice = totalprice)

            db.execute("INSERT INTO tradehistory(userid, symbol, price, quantity, tradetype) VALUES (:userid, :symbol, :price, :quantity, 'SELL')",
                        userid=session.get("user_id"), symbol = symbol, price = price, quantity = quantity)

        return index()

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Buy shares of stock"""
    if request.method == "GET":

        currentcash = db.execute("SELECT cash FROM users WHERE id = :userid",
                  userid=session.get("user_id"))

        currentcash = usd(round(currentcash[0]["cash"], 2))

        return render_template("cash.html", currentcash = currentcash)
    else:
        cash = int(request.form["cash"])

        db.execute("UPDATE users SET cash = cash + :cash WHERE id = :userid",
                          userid=session.get("user_id"), cash = cash)

        return render_template("cash.html", deposited = 1)




def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
