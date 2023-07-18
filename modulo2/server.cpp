#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <map>
#include <thread>
#include <vector>

#include "socket.hpp"

#define TAM_MAX 4096

using namespace std;

// Função executada em cada thread de cliente
void handleClient(int client_socket, const string& nickname, map<string, int>& nick_clients)
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
            break;
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
            msg = "(" + nickname + "): " + msg;
        }

        // Enviar a mensagem para todos os clientes
        for (const auto& pair : nick_clients)
        {
            if (send(pair.second, msg.c_str(), msg.size(), 0) == -1)
            {
                cout << "Falha ao enviar a mensagem para o cliente (" << pair.first << ") *-*\n";
            }
        }
    }

    close(client_socket);

    // Remover o cliente do mapa após fechar a conexão
    nick_clients.erase(nickname);
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
    cout << "Servidor na escuta! Esperando conexões!\n";

    map<string, int> nick_clients;

    while (true)
    {
        struct sockaddr_in endereco_client;
        socklen_t tam_endereco_client = sizeof(endereco_client);
        int client_socket = accept(server_socket, (struct sockaddr *)&endereco_client, &tam_endereco_client);
        if (client_socket == -1)
        {
            cout << "Falha ao conectar com o cliente :(\n";
            exit(-1);
        }
        cout << "Cliente conectado :D\nIP do cliente: " << inet_ntoa(endereco_client.sin_addr) << "\n";

        char buffer[TAM_MAX];
        string nickname;
        if (recv(client_socket, buffer, sizeof(buffer) - 1, 0) == -1)
        {
            cout << "Erro ao definir o nickname do cliente T-T\n";
            close(client_socket);
            continue;
        }
        nickname = buffer;
        cout << "Nickname definido: " << nickname << "\n";

        // Adicionar o nickname e o socket do cliente ao mapa
        nick_clients[nickname] = client_socket;

        // Criar uma nova thread para lidar com o cliente
        thread clientThread(handleClient, client_socket, nickname, ref(nick_clients));

        // Detach a thread para permitir que ela continue executando em segundo plano
        clientThread.detach();
    }

    close(server_socket);

    return 0;
}
