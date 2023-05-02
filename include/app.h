#pragma once


#include "cpu.h"
#include "render/render.h"

typedef struct {
  cpu_state cpu;
  render_state render;
} app_state;

extern app_state g_state;

void app_init(void);
void app_run(void);
void app_destroy(void);
