#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// #define DEBUG
#define PRINTI(instruction, opcode) printf("%s 0x%x\n", instruction, opcode)

#define STACK_SIZE 128
#define MEMORY_SIZE 4096

typedef struct {
  union { // registers
    uint8_t v[16];
    struct {
      uint8_t v0;
      uint8_t v1;
      uint8_t v2;
      uint8_t v3;
      uint8_t v4;
      uint8_t v5;
      uint8_t v6;
      uint8_t v7;
      uint8_t v8;
      uint8_t v9;
      uint8_t va;
      uint8_t vb;
      uint8_t vc;
      uint8_t vd;
      uint8_t ve;
      uint8_t vf;
    };
  };
  uint8_t delay;               // delay timer
  uint8_t sound;               // sound timer
  uint16_t stack[STACK_SIZE];  // stack
  uint16_t pc;                 // program counter
  uint8_t memory[MEMORY_SIZE]; // memory
  uint16_t memsize;            // amount of bytes loaded.
  uint16_t i;                  // memory address register
  uint16_t sp;                 // stack pointer
  bool keypad[16];             // keypad state.
  bool halt;                   // halt state. 
  uint8_t screen[2048];     // screen.
} cpu_state;

bool load_rom(cpu_state *state, const char *rom_path);
void load_fonts(cpu_state *state);
void cpu_init(cpu_state *state, const char *rom);
void cpu_emulate(cpu_state *state);
void cpu_print_state(cpu_state *state);
void cpu_keyhandle(cpu_state *state, int key, int action);
void cpu_render(cpu_state *state);


void cpu_op0(cpu_state *state, const uint16_t opcode);
void cpu_op1(cpu_state *state, const uint16_t opcode);
void cpu_op2(cpu_state *state, const uint16_t opcode);
void cpu_op3(cpu_state *state, const uint16_t opcode);
void cpu_op4(cpu_state *state, const uint16_t opcode);
void cpu_op5(cpu_state *state, const uint16_t opcode);
void cpu_op6(cpu_state *state, const uint16_t opcode);
void cpu_op7(cpu_state *state, const uint16_t opcode);
void cpu_op8(cpu_state *state, const uint16_t opcode);
void cpu_op9(cpu_state *state, const uint16_t opcode);
void cpu_opa(cpu_state *state, const uint16_t opcode);
void cpu_opb(cpu_state *state, const uint16_t opcode);
void cpu_opc(cpu_state *state, const uint16_t opcode);
void cpu_opd(cpu_state *state, const uint16_t opcode);
void cpu_ope(cpu_state *state, const uint16_t opcode);
void cpu_opf(cpu_state *state, const uint16_t opcode);
