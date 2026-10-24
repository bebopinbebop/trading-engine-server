# FIREWATCH🔥

## Quick Start

To start this project, run:
```bash
docker compose up --build --watch
```

## Project Components

This project uses `libwebsockets` as the main tool to create websockets between the backend `C Server` and the frontend `React Server`.

`libwebsockets` can be further detailed [here](https://github.com/warmcat/libwebsockets)

This project has to sides that work in tandem to produce a stock options evaluator.

There is the `backend`, which is a data aggregator and deterministic algorithm producing comperative result for an options trade, using C.

There is the `frontend`, which receives the options data and displays it using React.

There is `Docker`, which orchestrates the entire thing.

This project uses `cmake` as the compiler due to needing a more robust compiler to handle the multiple files automatically compared to `gcc`.