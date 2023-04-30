from flask import Flask, request
import mysql.connector
import yfinance as yfinance
import json
import datetime as dtime
from datetime import timedelta
from mysql.connector import errorcode
app = Flask(__name__)

@app.route('/')
def hello_geek():
    return '<h1>Hello from Flask & Docker</h2>'

@app.route('/bye')
def bye_geek():
    return '<h1>Bye from Flask & Docker</h2>'


@app.route('/yfinance/getPrice')
def getPrice():
    data = ""
    ticker = request.args.get('ticker')
    dtstr = request.args.get('datetime')
    datetime = dtime.datetime.strptime(dtstr,'%Y-%m-%d %H:%M:%S')
    nextHour = datetime + timedelta(days=1)
    print(str(datetime) + " " + str(nextHour))
    hist = yfinance.Ticker(ticker).history(start=datetime.strftime("%Y-%m-%d"), end=nextHour.strftime("%Y-%m-%d"), interval="1h")
    
    for index, row in hist.iterrows():
        d = dtime.datetime.strptime(str(index),'%Y-%m-%d %H:%M:%S%z')
        if d.timestamp() <= datetime.timestamp():
            data = ticker + " " + str(d) + " " + str(row.Open) + "<br>"
    return data

@app.route('/connect')
def conn_geek():
    try:
        cnx = mysql.connector.connect(user='user1', password='user1',
                                        host='couple-db',
                                        database='couple')
        username = request.args.get('username')
        query = ("SELECT uuid, username FROM Users "
         "WHERE username='" + username + "'")

        cursor = cnx.cursor()
        cursor.execute(query)

        for (uuid, username) in cursor:
            return "UUID: " + str(uuid) + " Username: " + username
        cursor.close()
        return "Error"
                            
    except mysql.connector.Error as err:
        if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
            return "Something is wrong with your user name or password"
        elif err.errno == errorcode.ER_BAD_DB_ERROR:
            return "Database does not exist"
        else:
            return err

if __name__ == "__main__":
    app.run(debug=True)