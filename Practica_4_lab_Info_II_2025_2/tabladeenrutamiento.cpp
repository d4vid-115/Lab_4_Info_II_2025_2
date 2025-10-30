#include "tabladeenrutamiento.h"

TablaDeEnrutamiento::TablaDeEnrutamiento() {}

#include "tabladeenrutamiento.h"
#include <iomanip>
#include <algorithm>
#include <numeric>

TablaDeEnrutamiento::TablaDeEnrutamiento() : ownerId("") {}

TablaDeEnrutamiento::TablaDeEnrutamiento(string routerId) : ownerId(routerId) {}

void TablaDeEnrutamiento::addEntry(const RoutingEntry& entry) {
    entries[entry.destination] = entry;
}

void TablaDeEnrutamiento::addEntry(string destination, int cost, vector<string> path) {
    RoutingEntry entry(destination, cost, path);
    entries[destination] = entry;
}

bool TablaDeEnrutamiento::removeEntry(string destination) {
    auto it = entries.find(destination);
    if (it != entries.end()) {
        entries.erase(it);
        return true;
    }
    return false;
}

void TablaDeEnrutamiento::clear() {
    entries.clear();
}

bool TablaDeEnrutamiento::hasRouteTo(string destination) const {
    return entries.find(destination) != entries.end();
}

int TablaDeEnrutamiento::getCostTo(string destination) const {
    auto it = entries.find(destination);
    if (it != entries.end()) {
        return it->second.cost;
    }
    return -1; // No hay ruta
}

vector<string> TablaDeEnrutamiento::getPathTo(string destination) const {
    auto it = entries.find(destination);
    if (it != entries.end()) {
        return it->second.path;
    }
    return vector<string>(); // Vector vacío
}

string TablaDeEnrutamiento::getNextHopTo(string destination) const {
    auto it = entries.find(destination);
    if (it != entries.end()) {
        return it->second.nextHop;
    }
    return ""; // No hay siguiente salto
}

RoutingEntry TablaDeEnrutamiento::getEntry(string destination) const {
    auto it = entries.find(destination);
    if (it != entries.end()) {
        return it->second;
    }
    return RoutingEntry(); // Entrada vacía
}

string TablaDeEnrutamiento::getOwnerId() const {
    return ownerId;
}

int TablaDeEnrutamiento::getNumEntries() const {
    return entries.size();
}

map<string, RoutingEntry> TablaDeEnrutamiento::getAllEntries() const {
    return entries;
}

void TablaDeEnrutamiento::updateTable(const map<string, RoutingEntry>& newEntries) {
    entries = newEntries;
}

void TablaDeEnrutamiento::display() const {
    cout << "\n╔════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║   Tabla de Enrutamiento del Router " << setw(25) << left << ownerId << "  ║" << endl;
    cout << "╠════════════════════════════════════════════════════════════════╣" << endl;

    if (entries.empty()) {
        cout << "║  (Tabla vacía - Sin rutas disponibles)                        ║" << endl;
        cout << "╚════════════════════════════════════════════════════════════════╝" << endl;
        return;
    }

    cout << "║ " << setw(12) << left << "Destino"
         << setw(8) << "Costo"
         << setw(12) << "Next Hop"
         << setw(30) << "Camino Completo" << " ║" << endl;
    cout << "╠════════════════════════════════════════════════════════════════╣" << endl;

    for (const auto& entry : entries) {
        cout << "║ " << setw(12) << left << entry.first
             << setw(8) << entry.second.cost
             << setw(12) << entry.second.nextHop << " ";

        // Mostrar camino
        string pathStr = "";
        for (size_t i = 0; i < entry.second.path.size(); i++) {
            pathStr += entry.second.path[i];
            if (i < entry.second.path.size() - 1) {
                pathStr += "→";
            }
        }
        cout << setw(29) << left << pathStr << "║" << endl;
    }

    cout << "╚════════════════════════════════════════════════════════════════╝" << endl;
    cout << "Total de rutas: " << entries.size() << endl;
}

void TablaDeEnrutamiento::displayCompact() const {
    cout << "\nTabla de " << ownerId << " (" << entries.size() << " rutas):" << endl;

    for (const auto& entry : entries) {
        cout << "  → " << entry.first
             << " [costo: " << entry.second.cost
             << ", via: " << entry.second.nextHop << "]" << endl;
    }
}

bool TablaDeEnrutamiento::equals(const TablaDeEnrutamiento& other) const {
    if (entries.size() != other.entries.size()) {
        return false;
    }

    for (const auto& entry : entries) {
        string dest = entry.first;

        if (!other.hasRouteTo(dest)) {
            return false;
        }

        if (entry.second.cost != other.getCostTo(dest)) {
            return false;
        }
    }

    return true;
}

string TablaDeEnrutamiento::getMostExpensiveRoute() const {
    if (entries.empty()) {
        return "";
    }

    int maxCost = -1;
    string maxDest = "";

    for (const auto& entry : entries) {
        if (entry.second.cost > maxCost) {
            maxCost = entry.second.cost;
            maxDest = entry.first;
        }
    }

    return maxDest;
}

string TablaDeEnrutamiento::getCheapestRoute() const {
    if (entries.empty()) {
        return "";
    }

    int minCost = numeric_limits<int>::max();
    string minDest = "";

    for (const auto& entry : entries) {
        if (entry.second.cost < minCost) {
            minCost = entry.second.cost;
            minDest = entry.first;
        }
    }

    return minDest;
}

int TablaDeEnrutamiento::getAverageCost() const {
    if (entries.empty()) {
        return 0;
    }

    int totalCost = 0;
    for (const auto& entry : entries) {
        totalCost += entry.second.cost;
    }

    return totalCost / entries.size();
}

vector<string> TablaDeEnrutamiento::getDestinations() const {
    vector<string> destinations;

    for (const auto& entry : entries) {
        destinations.push_back(entry.first);
    }

    return destinations;
}
