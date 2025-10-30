// Network.cpp
#include "Red.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>
#include <cstdlib>
#include <ctime>

Red::Red() {}

void Red::addRouter(string routerId) {
    if (routers.find(routerId) == routers.end()) {
        routers[routerId] = Router(routerId);
        cout << "Router " << routerId << " agregado a la red." << endl;
    } else {
        cout << "El router " << routerId << " ya existe en la red." << endl;
    }
}

void Red::removeRouter(string routerId) {
    if (routers.find(routerId) == routers.end()) {
        cout << "El router " << routerId << " no existe en la red." << endl;
        return;
    }

    // Primero, remover todas las referencias a este router en otros routers
    for (auto& pair : routers) {
        if (pair.first != routerId) {
            pair.second.removeNeighbor(routerId);
        }
    }

    // Luego eliminar el router
    routers.erase(routerId);
    cout << "Router " << routerId << " eliminado de la red." << endl;

    // Actualizar todas las tablas de enrutamiento
    updateAllRoutingTables();
}

bool Red::routerExists(string routerId) const {
    return routers.find(routerId) != routers.end();
}

void Red::addLink(string router1, string router2, int cost) {
    if (!routerExists(router1) || !routerExists(router2)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return;
    }

    if (router1 == router2) {
        cout << "Error: No se puede crear un enlace de un router consigo mismo." << endl;
        return;
    }

    // Los enlaces son bidireccionales
    routers[router1].addNeighbor(router2, cost);
    routers[router2].addNeighbor(router1, cost);

    cout << "Enlace agregado: " << router1 << " <-> " << router2
         << " (costo: " << cost << ")" << endl;
}

void Red::removeLink(string router1, string router2) {
    if (!routerExists(router1) || !routerExists(router2)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return;
    }

    routers[router1].removeNeighbor(router2);
    routers[router2].removeNeighbor(router1);

    cout << "Enlace eliminado entre " << router1 << " y " << router2 << endl;

    // Actualizar tablas de enrutamiento
    updateAllRoutingTables();
}

void Red::updateLinkCost(string router1, string router2, int newCost) {
    if (!routerExists(router1) || !routerExists(router2)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return;
    }

    routers[router1].updateNeighborCost(router2, newCost);
    routers[router2].updateNeighborCost(router1, newCost);

    cout << "Costo del enlace actualizado: " << router1 << " <-> " << router2
         << " (nuevo costo: " << newCost << ")" << endl;

    // Actualizar tablas de enrutamiento
    updateAllRoutingTables();
}

map<string, RoutingEntry> Red::calculateShortestPaths(string sourceId) {
    map<string, RoutingEntry> result;

    // Verificar que el router fuente existe
    if (routers.find(sourceId) == routers.end()) {
        return result;
    }

    // Inicialización
    // distances: mantiene la distancia más corta conocida desde source a cada nodo
    map<string, int> distances;

    // previous: mantiene el nodo previo en el camino más corto
    map<string, string> previous;

    // visited: conjunto de nodos ya procesados
    set<string> visited;

    // Priority queue: pair<distancia, nodo>
    // La priority_queue en C++ es un max-heap, pero queremos min-heap
    // Por eso usamos greater<pair<int, string>>
    priority_queue<pair<int, string>,
                   vector<pair<int, string>>,
                   greater<pair<int, string>>> pq;

    // Inicializar todas las distancias como infinito
    for (const auto& pair : routers) {
        distances[pair.first] = numeric_limits<int>::max();
    }

    // La distancia al nodo fuente es 0
    distances[sourceId] = 0;
    pq.push({0, sourceId});

    // Algoritmo de Dijkstra
    while (!pq.empty()) {
        // Extraer el nodo con menor distancia
        string currentNode = pq.top().second;
        int currentDist = pq.top().first;
        pq.pop();

        // Si ya visitamos este nodo, continuar
        if (visited.find(currentNode) != visited.end()) {
            continue;
        }

        // Marcar como visitado
        visited.insert(currentNode);

        // Revisar todos los vecinos
        map<string, int> neighbors = routers[currentNode].getNeighbors();
        for (const auto& neighbor : neighbors) {
            string neighborId = neighbor.first;
            int edgeCost = neighbor.second;

            // Calcular nueva distancia
            int newDist = currentDist + edgeCost;

            // Si encontramos un camino más corto
            if (newDist < distances[neighborId]) {
                distances[neighborId] = newDist;
                previous[neighborId] = currentNode;
                pq.push({newDist, neighborId});
            }
        }
    }

    // Construir las entradas de la tabla de enrutamiento
    for (const auto& pair : routers) {
        string destId = pair.first;

        // No incluir al mismo nodo fuente
        if (destId == sourceId) {
            continue;
        }

        // Si hay un camino válido
        if (distances[destId] != numeric_limits<int>::max()) {
            RoutingEntry entry;
            entry.destination = destId;
            entry.cost = distances[destId];

            // Reconstruir el camino desde destino hacia fuente
            vector<string> pathReverse;
            string current = destId;

            while (current != sourceId) {
                pathReverse.push_back(current);
                current = previous[current];
            }
            pathReverse.push_back(sourceId);

            // Invertir el camino para que vaya de fuente a destino
            entry.path = vector<string>(pathReverse.rbegin(), pathReverse.rend());

            result[destId] = entry;
        }
    }

    return result;
}

