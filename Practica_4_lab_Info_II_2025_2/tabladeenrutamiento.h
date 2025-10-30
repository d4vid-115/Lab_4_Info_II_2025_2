#ifndef TABLADEENRUTAMIENTO_H
#define TABLADEENRUTAMIENTO_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

// Estructura para representar una entrada en la tabla
struct RoutingEntry {
    string destination;      // Router destino
    int cost;               // Costo total para llegar
    vector<string> path;    // Camino completo (secuencia de nodos)
    string nextHop;         // Siguiente salto (primer router después del origen)

    RoutingEntry() : destination(""), cost(-1), nextHop("") {}

    RoutingEntry(string dest, int c, vector<string> p)
        : destination(dest), cost(c), path(p) {
        // El siguiente salto es el segundo nodo del camino (después del origen)
        if (path.size() >= 2) {
            nextHop = path[1];
        } else {
            nextHop = "";
        }
    }
};

class TablaDeEnrutamiento {
private:
    string ownerId;                           // ID del router dueño de esta tabla
    map<string, RoutingEntry> entries;        // Entradas de enrutamiento

public:
    // Constructores
    TablaDeEnrutamiento();
    TablaDeEnrutamiento(string routerId);

    // Operaciones básicas
    void addEntry(const RoutingEntry& entry);
    void addEntry(string destination, int cost, vector<string> path);
    bool removeEntry(string destination);
    void clear();

    // Consultas
    bool hasRouteTo(string destination) const;
    int getCostTo(string destination) const;
    vector<string> getPathTo(string destination) const;
    string getNextHopTo(string destination) const;
    RoutingEntry getEntry(string destination) const;

    // Getters
    string getOwnerId() const;
    int getNumEntries() const;
    map<string, RoutingEntry> getAllEntries() const;

    // Actualización masiva
    void updateTable(const map<string, RoutingEntry>& newEntries);

    // Visualización
    void display() const;
    void displayCompact() const;

    // Comparación (útil para detectar cambios)
    bool equals(const TablaDeEnrutamiento& other) const;

    // Análisis
    string getMostExpensiveRoute() const;
    string getCheapestRoute() const;
    int getAverageCost() const;
    vector<string> getDestinations() const;
};

#endif
