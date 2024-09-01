#include <stdio.h>
#include <winsock2.h>

struct Client {
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
    char buffer[1024];
};

void Client_init(struct Client* this) {
    printf("Estableciendo conexion con el servidor...\n\n");
    WSAStartup(MAKEWORD(2, 0), &this->WSAData);
    this->server = socket(AF_INET, SOCK_STREAM, 0);
    this->addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(5555);
    connect(this->server, (SOCKADDR *)&this->addr, sizeof(this->addr));
    printf("Conexion exitosa con el Servidor!\n");
}

void Client_Enviar(struct Client* this, const char* mensaje) {
    send(this->server, mensaje, strlen(mensaje), 0);
}

void Client_Recibir(struct Client* this) {
    memset(this->buffer, 0, sizeof(this->buffer));
    int bytesReceived = recv(this->server, this->buffer, sizeof(this->buffer) - 1, 0);
    if (bytesReceived > 0) {
        this->buffer[bytesReceived] = '\0';
    }
    printf("El servidor dice: %s\n", this->buffer);
}


void Client_CerrarSocket(struct Client* this) {
    closesocket(this->server);
    WSACleanup();
    printf("Socket cerrado.\n\n");
}

int main() {
    struct Client cliente;
    Client_init(&cliente);

    int opcion;
    int longitud;
    char mensaje[128];

    while (1) {
        printf("\nMenu:\n");
        printf("1. Generar Nombre de Usuario\n");
        printf("2. Generar Contrasena\n");
        printf("3. Salir\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);

        if (opcion == 3) {
            strcpy(mensaje, "SALIR");
            Client_Enviar(&cliente, mensaje);
            break;
        }

        if( opcion == 1 || opcion == 2 ) {
            printf("Introduce la longitud deseada: ");
        scanf("%d", &longitud);

            if (opcion == 1) {
                sprintf(mensaje, "NOMBRE %d", longitud);
            } else if (opcion == 2) {
                sprintf(mensaje, "CONTRASENA %d", longitud);
            }
        } else {
            printf("Opcion no valida.\n");
            continue;
        }

        Client_Enviar(&cliente, mensaje);
        Client_Recibir(&cliente);
    }

    Client_CerrarSocket(&cliente);

    return 0;
}


