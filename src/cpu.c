#include "cpu.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "app.h"
#include "render/render.h"
#include <linmath.h>

/* #define DEBUG */

bool load_rom(cpu_state *state, const char *rom_path) {
  FILE *fp = fopen(rom_path, "rb");
  if (fp == NULL) {
    printf("ERR: Couldn't open %s\n", rom_path);
    return false;
  }
  fseek(fp, 0L, SEEK_END);
  uint16_t size = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  if (size > MEMORY_SIZE) {
    printf("error: not enough memory for this rom!");
    fclose(fp);
    return false;
  }

  state->memsize = size;
  fread((state->memory) + 0x200, size, 1, fp);
  fclose(fp);

  return true;
}

void load_fonts(cpu_state *state) {
  uint8_t fonts[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  for (int i = 0; i < 80; i++) {
    state->memory[i] = fonts[i];
  }
}

void cpu_init(cpu_state *state, const char *rom) {
  state->pc = 0x200;
  load_rom(state, rom);
  load_fonts(state);

  time_t t;
  srand((unsigned)time(&t));
}

void cpu_emulate(cpu_state *state) {
  uint16_t op;
  uint8_t isig;

  cpu_input(state);

  op = (state->memory[state->pc] << 8u) | state->memory[state->pc + 1u];
  isig = (op & 0xf000) >> 12u; // get the instruction nib.

  state->pc += 2;

  switch (isig) {
  case 0x00: cpu_op0(state, op); break;
  case 0x01: cpu_op1(state, op); break;
  case 0x02: cpu_op2(state, op); break;
  case 0x03: cpu_op3(state, op); break;
  case 0x04: cpu_op4(state, op); break;
  case 0x05: cpu_op5(state, op); break;
  case 0x06: cpu_op6(state, op); break;
  case 0x07: cpu_op7(state, op); break;
  case 0x08: cpu_op8(state, op); break;
  case 0x09: cpu_op9(state, op); break;
  case 0x0a: cpu_opa(state, op); break;
  case 0x0b: cpu_opb(state, op); break;
  case 0x0c: cpu_opc(state, op); break;
  case 0x0d: cpu_opd(state, op); break;
  case 0x0e: cpu_ope(state, op); break;
  case 0x0f: cpu_opf(state, op); break;
  default:
    printf("error: unknown instruction!");
    exit(-1);
    break;
  }
}

void cpu_print_state(cpu_state *state) {
  printf("I: 0x%x\n", state->i);
  printf("PC: 0x%x\n", state->pc);
  printf("SP: 0x%x\n", state->sp);
  printf("MS: 0x%x\n", state->memsize);
  printf("DT: 0x%x\n", state->delay);
  printf("ST: 0x%x\n", state->sound);

  for (int i = 0; i < 16; i++)
    printf("v%d: 0x%x\n", i, state->v[i]);
}

void cpu_keyhandle(cpu_state *state, int key, int scancode, int action,
                   int mods) {
  switch (key) {
  case GLFW_KEY_1: state->keypad[0] = action; break;
  case GLFW_KEY_2: state->keypad[1] = action; break;
  case GLFW_KEY_3: state->keypad[2] = action; break;
  case GLFW_KEY_4: state->keypad[3] = action; break;
  case GLFW_KEY_Q: state->keypad[4] = action; break;
  case GLFW_KEY_W: state->keypad[5] = action; break;
  case GLFW_KEY_E: state->keypad[6] = action; break;
  case GLFW_KEY_R: state->keypad[7] = action; break;
  case GLFW_KEY_A: state->keypad[8] = action; break;
  case GLFW_KEY_S: state->keypad[9] = action; break;
  case GLFW_KEY_D: state->keypad[10] = action; break;
  case GLFW_KEY_F: state->keypad[11] = action; break;
  case GLFW_KEY_Z: state->keypad[12] = action; break;
  case GLFW_KEY_X: state->keypad[13] = action; break;
  case GLFW_KEY_C: state->keypad[14] = action; break;
  case GLFW_KEY_V: state->keypad[15] = action; break;
  }
}

void cpu_input(cpu_state *state) {
  state->keypad[0x1] = glfwGetKey(g_state.renderer.window, GLFW_KEY_1);
  state->keypad[0x2] = glfwGetKey(g_state.renderer.window, GLFW_KEY_2);
  state->keypad[0x3] = glfwGetKey(g_state.renderer.window, GLFW_KEY_3);
  state->keypad[0xC] = glfwGetKey(g_state.renderer.window, GLFW_KEY_4);
  state->keypad[0x4] = glfwGetKey(g_state.renderer.window, GLFW_KEY_Q);
  state->keypad[0x5] = glfwGetKey(g_state.renderer.window, GLFW_KEY_W);
  state->keypad[0x6] = glfwGetKey(g_state.renderer.window, GLFW_KEY_E);
  state->keypad[0xD] = glfwGetKey(g_state.renderer.window, GLFW_KEY_R);
  state->keypad[0x7] = glfwGetKey(g_state.renderer.window, GLFW_KEY_A);
  state->keypad[0x8] = glfwGetKey(g_state.renderer.window, GLFW_KEY_S);
  state->keypad[0x9] = glfwGetKey(g_state.renderer.window, GLFW_KEY_D);
  state->keypad[0xE] = glfwGetKey(g_state.renderer.window, GLFW_KEY_F);
  state->keypad[0xA] = glfwGetKey(g_state.renderer.window, GLFW_KEY_Z);
  state->keypad[0x0] = glfwGetKey(g_state.renderer.window, GLFW_KEY_X);
  state->keypad[0xB] = glfwGetKey(g_state.renderer.window, GLFW_KEY_C);
  state->keypad[0xF] = glfwGetKey(g_state.renderer.window, GLFW_KEY_V);
}

void cpu_render(cpu_state *state) {
  for (int j = 0; j < 32; j++) {
    for (int i = 0; i < 64; i++) {
      if (state->screen[i + j * 64] > 0) {
        render_quad((vec2){i * 16 + 8, 32 * 16 - j * 16 - 8}, (vec2){16, 16},
                    (vec4){0.01, 0.5, 0.5, 1});
      }
    }
  }
}

void cpu_op0(cpu_state *state, const uint16_t opcode) {
  switch (opcode) {
  case 0x00E0: // clear screen 0x00E0
    for (int j = 0; j < 32; j++) {
      for (int i = 0; i < 64; i++) {
        state->screen[i + j * 64] = 0x00;
      }
    }
#ifdef DEBUG
    PRINTI("0x00e0", opcode);
#endif
    break;
  case 0x00EE: // return from subroutine 0x00ee
    state->pc = state->stack[--state->sp];
#ifdef DEBUG
    PRINTI("0x00ee", opcode);
#endif
    break;
    // 0x0nnn can be ingnored.
  }
}

void cpu_op1(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with 1.
  // 0x1nnn jump to address nnn.
  uint16_t addr = opcode & 0x0FFFu;
  state->pc = addr;
#ifdef DEBUG
  PRINTI("0x1nnn", opcode);
#endif
}

void cpu_op2(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with 2.
  // 0x2nnn call subroutine at address nnn
  uint16_t addr = opcode & 0x0fff;
  state->stack[state->sp++] = state->pc;
  state->pc = addr;
#ifdef DEBUG
  PRINTI("0x2nnn", opcode);
#endif
}

void cpu_op3(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with 3.
  // 0x3xnn skip next instruction if Vx == nn.
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t nn = opcode & 0x00FF;
  if (state->v[x] == nn)
    state->pc += 2;
#ifdef DEBUG
  PRINTI("0x3xnn", opcode);
#endif
}

void cpu_op4(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with 4.
  // 0x4xnn skip next instruction if Vx != nn.
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t nn = opcode & 0x00FF;
  if (state->v[x] != nn)
    state->pc += 2;
#ifdef DEBUG
  PRINTI("0x4xnn", opcode);
#endif
}

void cpu_op5(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with 5.
  // 0x5xy0 skip next instruction if Vx == Vy.
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 8;
  if (state->v[x] == state->v[y])
    state->pc += 2;
  if ((opcode & 0x000F) != 0x0000) {
    printf("error: malformed instruction! %x\n", opcode);
  }
#ifdef DEBUG
  PRINTI("0x5xy0", opcode);
#endif
}

void cpu_op6(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with 6.
  // 0x6xnn set Vx to nn. Vx = nn;
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t nn = opcode & 0x00FF;
  state->v[x] = nn;
#ifdef DEBUG
  PRINTI("0x6xnn", opcode);
#endif
}

void cpu_op7(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with 7.
  // 0x7xnn add nn to Vx. Vx += nn.
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t nn = opcode & 0x00FF;
  state->v[x] += nn;
#ifdef DEBUG
  PRINTI("0x7xnn", opcode);
#endif
}

void cpu_op8(cpu_state *state, const uint16_t opcode) {
  uint8_t lnib = (opcode & 0x000F);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  uint16_t val;
#ifdef DEBUG
  printf("0x8xy%x %x\n", lnib, opcode);
#endif
  switch (lnib) {
  case 0x0000:
    // 0x8xy0 set the value of Vx to Vy. Vx = Vy
    state->v[x] = state->v[y];
    break;
  case 0x0001:
    // 0x8xy1 set the value of Vx to Vx or Vy(bitwise). Vx |= Vy
    state->v[x] |= state->v[y];
    break;
  case 0x0002:
    // 0x8xy2 set the value of Vx to Vx and Vy(bitwise). Vx &= Vy
    state->v[x] &= state->v[y];
    break;
  case 0x0003:
    // 0x8xy3 set the value of Vx to Vx xor Vy(bitwise). Vx ^= Vy
    state->v[x] ^= state->v[y];
    break;
  case 0x0004:
    // 0x8xy4 adds the value of Vy to Vx. Vf set to 1 when there is a carry and
    // 0 when not. Vx += Vy
    val = state->v[x] + state->v[y];

    if (val > 255)
      state->vf = 0x1;
    else
      state->vf = 0x0;

    state->v[x] = val & 0xff;
    break;
  case 0x0005:
    // 0x8xy5 subtracts the value of Vy to Vx. Vf set to 0 when there is a
    // borrow and 1 when not. Vx -= Vy

    if (state->v[x] > state->v[y])
      state->vf = 0x1;
    else
      state->vf = 0x0;

    state->v[x] -= state->v[y];
    break;
  case 0x0006:
    // 0x8xy6 stores the least signifigant bit of Vx in VF then shifts Vx to the
    // right by 1. Vx >>= 1
    state->vf = state->v[x] & 0x01;
    state->v[x] >>= 1;
    break;
  case 0x0007:
    // 0x8xy7 sets Vx to Vy minus Vx. VF is set to 0 when there is a borrow and
    // 1 when not. Vx = Vy - Vx.

    if (state->v[y] > state->v[x])
      state->vf = 0x1;
    else
      state->vf = 0x0;

    state->v[x] = state->v[y] - state->v[x];
    break;
  case 0x000e:
    // 0x8xye stores the most signifigant bit of Vx in VF then shifts Vx to the
    // left by 1. Vx <<= 1
    state->vf = (state->v[x] & 0x80) >> 7;
    state->v[x] <<= 1;
    break;
  default:
    printf("error: malformed instruction! 0x%x\n", opcode);
    exit(-1);
    break;
  }
}

void cpu_op9(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with 9.
  // 0x9xy0 skips the next instruction if vX does not equal vY.
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  if (state->v[x] != state->v[y])
    state->pc += 2;
#ifdef DEBUG
  PRINTI("0x9xy0", opcode);
#endif
  if ((opcode & 0x000F) != 0x0000) {
    printf("error: malformed instruction: 0x9xy0\n");
    exit(-1);
  }
}

void cpu_opa(cpu_state *state, const uint16_t opcode) {
  // only one struction starting with a.
  // 0xannn sets I to the address nnn. I = nnn
  uint16_t addr = opcode & 0x0FFF;
  state->i = addr;
#ifdef DEBUG
  PRINTI("0xannn", opcode);
#endif
}

void cpu_opb(cpu_state *state, const uint16_t opcode) {
  // only one struction starting with b.
  // 0xbnnn jumps to the address nnn + V0. PC = nnn + V0
  uint16_t addr = opcode & 0x0FFF;
  state->pc = addr + state->v0;
#ifdef DEBUG
  PRINTI("0xbnnn", opcode);
#endif
}

void cpu_opc(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with c.
  // 0xCXNN sets Vx to the result of rand and bitwise and with nn. Vx = rand() &
  // nn
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint16_t nn = opcode & 0x00FF;
  state->v[x] = (rand() % 255) & nn;
#ifdef DEBUG
  PRINTI("0xcxnn", opcode);
#endif
}

void cpu_opd(cpu_state *state, const uint16_t opcode) {
  // only one instruction starting with d.
  // 0xDxyn draws a sprite. draw(Vx, Vy, n)
  // 8 x n pixels in size at position Vx, Vy.
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  uint8_t n = opcode & 0x000F;

  uint8_t vx = state->v[x] % 64;
  uint8_t vy = state->v[y] % 32;

  uint8_t sprite_byte, sprite_pixel;
  uint8_t *screen_pixel;

  int i, j;

  for (j = 0; j < n; j++) {
    sprite_byte = state->memory[state->i + j];
    for (i = 0; i < 8; i++) {
      sprite_pixel = sprite_byte & (0x80 >> i);
      screen_pixel = &state->screen[(vy + j) * 64 + (i + vx)];

      if (sprite_pixel) {
        // collision
        if (*screen_pixel == 0xFF) {
          state->vf = 1;
        }
        *screen_pixel ^= 0xFF;
      }
    }
  }

#ifdef DEBUG
  PRINTI("0xdxyn", opcode);
#endif
}

void cpu_ope(cpu_state *state, const uint16_t opcode) {
  uint16_t isig = (opcode & 0x00FF);
  uint8_t x = (opcode & 0x0F00) >> 8;

#ifdef DEBUG
  printf("0xex%x\n", isig);
#endif
  switch (isig) {
  case 0x9e:
    // skip next instruction if key stored in Vx is pressed
    if (state->keypad[state->v[x]] > 0)
      state->pc += 2;
    break;
  case 0xa1:
    // skip next instruction if key stored in Vx is not pressed
    if (state->keypad[state->v[x]] == 0)
      state->pc += 2;
    break;
  default:
    printf("error: malformed instruction starting with e\n");
    exit(-1);
    break;
  }
}

void cpu_opf(cpu_state *state, const uint16_t opcode) {
  uint16_t isig = (opcode & 0x00FF);
  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t val;
#ifdef DEBUG
  printf("0xfx%x\n", isig);
#endif
  switch (isig) {
  case 0x07: state->v[x] = state->delay; break;
  case 0x0A:;
    uint8_t decr = 2;
    for (int i = 0; i < 16; i++) {
      if (state->keypad[i] > 0) {
        state->v[x] = i;
        decr = 0;
      }
    }
    state->pc -= decr;
    break;
  case 0x15: state->delay = state->v[x]; break;
  case 0x18: state->sound = state->v[x]; break;
  case 0x1e: state->i += state->v[x]; break;
  case 0x29:; state->i = (state->v[x] * 5); break;
  case 0x33:;
    val = state->v[x];
    // ones
    state->memory[state->i + 2] = val % 10;
    val /= 10;
    // tens
    state->memory[state->i + 1] = val % 10;
    val /= 10;
    // hundreds
    state->memory[state->i] = val % 10;
    break;
  case 0x55:
    for (int i = 0; i <= x; i++) {
      state->memory[state->i + i] = state->v[i];
    }
    break;
  case 0x65:
    for (int i = 0; i <= x; i++) {
      state->v[i] = state->memory[state->i + i];
    }
    break;
  default:
    printf("error: malformed instruction starting with f %x\n", opcode);
    exit(-1);
    break;
  }
}
