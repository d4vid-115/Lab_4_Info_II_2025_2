#include "Red.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <limits>

Red::Red() {}

void Red::agregarRouter(string idRouter) {
    if (routers.find(idRouter) == routers.end()) {
        routers[idRouter] = Router(idRouter);
        cout << "Router " << idRouter << " agregado a la red." << endl;
    } else {
        cout << "El router " << idRouter << " ya existe en la red." << endl;
    }
}

void Red::eliminarRouter(string idRouter) {
    if (routers.find(idRouter) == routers.end()) {
        cout << "El router " << idRouter << " no existe en la red." << endl;
        return;
    }

    // Primero, remover todas las referencias a este router en otros routers
    for (auto& par : routers) {
        if (par.first != idRouter) {
            par.second.eliminarVecino(idRouter);
        }
    }

    // Luego eliminar el router
    routers.erase(idRouter);
    cout << "Router " << idRouter << " eliminado de la red." << endl;

    // Actualizar todas las tablas de enrutamiento
    actualizarTodasLasTablas();
}

bool Red::existeRouter(string idRouter) const {
    return routers.find(idRouter) != routers.end();
}

void Red::agregarEnlace(string router1, string router2, int costo) {
    if (!existeRouter(router1) || !existeRouter(router2)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return;
    }

    if (router1 == router2) {
        cout << "Error: No se puede crear un enlace de un router consigo mismo." << endl;
        return;
    }

    // Los enlaces son bidireccionales
    routers[router1].agregarVecino(router2, costo);
    routers[router2].agregarVecino(router1, costo);

    cout << "Enlace agregado: " << router1 << " <-> " << router2
         << " (costo: " << costo << ")" << endl;
}

void Red::eliminarEnlace(string router1, string router2) {
    if (!existeRouter(router1) || !existeRouter(router2)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return;
    }

    routers[router1].eliminarVecino(router2);
    routers[router2].eliminarVecino(router1);

    cout << "Enlace eliminado entre " << router1 << " y " << router2 << endl;

    // Actualizar tablas de enrutamiento
    actualizarTodasLasTablas();
}

void Red::actualizarCostoEnlace(string router1, string router2, int nuevoCosto) {
    if (!existeRouter(router1) || !existeRouter(router2)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return;
    }

    routers[router1].actualizarCostoVecino(router2, nuevoCosto);
    routers[router2].actualizarCostoVecino(router1, nuevoCosto);

    cout << "Costo del enlace actualizado: " << router1 << " <-> " << router2
         << " (nuevo costo: " << nuevoCosto << ")" << endl;

    // Actualizar tablas de enrutamiento
    actualizarTodasLasTablas();
}

