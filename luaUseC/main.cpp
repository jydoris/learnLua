//
//  main.cpp
//  forTest
//
//  Created by Doris on 2019/11/12.
//  Copyright © 2019 Doris. All rights reserved.
//

#include <iostream>
#include <cmath>
//#include "Header.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static int l_sin(lua_State *L){
    double d = lua_tonumber(L, 1);
    lua_pushnumber(L, sin(d));
    return 1;
}

static int l_cos(lua_State *L){
    double d = lua_tonumber(L, 1);
    lua_pushnumber(L, cos(d));
    return 1;
}


static const struct luaL_Reg mylib[] =
{
    {"mycos", l_cos},
    {nullptr, nullptr}
};

int open_mylib(lua_State * L)
{
    luaL_newlib(L, mylib);
    return 1;
}

int main(int argc, const char * argv[]) {

    lua_State *lua_state;
    lua_state = luaL_newstate();
    luaL_openlibs(lua_state);

    //第一种方式
//    lua_pushcfunction(lua_state, l_sin);
//    lua_setglobal(lua_state, "mysin");
    lua_register(lua_state, "mysin", l_sin);

    //第二种C模块的方式
    luaL_requiref(lua_state,"mylib",open_mylib,1);
    lua_pop(lua_state, 1);


    std::string scriptPath = "config.lua";
    int status = luaL_loadfile(lua_state, scriptPath.c_str()) || lua_pcall(lua_state, 0, 0, 0);
    if(status == LUA_OK)
    {
        //nothing
    }
    else
    {
        std::string res = lua_tostring(lua_state, -1);
        std::cout << res << std::endl;
    }

    lua_close(lua_state);

    return 0;
}

