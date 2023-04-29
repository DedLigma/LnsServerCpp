#include <cstdlib>
#include <drogon/drogon.h>
#include <fstream>

typedef std::function<void(const drogon::HttpResponsePtr &)> Callback;

void indexHandler(const drogon::HttpRequestPtr &request, Callback &&callback) {
  Json::Value jsonBody;
  jsonBody["message"] = std::time(nullptr);

  // Формируем и отправляем ответ с JSON-объектом
  auto response = drogon::HttpResponse::newHttpJsonResponse(jsonBody);
  callback(response);
}

void changeHandler(const drogon::HttpRequestPtr &request, Callback &&callback) {
  Json::Value jsonBody;

  jsonBody["message"] =   " jigreiger";
  auto response = drogon::HttpResponse::newHttpJsonResponse(jsonBody);
  callback(response);
}

int main() {

  drogon::app()
      .registerHandler("/state", &indexHandler, {drogon::Get})
      .registerHandler("/change", &changeHandler, {drogon::Get})
      .addListener("127.0.0.1", 3001)
      .setThreadNum(8)
      .enableServerHeader(false)
      .run();

  return EXIT_SUCCESS;
}
