
#include <uv.h>
#include <stdlib.h>
#include "lwlog/lwlog.h"

#define BUF_SIZE (37)

#define UV_ERR(err, msg) lwlog_err("%s: [%s(%d): %s]\n", msg, uv_err_name((err)), (int)err, uv_strerror((err)))

#define UV_CHECK(err, msg) \
do { \
  if (err != 0) { \
    UV_ERR(err, msg); \
    exit(1); \
  } \
} while(0)

static const char *filename = "expected.txt";

int main() {
  int err = 0;
  uv_loop_t *loop = uv_default_loop();

  /* 1. Open file */
  uv_fs_t open_req;
  err = uv_fs_open(loop, &open_req, filename, O_RDONLY, S_IRUSR, NULL);
  if (err < 0) {
      UV_CHECK(err, "uv_fs_open");
  }

  /* 2. Create buffer and initialize it to turn it into a a uv_buf_t which adds length field */
  char buf[BUF_SIZE];
  uv_buf_t iov = uv_buf_init(buf, sizeof(buf));

  /* 3. Use the file descriptor (the .result of the open_req) to read from the file into the buffer */
  uv_fs_t read_req;
  err = uv_fs_read(loop, &read_req, open_req.result, &iov, 1, 0, NULL);
  if (err < 0) {
      UV_CHECK(err, "uv_fs_read");
  }

  /* 4. Report the contents of the buffer */
  //log_report("%s", buf);

  lwlog_info("Read buf: %s", buf);

  /* 5. Close the file descriptor (`open_req.result`) */
  uv_fs_t close_req;
  err = uv_fs_close(loop, &close_req, open_req.result, NULL);
  if (err < 0) {
      UV_CHECK(err, "uv_fs_close");
  }

  /* always clean up your requests when you no longer need them to free unused memory */
  uv_fs_req_cleanup(&open_req);
  uv_fs_req_cleanup(&read_req);
  uv_fs_req_cleanup(&close_req);

  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}
