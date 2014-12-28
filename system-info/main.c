
#include <uv.h>
#include <lwlog/lwlog.h>
#include <stdlib.h>

#define UV_ERR(err, msg) lwlog_err("%s: [%s(%d): %s]\n", msg, uv_err_name((err)), (int)err, uv_strerror((err)))

#define UV_CHECK(err, msg) \
do { \
  if (err != 0) { \
    UV_ERR(err, msg); \
    exit(1); \
  } \
} while(0)

int main() {
    int err = 0;
    double uptime;
    size_t resident_set_memory = 0;

    err = uv_uptime(&uptime);
    UV_CHECK(err, "uv_uptime");
    lwlog_info("Uptime: %f", uptime);

    err = uv_resident_set_memory(&resident_set_memory);
    UV_CHECK(err, "uv_resident_set_memory");
    lwlog_info("RSS: %ld", resident_set_memory);

    return 0;
}
