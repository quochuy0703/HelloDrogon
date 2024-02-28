#include <drogon/drogon.h>
// #include "./utils/ExcuteAwaiter.hpp"
#include "../plugins/SMTPMail.h"
#include "../utils/EmailHelper.hpp"

int main()
{
	// Set HTTP listener address and port
	//  drogon::app().addListener("0.0.0.0", 5555);
	// Load config file
	drogon::app().loadConfigFile("../config.json");
	drogon::app().setThreadNum(10);
	// drogon::app().loadConfigFile("../config.yaml");
	// Run HTTP framework,the method will block in the internal event loop
	drogon::app().registerHandler("/test_request_coro/{1}", [](drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback, std::string userId) -> drogon::AsyncTask
								  {
		auto client = drogon::HttpClient::newHttpClient("http://localhost:8000");
		Json::Value data;
		data["name"] = "Foo";
		data["description"] = "An optional description";
		data["price"] = 45.2;
		data["tax"] = 3.5;
		auto example_req = drogon::HttpRequest::newHttpJsonRequest(data);
		example_req->setPath("/items");
		example_req->setMethod(drogon::HttpMethod::Post);

		Json::Value ret;
		try {
			auto example_resp = co_await client->sendRequestCoro(example_req);
			if(example_resp->getBody().size() == 0)
				throw std::runtime_error("Why is there nothing");
			ret["request"] = example_resp->getBody().data();
		}
		catch(const std::exception& e) {
			std::cerr << "Error in request: " << e.what() << std::endl;
		}

		
		
		auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);

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
	drogon::app().registerHandler(
		"/mail2",
		[](const drogon::HttpRequestPtr &req,
		   std::function<void(const drogon::HttpResponsePtr &)> &&callback)
		{
			auto res = drogon::HttpResponse::newHttpResponse();
			res->setStatusCode(drogon::k200OK);

			std::vector<std::string> _to = {"test@test.com", "test2@test.com", "test3@test.com"};
			std::vector<std::string> _cc = {"test@test.com", "test2@test.com", "test3@test.com"};
			std::vector<std::string> _bcc = {"test@test.com", "test2bcc@test.com", "test3bcc@test.com"};

			app_helpers::email_helper::sendMail("huy@test.com", _to, "test mail", "Hello World!", _cc, _bcc);

			callback(res);
		});

	drogon::app().run();
	return 0;
}
