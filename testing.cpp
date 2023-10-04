#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "4000" // port to connect to on the server

int main()
{
    const char *ip_address = "127.0.0.1";
    int client_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (client_sock < 0)
    {
        std::cerr << "Socket error" << std::endl;
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(PORT));
    server_addr.sin_addr.s_addr = inet_addr(ip_address);

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Connection error" << std::endl;
        close(client_sock);
        return 1;
    }

    const char *message = "Hello, server!";
    send(client_sock, message, strlen(message), 0);

    char buffer[30000] = {0};
    int nbytes = recv(client_sock, buffer, sizeof(buffer), 0);

    if (nbytes <= 0)
    {
        std::cerr << "Receive error" << std::endl;
    }
    else
    {
        std::cout << "Received: " << buffer << std::endl;
    }
}








#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "9034" 


class Client {
private:
    int id;
    std::string request;
    std::string response;

public:
    Client(int id, const std::string& request) : id(id), request(request) {}

    int getId() const {
        return id;
    }

    const std::string& getRequest() const {
        return request;
    }

    void setResponse(const std::string& res) {
        response = res;
    }

    const std::string& getResponse() const {
        return response;
    }
};

class SelectServer {
private:
    std::vector<Client> clients;
    fd_set master;
    fd_set read_fds;
    int fdmax;
    int listener;

public:
    SelectServer() : fdmax(0), listener(0) {
        FD_ZERO(&master);
        FD_ZERO(&read_fds);
    }

    ~SelectServer() {
        close(listener);
    }

    void run() {
        // (same as before)
        struct addrinfo hints, *ai, *p;
        int yes = 1;
        int rv;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if ((rv = getaddrinfo(nullptr, PORT, &hints, &ai)) != 0) {
            throw std::runtime_error("getaddrinfo: " + std::string(gai_strerror(rv)));
        }

        for (p = ai; p != nullptr; p = p->ai_next) {
            listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (listener == -1) {
                continue;
            }

            setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

            if (bind(listener, p->ai_addr, p->ai_addrlen) == -1) {
                close(listener);
                continue;
            }

            break;
        }

        freeaddrinfo(ai);

        if (p == nullptr) {
            throw std::runtime_error("Failed to bind");
        }

        if (listen(listener, 10) == -1) {
            throw std::runtime_error("Failed to listen");
        }

        FD_SET(listener, &master);
        fdmax = listener;

        while (true) {
            read_fds = master;

            if (select(fdmax + 1, &read_fds, nullptr, nullptr, nullptr) == -1) {
                throw std::runtime_error("Select error");
            }

            for (int i = 0; i <= fdmax; ++i) {
                if (FD_ISSET(i, &read_fds)) {
                    if (i == listener) {
                        acceptNewConnection();
                    } else {
                        handleDataFromClient(i);
                    }
                }
            }
        }
    }

private:
    // (same as before)

    void acceptNewConnection() {
        // (same as before)
    }

    void handleDataFromClient(int clientSocket) {
        char buf[256];
        int nbytes = recv(clientSocket, buf, sizeof buf, 0);

        if (nbytes <= 0) {
            if (nbytes == 0) {
                std::cout << "Socket " << clientSocket << " hung up" << std::endl;
            } else {
                perror("recv");
            }
            close(clientSocket);
            FD_CLR(clientSocket, &master);
            removeClient(clientSocket);
        } else {
            std::string request(buf, nbytes);
            int clientId = getClientId(clientSocket);
            if (clientId != -1) {
                clients[clientId].setResponse("Response for Client " + std::to_string(clientId) + ": " + request);
            }
        }
    }

    int getClientId(int clientSocket) const {
        auto it = std::find_if(clients.begin(), clients.end(), [clientSocket](const Client& c) {
            return c.getId() == clientSocket;
        });

        if (it != clients.end()) {
            return std::distance(clients.begin(), it);
        }
        return -1;
    }

    void removeClient(int clientSocket) {
        auto it = std::remove_if(clients.begin(), clients.end(), [clientSocket](const Client& c) {
            return c.getId() == clientSocket;
        });

        if (it != clients.end()) {
            clients.erase(it, clients.end());
        }
    }
};

int main() {
    try {
        SelectServer server;
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
