---@diagnostic disable: lowercase-global
package = "getkey"
version = "dev-1"
source = {
  url = "git+https://github.com/Penguin-Spy/lua-getkey.git"
}
description = {
  summary = "simple c module to get a single character from stdin.",
  homepage = "https://github.com/Penguin-Spy/lua-getkey",
  license = "MPL-2.0"
}
dependencies = {
  "lua ~> 5.4",
}
external_dependencies = {
  NCURSES = {
    header = "ncurses.h",
    library = "ncurses"
  }
}
build = {
  type = "builtin",
  modules = {
    getkey = {
      sources = "getkey.c",
      libraries = { "ncurses", "ncursesw" },
      incdirs = { "$(NCURSES_INCDIR)" },
      libdirs = { "$(NCURSES_LIBDIR)" }
    }
  }
}
