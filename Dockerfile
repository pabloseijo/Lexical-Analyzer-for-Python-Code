FROM alpine:3.14

RUN apk update
RUN apk upgrade
RUN apk add build-base
RUN apk add valgrind

RUN mkdir /app

WORKDIR /app

COPY . .

ENTRYPOINT [ "tail", "-f", "/dev/null" ]

