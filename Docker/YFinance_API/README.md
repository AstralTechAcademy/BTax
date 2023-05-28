docker build -t yfinance-api .
docker rm yfinance_api
docker run --network couple-network -p 5001:5000 --name yfinance_api yfinance_api 
docker start yfinance_api 