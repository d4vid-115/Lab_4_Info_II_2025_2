#ifndef ROUTER_H
#define ROUTER_H

#include "tabladeenrutamiento.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

class Router {
private:
    string id;                           // Identificador del enrutador
    map<string, int> neighbors;          // Vecinos directos y sus costos
    TablaDeEnrutamiento TablaDeEnrutamiento;           // Tabla de enrutamiento (ahora es un objeto)

public:
    // Constructores
    Router();
    Router(string routerId);

    // Getters
    string getId() const;
    map<string, int> getNeighbors() const;
    TablaDeEnrutamiento getRoutingTable() const;  // Ahora devuelve objeto RoutingTable
    TablaDeEnrutamiento* getRoutingTablePtr();    // Para modificación directa

    // Métodos para gestionar vecinos
    void addNeighbor(string neighborId, int cost);
    void removeNeighbor(string neighborId);
    void updateNeighborCost(string neighborId, int newCost);
    bool isNeighbor(string routerId) const;

    // Métodos para la tabla de enrutamiento
    void updateRoutingTable(const map<string, RoutingEntry>& newTable);
    void displayRoutingTable() const;

    // Consultas de enrutamiento (delegan a RoutingTable)
    int getCostTo(string destination) const;
    vector<string> getPathTo(string destination) const;
    string getNextHopTo(string destination) const;
    bool hasRouteTo(string destination) const;

    // Análisis
    void displayStatistics() const;
};

#endif
