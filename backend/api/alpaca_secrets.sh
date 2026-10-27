#!/usr/bin/env/ bash

read -rsp "Alpaca API key: " ALPACA_KEY
printf '\n'
read -rsp "Alpaca secret key: " ALPACA_SECRET
printf '\n'
export ALPACA_KEY ALPACA_SECRET
