/********************************************************************
 * Name: numbers.c
 * Author: Rashaud Teague <rashaud.teague@gmail.com>
 * Date: 07/27/2009
 * License: GNU LGPL
 * Description: Extension functions to the standard C Library
 *********************************************************************/

#include "xstdlib.h"

/**
 * Converts a binary string to a base 10 decimal
 */
int bin2dec(const char *bin) {
	int i, str_size = strlen(bin);
	int dec = 0;
	
	for (i = 0; i < str_size; i++) {
		/* if the character is an "off" bit skip it in the loop */
		if (bin[i] == '0') continue;
		dec += pow(2, str_size - (i + 1));
	}
	
	return dec;
}

/**
 * Converts a character that "is" a number to its real data type
 * an integer
 */
int ctoi(unsigned char c) {
	switch (c) {
		case 48: return 0;
		case 49: return 1;
		case 50: return 2;
		case 51: return 3;
		case 52: return 4;
		case 53: return 5;
		case 54: return 6;
		case 55: return 7;
		case 56: return 8;
		case 57: return 9;
	}
	
	return -1;
}

char * dec2bin(int dec, char * buffer) {
	int i, j, bin_position = 0;
	int powers[BIT_SIZE];

	for (i = 0; i < BIT_SIZE; i++) { powers[i] = (int)pow(2, i); if ((int)pow(2, i + 1) > dec) break; }
	
	for (j = i; j >= 0; j--) {
		if (powers[j] <= dec) {
			buffer[bin_position] = '1';
			dec -= powers[j];
		} else {
			buffer[bin_position] = '0';
		}

		bin_position++;
	}

	return buffer;
}

void dec2frac(float decimal, float *le_fraction) {
	float mixed        = floor(decimal);
	if (decimal < 0) mixed = ceil(decimal);
	if (mixed == -0) mixed = 0;
	
	float denominator  = 100.0;
	float numerator    = xround(decimal - mixed, 2) * denominator;

	if (mixed <= -1) numerator *= -1;
	if (numerator == 0) {
		numerator = mixed;
		mixed = 0;
		denominator = 1;
	}
	
	le_fraction[0]     = mixed;
	le_fraction[1]     = numerator;
	le_fraction[2]     = denominator;
}

char *dec2hex(int dec, char *buffer) {
	sprintf(buffer, "%X", dec);
	return buffer;
}

/**
 * Returns the factorial of parameter n
 */
int fact(int n) {
	int fac = n, i;
	for (i = n-1; i > 0; i--) fac *= i;
	return fac;
}

int hex2dec(char *hex) {
	strtoupper(hex);
	
	int hex_size = (int)strlen(hex);

	int i, power = 1, sum = 0, eval, base, column;

	for (i = hex_size - 1, column = 1; i >= 0; i--, column++) {
		eval = 0;

		if (column == 1) { base = 1; } else if (column == 2) { base = 16; } else { power = column - 1; }
		
		switch (hex[i]) {
			case 'A': eval = (int)pow(base, power) * 10; break;
			case 'B': eval = (int)pow(base, power) * 11; break;
			case 'C': eval = (int)pow(base, power) * 12; break;
			case 'D': eval = (int)pow(base, power) * 13; break;
			case 'E': eval = (int)pow(base, power) * 14; break;
			case 'F': eval = (int)pow(base, power) * 15; break;
			default:  eval = (int)pow(base, power) * ctoi(hex[i]); break;
		}

		sum += eval;
	}
	
	return sum;
}

void reduce_frac(float *le_fraction, float *reduce) {
	int i;
	int gcd = 1;
	int increment = ((int)le_fraction[1] % 2 == 0) ? 1 : 2;

	int num = (int)le_fraction[1];
	
	if (le_fraction[1] < 0) num *= -1;
	
	for (i = 1; i <= num; i += increment) {
		if (num % i == 0 && (int)le_fraction[2] % i == 0) {
			if (i > gcd) gcd = i;
		}
	}
	
	reduce[0] = le_fraction[0];
	reduce[1] = le_fraction[1] / (float)gcd;
	reduce[2] = le_fraction[2] / (float)gcd;
}

float xround(const float n, int precision) {
	return floor(n * pow(10, precision) + 0.5) / pow(10, precision);
}

