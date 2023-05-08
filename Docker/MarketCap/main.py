from pycoingecko import CoinGeckoAPI
from datetime import datetime
import time

def main():
    # Initialize the client
    cg = CoinGeckoAPI()

    # Get the top 50 cryptocurrencies by market capitalization
    coins = cg.get_coins_markets(vs_currency='eur', per_page=1000)

    # Print the results
    for coin in coins:
        time.sleep(5)
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
            f.write("Timestamp: " + str(price[0]) + " Date: " + str(date)  + " Price: " + str(price[1]) + " USD\n")
            #print("Date: " + str(price[0]) + " Price: " + str(price[1]) + " USD")
        
        f = open("data/" + coin_id + "_eur.txt", "w")
        for price in market_data_eur['prices']:
            date = datetime.utcfromtimestamp(price[0] / 1000.0)
            #print("Date: " + str(price[0]) + " Price: " + str(price[1]) + " EUR")
            f.write("Timestamp: " + str(price[0]) + " Date: " + str(date)  + " Price: " + str(price[1]) + " EUR\n")



if __name__ == "__main__":
    main()