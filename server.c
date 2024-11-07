#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

int server_create() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);  // Creating a socket
    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Set up the server address structure
    server_addr.sin_family = AF_INET;  // IPv4
    server_addr.sin_port = htons(80);  // HTTP port
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept any incoming connections

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port 80...\n");

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        return 1;
    }

    printf("Client connected\n");

    char buffer[1024] = {0};  
    int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);  
    if (bytes_received < 0) {
        perror("Receive failed");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    printf("Request received: %s\n", buffer); 

    // Send an HTTP response
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>";
    send(client_fd, response, strlen(response), 0);  

    close(client_fd);
    close(server_fd);

    return 0;
}

int main() {
    return server_create();  // Start the server
}
