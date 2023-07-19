#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <csignal>

#define PORTA 12345

using namespace std;

int criar_socket();

struct sockaddr_in config_endereco(string ip);

struct sockaddr_in config_endereco();

void conectar(int socket, struct sockaddr_in *endereco);

void bind_(int socket, struct sockaddr_in &endereco);

void handleSignal(int signal);