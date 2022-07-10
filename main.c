#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
	uint8_t A;		// Accumulator
	uint8_t Y;		// Index register
	uint8_t X;		// Index register
	uint8_t PCH;	// Program Counter High
	uint8_t PCL;	// Program Counter Low
	uint8_t S;		// Stack pointer
	uint8_t P;		// Process status register (N, V, 1, B, D, I, Z, C)
} Cpu_state;

// BIT OPERATIONS

// Set bit (0-7) in registry
void set_bit(uint8_t *reg, int shift){
	*reg = *reg | (0x01 << shift); // OR (bitwise)
}

// Unset bit (0-7) in registry
void unset_bit(uint8_t *reg, int shift){
	*reg = *reg ^ (0x01 << shift); // XOR (bitwise)
	// could also be done as AND reg (NOT (0x01 << shift))
}

// Get bit (0-7) from registry
bool get_bit(uint8_t *reg, int shift) {
	return *reg & (0x01 << shift); // AND (bitwise)
}


//TODO Move to its own file
// INSTRUCTIONS

typedef enum {
	immediate, 				// Constant uint8_t value, immediately following instruction
	absolute,
	direct_page,
	absolute_indexed_x,
	absolute_indexed_y,
	dp_indexed_x,
	dp_indexed_indirect_x,
	dp_indexed_indirect_y
} mode;

// add with carry
void adc(Cpu_state *state, uint8_t *memory, uint16_t address, mode m){
	uint8_t carry = state->P & 0x01;
	uint8_t data;

	switch(m){
		case immediate :
			{ // Although brackets are not normally necessary for cases, they are if you want to declare a variable. This is due to the label nature of case statements in C

				// Get current program counter value
				uint16_t curr_PC;
				curr_PC = (state->PCH << 8) | state->PCL;

				// Data is next hex code
				data = memory[curr_PC + 1];
				break;
			}
		default:
			data = memory[address];
			printf("Panic time\n");
			exit(1);
	}
	printf("Pre: %u\n",state->A);
	state->A = state->A + data + carry;
	printf("Post: %u\n",state->A);

}


// END OF INSTRUCTIONS


int main() {
	/* TESTS FOR GET/SET BITS
	uint8_t reg = 0xdf; //1101 1111

	set_bit(&reg, 5);
	bool active = get_bit(&reg, 5);
	printf("Active 5: %d\n", active);

	active = get_bit(&reg, 4);
	printf("Active 4: %d\n", active);

	unset_bit(&reg, 5);
	active = get_bit(&reg, 5);
	printf("Active 5: %d\n", active);
	*/


	// Allocate memory (2^16)
	uint8_t* memory = (uint8_t *) malloc(0x10000 * sizeof(uint8_t));

	memory[0] = 4;
	memory[1] = 8;

	// Declare CPU state
	Cpu_state state = {
		.A = 0x00,
		.Y = 0x00,
		.X = 0x00,
		.PCH = 0x00,
		.PCL = 0x00,
		.S = 0x00,
		.P = 0x00
	};

	// TESTS FOR INSTRUCTIONS
	adc(&state, memory, 0x01, immediate);
	//adc(state, memory, 0x01, absolute); // Testing for proper panicking

	return 0;
}
