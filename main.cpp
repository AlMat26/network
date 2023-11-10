#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

vector<ip::tcp::socket*> sockets;

void read_byte(const char* chr, size_t size)
{
    for (int j = 0; j < size; j++)
    {
        for (int i = 7; i >= 0; i--)
        {
            cout << (((1 << i)&chr[j]) != 0);
        }
        cout << " ";
    }
    cout << "\n";
}

void server_message_thread(ip::tcp::socket* socket)
{
    while(socket->is_open())
    {
        int dataSize;

        socket->receive(buffer(&dataSize, sizeof(int)));
        cout << "Recive: " << dataSize << " data size." << "\n";

        string message;
        char data [dataSize];

        socket->receive(buffer(data, dataSize));

        message = string(data, dataSize);
        cout << "Recived: " << message << " with size: " << dataSize << "\n";

        read_byte(message.data(), message.size());
    }
}

void server_acceptor_thread(io_service* service, bool* running)
{
    vector<thread*> messageThreads;

    while(running)
    {
        ip::tcp::endpoint endpoint(ip::tcp::v4(), 6666);
        ip::tcp::acceptor acceptor(*service, endpoint);
        sockets.push_back(new ip::tcp::socket(*service));

        acceptor.accept(*sockets[sockets.size() - 1]);

        messageThreads.push_back(new thread(bind(server_message_thread, sockets[sockets.size()-1])));
    }
}

void server_func()
{
    bool running = true;
    io_service service;

    thread connectionThread(bind(server_acceptor_thread, &service, &running));

    while (running)
    {
        string input;
        getline(cin, input);
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

    string input;

    if(argc > 1)
        input = argv[1];

    if(input == "server")
        server_func();
    else if (input == "client")
        client_func();
    else
        cerr << "Неизвестный аргумент.\n";


    return 0;
}
