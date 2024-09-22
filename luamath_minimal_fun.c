#include "luamath_minimal_fun.h"


// Identity matrix
void lm_fun_mat_identity(matrix *out,int n)
{
        // Matrix dimension setting
        out->row = n;
        out->col = n;

        // Matrix type setting
        out->type = LUA_TNUMBER;

        // Matrix initialization
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i==j)
                {
                    out->data[i * n + j] = 1.0;
                }
                else
                {
                    out->data[i * n + j] = 0.0;
                }
                
            }
        }
}
