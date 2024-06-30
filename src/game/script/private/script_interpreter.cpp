#include "game/script/script_interpreter.h"

#include <absl/log/log.h>

#include "game/script/private/script_binding.h"

ScriptInterpreter::ScriptInterpreter(const std::string& file) {
  try {
    lua.open_libraries(
        // print, assert, and other base functions
        sol::lib::base,
        // require and other package functions
        sol::lib::package,
        // coroutine functions and utilities
        sol::lib::coroutine,
        // string library
        sol::lib::string,
        // all things math
        sol::lib::math,
        // the table manipulator and observer functions
        sol::lib::table,
        // the bit library
        sol::lib::bit32,
        // library for handling utf8: new to Lua
        sol::lib::utf8);

    lua.new_usertype<ScriptBinding>("UbahnScriptBinding", "new", sol::no_constructor, "instance",
                                    &ScriptBinding::Instance, "version", sol::var("0.1"), "addTrigger",
                                    &ScriptBinding::AddTrigger);

    lua.add_package_loader(
        +[](lua_State* raw_state) -> int {
          std::string module_name = sol::stack::get<std::string>(raw_state, 1);
          if (module_name == "ubahn") {
            std::string script = R"(
              return UbahnScriptBinding:instance()
            )";
            luaL_loadbuffer(raw_state, script.data(), script.size(), module_name.c_str());
          } else {
            sol::stack::push(raw_state,
                             std::format("U-Bahn interpreter could not provide module \"{}\"", module_name).c_str());
          }

          return 1;
        },
        true);

    lua.safe_script_file(file);
  } catch (const sol::error& e) {
    LOG(ERROR) << "Error during script initialization.";
  }
}

void ScriptInterpreter::CheckTriggers(const Train& train) { ScriptBinding::Instance()->CheckTriggers(train); }

void ScriptInterpreter::DrawDebug() { ScriptBinding::Instance()->DrawDebug(); }
