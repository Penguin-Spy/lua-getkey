/* Copyright © Penguin_Spy 2023
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <ncurses.h>

// gets a single character from stdin; blocking
int getkey(lua_State* L) {
  nodelay(stdscr, false); // enable delay
  lua_pushinteger(L, getch());
  return 1;
}

// checks if stdin has a character; returns immediatley
int haskey(lua_State* L) {
  nodelay(stdscr, true); // disable delay
  int has_char = getch();
  if(has_char != ERR) {
    ungetch(has_char); // put the char back
    lua_pushboolean(L, true);
  } else {
    lua_pushboolean(L, false);
  }
  nodelay(stdscr, false); // leave delay enabled
  return 1;
}

// continuously yields until stdin has a character, returns the character.
// (slightly) more efficient than equivalent code in Lua.
int getkey_co_k(lua_State* L, int status, lua_KContext ctx) {
  nodelay(stdscr, true); // disable delay
  int has_char = getch();
  if(has_char != ERR) {
    lua_pushinteger(L, has_char); // return the character
    nodelay(stdscr, false); // leave delay enabled
    return 1;
  } else {
    lua_yieldk(L, 0, 0, getkey_co_k); // continuously yield into this same function
  }
  nodelay(stdscr, false); // leave delay enabled
  return 0;
}
int getkey_co(lua_State* L) { // wrapper function to enter the yielding loop
  return getkey_co_k(L, 0, 0);
}

int luaopen_getkey(lua_State* L) {
  // initalize ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();
  endwin();

  // create the library table
  lua_newtable(L);
  lua_pushcfunction(L, getkey);
  lua_setfield(L, -2, "getkey");
  lua_pushcfunction(L, haskey);
  lua_setfield(L, -2, "haskey");
  lua_pushcfunction(L, getkey_co);
  lua_setfield(L, -2, "co");

  // make the library table callable as getkey
  lua_newtable(L);
  lua_pushcfunction(L, getkey);
  lua_setfield(L, -2, "__call");
  lua_setmetatable(L, -2);

  // return the library table
  return 1;
}
