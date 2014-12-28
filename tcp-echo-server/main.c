#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include "lwlog/lwlog.h"

#define DEFAULT_PORT (7000)
#define DEFAULT_BACKLOG (128)

uv_loop_t *loop;
struct sockaddr_in addr;

#define UV_ERR(err, msg) lwlog_err("%s: [%s(%d): %s]\n", msg, uv_err_name((err)), (int)err, uv_strerror((err)))

#define UV_CHECK(err, msg) \
do { \
  if (err != 0) { \
    UV_ERR(err, msg); \
    exit(1); \
  } \
} while(0)

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    // buf->base = (char *) malloc(suggested_size);
    // buf->len = suggested_size;
    *buf = uv_buf_init((char *) malloc(suggested_size), suggested_size);

    return;
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        UV_ERR(status, "Write error");
    }
    free(req);

    return;
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF) {
            UV_ERR(nread, "Read error");
        }
        uv_close((uv_handle_t *) client, NULL);
        return;
    }

    uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
    uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
    uv_write(req, client, &wrbuf, 1, echo_write);
    free(buf->base);

    return;
}

void on_new_connection(uv_stream_t *server, int status) {
    if (status == -1) {
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);

    if (uv_accept(server, (uv_stream_t *) client) == 0) {
        uv_read_start((uv_stream_t *) client, alloc_buffer, echo_read);
    } else {
        uv_close((uv_handle_t *) client, NULL);
    }

    return;
}

int main(void) {
    int err = 0;
    loop = uv_default_loop();

    uv_tcp_t server;

    err = uv_tcp_init(loop, &server);
    UV_CHECK(err, "tcp_init");

    err = uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
    UV_CHECK(err, "ip4_addr");

    err = uv_tcp_bind(&server, (const struct sockaddr *) &addr, 0);
    UV_CHECK(err, "tcp_bind");

    err = uv_listen((uv_stream_t *) &server, DEFAULT_BACKLOG, on_new_connection);
    UV_CHECK(err, "uv_listen");

    lwlog_info("listening on port %d", DEFAULT_PORT);

    return uv_run(loop, UV_RUN_DEFAULT);
}
