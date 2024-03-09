#include <iostream>
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;

string getResponse(const string& prompt) {
    const string endpoint = "https://api.openai.com/v1/completions";
    const string apiKey = "YOUR_OPENAI_API_KEY"; // Replace with your OpenAI API key

    http_client client(endpoint);
    uri_builder builder;

    builder.append_query("model", "text-davinci-003");
    builder.append_query("temperature", "0.7");
    builder.append_query("max_tokens", "150");

    http_request request(methods::POST);
    request.headers().set_content_type("application/json");
    request.headers().add("Authorization", "Bearer " + apiKey);

    json::value body;
    body["prompt"] = json::value::string(prompt);
    body["max_tokens"] = json::value::number(150);

    request.set_body(body);

    string response_text;

    client.request(request).then([&response_text](http_response response) {
        if (response.status_code() == status_codes::OK) {
            return response.extract_json();
        } else {
            throw runtime_error("Error with the request. Status code: " + to_string(response.status_code()));
        }
    }).then([&response_text](json::value jsonResponse) {
        response_text = jsonResponse.at("choices")[0].at("text").as_string();
    }).wait();

    return response_text;
}

int main() {
    cout << "Welcome to ChatGPT. Enter your prompt or type 'exit' to quit." << endl;

    string prompt;
    while (true) {
        cout << ">> ";
        getline(cin, prompt);

        if (prompt == "exit") {
            cout << "Goodbye!" << endl;
            break;
        }

        string response = getResponse(prompt);
        cout << "Response: " << response << endl;
    }

    return 0;
}