map<string, EntradaDeEnrutamiento> Red::calcularCaminosMasCortos(string idOrigen) {
    map<string, EntradaDeEnrutamiento> resultado;

    // Verificar que el router origen existe
    if (routers.find(idOrigen) == routers.end()) {
        return resultado;
    }

    // 1. Distancias: guarda la mejor distancia conocida a cada nodo
    map<string, int> distancias;

    // 2. Previo: guarda de donde venimos (para reconstruir camino)
    map<string, string> previo;

    // 3. Visitados: marca nodos con distancia definitiva
    set<string> visitados;

    // Inicializar todas las distancias como infinito
    for (const auto& par : routers) {
        distancias[par.first] = numeric_limits<int>::max();
    }

    // La distancia al nodo origen es 0
    distancias[idOrigen] = 0;

    // Mientras haya nodos sin visitar
    while (visitados.size() < routers.size()) {

        string nodoActual = "";
        int distanciaMinima = numeric_limits<int>::max();

        for (const auto& par : distancias) {
            string nodo = par.first;
            int distancia = par.second;

            // Si el nodo NO esta visitado Y tiene menor distancia
            if (visitados.find(nodo) == visitados.end() &&
                distancia < distanciaMinima) {
                distanciaMinima = distancia;
                nodoActual = nodo;
            }
        }

        // Si no encontramos ningun nodo alcanzable, terminar
        if (nodoActual == "" || distanciaMinima == numeric_limits<int>::max()) {
            break;
        }

        visitados.insert(nodoActual);

        map<string, int> vecinos = routers[nodoActual].obtenerVecinos();

        for (const auto& vecino : vecinos) {
            string idVecino = vecino.first;
            int costoEnlace = vecino.second;

            // Si el vecino ya fue visitado, ignorarlo
            if (visitados.find(idVecino) != visitados.end()) {
                continue;
            }

            // Calcular nueva distancia pasando por nodoActual
            int nuevaDistancia = distancias[nodoActual] + costoEnlace;

            // Si encontramos un camino MEJOR, actualizar
            if (nuevaDistancia < distancias[idVecino]) {
                distancias[idVecino] = nuevaDistancia;
                previo[idVecino] = nodoActual;
            }
        }
    }

    for (const auto& par : routers) {
        string idDestino = par.first;

        // No incluir al mismo nodo origen
        if (idDestino == idOrigen) {
            continue;
        }

        // Si hay un camino valido (distancia no es infinito)
        if (distancias[idDestino] != numeric_limits<int>::max()) {
            EntradaDeEnrutamiento entrada;
            entrada.destino = idDestino;
            entrada.costo = distancias[idDestino];

            vector<string> caminoInverso;
            string actual = idDestino;

            while (actual != idOrigen) {
                caminoInverso.push_back(actual);
                actual = previo[actual];
            }
            caminoInverso.push_back(idOrigen);

            // Invertir el camino para que vaya de origen a destino
            entrada.camino = vector<string>(caminoInverso.rbegin(),
                                            caminoInverso.rend());

            // Calcular siguiente salto
            if (entrada.camino.size() >= 2) {
                entrada.siguienteSalto = entrada.camino[1];
            }

            resultado[idDestino] = entrada;
        }
    }

    return resultado;
}

void Red::actualizarTodasLasTablas() {
    cout << "\nActualizando tablas de enrutamiento..." << endl;

    // Para cada router, calcular sus rutas más cortas
    for (auto& par : routers) {
        string idRouter = par.first;
        map<string, EntradaDeEnrutamiento> tablaEnrutamiento = calcularCaminosMasCortos(idRouter);
        par.second.actualizarTablaEnrutamiento(tablaEnrutamiento);
    }

    cout << "Tablas de enrutamiento actualizadas." << endl;
}

int Red::obtenerCostoEntre(string origen, string destino) const {
    if (!existeRouter(origen) || !existeRouter(destino)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return -1;
    }

    if (origen == destino) {
        return 0;
    }

    return routers.at(origen).obtenerCostoHacia(destino);
}

vector<string> Red::obtenerCaminoEntre(string origen, string destino) const {
    if (!existeRouter(origen) || !existeRouter(destino)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return vector<string>();
    }

    if (origen == destino) {
        return vector<string>{origen};
    }

    return routers.at(origen).obtenerCaminoHacia(destino);
}

void Red::mostrarRed() const {
    cout << "\n========== RED DE ENRUTADORES ==========" << endl;
    cout << "Total de routers: " << routers.size() << endl;
    cout << "\nConexiones:" << endl;

    for (const auto& par : routers) {
        string idRouter = par.first;
        map<string, int> vecinos = par.second.obtenerVecinos();

        cout << "\nRouter " << idRouter << " conectado a:" << endl;
        if (vecinos.empty()) {
            cout << "  (sin conexiones)" << endl;
        } else {
            for (const auto& vecino : vecinos) {
                cout << "  - " << vecino.first
                     << " (costo: " << vecino.second << ")" << endl;
            }
        }
    }
    cout << "========================================\n" << endl;
}

void Red::mostrarTablaRouter(string idRouter) const {
    if (!existeRouter(idRouter)) {
        cout << "Error: El router " << idRouter << " no existe." << endl;
        return;
    }

    routers.at(idRouter).mostrarTablaEnrutamiento();
}

