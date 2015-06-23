#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


char message[] = "Hello there!\n";
char buf[sizeof(message)];

int main()
{
    int sock;
    struct sockaddr_in addr;
    socklen_t len;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    len = sizeof(addr);

    printf("SEND\n");
    if (sendto(sock, message, sizeof(message) , 0 , (struct sockaddr *) &addr, len)==-1)
        {
            perror("sendto");
        }
    if (recvfrom(sock, buf, sizeof(message), 0, (struct sockaddr *) &addr, &len) == -1)
        {
            perror("recvfrom");
        }
    
    printf(buf);
    close(sock);

    return 0;
}