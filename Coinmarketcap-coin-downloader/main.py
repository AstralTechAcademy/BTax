import sys
import wget
import os 

coins ={
    "btc":"1",
    "ada":"2010",
    "eth":"1027",
    "dot":"6636",
    "matic":"3890",
    "usdc":"3408",
    "shib":"5994",
    "usdt":"825",
    "bnb":"1839",
    "xrp":"52",
    "busd":"4687",
    "doge":"74",
    "sol":"5426",
    "ltc":"2",
    "trx":"1958",
    "avax":"5805",
    "dai":"4943",
    "uni":"7083",
    "atom":"3794",
    "link":"1975",
    "okb":"3897",
    "cro":"3635",
    "vet":"3077",
    "icp":"8916",
    "algo":"4030",
    "qnt":"3155",
    "ftm":"3513",
    "mana":"1966",
    "chz":"4066",
    "lunc":"4172",
    "xtz":"2011",
    "aave":"7278",
    "cake":"7186",
    "xlm":"512",
    "xmr":"328",
    "zec":"1437",
    "miota":"1720",
    "luna":"20314"
} 

shares = {
    "san": "santander--big",
    "msft": "microsoft--big",
    "aapl": "apple--big",
    "pg": "procter-and-gamble--big",
    "voo": "vanguard--big",
    "icln": "ishares--big",
    "edp": "energias-br-on-nm--big",
    "abb": "abb--big",
    "slr": "solaria-energia-y-medio-ambiente--big",
    "nestle": "nestle--big",
    "ibe": "iberdrola--big",
    "xom": "exxon--big",
    "engi": "engie--big",
    "axp": "american-express--big",
    "v": "visa--big",
    "ma": "mastercard--big",
}

def main(): 
    path = "coins"
    pathS = "shares"
    try: 
        os.mkdir(path) 
    except OSError as error: 
        print(error)  

    try: 
        os.mkdir(pathS) 
    except OSError as error: 
        print(error)  

    for coin in coins:
        coinNumber = coins[coin]
        ticker = coin
        url = "https://s2.coinmarketcap.com/static/img/coins/64x64/" + coinNumber + ".png"
        wget.download(url, path + "/" + ticker + ".png")

    for share in shares:
        sNumber = shares[share]
        ticker = share
        url = "https://s3-symbol-logo.tradingview.com/" + sNumber + ".svg"
        wget.download(url, pathS + "/" + ticker + ".svg")

if __name__ == "__main__":
    sys.exit(main())