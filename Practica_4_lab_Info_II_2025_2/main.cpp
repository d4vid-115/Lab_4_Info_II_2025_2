#include "Red.h"
#include <iostream>
#include <string>

using namespace std;

void imprimirMenu() {
    cout << "\n========== SIMULADOR DE RED DE ENRUTADORES ==========" << endl;
    cout << "1.  Agregar router" << endl;
    cout << "2.  Eliminar router" << endl;
    cout << "3.  Agregar enlace" << endl;
    cout << "4.  Eliminar enlace" << endl;
    cout << "5.  Actualizar costo de enlace" << endl;
    cout << "6.  Mostrar red completa" << endl;
    cout << "7.  Mostrar tabla de enrutamiento de un router" << endl;
    cout << "8.  Consultar costo entre dos routers" << endl;
    cout << "9.  Consultar camino entre dos routers" << endl;
    cout << "10. Cargar red desde archivo" << endl;
    cout << "11. Guardar red en archivo" << endl;
    cout << "12. Generar red aleatoria" << endl;
    cout << "13. Crear red de ejemplo (A, B, C, D)" << endl;
    cout << "0.  Salir" << endl;
    cout << "====================================================" << endl;
    cout << "Opcion: ";
}

void crearRedDeEjemplo(Red& red) {
    cout << "\nCreando red de ejemplo del PDF..." << endl;

    // Limpiar red actual
    red = Red();

    // Crear la red del ejemplo: A-B-C-D
    red.agregarRouter("A");
    red.agregarRouter("B");
    red.agregarRouter("C");
    red.agregarRouter("D");

    red.agregarEnlace("A", "B", 4);
    red.agregarEnlace("A", "C", 10);
    red.agregarEnlace("B", "D", 1);
    red.agregarEnlace("C", "D", 2);

    red.actualizarTodasLasTablas();

    cout << "Red de ejemplo creada exitosamente." << endl;
}

int main() {
    Red red;
    int opcion;

    cout << "=== BIENVENIDO AL SIMULADOR DE RED DE ENRUTADORES ===" << endl;
    cout << "Este programa simula el enrutamiento en una red." << endl;

    do {
        imprimirMenu();
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer del newline

        switch(opcion) {
        case 1: { // Agregar router
            cout << "\nIngrese ID del router: ";
            string idRouter;
            getline(cin, idRouter);
            red.agregarRouter(idRouter);
            break;
        }

        case 2: { // Eliminar router
            cout << "\nIngrese ID del router a eliminar: ";
            string idRouter;
            getline(cin, idRouter);
            red.eliminarRouter(idRouter);
            break;
        }

        case 3: { // Agregar enlace
            cout << "\nIngrese ID del primer router: ";
            string r1;
            getline(cin, r1);

            cout << "Ingrese ID del segundo router: ";
            string r2;
            getline(cin, r2);

            cout << "Ingrese el costo del enlace: ";
            int costo;
            cin >> costo;
            cin.ignore();

            red.agregarEnlace(r1, r2, costo);
            red.actualizarTodasLasTablas();
            break;
        }

        case 4: { // Eliminar enlace
            cout << "\nIngrese ID del primer router: ";
            string r1;
            getline(cin, r1);

            cout << "Ingrese ID del segundo router: ";
            string r2;
            getline(cin, r2);

            red.eliminarEnlace(r1, r2);
            break;
        }

        case 5: { // Actualizar costo
            cout << "\nIngrese ID del primer router: ";
            string r1;
            getline(cin, r1);

            cout << "Ingrese ID del segundo router: ";
            string r2;
            getline(cin, r2);

            cout << "Ingrese el nuevo costo: ";
            int costo;
            cin >> costo;
            cin.ignore();

            red.actualizarCostoEnlace(r1, r2, costo);
            break;
        }

        case 6: { // Mostrar red
            red.mostrarRed();
            break;
        }

        case 7: { // Mostrar tabla de un router
            cout << "\nIngrese ID del router: ";
            string idRouter;
            getline(cin, idRouter);
            red.mostrarTablaRouter(idRouter);
            break;
        }

        case 8: { // Consultar costo
            cout << "\nIngrese router origen: ";
            string origen;
            getline(cin, origen);

            cout << "Ingrese router destino: ";
            string destino;
            getline(cin, destino);

            int costo = red.obtenerCostoEntre(origen, destino);
            if (costo == -1) {
                cout << "No hay ruta entre " << origen << " y " << destino << endl;
            } else if (costo == 0) {
                cout << "El origen y destino son el mismo router." << endl;
            } else {
                cout << "Costo de " << origen << " a " << destino << ": " << costo << endl;
            }
            break;
        }

        case 9: { // Consultar camino
            cout << "\nIngrese router origen: ";
            string origen;
            getline(cin, origen);

            cout << "Ingrese router destino: ";
            string destino;
            getline(cin, destino);

            vector<string> camino = red.obtenerCaminoEntre(origen, destino);

            if (camino.empty()) {
                cout << "No hay ruta entre " << origen << " y " << destino << endl;
            } else {
                cout << "Camino de " << origen << " a " << destino << ": ";
                for (size_t i = 0; i < camino.size(); i++) {
                    cout << camino[i];
                    if (i < camino.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << endl;

                int costo = red.obtenerCostoEntre(origen, destino);
                cout << "Costo total: " << costo << endl;
            }
            break;
        }

        case 10: { // Cargar desde archivo
            cout << "\nIngrese nombre del archivo: ";
            string nombreArchivo;
            getline(cin, nombreArchivo);
            red.cargarDesdeArchivo(nombreArchivo);
            break;
        }

        case 11: { // Guardar en archivo
            cout << "\nIngrese nombre del archivo: ";
            string nombreArchivo;
            getline(cin, nombreArchivo);
            red.guardarEnArchivo(nombreArchivo);
            break;
        }

        case 12: { // Generar red aleatoria
            cout << "\nIngrese numero de routers: ";
            int numRouters;
            cin >> numRouters;

            cout << "Ingrese numero de enlaces: ";
            int numEnlaces;
            cin >> numEnlaces;
            cin.ignore();

            red.generarRedAleatoria(numRouters, numEnlaces);
            break;
        }

        case 13: { // Red de ejemplo
            crearRedDeEjemplo(red);
            break;
        }

        case 0: {
            cout << "\n¡Gracias por usar el simulador!" << endl;
            break;
        }

        default:
            cout << "\nOpcion invalida. Intente de nuevo." << endl;
        }

    } while (opcion != 0);

    return 0;
}
