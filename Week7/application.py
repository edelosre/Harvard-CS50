import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    try:
        with open('survey.csv', 'a', newline='') as csvfile:
            fieldnames = ['Gender', 'Height', "Weight", "MetricSystem"]
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

            writer.writerow({'Gender': request.form["gender"], 'Height': request.form["height"], "Weight": request.form["weight"], "MetricSystem": request.form["metric"]})
        return get_sheet()
    except:
        return render_template("error.html", message="There was an error processing your request.")


@app.route("/sheet", methods=["GET"])
def get_sheet():
    try:
        csvfile = open('survey.csv')
        surveydata = list(csv.reader(csvfile))

        for i in range(0, len(surveydata)):
            print(surveydata[i])
            print(i)
        return render_template("sheet.html", surveydata=surveydata)

    except:
        return render_template("error.html", message="There was an error processing your request.")
