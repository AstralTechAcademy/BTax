from pycoingecko import CoinGeckoAPI
from datetime import datetime
import mysql.connector
import time

def main():
    getData()


def getData():

    # Conexión a la base de datos
    mydb = mysql.connector.connect(
        host="34.175.153.199",
        user="zendaya",
        password="20greatestShowman22",
        database="marketcap"
        )

    print("Connected: " + str(mydb.is_connected()))
    
    # Initialize the client
    cg = CoinGeckoAPI()

    # Get the top 50 cryptocurrencies by market capitalization
    coins = cg.get_coins_markets(vs_currency='eur', per_page=1000)

    # Print the results
    for coin in coins:
        marketdata = []
        time.sleep(30)
        print(coin['id'] + " : " + coin['symbol'] + " : " + str(coin['current_price']))

        coin_id = coin['id']
        start_date = '2022-01-01'
        end_date = '2022-05-01'

        # Get the market data for the coin over the specified date range
        market_data_usd = cg.get_coin_market_chart_by_id(id=coin_id, vs_currency='usd', days=1825)

        # Get the market data for the coin over the specified date range in EUR
        market_data_eur = cg.get_coin_market_chart_by_id(id=coin_id, vs_currency='eur', days=1825)
        
        f = open("data/" + coin_id + "_usd.txt", "w")
        for price in market_data_usd['prices']:
            date = datetime.utcfromtimestamp(price[0] / 1000.0)
            #f.write("Timestamp: " + str(price[0]) + " Date: " + str(date)  + " Price: " + str(price[1]) + " USD\n")
            #print("Date: " + str(price[0]) + " Price: " + str(price[1]) + " USD")
            marketdata.append({"timestamp": price[0], 
                                "date": date,
                                "price_usd" : price[1]})
        
        f = open("data/" + coin_id + "_eur.txt", "w")
        index = 0
        for price in market_data_eur['prices']:
            date = datetime.utcfromtimestamp(price[0] / 1000.0)
            #print("Date: " + str(price[0]) + " Price: " + str(price[1]) + " EUR")
            #f.write("Timestamp: " + str(price[0]) + " Date: " + str(date)  + " Price: " + str(price[1]) + " EUR\n")
            marketdata[index]["price_eur"] = price[1]
            index += 1
        
        for d in marketdata:
            print("Timestamp: " + str(d["timestamp"]) + " Date: " + str(d["date"]) + " Price_USD: " + str(d["price_usd"]) + " Price_EUR: " + str(d["price_eur"]))
        
        saveData(coin['symbol'], marketdata, mydb)
    mydb.close()

def saveData(ticker, marketdata, mydb):
    cursor = mydb.cursor()
    query = "INSERT INTO historic_data (ticker, timestamp, datetime, price_eur, price_usd) VALUES (%s, %s, %s, %s, %s)"
    for d in marketdata:
        values = (ticker, d["timestamp"], d["date"], d["price_eur"], d["price_usd"])
        try:
            cursor.execute(query, values)
        except mysql.connector.Error as err:
            if not ("Duplicate" in err.msg):
                print(f"Error: {err.msg}")
    mydb.commit()
    

if __name__ == "__main__":
    main()