void Red::cargarDesdeArchivo(string nombreArchivo) {
    ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
        return;
    }

    cout << "\nCargando red desde archivo: " << nombreArchivo << endl;

    // Limpiar la red actual
    routers.clear();

    string linea;
    int numeroLinea = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;

        // Ignorar líneas vacías y comentarios
        if (linea.empty() || linea[0] == '#') {
            continue;
        }

        // Usar stringstream para parsear la línea
        stringstream ss(linea);
        string router1, router2;
        int costo;

        // Formato esperado: RouterA RouterB Costo
        if (ss >> router1 >> router2 >> costo) {
            // Agregar routers si no existen
            if (!existeRouter(router1)) {
                agregarRouter(router1);
            }
            if (!existeRouter(router2)) {
                agregarRouter(router2);
            }

            // Agregar el enlace
            agregarEnlace(router1, router2, costo);
        } else {
            cout << "Advertencia: Linea " << numeroLinea
                 << " tiene formato invalido y sera ignorada." << endl;
        }
    }

    archivo.close();

    // Calcular tablas de enrutamiento
    actualizarTodasLasTablas();

    cout << "Red cargada exitosamente. Total de routers: "
         << routers.size() << endl;
}

void Red::guardarEnArchivo(string nombreArchivo) const {
    ofstream archivo(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error: No se pudo crear el archivo " << nombreArchivo << endl;
        return;
    }

    archivo << "# Archivo de topologia de red" << endl;
    archivo << "# Formato: Router1 Router2 Costo" << endl;
    archivo << "# Los enlaces son bidireccionales" << endl;
    archivo << endl;

    // Para evitar duplicados (A-B y B-A), usamos un set de pares ordenados
    set<pair<string, string>> enlacesGuardados;

    for (const auto& par : routers) {
        string idRouter = par.first;
        map<string, int> vecinos = par.second.obtenerVecinos();

        for (const auto& vecino : vecinos) {
            string idVecino = vecino.first;
            int costo = vecino.second;

            // Crear par ordenado
            pair<string, string> enlace;
            if (idRouter < idVecino) {
                enlace = {idRouter, idVecino};
            } else {
                enlace = {idVecino, idRouter};
            }

            // Si no hemos guardado este enlace
            if (enlacesGuardados.find(enlace) == enlacesGuardados.end()) {
                archivo << idRouter << " " << idVecino << " " << costo << endl;
                enlacesGuardados.insert(enlace);
            }
        }
    }

    archivo.close();
    cout << "Red guardada en: " << nombreArchivo << endl;
}

