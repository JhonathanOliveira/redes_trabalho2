#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <map>
#include <thread>

#include "socket.hpp"

#define TAM_MAX 4096

using namespace std;

// Função executada em cada thread de cliente
void handleClient(int client_socket, const string& nickname)
{
    char buffer[TAM_MAX];
    string msg;
    bool flag = true;

    while (flag)
    {
        memset(buffer, 0, sizeof(buffer));

        if (recv(client_socket, buffer, sizeof(buffer) - 1, 0) == -1)
        {
            cout << "Erro ao receber a mensagem do cliente T-T\n";
        }

        msg = buffer;

        if (msg == "/exit")
        {
            cout << "Cliente (" << nickname << ") solicitou /exit\n";
            cout << "Encerrando conexão com o cliente (" << nickname << ").\n";
            msg = "Encerrando conexão com o cliente.\n";
            flag = false;
        }
        else
        {
            cout << "(" << nickname << "): " << msg << "\n";
        }

        if (send(client_socket, msg.c_str(), msg.size(), 0) == -1)
        {
            cout << "Falha ao enviar a mensagem *-*\n";
            exit(-1);
        }
    }

    close(client_socket);
}

int main(void)
{
    int server_socket = criar_socket();
    struct sockaddr_in endereco_server = config_endereco();
    bind_(server_socket, endereco_server);
    if (listen(server_socket, 10) == -1)
    {
        cout << "Erro ao configurar o servidor para a escuta!\n";
        exit(-1);
    }
    cout << "Servidor na escuta! Esperando conexoes!\n";

    map<string, string> nick_clients;

    while (true)
    {
        struct sockaddr_in endereco_client;
        socklen_t tam_endereco_client = sizeof(endereco_client);
        int client_socket = accept(server_socket, (struct sockaddr *)&endereco_client, &tam_endereco_client);
        if (client_socket == -1)
        {
            cout << "Falha ao conectar com client :(\n";
            exit(-1);
        }
        cout << "Cliente conectado :D\n IP do cliente: " << inet_ntoa(endereco_client.sin_addr) << "\n";

        char buffer[TAM_MAX];
        string nickname;
        if (recv(client_socket, buffer, sizeof(buffer) - 1, 0) == -1)
        {
            cout << "Erro ao definir o nickname do cliente T-T\n";
        }
        nickname = buffer;
        cout << "Nickname definido: " << nickname << "\n";

        // Adicione o nickname e o socket do cliente ao mapa
        nick_clients[nickname] = client_socket;

        // Crie uma nova thread para lidar com o cliente
        thread clientThread(handleClient, client_socket, nickname);

        // Detach a thread para permitir que ela continue executando em segundo plano
        clientThread.detach();
    }

    close(server_socket);

    return 0;
}
