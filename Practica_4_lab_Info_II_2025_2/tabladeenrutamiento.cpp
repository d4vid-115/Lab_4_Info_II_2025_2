#include "tabladeenrutamiento.h"
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <limits>

TablaDeEnrutamiento::TablaDeEnrutamiento() : idPropietario("") {}

TablaDeEnrutamiento::TablaDeEnrutamiento(string idRouter) : idPropietario(idRouter) {}

void TablaDeEnrutamiento::agregarEntrada(const EntradaDeEnrutamiento& entrada) {
    entradas[entrada.destino] = entrada;
}

void TablaDeEnrutamiento::agregarEntrada(string destino, int costo, vector<string> camino) {
    EntradaDeEnrutamiento entrada(destino, costo, camino);
    entradas[destino] = entrada;
}

bool TablaDeEnrutamiento::eliminarEntrada(string destino) {
    auto it = entradas.find(destino);
    if (it != entradas.end()) {
        entradas.erase(it);
        return true;
    }
    return false;
}

void TablaDeEnrutamiento::limpiar() {
    entradas.clear();
}

bool TablaDeEnrutamiento::tieneRutaHacia(string destino) const {
    return entradas.find(destino) != entradas.end();
}

int TablaDeEnrutamiento::obtenerCostoHacia(string destino) const {
    auto it = entradas.find(destino);
    if (it != entradas.end()) {
        return it->second.costo;
    }
    return -1; // No hay ruta
}

vector<string> TablaDeEnrutamiento::obtenerCaminoHacia(string destino) const {
    auto it = entradas.find(destino);
    if (it != entradas.end()) {
        return it->second.camino;
    }
    return vector<string>(); // Vector vacío
}

string TablaDeEnrutamiento::obtenerSiguienteSaltoHacia(string destino) const {
    auto it = entradas.find(destino);
    if (it != entradas.end()) {
        return it->second.siguienteSalto;
    }
    return ""; // No hay siguiente salto
}

EntradaDeEnrutamiento TablaDeEnrutamiento::obtenerEntrada(string destino) const {
    auto it = entradas.find(destino);
    if (it != entradas.end()) {
        return it->second;
    }
    return EntradaDeEnrutamiento(); // Entrada vacía
}

string TablaDeEnrutamiento::obtenerIdPropietario() const {
    return idPropietario;
}

int TablaDeEnrutamiento::obtenerNumeroEntradas() const {
    return entradas.size();
}

map<string, EntradaDeEnrutamiento> TablaDeEnrutamiento::obtenerTodasLasEntradas() const {
    return entradas;
}

void TablaDeEnrutamiento::actualizarTabla(const map<string, EntradaDeEnrutamiento>& nuevasEntradas) {
    entradas = nuevasEntradas;
}

void TablaDeEnrutamiento::mostrar() const {
    cout << "\n================================================" << endl;
    cout << "  Tabla de Enrutamiento del Router " << idPropietario << endl;
    cout << "================================================" << endl;

    if (entradas.empty()) {
        cout << "  (Tabla vacia - Sin rutas disponibles)" << endl;
        cout << "================================================" << endl;
        return;
    }

    cout << left << setw(12) << "Destino"
         << setw(8) << "Costo"
         << setw(14) << "Sig. Salto"
         << "Camino Completo" << endl;
    cout << "------------------------------------------------" << endl;

    for (const auto& entrada : entradas) {
        cout << left << setw(12) << entrada.first
             << setw(8) << entrada.second.costo
             << setw(14) << entrada.second.siguienteSalto;

        // Mostrar camino
        for (size_t i = 0; i < entrada.second.camino.size(); i++) {
            cout << entrada.second.camino[i];
            if (i < entrada.second.camino.size() - 1) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    cout << "------------------------------------------------" << endl;
    cout << "Total de rutas: " << entradas.size() << endl << endl;
}

void TablaDeEnrutamiento::mostrarCompacta() const {
    cout << "\nTabla de " << idPropietario << " (" << entradas.size() << " rutas):" << endl;

    for (const auto& entrada : entradas) {
        cout << "  -> " << entrada.first
             << " [costo: " << entrada.second.costo
             << ", via: " << entrada.second.siguienteSalto << "]" << endl;
    }
}

bool TablaDeEnrutamiento::esIgualA(const TablaDeEnrutamiento& otra) const {
    if (entradas.size() != otra.entradas.size()) {
        return false;
    }

    for (const auto& entrada : entradas) {
        string dest = entrada.first;

        if (!otra.tieneRutaHacia(dest)) {
            return false;
        }

        if (entrada.second.costo != otra.obtenerCostoHacia(dest)) {
            return false;
        }
    }

    return true;
}

string TablaDeEnrutamiento::obtenerRutaMasCostosa() const {
    if (entradas.empty()) {
        return "";
    }

    int costoMaximo = -1;
    string destinoMaximo = "";

    for (const auto& entrada : entradas) {
        if (entrada.second.costo > costoMaximo) {
            costoMaximo = entrada.second.costo;
            destinoMaximo = entrada.first;
        }
    }

    return destinoMaximo;
}

string TablaDeEnrutamiento::obtenerRutaMasEconomica() const {
    if (entradas.empty()) {
        return "";
    }

    int costoMinimo = numeric_limits<int>::max();
    string destinoMinimo = "";

    for (const auto& entrada : entradas) {
        if (entrada.second.costo < costoMinimo) {
            costoMinimo = entrada.second.costo;
            destinoMinimo = entrada.first;
        }
    }

    return destinoMinimo;
}

int TablaDeEnrutamiento::obtenerCostoPromedio() const {
    if (entradas.empty()) {
        return 0;
    }

    int costoTotal = 0;
    for (const auto& entrada : entradas) {
        costoTotal += entrada.second.costo;
    }

    return costoTotal / entradas.size();
}

vector<string> TablaDeEnrutamiento::obtenerDestinos() const {
    vector<string> destinos;

    for (const auto& entrada : entradas) {
        destinos.push_back(entrada.first);
    }

    return destinos;
}