void Red::generarRedAleatoria(int numRouters, int numEnlaces) {
    if (numRouters < 2) {
        cout << "Error: Se necesitan al menos 2 routers." << endl;
        return;
    }

    // Calcular número máximo de enlaces posibles
    int maxEnlaces = (numRouters * (numRouters - 1)) / 2;

    if (numEnlaces > maxEnlaces) {
        cout << "Advertencia: Demasiados enlaces solicitados. ";
        cout << "Usando maximo posible: " << maxEnlaces << endl;
        numEnlaces = maxEnlaces;
    }

    // Limpiar red actual
    routers.clear();

    cout << "\nGenerando red aleatoria..." << endl;
    cout << "Routers: " << numRouters << ", Enlaces: " << numEnlaces << endl;

    // Inicializar generador de números aleatorios
    srand(time(NULL));

    // Crear routers con IDs: R0, R1, R2, ...
    vector<string> idsRouters;
    for (int i = 0; i < numRouters; i++) {
        string id = "R" + to_string(i);
        agregarRouter(id);
        idsRouters.push_back(id);
    }

    // Primero, crear un árbol de expansión para garantizar conectividad
    cout << "Creando conectividad basica..." << endl;

    for (int i = 1; i < numRouters; i++) {
        // Conectar router i con un router aleatorio entre 0 e i-1
        int previoAleatorio = rand() % i;
        int costo = (rand() % 20) + 1; // Costo entre 1 y 20

        agregarEnlace(idsRouters[i], idsRouters[previoAleatorio], costo);
    }

    // Ahora agregar enlaces adicionales aleatorios
    int enlacesCreados = numRouters - 1;
    int enlacesPorCrear = numEnlaces - enlacesCreados;

    cout << "Agregando enlaces adicionales..." << endl;

    // Set para rastrear enlaces ya creados
    set<pair<string, string>> enlacesExistentes;

    // Registrar enlaces existentes
    for (const auto& par : routers) {
        string r1 = par.first;
        map<string, int> vecinos = par.second.obtenerVecinos();
        for (const auto& vecino : vecinos) {
            string r2 = vecino.first;
            pair<string, string> enlace = (r1 < r2) ?
                                              make_pair(r1, r2) : make_pair(r2, r1);
            enlacesExistentes.insert(enlace);
        }
    }

    // Crear enlaces adicionales
    int intentos = 0;
    int maxIntentos = numEnlaces * 10;

    while (enlacesCreados < numEnlaces && intentos < maxIntentos) {
        intentos++;

        // Elegir dos routers aleatorios diferentes
        int idx1 = rand() % numRouters;
        int idx2 = rand() % numRouters;

        if (idx1 == idx2) {
            continue;
        }

        string r1 = idsRouters[idx1];
        string r2 = idsRouters[idx2];

        // Crear par ordenado
        pair<string, string> enlace = (r1 < r2) ?
                                          make_pair(r1, r2) : make_pair(r2, r1);

        // Si el enlace no existe, crearlo
        if (enlacesExistentes.find(enlace) == enlacesExistentes.end()) {
            int costo = (rand() % 20) + 1;
            agregarEnlace(r1, r2, costo);
            enlacesExistentes.insert(enlace);
            enlacesCreados++;
        }
    }

    // Calcular tablas de enrutamiento
    actualizarTodasLasTablas();

    cout << "Red generada: " << routers.size() << " routers, "
         << enlacesCreados << " enlaces." << endl;

    // Guardar en archivo para referencia
    guardarEnArchivo("red_aleatoria.txt");
}

bool Red::routerTieneRutaHacia(string origen, string destino) const {
    if (!existeRouter(origen)) {
        return false;
    }
    return routers.at(origen).tieneRutaHacia(destino);
}

string Red::obtenerSiguienteSalto(string origen, string destino) const {
    if (!existeRouter(origen)) {
        return "";
    }
    return routers.at(origen).obtenerSiguienteSaltoHacia(destino);
}

void Red::mostrarEstadisticasRouter(string idRouter) const {
    if (!existeRouter(idRouter)) {
        cout << "Error: El router " << idRouter << " no existe." << endl;
        return;
    }
    routers.at(idRouter).mostrarEstadisticas();
}

TablaDeEnrutamiento Red::obtenerTablaRouter(string idRouter) const {
    if (!existeRouter(idRouter)) {
        return TablaDeEnrutamiento();
    }
    return routers.at(idRouter).obtenerTablaEnrutamiento();
}

void Red::compararTablasRouters(string router1, string router2) const {
    if (!existeRouter(router1) || !existeRouter(router2)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return;
    }

    TablaDeEnrutamiento tabla1 = routers.at(router1).obtenerTablaEnrutamiento();
    TablaDeEnrutamiento tabla2 = routers.at(router2).obtenerTablaEnrutamiento();

    cout << "\n=== Comparacion de tablas ===" << endl;
    cout << router1 << ": " << tabla1.obtenerNumeroEntradas() << " rutas, "
         << "costo promedio: " << tabla1.obtenerCostoPromedio() << endl;
    cout << router2 << ": " << tabla2.obtenerNumeroEntradas() << " rutas, "
         << "costo promedio: " << tabla2.obtenerCostoPromedio() << endl;
}
