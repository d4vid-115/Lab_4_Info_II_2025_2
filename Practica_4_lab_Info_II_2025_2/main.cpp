#include "Red.h"
#include <iostream>
#include <string>

using namespace std;

void printMenu() {
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
    cout << "Opción: ";
}

void createExampleNetwork(Red& net) {
    cout << "\nCreando red de ejemplo del PDF..." << endl;

    // Limpiar red actual
    net = Red();

    // Crear la red del ejemplo: A-B-C-D
    net.addRouter("A");
    net.addRouter("B");
    net.addRouter("C");
    net.addRouter("D");

    net.addLink("A", "B", 4);
    net.addLink("A", "C", 10);
    net.addLink("B", "D", 1);
    net.addLink("C", "D", 2);

    net.updateAllRoutingTables();

    cout << "Red de ejemplo creada exitosamente." << endl;
}

int main() {
    Red red;
    int option;
    string input;

    cout << "=== BIENVENIDO AL SIMULADOR DE RED DE ENRUTADORES ===" << endl;
    cout << "Este programa simula el enrutamiento en una red." << endl;

    do {
        printMenu();
        cin >> option;
        cin.ignore(); // Limpiar el buffer del newline

        switch(option) {
        case 1: { // Agregar router
            cout << "\nIngrese ID del router: ";
            string routerId;
            getline(cin, routerId);
            red.addRouter(routerId);
            break;
        }

        case 2: { // Eliminar router
            cout << "\nIngrese ID del router a eliminar: ";
            string routerId;
            getline(cin, routerId);
            red.removeRouter(routerId);
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
            int cost;
            cin >> cost;
            cin.ignore();

            red.addLink(r1, r2, cost);
            red.updateAllRoutingTables();
            break;
        }

        case 4: { // Eliminar enlace
            cout << "\nIngrese ID del primer router: ";
            string r1;
            getline(cin, r1);

            cout << "Ingrese ID del segundo router: ";
            string r2;
            getline(cin, r2);

            red.removeLink(r1, r2);
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
            int cost;
            cin >> cost;
            cin.ignore();

            red.updateLinkCost(r1, r2, cost);
            break;
        }

        case 6: { // Mostrar red
            red.displayNetwork();
            break;
        }

        case 7: { // Mostrar tabla de un router
            cout << "\nIngrese ID del router: ";
            string routerId;
            getline(cin, routerId);
            red.displayRouterTable(routerId);
            break;
        }

        case 8: { // Consultar costo
            cout << "\nIngrese router origen: ";
            string source;
            getline(cin, source);

            cout << "Ingrese router destino: ";
            string dest;
            getline(cin, dest);

            int cost = red.getCostBetween(source, dest);
            if (cost == -1) {
                cout << "No hay ruta entre " << source << " y " << dest << endl;
            } else if (cost == 0) {
                cout << "El origen y destino son el mismo router." << endl;
            } else {
                cout << "Costo de " << source << " a " << dest << ": " << cost << endl;
            }
            break;
        }

        case 9: { // Consultar camino
            cout << "\nIngrese router origen: ";
            string source;
            getline(cin, source);

            cout << "Ingrese router destino: ";
            string dest;
            getline(cin, dest);

            vector<string> path = red.getPathBetween(source, dest);

            if (path.empty()) {
                cout << "No hay ruta entre " << source << " y " << dest << endl;
            } else {
                cout << "Camino de " << source << " a " << dest << ": ";
                for (size_t i = 0; i < path.size(); i++) {
                    cout << path[i];
                    if (i < path.size() - 1) {
                        cout << " -> ";
                    }
                }
                cout << endl;

                int cost = red.getCostBetween(source, dest);
                cout << "Costo total: " << cost << endl;
            }
            break;
        }

        case 10: { // Cargar desde archivo
            cout << "\nIngrese nombre del archivo: ";
            string filename;
            getline(cin, filename);
            red.loadFromFile(filename);
            break;
        }

        case 11: { // Guardar en archivo
            cout << "\nIngrese nombre del archivo: ";
            string filename;
            getline(cin, filename);
            red.saveToFile(filename);
            break;
        }

        case 12: { // Generar red aleatoria
            cout << "\nIngrese número de routers: ";
            int numRouters;
            cin >> numRouters;

            cout << "Ingrese número de enlaces: ";
            int numLinks;
            cin >> numLinks;
            cin.ignore();

            red.generateRandomNetwork(numRouters, numLinks);
            break;
        }

        case 13: { // Red de ejemplo
            createExampleNetwork(red);
            break;
        }

        case 0: {
            cout << "\n¡Gracias por usar el simulador!" << endl;
            break;
        }

        default:
            cout << "\nOpción inválida. Intente de nuevo." << endl;
        }

    } while (option != 0);

    return 0;
}
