#include "Router.h"
#include <iostream>
#include <iomanip>

Router::Router() : id(""), TablaDeEnrutamiento() {}

Router::Router(string routerId) : id(routerId), TablaDeEnrutamiento(routerId) {}

string Router::getId() const {
    return id;
}

map<string, int> Router::getNeighbors() const {
    return neighbors;
}

TablaDeEnrutamiento Router::getRoutingTable() const {
    return TablaDeEnrutamiento;
}

TablaDeEnrutamiento* Router::getRoutingTablePtr() {
    return &TablaDeEnrutamiento;
}

void Router::addNeighbor(string neighborId, int cost) {
    neighbors[neighborId] = cost;
}

void Router::removeNeighbor(string neighborId) {
    neighbors.erase(neighborId);
}

void Router::updateNeighborCost(string neighborId, int newCost) {
    if (neighbors.find(neighborId) != neighbors.end()) {
        neighbors[neighborId] = newCost;
    }
}

bool Router::isNeighbor(string routerId) const {
    return neighbors.find(routerId) != neighbors.end();
}

void Router::updateRoutingTable(const map<string, RoutingEntry>& newTable) {
    TablaDeEnrutamiento.updateTable(newTable);
}

void Router::displayRoutingTable() const {
    TablaDeEnrutamiento.display();
}

int Router::getCostTo(string destination) const {
    return TablaDeEnrutamiento.getCostTo(destination);
}

vector<string> Router::getPathTo(string destination) const {
    return TablaDeEnrutamiento.getPathTo(destination);
}

string Router::getNextHopTo(string destination) const {
    return TablaDeEnrutamiento.getNextHopTo(destination);
}

bool Router::hasRouteTo(string destination) const {
    return TablaDeEnrutamiento.hasRouteTo(destination);
}

void Router::displayStatistics() const {
    cout << "\n=== Estadísticas del Router " << id << " ===" << endl;
    cout << "Vecinos directos: " << neighbors.size() << endl;
    cout << "Rutas conocidas: " << TablaDeEnrutamiento.getNumEntries() << endl;

    if (TablaDeEnrutamiento.getNumEntries() > 0) {
        cout << "Costo promedio: " << TablaDeEnrutamiento.getAverageCost() << endl;

        string mostExpensive = TablaDeEnrutamiento.getMostExpensiveRoute();
        if (!mostExpensive.empty()) {
            cout << "Ruta más costosa: " << mostExpensive
                 << " (costo: " << TablaDeEnrutamiento.getCostTo(mostExpensive) << ")" << endl;
        }

        string cheapest = TablaDeEnrutamiento.getCheapestRoute();
        if (!cheapest.empty()) {
            cout << "Ruta más económica: " << cheapest
                 << " (costo: " << TablaDeEnrutamiento.getCostTo(cheapest) << ")" << endl;
        }
    }

    cout << "===================================\n" << endl;
}
