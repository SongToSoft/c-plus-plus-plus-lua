#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <chrono>

#include <cstdlib>
#include <windows.h>

extern "C" {
#include "lua542/include/lua.h"
#include "lua542/include/lauxlib.h"
#include "lua542/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "lua542/liblua54.a")
#endif

using namespace std::chrono;

struct incomePlace {
	std::string name = "";
	int cost = 0;
	int income = 0;
	int count = 0;
};

enum class eGameState {
	UNDEFINED,
	STARTED,
	ENDED
};

class game {
public:
	game();
	~game();

	void start();
private:
	void update();
	void draw();
	void handleKeyboard();

	bool checkLuaState(lua_State* luaState, int r);
	void printLuaTable(lua_State* luaState);
	void printText(std::string text);
	void loadIncomePlace(lua_State* luaState);
	void loadIncomePlaces();
	void changeCookies(int value);
	void buyPlace(int placeCount);
	void getIncome();

	void load();
	void save();

	eGameState gameState = eGameState::UNDEFINED;
	lua_State* luaState;
	int clickIncome = 0;
	int currentCount = 0;
	std::vector<incomePlace> places;
	incomePlace tmpPlace;
	std::chrono::time_point<std::chrono::system_clock> incomeTime;
	float incomeTimeDif = 1.0f;
	bool isSpacePressed = false;
	std::pair<char, bool> pressedBuyKey = std::pair<char, bool>(' ', false);
	std::vector<std::pair<char, int>> buyKeyboards = { std::pair<char, int>('1', 0),
													   std::pair<char, int>('2', 1),
													   std::pair<char, int>('3', 2),
													   std::pair<char, int>('4', 3),
													   std::pair<char, int>('5', 4),
													   std::pair<char, int>('6', 5),
													   std::pair<char, int>('7', 6),
													   std::pair<char, int>('8', 7),
													   std::pair<char, int>('9', 8) };
};
