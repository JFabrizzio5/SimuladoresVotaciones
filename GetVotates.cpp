#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

// Callback function to handle the response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to extract value from JSON string
string extractValue(const string& json, const string& key) {
    size_t start = json.find("\"" + key + "\":");
    if (start == string::npos) return "";
    start = json.find(":", start) + 1;
    while (json[start] == ' ' || json[start] == '\"' || json[start] == ':') ++start;
    size_t end = json.find_first_of(",}", start);
    if (json[start] == '\"') --end;
    return json.substr(start, end - start);
}

int main() {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        // Set the URL for the GET request
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/2");

        // Set the callback function to handle the response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Pass the buffer to the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Disable SSL verification (not recommended for production)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // Perform the request and get the result code
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Extract and print the specific fields
            string userId = extractValue(readBuffer, "userId");
            string title = extractValue(readBuffer, "title");
            string body = extractValue(readBuffer, "body");

            cout << "ID del usuario: " << userId << endl;
            cout << "Titulo: " << title << endl;
            cout << "Información: " << body << endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}

