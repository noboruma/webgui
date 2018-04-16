#include <pistache/endpoint.h>
#include <pistache/net.h>

#include <thread>

char const * raw_index_html =
#include "index.html"
;

namespace {
    struct HelloHandler : public Pistache::Http::Handler
    {
        HTTP_PROTOTYPE(HelloHandler)

        void onRequest(const Pistache::Http::Request& request, Pistache::Http::ResponseWriter writer)
        {
            if (request.resource() == "/resources/js/drawMainCanvas.js") {
                Pistache::Http::serveFile(writer, "../resources/js/drawMainCanvas.js");
            } else {
                writer.send(Pistache::Http::Code::Ok, raw_index_html);
            }
        }
    };
}

#define WEBVIEW_IMPLEMENTATION
#include <webview/webview.h>
#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInt, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
#else
int main(int argc, char const **argv) {
#endif
    Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(9080));

    auto opts = Pistache::Http::Endpoint::options().threads(1);
    Pistache::Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(std::make_shared<HelloHandler>());
    server.serveThreaded();

    std::thread client([addr=std::move(addr)]()
                       {
                           auto protocol = "http://";
                           auto host = protocol+addr.host()+":";
                           host+=std::to_string(static_cast<int16_t>(addr.port()));
                           webview("self-hosted Hello, world",
                                   host.c_str(), 800, 600, 1);
                       });

    client.join();

    return 0;
}
