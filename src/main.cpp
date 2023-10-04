#include "../inc/main.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// 533 - 50 = 483
// 541 - 50 = 491
#define RESPONSE_200 "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 483\r\n\r\n<!DOCTYPE html>\r\n<html lang='en'>\r\n<head>\r\n<meta charset='UTF-8'>\r\n<meta name='viewport' content='width=device-width, initial-scale=1.0'>\r\n<title>Document</title>\r\n<style>\r\n*{margin:0;padding:0;box-sizing:border-box;}\r\nbody {\r\nwidth: 100%;\r\nheight: 100vh;\r\ndisplay: flex;\r\nalign-items: center;\r\njustify-content: center;\r\nbackground-color: #2e2e2e;\r\ncolor: #fff;\r\n}\r\nh1 {\r\nfont-family: 'Courier New', Courier, monospace;\r\n}\r\n</style>\r\n</head>\r\n<body>\r\n<h1>ASTRO</h1>\r\n</body>\r\n</html>"
#define RESPONSE_404 "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 491\r\n\r\n<!DOCTYPE html>\r\n<html lang='en'>\r\n<head>\r\n<meta charset='UTF-8'>\r\n<meta name='viewport' content='width=device-width, initial-scale=1.0'>\r\n<title>Document</title>\r\n<style>\r\n*{margin:0;padding:0;box-sizing:border-box;}\r\nbody {\r\nwidth: 100%;\r\nheight: 100vh;\r\ndisplay: flex;\r\nalign-items: center;\r\njustify-content: center;\r\nbackground-color: #2e2e2e;\r\ncolor: #fff;\r\n}\r\nh1 {\r\nfont-family: 'Courier New', Courier, monospace;\r\n}\r\n</style>\r\n</head>\r\n<body>\r\n<h1>404 Not Found</h1>\r\n</body>\r\n</html>"

bool setSocketReuseAddress(int sockfd)
{
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    {
        perror("setsockopt");
        return false;
    }
    return true;
}

void mini_webserver()
{
    const char* ip_address = "127.0.0.1";
    int port = 5000;
    fd_set master;   // master file descriptor list
    fd_set read_fds; // temp file descriptor list for select()
    fd_set write_fds;
    int fdmax;

    // Create socket
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        std::cerr << "Socket error" << std::endl;
        exit(1);
    }

    if (!setSocketReuseAddress(server_sock))
    {
        std::cerr << "Failed to set SO_REUSEADDR option" << std::endl;
        close(server_sock);
        exit(1);
    }
    // Bind to address and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Bind error" << std::endl;
        close(server_sock);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0)
    {
        std::cerr << "Listen error" << std::endl;
        close(server_sock);
        exit(1);
    }

    std::cout << "Mini web server is listening on http://" << ip_address << ":" << port << std::endl;
    
    FD_ZERO(&master); // clear the master and temp sets
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_SET(server_sock, &master); // add the listener to the master set
    fdmax = server_sock;          // so far, it's this one

    while (true)
    {
        read_fds = master;  // copy it
        write_fds = master; // copy it
        
        if (select(fdmax + 1, &read_fds, &write_fds, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        for (int i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds)) // we got one for reading!!
            {
                if (i == server_sock)
                {
                    // handle new connections
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);

                    if (client_sock < 0)
                    {
                        std::cerr << "Accept error" << std::endl;
                    }
                    else
                    {
                        FD_SET(client_sock, &master); // add to master set
                        if (client_sock > fdmax)     // keep track of the max
                        {
                            fdmax = client_sock;
                        }
                        std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << " on socket " << client_sock << std::endl;
                    }
                }
                else
                {
                    // handle data from a client
                    char buffer[30000] = {0};
                    int nbytes = recv(i, buffer, sizeof(buffer), 0);

                    if (nbytes <= 0)
                    {
                        // got error or connection closed by client
                        if (nbytes == 0)
                        {
                            // connection closed
                            std::cout << "Socket " << i << " hung up" << std::endl;
                        }
                        else
                        {
                            perror("recv");
                        }
                        close(i);             // bye!
                        FD_CLR(i, &master);  // remove from master set
                    }
                    else
                    {
                        // process received data (modify this part as needed)
                        // For now, let's just echo the received data back to the client
                        std::cout << "Received: " << buffer << std::endl;
                        // You can modify this part to process the request and send the response back
                        // For now, let's just echo the received data back to the client
                        if (FD_ISSET(i, &write_fds)) // we can write to this socket
                        {
                            const char* response = RESPONSE_200;
                            send(i, response, strlen(response), 0);
                        }
                    }
                }
            }
        }
    }
    close(server_sock); // Close the server socket
}
// Accept incoming connection
// struct sockaddr_in client_addr;
// socklen_t client_len = sizeof(client_addr);
// int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
// // printing the request as it is
// char buffer[30000] = {0};
// read(client_sock, buffer, 30000);
// std::cout << buffer << std::endl;
// if (client_sock < 0)
// {
//     std::cerr << "Accept error" << std::endl;
//     continue;
// }

// // Send "hello world" response
// const char* response = RESPONSE_200;
// send(client_sock, response, strlen(response), 0);

// // Close the client socket
// close(client_sock);
int main()
{
    mini_webserver();
    return 0;
}
