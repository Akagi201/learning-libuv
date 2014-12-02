
#include <uv.h>
#include <log/log.h>
#include <stdlib.h>

#define CHECK(r, msg) if (r) {                                                       \
  log_error("%s: [%s(%d): %s]\n", msg, uv_err_name((r)), (int) r, uv_strerror((r))); \
  exit(1);                                                                           \
}

int main() {
    int err;

    double uptime;
    err = uv_uptime(&uptime);
    CHECK(err, "uv_uptime");
    log_info("Uptime: %f", uptime);

    size_t resident_set_memory;
    err = uv_resident_set_memory(&resident_set_memory);
    CHECK(err, "uv_resident_set_memory");
    log_info("RSS: %ld", resident_set_memory);

    return 0;
}
