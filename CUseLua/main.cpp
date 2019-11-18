//
//  main.cpp
//  forTest
//
//  Created by Doris on 2019/11/12.
//  Copyright © 2019 Doris. All rights reserved.
//

#include <iostream>
#include <sstream>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

int pcall_callback_err_fun(lua_State* L)
{
    std::cout << "Inner error function " << std::endl;

    std::string err = lua_tostring(L, -1);
    std::cout << err ;
    lua_pop(L, 1); //这里即使不pop，当lua_pcall调用结束的时候，也会销毁栈里的信息

    lua_Debug debug;
    int ret = lua_getstack(L, 2, &debug); // 0是pcall_callback_err_fun自己, 1是error函数, 2是真正出错的函数
    if(ret != 1)
        std::cout << "get stack wrong.\n";
    lua_getinfo(L, "Sln", &debug); //S对应的是short_src，l对应的是currentline，n对应name和namewhat

    std::string res = debug.short_src;
    res += ":line " ;
    res += std::to_string(debug.currentline) ;
    if(debug.name != nullptr)
    {
        res += "(" ;
        res += debug.namewhat ;
        res += " " ;
        res += debug.name ;
        res += ")";
    }
    lua_pushstring(L,res.c_str());

    return 1;
}

int main(int argc, const char * argv[]) {
    //1. 初始化Lua虚拟机
    lua_State *lua_state;
    lua_state = luaL_newstate();

    luaL_openlibs(lua_state);
    std::string scriptPath = "config.lua";
    int status = luaL_loadfile(lua_state, scriptPath.c_str()) || lua_pcall(lua_state, 0, 0, 0);
    if(status == LUA_OK)
    {
        lua_pushcfunction(lua_state, pcall_callback_err_fun);
        int error_pos = lua_gettop(lua_state);
        lua_getglobal(lua_state,"myadd");
        lua_pushnumber(lua_state, 2);
        lua_pushnumber(lua_state, 3);
        if(lua_pcall(lua_state, 2, 1, error_pos) != 0)
        {
            std::string errorInfo = lua_tostring(lua_state, -1);
            std::cout << errorInfo << std::endl;
            lua_pop(lua_state, 1);
            int infoNum = lua_gettop(lua_state);
            std::cout << infoNum << std::endl;
        }
        else
        {
            if(lua_isnumber(lua_state, -1))
            {
                std::cout << "the result of add function is " << lua_tonumber(lua_state, -1) << std::endl;
                lua_pop(lua_state, 1);
            }
            else
            {
                std::cout<< "Wrong when get the result of function myadd.\n";
            }
        }
    }
    else
    {
        std::cout << "Error occured when opening lua file!" << std::endl;
    }

    lua_close(lua_state);

    return 0;
}


//extern "C"
//{
//#include "lua.h"
//#include "lualib.h"
//#include "lauxlib.h"
//};
//
//
//
//
//int pcall_callback_err_fun(lua_State* L)
//{
//
//    if(!lua_isstring(L,1))                      //错误信息不是string类型
//        return 1;                               // 不做处理
//    lua_getfield(L,-1,"debug");    //获取debug函数，压入栈中
//    if(lua_istable(L,-1))
//    {
//        lua_pop(L,1);
//        return 1;
//    }
//    lua_getfield(L,-1,"traceback");             //获取debug.tracenack函数,压入栈中
//    if(lua_isfunction(L,-1))
//    {
//        lua_pop(L,2);
//        return 1;
//    }
//    lua_pushvalue(L,1);                         // 传递错误信息
//    lua_pushinteger(L,2);                       // skip this function and traceback
//    lua_call(L,2,1);                            // 调用debug.traceback
//    return 1;
//}
//
//
//int main()
//{
//    lua_State * L = luaL_newstate();    //创建lua运行环境
//    luaL_openlibs(L);
//    int ret = luaL_loadfile(L,"config.lua") ;       //加载lua脚本文件
//    ret = lua_pcall(L, 0, 0, 0);
//
//
//    lua_pushcfunction(L, pcall_callback_err_fun);
//    int pos_err = lua_gettop(L);
//    lua_getglobal(L,"f");               //调用lua中的函数f
//    ret = lua_pcall(L, 0, 1, pos_err) ;
//    if ( ret != 0 )
//    {
//        int t = lua_type(L, -1);
//        const char* err = lua_tostring(L,-1);
//        printf("Error: %s\n", err);
//        lua_pop(L, 1);
//    }
//    lua_close(L) ;
//
//
//    return 0;
//}
