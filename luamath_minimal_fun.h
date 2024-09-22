#include "lauxlib.h"
#include "lua.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
	int type;          // LUA_TNUMBER or LUA_TBOOLEAN
	int row;           // data number of rows
	int col;           // data number of columns
	LUA_NUMBER data[]; // Array where matrix values are collected
}matrix;

// Identity matrix
void lm_fun_mat_identity(matrix *out,int n);