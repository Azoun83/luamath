#include "luamath_minimal_fun.h"


#define LUAMATH_METATABLE "luamathminimal"

static int lm_new(lua_State *L);
static int lm_identity(lua_State *L);
static int lm_print(lua_State *L);
