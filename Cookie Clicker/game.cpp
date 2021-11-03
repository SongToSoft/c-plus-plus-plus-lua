#include "game.h"
#include <string>

game::game() {

}

game::~game() {
}

void game::test() {
    luaState = luaL_newstate();
    luaL_openlibs(luaState);
    if (checkLuaState(luaState, luaL_dofile(luaState, "luaScripts/testLuaScript.lua"))) {
        lua_getglobal(luaState, "a");
        if (lua_isnumber(luaState, -1)) {
            float result = (float)lua_tonumber(luaState, -1);
            std::cout << result << std::endl;
        }

        lua_getglobal(luaState, "AddStuff");
        if (lua_isfunction(luaState, -1)) {
            lua_pushnumber(luaState, 3.5f);
            lua_pushnumber(luaState, 7.1f);

            if (checkLuaState(luaState, lua_pcall(luaState, 2, 1, 0))) {
                std::cout << "[C++] call lua Script: " << (float)lua_tonumber(luaState, -1) << std::endl;
            }
        }
    }
    std::cout << "Wait click" << std::endl;
    std::getchar();
}

void game::start() {
    gameState = eGameState::STARTED;
    incomeTime = std::chrono::system_clock::now();

    luaState = luaL_newstate();
    luaL_openlibs(luaState);

    loadIncomePlaces();
    load();

    draw();
    update();
    lua_close(luaState);
}

void game::update() {
    while (gameState == eGameState::STARTED) {
        handleKeyboard();
        getIncome();
    }
}

void game::draw() {
    system("cls");
    printText("Press space to get Cookies");
    printText("Current Cookies: " + std::to_string(currentCount));
    printText("------------------------------------------------");
    int keyCount = 1;
    for (auto place : places) {
        printText(place.name + ": income - " + std::to_string(place.income) + ", cost - " + std::to_string(place.cost) + ", count - " + std::to_string(place.count) + ", [" + std::to_string(keyCount) + "] - press for buy");
        ++keyCount;
    }
    printText("Press ESC to quit with Save");
    if (!errorMsg.empty()) {
        printText(errorMsg);
        errorMsg = "";
    }
}

void game::handleKeyboard() {
    if (((GetAsyncKeyState(VK_SPACE) & 0x8000)) && !isSpacePressed) {
        isSpacePressed = true;
        changeCookies(clickIncome);
        draw();
    }
    if (GetAsyncKeyState(VK_SPACE) == 0 && isSpacePressed) {
        isSpacePressed = false;
    }

    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    {
        save();
        gameState = eGameState::ENDED;
    }

    for (auto key : buyKeyboards) {
        if (((GetAsyncKeyState(key.first) & 0x8000) && !pressedBuyKey.second)) {
            pressedBuyKey.first = key.first;
            pressedBuyKey.second = true;
            buyPlace(key.second);
        }
        if (GetAsyncKeyState(key.first) == 0 && pressedBuyKey.second && (pressedBuyKey.first == key.first)) {
            pressedBuyKey.second = false;
        }
    }
}

bool game::checkLuaState(lua_State* luaState, int r) {
    if (r == LUA_OK) {
        return true;
    }
    else {
        errorMsg = lua_tostring(luaState, -1);
        return false;
    }
}

void game::printLuaTable(lua_State* luaState) {
    lua_pushnil(luaState);

    while (lua_next(luaState, -2) != 0)
    {
        if (lua_isstring(luaState, -1)) {
            std::cout << lua_tostring(luaState, -2) << " = " << lua_tostring(luaState, -1) << std::endl;
        }
        else if (lua_isnumber(luaState, -1)) {
            std::cout << lua_tostring(luaState, -2) << " = " << lua_tonumber(luaState, -1) << std::endl;
        }
        else if (lua_istable(luaState, -1)) {
            printLuaTable(luaState);
            std::cout << std::endl;
        }

        lua_pop(luaState, 1);
    }
}

void game::printText(std::string text) {
    if (checkLuaState(luaState, luaL_dofile(luaState, "luaScripts/luaFunctions.lua"))) {
        lua_getglobal(luaState, "printText");
        if (lua_isfunction(luaState, -1)) {
            lua_pushstring(luaState, text.c_str());
            checkLuaState(luaState, lua_pcall(luaState, 1, 0, 0));
        }
    }
}