void Red::updateAllRoutingTables() {
    cout << "\nActualizando tablas de enrutamiento..." << endl;

    // Para cada router, calcular sus rutas más cortas
    for (auto& pair : routers) {
        string routerId = pair.first;
        map<string, RoutingEntry> routingTable = calculateShortestPaths(routerId);
        pair.second.updateRoutingTable(routingTable);
    }

    cout << "Tablas de enrutamiento actualizadas." << endl;
}

int Red::getCostBetween(string source, string destination) const {
    if (!routerExists(source) || !routerExists(destination)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return -1;
    }

    if (source == destination) {
        return 0;
    }

    return routers.at(source).getCostTo(destination);
}

vector<string> Red::getPathBetween(string source, string destination) const {
    if (!routerExists(source) || !routerExists(destination)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return vector<string>();
    }

    if (source == destination) {
        return vector<string>{source};
    }

    return routers.at(source).getPathTo(destination);
}

void Red::displayNetwork() const {
    cout << "\n========== RED DE ENRUTADORES ==========" << endl;
    cout << "Total de routers: " << routers.size() << endl;
    cout << "\nConexiones:" << endl;

    for (const auto& pair : routers) {
        string routerId = pair.first;
        map<string, int> neighbors = pair.second.getNeighbors();

        cout << "\nRouter " << routerId << " conectado a:" << endl;
        if (neighbors.empty()) {
            cout << "  (sin conexiones)" << endl;
        } else {
            for (const auto& neighbor : neighbors) {
                cout << "  - " << neighbor.first
                     << " (costo: " << neighbor.second << ")" << endl;
            }
        }
    }
    cout << "========================================\n" << endl;
}

void Red::displayRouterTable(string routerId) const {
    if (!routerExists(routerId)) {
        cout << "Error: El router " << routerId << " no existe." << endl;
        return;
    }

    routers.at(routerId).displayRoutingTable();
}

// Continuación de Network.cpp

void Red::loadFromFile(string filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    cout << "\nCargando red desde archivo: " << filename << endl;

    // Limpiar la red actual
    routers.clear();

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;

        // Ignorar líneas vacías y comentarios
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Usar stringstream para parsear la línea
        stringstream ss(line);
        string router1, router2;
        int cost;

        // Formato esperado: RouterA RouterB Costo
        if (ss >> router1 >> router2 >> cost) {
            // Agregar routers si no existen
            if (!routerExists(router1)) {
                addRouter(router1);
            }
            if (!routerExists(router2)) {
                addRouter(router2);
            }

            // Agregar el enlace
            addLink(router1, router2, cost);
        } else {
            cout << "Advertencia: Linea " << lineNum
                 << " tiene formato invalido y sera ignorada." << endl;
        }
    }

    file.close();

    // Calcular tablas de enrutamiento
    updateAllRoutingTables();

    cout << "Red cargada exitosamente. Total de routers: "
         << routers.size() << endl;
}

void Red::saveToFile(string filename) const {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: No se pudo crear el archivo " << filename << endl;
        return;
    }

    file << "# Archivo de topología de red" << endl;
    file << "# Formato: Router1 Router2 Costo" << endl;
    file << "# Los enlaces son bidireccionales" << endl;
    file << endl;

    // Para evitar duplicados (A-B y B-A), usamos un set de pares ordenados
    set<pair<string, string>> savedLinks;

    for (const auto& pair : routers) {
        string routerId = pair.first;
        map<string, int> neighbors = pair.second.getNeighbors();

        for (const auto& neighbor : neighbors) {
            string neighborId = neighbor.first;
            int cost = neighbor.second;

            // Crear par ordenado
            pair<string, string> link;
            if (routerId < neighborId) {
                link = {routerId, neighborId};
            } else {
                link = {neighborId, routerId};
            }

            // Si no hemos guardado este enlace
            if (savedLinks.find(link) == savedLinks.end()) {
                file << routerId << " " << neighborId << " " << cost << endl;
                savedLinks.insert(link);
            }
        }
    }

    file.close();
    cout << "Red guardada en: " << filename << endl;
}

