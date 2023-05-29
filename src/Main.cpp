#include "./Game/Game.h"
#include <sol/sol.hpp>
#include <iostream>

int nativeCPPCubeFunction(int n) {
    return n * n * n;
}

void TestLua() {
    sol::state lua;

    lua.open_libraries(sol::lib::base);

    //Cpp function exposition to Lua
    lua["cube"] = nativeCPPCubeFunction;

    lua.script_file("./assets/scripts/myscript.lua");

    int test = lua["some_variable"];
    //std::cout << "the value of test is " << test << std::endl;

    bool isFullScreen = lua["config"]["fullscreen"];
   // std::cout << "We read the value fullscreen: " << isFullScreen << std::endl;

    sol::table config = lua["config"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];

    //std::cout << "From the config we have the width: " << width << " and the height: " << height << std::endl;


    sol::function functionFactorial = lua["factorial"];
    int functionResult = functionFactorial(7);
    std::cout << "The Lua script calculated 5! as " << functionResult << std::endl;
}

int main(int argc, char* argv[]) {

    //Game game;

    //game.Initialize();
    //game.Run();
    //game.Destroy();

    TestLua();

    return 0;
}
