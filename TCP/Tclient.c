#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 

int main(int argc, char *argv[]) { 
    struct sockaddr_in server; 
    int sd; 
    char buffer[200]; 

    if (argc < 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("Socket failed"); 
        exit(1); 
    } 

    bzero(&server, sizeof(server)); 
    server.sin_family = AF_INET; 
    server.sin_port = htons(atoi(argv[2])); 
    inet_pton(AF_INET, argv[1], &server.sin_addr); 

    if(connect(sd, (struct sockaddr *)&server, sizeof(server)) < 0) { 
        perror("Connection failed"); 
        exit(1); 
    } 

    printf("Enter string to reverse: ");
    fgets(buffer, sizeof(buffer), stdin); 
    
    // Safety check for newline removal [cite: 2]
    size_t input_len = strlen(buffer);
    if (input_len > 0 && buffer[input_len - 1] == '\n') {
        buffer[input_len - 1] = '\0';
    }

    write(sd, buffer, sizeof(buffer));
    read(sd, buffer, sizeof(buffer)); 
    
    printf("Reversed string: %s\n", buffer); 
    
    close(sd); 
    return 0;
}
