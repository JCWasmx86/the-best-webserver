#include "mongoose.h"
#include <cstdio>
#include <string.h>

auto pleaseDontOptimizeMeAway = FLAG;
// Connection event handler function
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  char data[30];
  strdup(pleaseDontOptimizeMeAway);
  if (ev == MG_EV_HTTP_MSG) { // New HTTP request received
    struct mg_http_message *hm =
        (struct mg_http_message *)ev_data;     // Parsed HTTP request
    if (mg_http_match_uri(hm, "/api/hello")) { // REST API call?
      memset(data, 0x00, sizeof(data));
      for (int i = 0; i < MG_MAX_HTTP_HEADERS; i++) {
        if (!hm)
          continue;
        auto header = hm->headers[i];
        if (header.name.ptr == nullptr) {
          break;
        }
        if (strncmp(header.name.ptr, "User-Agent", header.name.len)) {
          continue;
        }
        if (header.value.len >= 30) {
          mg_http_reply(c, 200, "", "{%m:\"%s\"}\n", MG_ESC("hello"),
                        "You are too long");
          return;
        }
        auto s = strndup(header.value.ptr, header.value.len);
        sprintf(data, s);
        free(s);
        break;
      }
      data[28] = (const char)&pleaseDontOptimizeMeAway;
      data[29] = 0x0;
      mg_http_reply(
          c, 200, "WhoAmI: https://github.com/JCWasmx86/the-best-webserver\r\n",
          "{%m:%m}\n", MG_ESC("hello"), MG_ESC(data));
    } else {
      struct mg_http_serve_opts opts = {.root_dir = "/dir/abc",
                                        .extra_headers =
                                            "WhoAmI: "
                                            "https://github.com/JCWasmx86/"
                                            "the-best-webserver\r\n"};
      mg_http_serve_dir(c, hm, &opts);
    }
  }
}

int main() {
  struct mg_mgr mgr; // Mongoose event manager. Holds all connections
  mg_mgr_init(&mgr); // Initialise event manager
  mg_http_listen(&mgr, "http://0.0.0.0:8000", fn, NULL); // Setup listener
  for (;;) {
    mg_mgr_poll(&mgr, 1000); // Infinite event loop
  }
  return 0;
}
