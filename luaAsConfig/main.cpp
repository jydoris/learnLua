//
//  main.cpp
//  forTest
//
//  Created by Doris on 2019/11/12.
//  Copyright © 2019 Doris. All rights reserved.
//

#include <iostream>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


struct color
{
    std::string name;
    float r,g,b;
};
color myColor[4] = {
    {"red", 1.0, 0.0, 0.0},
    {"green", 0.0, 1.0, 0.0},
    {"blue", 0.0, 0.0, 1.0},
    {"null", 0.0, 0.0, 0.0}
};
void setfield(lua_State *L, std::string key, float value)
{
    lua_pushstring(L, key.c_str());
    lua_pushnumber(L, value);
    lua_settable(L, -3);
}

void setColor(lua_State *L, color c)
{
    lua_newtable(L);
    setfield(L, "r", c.r);
    setfield(L, "g", c.g);
    setfield(L, "b", c.b);
    lua_setglobal(L, c.name.c_str());
}



float getfield(lua_State *L, std::string key)
{
//    lua_pushstring(L, key.c_str());
//    lua_gettable(L, -2);
    lua_getfield(L, -1, key.c_str());
    if (!lua_isnumber(L, -1)) {
        std::cout << "the color is not a number!" << std::endl;
    }
    float res = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return res;
}

int main(int argc, const char * argv[]) {
    //1. 初始化Lua虚拟机
    lua_State *lua_state;
    lua_state = luaL_newstate();

    int i = 0;
    while(myColor[i].name != "null")
        setColor(lua_state, myColor[i++]);

    std::string scriptPath = "config.lua";
    int status = luaL_loadfile(lua_state, scriptPath.c_str()) || lua_pcall(lua_state, 0, LUA_MULTRET, 0);
    if(status == LUA_OK)
    {
        lua_getglobal(lua_state,"background");
        float red = getfield(lua_state, "r");
        float green = getfield(lua_state, "g");
        float blue = getfield(lua_state, "b");
        std::cout << "The color is (" << red  << "," << green << "," << blue<<")" << std::endl;
    }
    else
    {
        std::cout << "Error occured!" << std::endl;
    }

    lua_close(lua_state);

    return 0;
}
