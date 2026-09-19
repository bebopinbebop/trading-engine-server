# FIREWATCH🔥

FROM ubuntu:24.04

# INSTALL NECESSARY PACKAGES SO THE BOX CAN RUN C CODE - IF DURING DEVELOPMENT OF C CODE SOMETHING DOESN'T WORK, ADD TO THIS
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake pkg-config libwebsockets-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app/backend

COPY backend/CMakeLists.txt ./
COPY backend/*.c backend/*.h ./

RUN cmake -S . -B build && cmake --build build

EXPOSE 8080

CMD ["./build/firewatch"]