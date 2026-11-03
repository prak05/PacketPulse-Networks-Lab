#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 

int main(int argc, char *argv[]) { 
    struct sockaddr_in server, cli; 
    socklen_t cli_len; // Use socklen_t for portability
    int sd, data, i, len; 
    char temp; 
    char buffer[200]; // Increased to 200 to match client 

    if (argc < 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("Socket failed");
        exit(1); 
    } 

    bzero(&server, sizeof(server)); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(atoi(argv[1])); 
    server.sin_addr.s_addr = htonl(INADDR_ANY); 

    if(bind(sd, (struct sockaddr*) &server, sizeof(server)) < 0) { 
        perror("bind failed"); 
        exit(1); 
    }

    listen(sd, 5); 
    printf("Server listening on port %s...\n", argv[1]);

    // CRITICAL FIX: cli_len MUST be initialized 
    cli_len = sizeof(struct sockaddr_in); 
    if((data = accept(sd, (struct sockaddr *)&cli, &cli_len)) < 0) { 
        perror("accept failed");
        exit(1); 
    } 

    // Read exactly what the client sends
    read(data, buffer, sizeof(buffer)); 
    len = strlen(buffer); 

    // FIX: Use < len/2 to avoid swapping the middle character twice
    for(i = 0; i < len/2; i++) { 
        temp = buffer[i]; 
        buffer[i] = buffer[len - 1 - i]; 
        buffer[len - 1 - i] = temp; 
    } 

    write(data, buffer, sizeof(buffer)); 
    
    close(data); 
    close(sd); 
    return 0;
}
