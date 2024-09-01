package test;

import modelo.Client;

import java.io.IOException;
import java.util.Scanner;

public class testCliente {

    public static void main(String[] args) {
        Client cliente = new Client();
        try {
            cliente.init();

            Scanner scanner = new Scanner(System.in);
            int opcion;
            int longitud;
            String mensaje;

            while (true) {
                System.out.println("\nMenú:");
                System.out.println("1. Generar Nombre de Usuario");
                System.out.println("2. Generar Contraseña");
                System.out.println("3. Salir");
                System.out.print("Elige una opción: ");
                opcion = scanner.nextInt();

                if (opcion == 3) {
                    mensaje = "SALIR";
                    cliente.enviar(mensaje);
                    break;
                }

                if (opcion == 1 || opcion == 2) {
                    System.out.print("Introduce la longitud deseada: ");
                    longitud = scanner.nextInt();

                    if (opcion == 1) {
                        mensaje = "NOMBRE " + longitud;
                    } else {
                        mensaje = "CONTRASENA " + longitud;
                    }

                    cliente.enviar(mensaje);
                    cliente.recibir();
                } else {
                    System.out.println("Opción no valida.");
                }
            }

            cliente.cerrarSocket();
            scanner.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