void game::loadIncomePlace(lua_State* luaState) {
    lua_pushnil(luaState);

    while (lua_next(luaState, -2) != 0)
    {
        if (lua_isstring(luaState, -1) && !lua_isnumber(luaState, -1)) {
            tmpPlace.name = lua_tostring(luaState, -1);
        }
        else if (lua_isnumber(luaState, -1)) {
            auto fieldName = (std::string)lua_tostring(luaState, -2);
            if (fieldName.find("Cost") != std::string::npos) {
                tmpPlace.cost = (int)lua_tonumber(luaState, -1);
            }
            else {
                tmpPlace.income = (int)lua_tonumber(luaState, -1);
            }
        }
        else if (lua_istable(luaState, -1)) {
            if (!tmpPlace.name.empty()) {
                places.push_back(tmpPlace);
            }
            loadIncomePlace(luaState);
        }

        lua_pop(luaState, 1);
    }
}

void game::loadIncomePlaces() {
    if (checkLuaState(luaState, luaL_dofile(luaState, "luaScripts/luaValues.lua"))) {
        lua_getglobal(luaState, "incomePlaces");
        loadIncomePlace(luaState);

        lua_getglobal(luaState, "playerClickIcome");
        clickIncome = (int)lua_tonumber(luaState, -1);

        lua_getglobal(luaState, "incomeTimeDif");
        incomeTimeDif = (float)lua_tonumber(luaState, -1);
    }
}

void game::changeCookies(int value) {
    if (checkLuaState(luaState, luaL_dofile(luaState, "luaScripts/luaFunctions.lua"))) {
        lua_getglobal(luaState, "increaseValues");
        if (lua_isfunction(luaState, -1)) {
            lua_pushnumber(luaState, currentCount);
            lua_pushnumber(luaState, value);

            if (checkLuaState(luaState, lua_pcall(luaState, 2, 1, 0))) {
                currentCount = (int)lua_tonumber(luaState, -1);
            }
        }
    }
}

void game::buyPlace(int placeCount) {
    if (checkLuaState(luaState, luaL_dofile(luaState, "luaScripts/luaFunctions.lua"))) {
        lua_getglobal(luaState, "compareValues");
        if (lua_isfunction(luaState, -1)) {
            lua_pushnumber(luaState, currentCount);
            lua_pushnumber(luaState, places[placeCount].cost);

            if (checkLuaState(luaState, lua_pcall(luaState, 2, 1, 0))) {
                auto canBuy = (int)lua_toboolean(luaState, -1);
                if (canBuy) {
                    changeCookies(-places[placeCount].cost);
                    places[placeCount].count += 1;
                    draw();
                }
            }
        }
    }
}

void game::getIncome() {
    if (((std::chrono::system_clock::now() - incomeTime) / 1s) > incomeTimeDif) {
        incomeTime = std::chrono::system_clock::now();
        for (auto place : places) {
            if (place.count != 0) {
                changeCookies(place.count * place.income);
                draw();
            }
        }
    }
}

void game::load() {
    std::string loadValues = "";
    if (checkLuaState(luaState, luaL_dofile(luaState, "luaScripts/luaFunctions.lua"))) {
        lua_getglobal(luaState, "readFromFile");
        if (lua_isfunction(luaState, -1)) {
            if (checkLuaState(luaState, lua_pcall(luaState, 0, 1, 0))) {
                size_t lenSize = 0;
                loadValues = (std::string)lua_tolstring(luaState, -1, &lenSize);
            }
        }
    }

    if (!loadValues.empty()) {
        std::string delimiter = " ";
        std::vector<std::string> values;
        size_t pos;
        while ((pos = loadValues.find(delimiter)) != std::string::npos) {
            values.push_back(loadValues.substr(0, pos));
            loadValues.erase(0, pos + delimiter.length());
        }
        for (size_t i = 0; i < places.size(); ++i) {
            places[i].count = std::stoi(values[i]);
        }
        currentCount = std::stoi(values[places.size()]);
    }
}

void game::save() {
    std::string result = "";
    for (auto place : places) {
        result += std::to_string(place.count) + " "; 
    }
    result += std::to_string(currentCount) + " ";

    if (checkLuaState(luaState, luaL_dofile(luaState, "luaScripts/luaFunctions.lua"))) {
        lua_getglobal(luaState, "writeToFile");
        if (lua_isfunction(luaState, -1)) {
            lua_pushstring(luaState, result.c_str());

            checkLuaState(luaState, lua_pcall(luaState, 1, 0, 0));
        }
    }
}
