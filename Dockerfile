FROM alpine:3.20 AS build
COPY . /build
# ncurses-dev and readline-dev still needed for header files (curses.h, readline/readline,history.h)
RUN apk add --virtual --no-cache make gcc musl-dev ncurses-static ncurses-dev readline-static readline-dev && \
    make XTRAFLAGS=-static LDFLAGS="-lreadline -ltinfo -lft" -C/build

FROM busybox
COPY --from=build /build/minishell .
VOLUME /hostfs
CMD ["./minishell"]

# For dynamically linking with .so files instead of static linking with .a files for libreadline and libtinfo/libcurses
# RUN apk add --virtual --no-cache make gcc musl-dev ncurses-dev readline-dev && \
#     make -C/build
