#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <string>
#include <boost/asio.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using namespace std;
using namespace boost::asio;

set<connection_hdl, owner_less<connection_hdl>> clients;
serial_port* serialPtr = nullptr;

void on_open(server* s, connection_hdl hdl) {
    clients.insert(hdl);
    cout << "🔗 Web client connected." << endl;
}

void on_close(server* s, connection_hdl hdl) {
    clients.erase(hdl);
    cout << "❌ Web client disconnected." << endl;
}

void on_message(server* s, connection_hdl hdl, server::message_ptr msg) {
    string payload = msg->get_payload();
    cout << "📩 Received command: " << payload << endl;

    if (serialPtr && serialPtr->is_open()) {
        try {
            write(*serialPtr, buffer(payload));
        } catch (boost::system::system_error& e) {
            cerr << "⚠️ Serial write error: " << e.what() << endl;
        }
    }
}

int main() {
    // 打开串口连接 Arduino
    io_service io;
    serial_port serial(io);

    try {
        serial.open("/dev/ttyACM0");  // 或 /dev/ttyUSB0
        serial.set_option(serial_port_base::baud_rate(9600));
        serial.set_option(serial_port_base::character_size(8));
        serial.set_option(serial_port_base::parity(serial_port_base::parity::none));
        serial.set_option(serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        serial.set_option(serial_port_base::flow_control(serial_port_base::flow_control::none));
        cout << "✅ Serial connected to Arduino." << endl;
    } catch (boost::system::system_error& e) {
        cerr << "❌ Failed to open serial port: " << e.what() << endl;
        return 1;
    }

    serialPtr = &serial;

    // 启动 WebSocket 服务器
    server ws_server;

    ws_server.init_asio();

    ws_server.set_open_handler(bind(&on_open, &ws_server, ::_1));
    ws_server.set_close_handler(bind(&on_close, &ws_server, ::_1));
    ws_server.set_message_handler(bind(&on_message, &ws_server, ::_1, ::_2));

    ws_server.listen(9002); // 监听端口
    ws_server.start_accept();

    cout << "🌐 WebSocket server started on ws://localhost:9002" << endl;
    ws_server.run();

    return 0;
}
