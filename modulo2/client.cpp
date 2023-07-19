#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <csignal>

#include "socket.hpp"

#define TAM_MAX 4095

using namespace std;

// Função executada em uma thread separada para receber mensagens do servidor
void receiveMessages(int client_socket, string nickname)
{
    char buffer[TAM_MAX];
    int bytesRead;

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));

        // Recebe a mensagem enviada do servidor
        bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead == -1)
        {
            break;
        }
        else if (bytesRead == 0)
        {
            cout << "Conexão encerrada pelo servidor\n";
            break;
        }

        // Imprime a mensagem recebida
        cout << buffer << "\n";
        if(buffer == "Cliente (" + nickname + ") desconectou"){
            break;
        }
    }
}

int main(void)
{
    signal(SIGINT, handleSignal);
    
    int client_socket = criar_socket();

    // Configurando o endereço do servidor
    struct sockaddr_in endereco_server = config_endereco("127.0.0.1");

    cout << "[IRC Chat]\n\n";
    cout << "Comandos:\n";
    cout << "- /connect - Conectar ao servidor:\n";
    cout << "- /quit - Sair:\n";

    string action;
    getline(cin, action);

    while (action != "/connect") {

        if (action == "/quit" || cin.eof())
        {
            close(client_socket);
            return 0;
        }
        else {
            cout << "Comando inválido. Tente novamente\n";
            cout << "Comandos:\n";
            cout << "- /connect - Conectar ao servidor:\n";
            cout << "- /quit - Sair:\n";
            getline(cin, action);
        }

    }

    // Conectando ao servidor
    conectar(client_socket, &endereco_server);
    cout << "Sucesso ao conectar ao servidor!\n";

    // Definindo um nickname
    string nickname;
    cout << "Defina um nickname: ";
    getline(cin, nickname);

    // Envia o nickname para o servidor
    if (send(client_socket, nickname.c_str(), nickname.size(), 0) == -1)
    {
        cout << "Erro ao definir o nickname\n";
        exit(-1);
    }

    // Cria uma thread separada para receber mensagens do servidor
    thread receiveThread(receiveMessages, client_socket, nickname);

    // Enviando mensagens para o servidor
    string msg;

    while (true)
    {
        cout << "Digite uma mensagem para enviar\n\t/quit - Encerrar conexão\n\t/ping - Testar conexão com o servidor\n";
        getline(cin, msg);

        // Trata o caso EOF
        if (cin.eof())
        {
            msg = "/quit";
        }


        // Envia a mensagem para o servidor
        if (send(client_socket, msg.c_str(), msg.size(), 0) == -1)
        {
            cout << "Erro ao enviar a mensagem\n";

            exit(-1);
        }

        // Verifica se deve encerrar a conexão
        if (msg == "/quit")
        {
            break;
        }
    }

    // Fechando o socket do cliente
    close(client_socket);

    // Aguarda a thread de recebimento de mensagens terminar
    receiveThread.join();

    return 0;
}
