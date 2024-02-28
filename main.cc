#include <drogon/drogon.h>
#include <iostream>
#include <chrono>
#include "./utils/ExecuteAwaiter.hpp"
#include "./utils/EmailHelper.hpp"

using namespace drogon;

using namespace std::chrono;

int main()
{
	// Set HTTP listener address and port
	// drogon::app().addListener("0.0.0.0", 5555);
	// Load config file
	drogon::app().loadConfigFile("../config.json");
	// drogon::app().loadConfigFile("../config.yaml");
	// Run HTTP framework,the method will block in the internal event loop
	drogon::app().registerHandler("/test_request_coro/{1}", [](drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback, std::string userId) -> drogon::AsyncTask
								  {
		auto client = drogon::HttpClient::newHttpClient("https://fakestoreapi.com");
		auto example_req = drogon::HttpRequest::newHttpRequest();
		example_req->setPath("/products/1");
		Json::Value ret;
		try {
			auto example_resp = co_await client->sendRequestCoro(example_req);
			if(example_resp->getBody().size() == 0)
				throw std::runtime_error("Why is there nothing");
			Json::Reader reader;
			reader.parse(example_resp->getBody().data(), ret);
		}
		catch(const std::exception& e) {
			std::cerr << "Error in request: " << e.what() << std::endl;
		}
		
		auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
		callback(resp); });

	drogon::app().registerHandler("/test_future/{1}", [](const drogon::HttpRequestPtr req, std::function<void(const drogon::HttpResponsePtr &)> callback, std::string userId) -> drogon::Task<>
								  {
									  // auto client = drogon::HttpClient::newHttpClient("http://localhost:8000");
									  // auto example_req = drogon::HttpRequest::newHttpRequest();
									  // example_req->setPath("/");

									  // Json::Value ret;

									  // std::promise<bool> valid;
									  // try {
									  // 	client->sendRequest(example_req, [&]( drogon::ReqResult result, const drogon::HttpResponsePtr &response){
									  // 		std::cout << "Send request" << std::endl;
									  // 		if(result == ReqResult::Ok){
									  // 			auto data = response->getBody().data();
									  // 			Json::Reader reader;
									  // 			reader.parse(data, ret);
									  // 			valid.set_value(true);
									  // 		}else{
									  // 			std::cout << "Error" << std::endl;
									  // 			valid.set_value(false);
									  // 		};
									  // 	});
									  // }
									  // catch(const std::exception& e) {
									  // 	std::cerr << "Error in request: " << e.what() << std::endl;
									  // }

									  // std::cout << "After Send request" << std::endl;
									  // bool api_ok = valid.get_future().get();
									  // auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
									  // callback(resp);
									  co_await app_helpers::execute_awaiter::executeIntensiveFunction([]()
																		{
										std::this_thread::sleep_for(seconds(30));
										return; });

									  auto resp = HttpResponse::newHttpResponse();
									  resp->setBody("Hi there, this is another hello from the sleep2Hello Controller");
									  callback(resp); 
									  co_return; });

	drogon::app().registerHandler("/mail2", [](const HttpRequestPtr &req,
											   std::function<void(const HttpResponsePtr &)> &&callback)
								  {

		Json::Value ret;
		app_helpers::email_helper::sendMail();
		ret["test"] = true;
		auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
		callback(resp); });
	drogon::app().run();
	return 0;
}
