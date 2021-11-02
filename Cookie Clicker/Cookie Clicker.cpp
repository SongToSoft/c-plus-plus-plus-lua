#include "game.h"


//void test() {
//    if (checkLuaState(luaState, luaL_dofile(luaState, "luaScripts/testLuaScript.lua"))) {
//        lua_getglobal(luaState, "a");
//        if (lua_isnumber(luaState, -1)) {
//            float result = (float)lua_tonumber(luaState, -1);
//            std::cout << result << std::endl;
//        }
//
//        lua_getglobal(luaState, "AddStuff");
//        if (lua_isfunction(luaState, -1)) {
//            lua_pushnumber(luaState, 3.5f);
//            lua_pushnumber(luaState, 7.1f);
//
//            if (checkLuaState(luaState, lua_pcall(luaState, 2, 1, 0))) {
//                std::cout << "[C++] call lua Script: " << (float)lua_tonumber(luaState, -1) << std::endl;
//            }
//        }
//    }
//    std::cout << "Wait click" << std::endl;
//}

int main()
{

    game game;
    game.start();

    return 0; 
}

