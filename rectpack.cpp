
#include <vector>

#include "lua.hpp"

#include "GuillotineBinPack.h"
#include "MaxRectsBinPack.h"


#if (LUA_VERSION_NUM >= 502)
#define luaL_register(L,n,f)  luaL_newlib(L,f)
#endif

#define luaL_check(c, ...)		if (!(c)) luaL_error(L, __VA_ARGS__)

#define AUTHORS 	"Peter.Q"
#define VERSION		"LuaRectanglePacking 1.0"
#define RELEASE		"LuaRectanglePacking 1.0.1"

static int api_guillotine(lua_State *L)
{
	int args = lua_gettop(L);
	luaL_check(args >= 4, "need more args than '%d'", args);
	
	int w = luaL_checkint(L, 1);
	int h = luaL_checkint(L, 2);
	int f = luaL_checkint(L, 3);
	int g = luaL_checkint(L, 4);
	std::vector<RectSize> recs;
	for (int i = 5; i <= args; i += 3)
	{
		RectSize r;
		r.index = lua_tonumber(L, i);
		r.width = lua_tonumber(L, i + 1);
		r.height = lua_tonumber(L, i + 2);
		recs.push_back(r);
	}
	GuillotineBinPack pack(w, h);
	pack.Insert(recs, true, (GuillotineBinPack::FreeRectChoiceHeuristic)f, (GuillotineBinPack::GuillotineSplitHeuristic)g);
	std::vector<Rect> & rec = pack.GetUsedRectangles();
	lua_newtable(L);
	for (size_t i = 0; i < rec.size(); ++i)
	{
		lua_newtable(L);
		
		lua_pushnumber(L, rec[i].index);
		lua_rawseti(L, -2, 1);
		lua_pushnumber(L, rec[i].y);
		lua_rawseti(L, -2, 2);
		lua_pushnumber(L, rec[i].x);
		lua_rawseti(L, -2, 3);
		
		lua_rawseti(L, -2, i + 1);
	}
	lua_pushnumber(L, pack.Occupancy());
	return 2;
}

static int api_maxrect(lua_State *L)
{
	int args = lua_gettop(L);
	luaL_check(args >= 3, "need more args than '%d'", args);
	
	int w = luaL_checkint(L, 1);
	int h = luaL_checkint(L, 2);
	int f = luaL_checkint(L, 3);
	std::vector<RectSize> recs;
	for (int i = 4; i <= args; i += 3)
	{
		RectSize r;
		r.index = lua_tonumber(L, i);
		r.width = lua_tonumber(L, i + 1);
		r.height = lua_tonumber(L, i + 2);
		recs.push_back(r);
	}
	MaxRectsBinPack pack(w, h);
	std::vector<Rect> rec;
	pack.Insert(recs, rec, (MaxRectsBinPack::FreeRectChoiceHeuristic)f);
	lua_newtable(L);
	for (size_t i = 0; i < rec.size(); ++i)
	{
		lua_newtable(L);
		
		lua_pushnumber(L, rec[i].index);
		lua_rawseti(L, -2, 1);
		lua_pushnumber(L, rec[i].y);
		lua_rawseti(L, -2, 2);
		lua_pushnumber(L, rec[i].x);
		lua_rawseti(L, -2, 3);
		
		lua_rawseti(L, -2, i + 1);
	}
	lua_pushnumber(L, pack.Occupancy());
	return 2;
}

static const struct luaL_Reg thislib[] = {
	{"guillotine", api_guillotine},
	{"maxrect", api_maxrect},
	{NULL, NULL}
};

extern "C" {
	LUALIB_API int luaopen_rectpack (lua_State *L);
}

LUALIB_API int luaopen_rectpack (lua_State *L) {
	luaL_register(L, "rectpack", thislib);
	
	lua_pushstring(L, "VERSION");
	lua_pushstring(L, VERSION);
	lua_settable(L, -3);
	
	lua_pushstring(L, "RELEASE");
	lua_pushstring(L, RELEASE);
	lua_settable(L, -3);
	
	lua_pushstring(L, "AUTHORS");
	lua_pushstring(L, AUTHORS);
	lua_settable(L, -3);
	return 1;
}

