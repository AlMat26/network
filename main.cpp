#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

void read_byte(const char* chr, size_t size)
{
    for (int i = 8*size - 1; i >= 0; i--)
    {
        cout << (((1 << i)&chr[i]) != 0);
    }
    cout << "\n";
}

void server_func()
{
    io_service service;
    ip::tcp::endpoint endpoint (ip::tcp::v4(), 6666);
    ip::tcp::acceptor acceptor(service, endpoint);
    ip::tcp::socket socket(service);

    acceptor.accept(socket);

    while (socket.is_open())
    {
        int dataSize;

        socket.receive(buffer(&dataSize, sizeof(int)));
        cout << "Recive: " << dataSize << " data size." << "\n";

        string message;
        char data [dataSize];

        socket.receive(buffer(data, dataSize));
        cout << "Recived: " << data << " with size: " << dataSize << "\n";

        message = data;
        message.resize(dataSize);

        cout << message << "\n";

        read_byte(message.data(), message.size());
    }
}

void client_func()
{
    io_service service;
    ip::tcp::endpoint endpoint;
    ip::tcp::socket socket(service);

    endpoint.port(6666);
    endpoint.address(ip::address::from_string("127.0.0.1"));

    socket.connect(endpoint);

    while(socket.is_open())
    {
        string message;
        getline(cin, message);
        int size = message.size();

        char data [size];
        strncpy(data, message.c_str(), size);

        cout << "message: " << message << "\nsize: " << size << "\nsizeof(size): " << sizeof(size) << "\n";

        socket.send(buffer(&size, sizeof(int)));
        cout << "Send: " << size << " with sizeof(int): " << sizeof(int) << "\n";
        socket.send(buffer(data, size));
        cout << "Send: " << message << " with sizeof(char): " << sizeof(char) << "\n";

        read_byte(message.c_str(), message.size());
    }
}

int main(int argc, char **argv) {

    string input = argv[1];

    try
    {
        if(input == "server")
            server_func();
        else if (input == "client")
            client_func();
        else
            cerr << "Неизвестный аргумент.\n";
    } catch (exception e)
    {
        cerr << e.what() << "\n";
    }

    return 0;
}
