#include "luamath_minimal.h"


// MODULE FUNCTIONS <--//
/* LM_NEW */
static int lm_new(lua_State *L)
{
    int i,j;

    // Stack: table
    if ((lua_gettop(L) != 1)||(!lua_istable(L, 1)))
    {
        luaL_error(L,"ONLY one Lua table as input is required.\n");
    }

    // Get table row number
    lua_len(L,1); // Stack: table m
    int m = luaL_checkinteger(L,-1);
    lua_pop(L,1); // Stack: table

    // Initialize column and type checks
    int n    = 0;
    int type = LUA_TNONE;

    // Variables to check the Lua table type and column  number consistency
    int check_col_after   = 0; // Number of element in the current row
    int check_type_after  = 0; // Element type of j-th element of current row

    //Check every row has same dimension
    // !! One-based for cycle because      !!
    // !! Lua tables are one-based indexed !!
    for (i = 1; i <= m; i++)
    {
        // Get i-th row
        lua_pushinteger(L,i); // Stack: table i
        lua_gettable(L,1);    // Stack: table table[i]

        // Get i-th row length
        lua_len(L,-1); // Stack: table table[i] n
        check_col_after = luaL_checkinteger(L,-1);

        // Check empty rows
        if (check_col_after==0)
        {
            luaL_error(L,"\nLua table cannot contain empty rows.\n");
        }
        lua_pop(L,1); // Stack: table table[i]

        // Initialize n for i=1
        if (i==1)
        {
            // Every row i>1 will be checked to have n columns
            n = check_col_after;
        }
        // Check the i-th column number compared with the 1st one
        else if (check_col_after!=n)
        {
            luaL_error(L,"\nRows with different element numbers, row N° %d\n", i);
        }

        // Check all data are same type
        // !! One-based for cycle because      !!
        // !! Lua tables are one-based indexed !!
        for (j = 1; j <= n; j++)
        {
            // Get the j-th element of the i-th row
            lua_pushinteger(L,j); // Stack: table table[i] j
            lua_gettable(L,-2);   // Stack: table table[i] table[i][j]
            check_type_after = lua_type(L,-1);

            // Check (i,j) element type is bool or number. NO other types are accepted
            if (check_type_after!=LUA_TNUMBER && check_type_after!=LUA_TBOOLEAN)
            {
                luaL_error(L,"\nOnly bool and number types are accepted."
                             "\nElement in row %d and column %d is %s type.\n",i,j,
                           lua_typename(L,check_type_after));
                return 0;
            }

            // Initialize the type for i=1 and j=1
            // Every type of the following indices MUST be equal to type
            if ((i == 1)&&(j == 1))
            {
                type = check_type_after;
            }
            // Check all other type MUST be equal to type
            else if (check_type_after!=type)
            {
                luaL_error(L,"\nElement in row %d and column %d is different from previous one (%s vs %s).\n",i,j,
                           lua_typename(L,check_type_after),
                           lua_typename(L,type));
            }
            lua_pop(L,1); // Stack: table table[i]
        }
        lua_pop(L,1); // Stack: table
    }

    // Computation of memory allocation
    size_t nbyte = sizeof(matrix) + sizeof(LUA_NUMBER) * (m * n);

    // Memory allocation
    matrix* out = (matrix *)lua_newuserdata(L,nbyte); // Stack: table out

    // Matrix dimension and type setting
    out->row  = m;
    out->col  = n;
    out->type = type;

    // Populate matrix data
    for (i=1;i<=m;i++)
    {
        lua_pushinteger(L,i); // Stack: table out i
        lua_gettable(L,1);    // Stack: table out table[i]

        for (j=1;j<=n;j++)
        {
            lua_pushinteger(L,j); // Stack: table out table[i] j
            lua_gettable(L,-2);   // Stack: table out table[i] table[i][j]
            if (out->type==LUA_TBOOLEAN)
            {
                out->data[(i-1)*n+(j-1)] = lua_toboolean(L,-1); // 1-based index to 0-based
            }
            else if (out->type==LUA_TNUMBER)
            {
                out->data[(i-1)*n+(j-1)] = luaL_checknumber(L,-1); // 1-based index to 0-based
            }
            lua_pop(L,1); // Stack: table out table[i]
        }
         lua_pop(L,1); // Stack: table out
    }

    // Link the userdata out to the metatable LUAMATH_METATABLE
    luaL_getmetatable(L,LUAMATH_METATABLE);
    lua_setmetatable(L,-2);

    // Returning the userdata out
    return 1;

}

 /* LM_IDENTITY */
static int lm_identity(lua_State *L)
{

    if (lua_gettop(L)==1)
    {
        int n = luaL_checkinteger(L, 1);
        if (n <= 0)
        {
            luaL_error(L,"Square matrix dimension MUST be positive.\n");
        }
        size_t nbyte = sizeof(matrix) + sizeof(LUA_NUMBER) * (n * n);
        matrix* out = (matrix *)lua_newuserdata(L,nbyte);
        lm_fun_mat_identity(out,n);

    }
    else
    {
        luaL_error(L,"\nONLY one integer is accepted as input.\n");
    }

    // Link the userdata to the metatable LUAMATH_METATABLE
    luaL_getmetatable(L,LUAMATH_METATABLE);
    lua_setmetatable(L, -2);

}

/* LM_PRINT */
static int lm_print(lua_State *L)
{
    if (lua_gettop(L)>1)
    {
        luaL_error(L,"\nInput must be one luamath  userdata matrix");
    }
    matrix *p = luaL_checkudata(L,1,LUAMATH_METATABLE);

    if (p->type==LUA_TNUMBER)
    {
        for (int i=0;i<p->row;i++)
        {
            for (int j=0;j<p->col;j++)
            {
                printf("%8.2f",p->data[i*p->col+j]);
            }
            printf("\n");
        }
    }
    else if (p->type==LUA_TBOOLEAN)
    {
        for (int i=0;i<p->row;i++)
        {
            for (int j=0;j<p->col;j++)
            {
                if (p->data[i*p->col+j]==1)
                {
                    printf("  true");
                }
                else
                {
                    printf(" false");
                }
            }
            printf("\n");
        }
    }
    return 0;
}


static const struct luaL_Reg luamath_f [] = {
    {       "new",       lm_new},
    {  "identity",  lm_identity},
    {     "print",     lm_print},
    {        NULL,         NULL}
};

LUA_API int luaopen_luamathminimal (lua_State *L)
{
    luaL_newmetatable(L,LUAMATH_METATABLE);
    luaL_newlib(L, luamath_f);
    return 1;
}
