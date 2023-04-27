#include <drogon/drogon.h>
#include <iostream>
int main() {

    std::cout << "server start12312312" << std::endl;
    //Set HTTP listener address and port
    drogon::app().addListener("127.0.0.14",3010);
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
