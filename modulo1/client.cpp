#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#include "socket.hpp"

#define TAM_MAX 4095

using namespace std;

int main(void){

    int client_socket = criar_socket();

    // configurando endereco do server
    struct sockaddr_in endereco_server = config_endereco("127.0.0.1");

    // conectando ao servidor
    conectar(client_socket, &endereco_server);
    cout << "Sucesso ao conectar ao server!\n";

    char buffer[TAM_MAX];
    string msg;

    //enviando mensagens para o servidor
    while(true){
        // se essa flag esta ativada ainda nao enviamos a mensagem anterior por completo, entao nao podemos ler uma nova mensagem
        cout << "Digite uma mensagem para enviar\n\t/exit - encerrar conexao\n";
        cin >> msg;

        // envia a mensagem para o servidor
        if(send(client_socket, msg.c_str(), msg.size(), 0) == -1){
            cout << "Erro ao enviar a mensagem D:\n";
            exit(-1);
        }
        
        // verifica se deve encerrar a conexao
        if(msg == "/exit"){
            break;
        }

        memset(buffer, 0, sizeof(buffer)); // setando o buffer para 0 para tirar lixo

        // recebe a mensagem enviada de volta do server
        int bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead == -1) {
            cout << "Erro ao receber a mensagem de volta do server\n";
        }

        cout << "Mensagem recebida pelo server foi: '" << buffer << "'\n";
    }

    // fechando o socket do client
    close(client_socket);

    return 0;
}