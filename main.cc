#include <drogon/drogon.h>
// #include "./utils/ExcuteAwaiter.hpp"

int main()
{
	// Set HTTP listener address and port
	//  drogon::app().addListener("0.0.0.0", 5555);
	// Load config file
	drogon::app().loadConfigFile("../config.json");
	// drogon::app().loadConfigFile("../config.yaml");
	// Run HTTP framework,the method will block in the internal event loop
	drogon::app().registerHandler("/test_request_coro/{1}", [](drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback, std::string userId) -> drogon::AsyncTask
								  {
		auto client = drogon::HttpClient::newHttpClient("http://localhost:8000");
		auto example_req = drogon::HttpRequest::newHttpRequest();
		example_req->setPath("/");

		try {
			auto example_resp = co_await client->sendRequestCoro(example_req);
			if(example_resp->getBody().size() == 0)
				throw std::runtime_error("Why is there nothing");
		}
		catch(const std::exception& e) {
			std::cerr << "Error in request: " << e.what() << std::endl;
		}
		auto resp = drogon::HttpResponse::newHttpResponse();
		callback(resp); });
	// drogon::app().registerHandler("/test_request_coro2/{1}", [](drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback, std::string userId) -> drogon::Task<>
	// 							  {
	// 	co_await executeIntensiveFunction([]() {
	//         using namespace std::chrono_literals;
	//         std::this_thread::sleep_for(10000ms);
	//         return;
	//     });

	//     auto resp = HttpResponse::newHttpResponse();
	//     resp->setBody("Hi there, this is another hello from the sleep2Hello Controller");
	//     callback(resp);
	//     co_return; });
	drogon::app().run();
	return 0;
}
