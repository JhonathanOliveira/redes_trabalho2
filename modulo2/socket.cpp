#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#include "socket.hpp"

using namespace std;

    // Cria um socket
    // Parâmetros:
    //      AF_INET: especifica domínio IPv4
    //      SOCK_STREAM: especifica que é um socket de fluxo
    //      0: define o protocolo, quando é zero o SO decide qual vai ser o melhor protocolo

int criar_socket(){
    int socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_ == -1){
        cout << "Erro ao criar socket!\n";
        exit(-1);
    }

    return socket_;
}

struct sockaddr_in config_endereco(string ip){
    struct sockaddr_in endereco_socket;
    endereco_socket.sin_family = AF_INET; // IPv4
    endereco_socket.sin_addr.s_addr = inet_addr(ip.c_str()); // Aceita conexões vindas do endereco IP do socket apenas
    endereco_socket.sin_port = htons(PORTA); // Define a porta em que o server está escutando

    return endereco_socket;
}

struct sockaddr_in config_endereco(){
    struct sockaddr_in endereco_socket;
    endereco_socket.sin_family = AF_INET; // IPv4
    endereco_socket.sin_addr.s_addr = INADDR_ANY; // Aceita conexões vindas de qualquer endereco IP
    endereco_socket.sin_port = htons(PORTA); // Define a porta em que o server está escutando

    return endereco_socket;
}

void conectar(int socket, struct sockaddr_in *endereco){
    if(connect(socket, (struct sockaddr*)endereco, sizeof(*endereco)) == -1){
        cout << "Erro ao conectar ao servidor!\n";
        exit(-1);
    }
}

void bind_(int socket, struct sockaddr_in &endereco) {
    if (bind(socket, (struct sockaddr*)&endereco, sizeof(endereco)) == -1) {
        cout << "Erro ao vincular socket ao endereco!(bind)\n";
        cout << strerror(errno) << "\n";
        exit(-1);
    }
}