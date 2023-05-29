#include "./Game/Game.h"
#include <sol/sol.hpp>
#include <iostream>

void TestLua() {
    sol::state lua;

    lua.open_libraries(sol::lib::base);
    lua.script_file("./assets/scripts/myscript.lua");

    int test = lua["some_variable"];
    //std::cout << "the value of test is " << test << std::endl;

    bool isFullScreen = lua["config"]["fullscreen"];
   // std::cout << "We read the value fullscreen: " << isFullScreen << std::endl;

    sol::table config = lua["config"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];

    //std::cout << "From the config we have the width: " << width << " and the height: " << height << std::endl;

    int factorial = lua["factorial_number"];
    std::cout << "The factorial number is " << factorial << std::endl;

    sol::optional<double> secondTest = lua["config"]["test_value"];
    if (secondTest != sol::nullopt)
        std::cout << secondTest.value() << std::endl;
    else
        std::cout << "No such value exists" << std::endl;
}

int main(int argc, char* argv[]) {

    //Game game;

    //game.Initialize();
    //game.Run();
    //game.Destroy();

    TestLua();

    return 0;
}
