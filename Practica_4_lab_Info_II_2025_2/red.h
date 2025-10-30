#ifndef RED_H
#define RED_H

#include "Router.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

class Red {
private:
    map<string, Router> routers;  // Todos los enrutadores de la red

    // Método auxiliar para calcular rutas más cortas (Dijkstra)
    map<string, RoutingEntry> calculateShortestPaths(string sourceId);

public:
    // Constructor
    Red();

    // Gestión de enrutadores
    void addRouter(string routerId);
    void removeRouter(string routerId);
    bool routerExists(string routerId) const;

    // Gestión de enlaces
    void addLink(string router1, string router2, int cost);
    void removeLink(string router1, string router2);
    void updateLinkCost(string router1, string router2, int newCost);

    // Actualización de tablas de enrutamiento
    void updateAllRoutingTables();

    // Consultas
    int getCostBetween(string source, string destination) const;
    vector<string> getPathBetween(string source, string destination) const;
    void displayNetwork() const;
    void displayRouterTable(string routerId) const;

    // Carga y guardado
    void loadFromFile(string filename);
    void saveToFile(string filename) const;

    // Generación aleatoria
    void generateRandomNetwork(int numRouters, int numLinks);

    // Nuevos métodos que aprovechan RoutingTable
    bool routerHasRouteTo(string source, string destination) const;
    string getNextHop(string source, string destination) const;
    void displayRouterStatistics(string routerId) const;
    TablaDeEnrutamiento getRouterTable(string routerId) const;
    void compareRouterTables(string router1, string router2) const;
};

#endif // RED_H
