//
//  main.c
//  IRIServer
//
//  Created by B.R.Niamut on 5/8/13.
//  Copyright (c) 2013 B.R.Niamut . All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define CONNECTIE_POORT "3490"  // De poort waar de connectie op luistert
#define GROOTTE_QUEUE 10     // Hoveel wachtende connecties er in de wachtrij zitten

void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int socket_file_descriptor, new_file_descriptor;  // de twee sockets. Luistert op socket_file_descriptor en krijgt nieuwe connecties op new_file_descriptor
    struct addrinfo hints, *serverinfo, *p;
    struct sockaddr_storage their_addr; //info van degene die geconnect heeft
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rv = getaddrinfo(NULL, CONNECTIE_POORT, &hints, &serverinfo)) != 0) {
        fprintf(stderr, "adressinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // ga door alle resultaten en connect bij degenen waar je kan
    for(p = serverinfo; p != NULL; p = p->ai_next) {
        if ((socket_file_descriptor = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        
        if (setsockopt(socket_file_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        
        if (bind(socket_file_descriptor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_file_descriptor);
            perror("server: bind");
            continue;
        }
        
        break;
    }
    
    if (p == NULL)  {
        fprintf(stderr, "Server kon niet binden \n");
        return 2;
    }
    
    freeaddrinfo(serverinfo); 
    
    if (listen(socket_file_descriptor, GROOTTE_QUEUE) == -1) {
        perror("luistert");
        exit(1);
    }
    
    sa.sa_handler = sigchld_handler; // pak alle dode processen
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    printf("server: wacht op connectie...\n");
    
    
    //Hier wordt gewacht op een connectie
    while(1) {
        sin_size = sizeof their_addr;
        new_file_descriptor = accept(socket_file_descriptor, (struct sockaddr *)&their_addr, &sin_size);
        if (new_file_descriptor == -1) {
            perror("accept");
            continue;
        }
        
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("Connectie van: %s\n", s);
        
        if (!fork()) { 
            close(socket_file_descriptor); 
            if (send(new_file_descriptor, "Connectie is actief", 13, 0) == -1)
                perror("send");
            close(new_file_descriptor);
            exit(0);
        }
        close(new_file_descriptor); 
    }
    
    return 0;
}

