#include <pistache/endpoint.h>

#include <thread>
#include <mutex>
#include <chrono>

#include "html.h"

using namespace Pistache;

namespace {
    struct HelloHandler : public Http::Handler {
        HTTP_PROTOTYPE(HelloHandler)

        void onRequest(const Http::Request& request, Http::ResponseWriter writer) {
            if (request.resource() == "/resources/js/drawMainCanvas.js") {
                Http::serveFile(writer, "../resources/js/drawMainCanvas.js");
            } else {

                std::string s(___html,
                              ___html_len);
                writer.send(Http::Code::Ok, s.c_str());
            }
        }
    };
}

#define WEBVIEW_IMPLEMENTATION
#include <webview/webview.h>

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInt, HINSTANCE hPrevInst, LPSTR lpCmdLine,
                   int nCmdShow) {
#else
int main(int argc, char const **argv) {
#endif
    std::thread server([](){
        Http::listenAndServe<HelloHandler>("*:9080");
    });
    std::thread client([](){
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
        webview("self-hosted Hello, world",
                "http://localhost:9080", 800, 600, 1);
    });

    client.join();
    server.join();

    return 0;
}
