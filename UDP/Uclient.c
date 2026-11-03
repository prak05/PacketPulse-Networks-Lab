#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<fcntl.h>
#include<stdlib.h> 
#include<unistd.h>  

int main(int argc, char *argv[]) 
{ 
    int i, j, n; 
    int sock_fd; 
    struct sockaddr_in servaddr;
    int matrix_1[10][10], matrix_2[10][10], matrix_product[10][10]; 
    int size[2][2]; 
    int num_rows_1, num_cols_1, num_rows_2, num_cols_2; 
    
    if(argc != 3) 
    { 
        fprintf(stderr, "Usage: ./Uclient IPaddress_of_server port\n"); 
        exit(1); 
    } 
    
    printf("Enter the number of rows of first matrix: ");
    scanf("%d", &num_rows_1); 
    
    printf("Enter the number of columns of first matrix: ");
    scanf("%d", &num_cols_1); 
    
    // Bounds checking
    if(num_rows_1 > 10 || num_cols_1 > 10) {
        printf("Matrix dimensions exceed 10x10 limit\n");
        exit(1);
    }
    
    printf("Enter the values row by row one on each line:\n");
    for (i = 0; i < num_rows_1; i++) 
        for (j = 0; j < num_cols_1; j++) 
        { 
            scanf("%d", &matrix_1[i][j]); 
        } 
    
    size[0][0] = num_rows_1; 
    size[0][1] = num_cols_1; 
    
    printf("Enter the number of rows of second matrix: ");
    scanf("%d", &num_rows_2); 
    
    printf("Enter the number of columns of second matrix: ");
    scanf("%d", &num_cols_2); 
    
    if(num_cols_1 != num_rows_2) 
    { 
        printf("MATRICES CANNOT BE MULTIPLIED (cols1 != rows2)\n"); 
        exit(1); 
    }
    
    // Bounds checking for second matrix
    if(num_rows_2 > 10 || num_cols_2 > 10) {
        printf("Matrix dimensions exceed 10x10 limit\n");
        exit(1);
    }
    
    printf("Enter the values row by row one on each line:\n");
    for (i = 0; i < num_rows_2; i++) 
        for(j = 0; j < num_cols_2; j++) 
        {
            scanf("%d", &matrix_2[i][j]); 
        } 
    
    size[1][0] = num_rows_2; 
    size[1][1] = num_cols_2; 
    
    if((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        printf("Cannot create socket\n"); 
        exit(1); 
    } 
    
    bzero((char*)&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(atoi(argv[2])); 
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        printf("Invalid IP address: %s\n", argv[1]);
        close(sock_fd);
        exit(1);
    }
    
    printf("Connecting to server %s:%s...\n", argv[1], argv[2]);
    
    
    n = sendto(sock_fd, size, sizeof(size), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    if(n < 0) 
    { 
        perror("error in size sending"); 
        close(sock_fd);
        exit(1); 
    } 
    
    
    n = sendto(sock_fd, matrix_1, sizeof(matrix_1), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    if(n < 0) 
    { 
        perror("error in matrix 1 sending");
        close(sock_fd);
        exit(1); 
    } 
    
    
    n = sendto(sock_fd, matrix_2, sizeof(matrix_2), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    if(n < 0) {
        perror("error in matrix 2 sending"); 
        close(sock_fd);
        exit(1);
    }
    
    
    if((n = recvfrom(sock_fd, matrix_product, sizeof(matrix_product), 0, NULL, NULL)) == -1) 
    { 
        perror("read error from server:");
        close(sock_fd);
        exit(1); 
    } 
    
    printf("\n\nTHE PRODUCT OF MATRICES IS:\n\n");
    for(i = 0; i < num_rows_1; i++) 
    { 
        for(j = 0; j < num_cols_2; j++) 
        { 
            printf("%d\t", matrix_product[i][j]); 
        } 
        printf("\n"); 
    } 
    
    close(sock_fd);
    return 0;
}

