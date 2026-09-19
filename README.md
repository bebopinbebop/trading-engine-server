#FIREWATCH🔥

WORKDIR backend/
COPY . .
RUN sudo apt update
RUN sudo apt install build-essential cmake gdb pkg-config libwebsockets-dev
RUN cmake -S . -B build
EXPOSE 8080
CMD["./build/trading_engine_server","start"]
    