// Server side C program to demonstrate Socket programming
#include "debug.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>

#define PORT 8080
#define SEND_CONTENT_0 "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"
#define SEND_CONTENT_1 "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 212\n\n\
<!DOCTYPE html>\
<html>\
    <header></header>\
    <body>\
        <form method=\"post\">\
            <input name=\"title\">\
            <input name=\"author\">\
            <input type=\"submit\">\
        </form>\
    </body>\
</html>"
#define SEND_CONTENT_2 "HTTP/1.1 200 OK\nContent-Type: text/plain\nSet-Cookie: BLYU=0\nContent-Length: 12\n\nHello world!"

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    char *hello = SEND_CONTENT_2;
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
bool optval = true;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        //for (;;)
        //{
            valread = read( new_socket , buffer, 30000);
            printf("%s(%zu)\n",buffer, valread);
        //}
        //FORx((unsigned char *)buffer, valread)
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}
