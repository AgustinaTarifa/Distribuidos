package modelo;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Client {
    private Socket server;
    private PrintWriter out;
    private BufferedReader in;
    private char[] buffer = new char[1024];

    public void init() throws IOException {
        System.out.println("Estableciendo conexion con el servidor...\n");
        server = new Socket("127.0.0.1", 5555);
        out = new PrintWriter(server.getOutputStream(), true);
        in = new BufferedReader(new InputStreamReader(server.getInputStream()));
        System.out.println("Conexion exitosa con el Servidor!");
    }

    public void enviar(String mensaje) {
        out.println(mensaje);
    }

    public void recibir() throws IOException {
        int bytesReceived = in.read(buffer, 0, buffer.length);
        if (bytesReceived > 0) {
            String respuesta = new String(buffer, 0, bytesReceived);
            System.out.println("El servidor dice: " + respuesta);
        }
    }

    public void cerrarSocket() throws IOException {
        in.close();
        out.close();
        server.close();
        System.out.println("Socket cerrado.\n");
    }
}
    
