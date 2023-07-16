#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#define TAM_MAX 4096

using namespace std;

int main(void){

    // cria um socket para o servidor
    // parametros:
    //      AF_INET: especifica dominio ipv4
    //      SOCK_STREAM: especifica que eh um socket de fluxo
    //      0: define o protocolo, quando eh zero o SO decide qual vai ser o melhor protocolo
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1){
        cout << "Erro ao criar socket do server!\n";
        exit(-1);
    }

    
    struct sockaddr_in endereco_server;
    endereco_server.sin_family = AF_INET; // ipv4
    endereco_server.sin_addr.s_addr = INADDR_ANY; // aceita conexoes vindas de qualquer endereco IP
    endereco_server.sin_port = htons(12345); // definindo a porta

    // associando um endereco IP e uma porta a um socket
    if(bind(server_socket, (struct sockaddr*)&endereco_server, (socklen_t)sizeof(endereco_server)) == -1){
        cout << "Erro ao vincular socket ao endereco!(bind)\n";
        exit(-1);
    }
    
    // configura o servidor para ficar escutando, esperando um client socket
    // 10 eh numero maximo de conexoes que podem ficar na espera
    if(listen(server_socket, 10) == -1){
        cout << "Erro ao configurar o servidor para a escuta!\n";
        exit(-1);
    }
    cout << "Servidor na escuta! Esperando conexoes!\n";

    struct sockaddr_in endereco_client;
    socklen_t tam_endereco_client = sizeof(endereco_client);

    // fazendo a conexao entre o socket clint e o server
    int client_socket = accept(server_socket, (struct sockaddr*)&endereco_client, &tam_endereco_client);
    if(client_socket == -1){
        cout << "Falha ao conectar com client :(\n";
        exit(-1);
    }
    cout << "Cliente conectado :D\n IP do cliente: " << inet_ntoa(endereco_client.sin_addr) << "\n";

    char buffer[TAM_MAX];
    string msg;

    bool flag = true;

    // recebendo mensagens
    while(flag){
        memset(buffer, 0, sizeof(buffer)); // setando a memoria para evitar lixo
        
        // a funcao recv recebe dados de um socket(client no caso) e armazena no buffer
        if(recv(client_socket, buffer, sizeof(buffer)-1, 0) == -1){ // se retornar -1 nao deu certo
            cout << "Erro ao receber a mensagem do cliente T-T\n";
        }

        // armazenando a mensagem em uma variavel do tipo string para facilitar printar
        msg = buffer;

        // verificando se o solicitou interromper a conexao
        if(msg == "/exit"){
            cout << "Cliente solicitou /exit\n";
            cout << "Encerrando conexão com o server.\n";
            msg = "Encerrando conexão com o server.\n"; // modificando a mensagem
            flag = false; // ativando a flag para parar de receber mensagens
        }else{
            cout << "Mensagem: " << msg << "\n"; // se nao solicitou, apenas printamos a mensagem do client
        }
        
        // enviando a mensagem para todos da rede
        if (send(client_socket, msg.c_str(), msg.size(), 0) == -1){
            cout << "Falha ao enviar a mensagem *-*\n";
            exit(-1);
        }
    }

    // fechando ambos os sockets, servidor e cliente
    close(client_socket);
    close(server_socket);

    return 0;
}

