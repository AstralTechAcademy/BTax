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
    yTicker = yfinance.Ticker(ticker + "-EUR")
    fiatConverRate = 1.0
    if not yTicker.history_metadata:
        yTicker = yfinance.Ticker(ticker + "-USD")
        eurusdTicker = yfinance.Ticker("EURUSD=X")
        fiatConverRate = eurusdTicker.history().tail(1)['Close'].iloc[0]
        if not yTicker.history_metadata:
            return "{\"ticker\":\"" + ticker + "\",\"error\": \"Ticker not valid or data not available\"}"

    dtstr = request.args.get('datetime')
    datetime = dtime.datetime.strptime(dtstr,'%Y-%m-%d %H:%M:%S')
    nextHour = datetime + timedelta(days=1)

    hist = yTicker.history(start=datetime.strftime("%Y-%m-%d"), end=nextHour.strftime("%Y-%m-%d"), interval="1d")    
    for index, row in hist.iterrows():
        d = dtime.datetime.strptime(str(index),'%Y-%m-%d %H:%M:%S%z')
        if d.timestamp() <= datetime.timestamp():
            price = row.Open / fiatConverRate
            data = "{\"ticker\":\"" + ticker + "\",\"datetime\":\"" + str(d) + "\",\"price\":" + str(price) +  ",\"price_no_converted\":" +  str(row.Open)  + "}"
    
    if not data:
        data = "{\"ticker\":\"" + ticker + "\",\"error\": \"Ticker not valid or data not available\"}"
    
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