void Red::generateRandomNetwork(int numRouters, int numLinks) {
    if (numRouters < 2) {
        cout << "Error: Se necesitan al menos 2 routers." << endl;
        return;
    }

    // Calcular número máximo de enlaces posibles
    int maxLinks = (numRouters * (numRouters - 1)) / 2;

    if (numLinks > maxLinks) {
        cout << "Advertencia: Demasiados enlaces solicitados. ";
        cout << "Usando maximo posible: " << maxLinks << endl;
        numLinks = maxLinks;
    }

    // Limpiar red actual
    routers.clear();

    cout << "\nGenerando red aleatoria..." << endl;
    cout << "Routers: " << numRouters << ", Enlaces: " << numLinks << endl;

    // Inicializar generador de números aleatorios
    srand(time(NULL));

    // Crear routers con IDs: R0, R1, R2, ...
    vector<string> routerIds;
    for (int i = 0; i < numRouters; i++) {
        string id = "R" + to_string(i);
        addRouter(id);
        routerIds.push_back(id);
    }

    // Primero, crear un árbol de expansión para garantizar conectividad
    // Esto asegura que todos los nodos sean alcanzables
    cout << "Creando conectividad básica..." << endl;

    for (int i = 1; i < numRouters; i++) {
        // Conectar router i con un router aleatorio entre 0 e i-1
        int randomPrev = rand() % i;
        int cost = (rand() % 20) + 1; // Costo entre 1 y 20

        addLink(routerIds[i], routerIds[randomPrev], cost);
    }

    // Ahora agregar enlaces adicionales aleatorios
    int linksCreated = numRouters - 1; // Ya creamos (numRouters-1) enlaces
    int linksToCreate = numLinks - linksCreated;

    cout << "Agregando enlaces adicionales..." << endl;

    // Set para rastrear enlaces ya creados
    set<pair<string, string>> existingLinks;

    // Registrar enlaces existentes
    for (const auto& pair : routers) {
        string r1 = pair.first;
        map<string, int> neighbors = pair.second.getNeighbors();
        for (const auto& neighbor : neighbors) {
            string r2 = neighbor.first;
            pair<string, string> link = (r1 < r2) ?
                                            make_pair(r1, r2) : make_pair(r2, r1);
            existingLinks.insert(link);
        }
    }

    // Crear enlaces adicionales
    int attempts = 0;
    int maxAttempts = numLinks * 10; // Evitar bucle infinito

    while (linksCreated < numLinks && attempts < maxAttempts) {
        attempts++;

        // Elegir dos routers aleatorios diferentes
        int idx1 = rand() % numRouters;
        int idx2 = rand() % numRouters;

        if (idx1 == idx2) {
            continue;
        }

        string r1 = routerIds[idx1];
        string r2 = routerIds[idx2];

        // Crear par ordenado
        pair<string, string> link = (r1 < r2) ?
                                        make_pair(r1, r2) : make_pair(r2, r1);

        // Si el enlace no existe, crearlo
        if (existingLinks.find(link) == existingLinks.end()) {
            int cost = (rand() % 20) + 1;
            addLink(r1, r2, cost);
            existingLinks.insert(link);
            linksCreated++;
        }
    }

    // Calcular tablas de enrutamiento
    updateAllRoutingTables();

    cout << "Red generada: " << routers.size() << " routers, "
         << linksCreated << " enlaces." << endl;

    // Guardar en archivo para referencia
    saveToFile("random_network.txt");
}

bool Red::routerHasRouteTo(string source, string destination) const {
    if (!routerExists(source)) {
        return false;
    }
    return routers.at(source).hasRouteTo(destination);
}

string Red::getNextHop(string source, string destination) const {
    if (!routerExists(source)) {
        return "";
    }
    return routers.at(source).getNextHopTo(destination);
}

void Red::displayRouterStatistics(string routerId) const {
    if (!routerExists(routerId)) {
        cout << "Error: El router " << routerId << " no existe." << endl;
        return;
    }
    routers.at(routerId).displayStatistics();
}

TablaDeEnrutamiento Red::getRouterTable(string routerId) const {
    if (!routerExists(routerId)) {
        return TablaDeEnrutamiento();
    }
    return routers.at(routerId).getRoutingTable();
}

void Red::compareRouterTables(string router1, string router2) const {
    if (!routerExists(router1) || !routerExists(router2)) {
        cout << "Error: Uno o ambos routers no existen." << endl;
        return;
    }

    TablaDeEnrutamiento table1 = routers.at(router1).getRoutingTable();
    TablaDeEnrutamiento table2 = routers.at(router2).getRoutingTable();

    cout << "\n=== Comparación de tablas ===" << endl;
    cout << router1 << ": " << table1.getNumEntries() << " rutas, "
         << "costo promedio: " << table1.getAverageCost() << endl;
    cout << router2 << ": " << table2.getNumEntries() << " rutas, "
         << "costo promedio: " << table2.getAverageCost() << endl;
}
