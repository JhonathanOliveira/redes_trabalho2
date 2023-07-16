#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#define TAM_MAX 4095

using namespace std;

int main(void){

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1){
        cout << "Erro ao criar o socket do client";
        exit(-1);
    }

    // configurando endereco do server
    struct sockaddr_in endereco_server;
    endereco_server.sin_family = AF_INET; // ipv4
    endereco_server.sin_addr.s_addr = inet_addr("127.0.0.1"); // aceita conexoes vindas do endereco IP do server apenas
    endereco_server.sin_port = htons(12345); // definindo a porta em que o server esta escutando

    // conectando ao servidor
    if(connect(client_socket, (struct sockaddr*)&endereco_server, sizeof(endereco_server)) == -1){
        cout << "Erro ao conectar ao servidor!\n";
        exit(-1);
    }
    cout << "Sucesso ao conectar ao server!\n";

    char buffer[TAM_MAX];
    string msg;

    //enviando mensagens para o servidor
    while(true){
        // se essa flag esta ativada ainda nao enviamos a mensagem anterior por completo, entao nao podemos ler uma nova mensagem
        cout << "Digite uma mensagem para enviar(/exit - encerrar conexao)\n";
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