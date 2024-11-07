// gcc ncurses.c -l ncurses -o ncurses && ./ncurses

#include <stdio.h>
#include<netinet/in.h>
#include <unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>

int client_create(){
    int s=socket(AF_INET,SOCK_STREAM,0);//creating a socket(ipv4,tcp,type)
    struct sockaddr_in addr;//declaring a structure of server
    if(s<0) {
        perror("Socket creation failed");
        return 1;
    }
    addr.sin_addr.s_addr=inet_addr("52.220.155.145");//serverip
    addr.sin_family=AF_INET;//address family(ipv4)
    addr.sin_port=htons(80);
    if(connect(s,(struct sockaddr*)&addr,sizeof(addr))<0) {
        perror("Bind failed");
        return 1;
    }
    char *msg="GET / HTTP/1.1\r\nHost: https://www.ronit-ahuja.netlify.app\r\n\r\n";
    send(s,msg,strlen(msg),0);
    char buffer[1024]={0};
    recv(s,buffer,1024,0);
    printf("%s",buffer);
    close(s);
    return 0;
}
int main() {
    client_create();
}
