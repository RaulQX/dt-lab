#ifndef __PARSER_AT__
#define __PARSER_AT__

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_MAX_LINE_SIZE 128

typedef struct
{
	uint8_t ok;
	uint8_t data[AT_COMMAND_MAX_LINES][AT_COMMAND_MAX_LINE_SIZE + 1];
	uint32_t line_count;
} AT_COMMAND_DATA;

typedef enum
{
	STATE_MACHINE_NOT_READY,
	STATE_MACHINE_READY_OK,
	STATE_MACHINE_READY_WITH_ERROR
} STATE_MACHINE_RETURN_VALUE;

typedef enum
{
	_0_START_STATE = 0,
	_1_FIRST_CR = 1,
	_2_FIRST_LF = 2,
	_3_OK_O = 3,
	_4_ERROR_E = 4,
	_5_FIRST_PLUS = 5,
	_6_NEXT_ASCII = 6,
	_7_FIRST_CR_ASCII = 7,
	_8_FIRST_LF_ASCII = 8,
	_9_SECOND_CR_ASCII = 9,
	_10_SECOND_LF_ASCII = 10,
	_11_ERROR_FIRST_R = 11,
	_12_ERROR_SECOND_R = 12,
	_13_ERROR_O = 13,
	_14_ERROR_THIRD_R = 14,
	_15_ERROR_CR = 15,
	// 16 is a final state in diagram
	_17_OK_K = 17,
	_18_OK_CR = 18,
	// 19 is a final state in diagram
} State_Machine;

STATE_MACHINE_RETURN_VALUE data_parser(uint8_t currentCharacter);
void print_data_structure(AT_COMMAND_DATA data);

#endif
