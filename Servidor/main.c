#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct Server {
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
    char buffer[1024];
};

void Server_init(struct Server* this) {
    WSAStartup(MAKEWORD(2, 0), &this->WSAData);
    this->server = socket(AF_INET, SOCK_STREAM, 0);
    this->serverAddr.sin_addr.s_addr = INADDR_ANY;
    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(5555);

    bind(this->server, (SOCKADDR *)&this->serverAddr, sizeof(this->serverAddr));
    listen(this->server, 0);

    printf("Server esperando para conectar...\n");
    int clientAddrSize = sizeof(this->clientAddr);
    this->client = accept(this->server, (SOCKADDR *)&this->clientAddr, &clientAddrSize);
    if (this->client != INVALID_SOCKET) {
        printf("Conexion exitosa con el Cliente!\n");
    }
}

void Server_Recibir(struct Server* this) {
    recv(this->client, this->buffer, sizeof(this->buffer), 0);
}

void Server_Enviar(struct Server* this, const char* mensaje) {
    send(this->client, mensaje, strlen(mensaje) + 1, 0);
    printf("Mensaje enviado al cliente.\n");
}


char* generarNombreUsuario(int longitud) {
    static const char* vocales = "aeiou";
    static const char* consonantes = "bcdfghjklmnpqrstvwxyz";
    char* nombre = (char*)malloc(longitud + 1);
    int empezarConVocal = rand() % 2;

    for (int i = 0; i < longitud; i++) {
        if ((i % 2 == 0 && empezarConVocal) || (i % 2 != 0 && !empezarConVocal)) {
            nombre[i] = vocales[rand() % 5];
        } else {
            nombre[i] = consonantes[rand() % 21];
        }
    }
    nombre[longitud] = '\0';
    return nombre;
}

char* generarContrasena(int longitud) {
    static const char* caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char* contrasena = (char*)malloc(longitud + 1);

    for (int i = 0; i < longitud; i++) {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0';
    return contrasena;
}

void Server_CerrarSocket(struct Server* this) {
    closesocket(this->client);
    WSACleanup();
    printf("Socket cerrado, cliente desconectado.\n");
}

int main() {
    srand(time(NULL));
    struct Server servidor;
    Server_init(&servidor);

    while (1) {
        Server_Recibir(&servidor);

        char tipoSolicitud[20];
        int longitud;
        sscanf(servidor.buffer, "%s %d", tipoSolicitud, &longitud);

        if (strcmp(tipoSolicitud, "SALIR") == 0) {
            printf("Recibida senal de salida del cliente.\n");
            break;
        }

        char* respuesta = NULL;

        if (strcmp(tipoSolicitud, "NOMBRE") == 0) {
            if (longitud >= 5 && longitud <= 15) {
                respuesta = generarNombreUsuario(longitud);
            } else {
                respuesta = "ERROR! Longitud no valida para nombre de usuario.";
            }
        } else if (strcmp(tipoSolicitud, "CONTRASENA") == 0) {
            if (longitud >= 8 && longitud <= 50) {
                respuesta = generarContrasena(longitud);
            } else {
                respuesta = "ERROR! Longitud no valida para contrasena.";
            }
        } else {
            respuesta = "ERORR! Solicitud desconocida.";
        }

        Server_Enviar(&servidor, respuesta);

        if (respuesta != NULL && (strcmp(tipoSolicitud, "NOMBRE") == 0 || strcmp(tipoSolicitud, "CONTRASENA") == 0)) {
            free(respuesta);
        }

        memset(servidor.buffer, 0, sizeof(servidor.buffer));
    }

    Server_CerrarSocket(&servidor);
    return 0;
}
