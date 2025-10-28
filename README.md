# WicSave 
a lib to save whatever you want as a key, value elements allowing to backup your struct


```cpp
#include <wicsave/SaveData.hpp>
#include <iostream>

int main() {
    wicsave::SaveData save("player.wic");

    // Store player data
    int level = 10;
    float health = 83.2f;
    std::string name = "Alyx";

    save.AddData("level", level, sizeof(level));
    save.AddData("health", health, sizeof(health));
    save.AddData("name", name, name.size());

    save.Save(); // writes to "player.wic"

    // Later...
    wicsave::SaveData load("player.wic");
    load.Load();

    std::cout << "Loaded level: " << std::any_cast<int>(load.GetData("level")) << "\n";
    std::cout << "Player: " << std::any_cast<std::string>(load.GetData("name")) << "\n";
}
```

## integration 
using premake you can taillor it for each project you want (based for wicgames which use [premake](https://premake.github.io) )
