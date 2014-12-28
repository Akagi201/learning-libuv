
#include <uv.h>
#include <stdlib.h>
#include "lwlog/lwlog.h"

#define BUF_SIZE (37)
static const char *filename = "expected.txt";

#define UV_ERR(err, msg) lwlog_err("%s: [%s(%d): %s]\n", msg, uv_err_name((err)), (int)err, uv_strerror((err)))

#define UV_CHECK(err, msg) \
do { \
  if (err != 0) { \
    UV_ERR(err, msg); \
    exit(1); \
  } \
} while(0)

/* forward declarations */
void open_cb(uv_fs_t *);

void read_cb(uv_fs_t *);

void close_cb(uv_fs_t *);

typedef struct context_struct {
    uv_fs_t *open_req;
    uv_fs_t *read_req;
} context_t;

void open_cb(uv_fs_t *open_req) {
    int err = 0;
    if (open_req->result < 0) {
        UV_CHECK(open_req->result, "uv_fs_open callback");
    }

    context_t *context = open_req->data;

    /* 3. Create buffer and initialize it */
    size_t buf_len = sizeof(char) * BUF_SIZE;
    char *buf = malloc(buf_len);
    uv_buf_t iov = uv_buf_init(buf, buf_len);

    /* 4. Setup read request */
    uv_fs_t *read_req = malloc(sizeof(uv_fs_t));
    context->read_req = read_req;
    read_req->data = context;

    /* 5. Read from the file into the buffer */
    err = uv_fs_read(uv_default_loop(), read_req, open_req->result, &iov, 1, 0, read_cb);
    if (err < 0) {
        UV_CHECK(err, "uv_fs_read");
    }

    return;
}

void read_cb(uv_fs_t *read_req) {
    int err = 0;;
    if (read_req->result < 0) {
        UV_CHECK(read_req->result, "uv_fs_read callback");
    }

    context_t *context = read_req->data;

    /* 7. Report the contents of the buffer */
    //log_report("%s", read_req->bufs->base);
    lwlog_info("Read buf: %s", read_req->bufs->base);

    free(read_req->bufs->base);

    /* 6. Setup close request */
    uv_fs_t *close_req = malloc(sizeof(uv_fs_t));
    close_req->data = context;

    /* 8. Close the file descriptor */
    err = uv_fs_close(uv_default_loop(), close_req, context->open_req->result, close_cb);
    if (err < 0) {
        UV_CHECK(err, "uv_fs_close");
    }

    return;
}

void close_cb(uv_fs_t *close_req) {
    if (close_req->result < 0) {
        UV_CHECK(close_req->result, "uv_fs_close callback");
    }

    context_t *context = close_req->data;

    /* 9. Cleanup all requests and context */
    uv_fs_req_cleanup(context->open_req);
    uv_fs_req_cleanup(context->read_req);
    uv_fs_req_cleanup(close_req);
    free(context);

    return;
}

void init(uv_loop_t *loop) {
    int err = 0;

    uv_fs_t *open_req = malloc(sizeof(uv_fs_t));

    /* 1. Setup open request */
    context_t *context = malloc(sizeof(context_t));
    context->open_req = open_req;
    open_req->data = context;

    /* 2. Open file */
    err = uv_fs_open(loop, open_req, filename, O_RDONLY, S_IRUSR, open_cb);
    if (err < 0) {
        UV_CHECK(err, "uv_fs_open");
    }

    return;
}

int main(void) {
    uv_loop_t *loop = uv_default_loop();

    init(loop);

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
