#include <uv.h>
#include <stdlib.h>
#include "lwlog/lwlog.h"

#define BUF_SIZE (37)
static const char *filename = "expected.txt";

/* Making our lifes a bit easier by using this global, a better solution in the next exercise ;) */
static uv_fs_t open_req;

#define UV_ERR(err, msg) lwlog_err("%s: [%s(%d): %s]\n", msg, uv_err_name((err)), (int)err, uv_strerror((err)))

#define UV_CHECK(err, msg) \
do { \
  if (err != 0) { \
    UV_ERR(err, msg); \
    exit(1); \
  } \
} while(0)

void read_cb(uv_fs_t *read_req) {
    int err = 0;
    if (read_req->result < 0) {
        UV_CHECK(read_req->result, "uv_fs_read callback");
    }

    /* 4. Report the contents of the buffer */
    //log_report("%s", read_req->bufs->base);
    lwlog_info("Read buf: %s", read_req->bufs->base);

    /* 5. Close the file descriptor */
    uv_fs_t close_req;
    err = uv_fs_close(read_req->loop, &close_req, open_req.result, NULL);
    if (err < 0) {
        UV_CHECK(err, "uv_fs_close");
    }

    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(read_req);
    uv_fs_req_cleanup(&close_req);

    return;
}

int main() {
    int err = 0;
    uv_loop_t *loop = uv_default_loop();

    /* 1. Open file */
    err = uv_fs_open(loop, &open_req, filename, O_RDONLY, S_IRUSR, NULL);
    if (err < 0) {
      UV_CHECK(err, "uv_fs_open");
    }

    /* 2. Create buffer and initialize it to turn it into a a uv_buf_t */
    char buf[BUF_SIZE];
    uv_buf_t iov = uv_buf_init(buf, sizeof(buf));

    /* 3. Use the file descriptor (the .result of the open_req) to read from the file into the buffer */
    uv_fs_t read_req;
    err = uv_fs_read(loop, &read_req, open_req.result, &iov, 1, 0, read_cb);
    if (err < 0) {
        UV_CHECK(err, "uv_fs_read");
    }

    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
