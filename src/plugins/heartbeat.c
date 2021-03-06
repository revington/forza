#include <uv.h>
#include <forza.h>

static uv_timer_t heartbeat_timer;

void heartbeat__send(uv_timer_t *timer, int status) {
  forza_metric_t* metric = forza_new_metric();

  metric->service = "health/process/heartbeat";
  metric->metric = 1;
  forza_send(metric);

  forza_free_metric(metric);
}

void heartbeat__process_exit_cb(int exit_status, int term_signal) {
  uv_timer_stop(&heartbeat_timer);
}

int heartbeat_init(forza_plugin_t* plugin) {

  //
  // Set heartbeat timer to stop when process exits
  //
  plugin->process_exit_cb = heartbeat__process_exit_cb;

  uv_timer_init(uv_default_loop(), &heartbeat_timer);
  uv_timer_start(&heartbeat_timer, heartbeat__send, 0, 90000);

  return 0;
}
