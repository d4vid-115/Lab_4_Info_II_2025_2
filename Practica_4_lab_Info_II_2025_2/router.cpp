#include "Router.h"
#include <iostream>
#include <iomanip>

Router::Router() : id(""), tablaEnrutamiento() {}

Router::Router(string idRouter) : id(idRouter), tablaEnrutamiento(idRouter) {}

string Router::obtenerId() const {
    return id;
}

map<string, int> Router::obtenerVecinos() const {
    return vecinos;
}

TablaDeEnrutamiento Router::obtenerTablaEnrutamiento() const {
    return tablaEnrutamiento;
}

TablaDeEnrutamiento* Router::obtenerPunteroTablaEnrutamiento() {
    return &tablaEnrutamiento;
}

void Router::agregarVecino(string idVecino, int costo) {
    vecinos[idVecino] = costo;
}

void Router::eliminarVecino(string idVecino) {
    vecinos.erase(idVecino);
}

void Router::actualizarCostoVecino(string idVecino, int nuevoCosto) {
    if (vecinos.find(idVecino) != vecinos.end()) {
        vecinos[idVecino] = nuevoCosto;
    }
}

bool Router::esVecino(string idRouter) const {
    return vecinos.find(idRouter) != vecinos.end();
}

void Router::actualizarTablaEnrutamiento(const map<string, EntradaDeEnrutamiento>& nuevaTabla) {
    tablaEnrutamiento.actualizarTabla(nuevaTabla);
}

void Router::mostrarTablaEnrutamiento() const {
    tablaEnrutamiento.mostrar();
}

int Router::obtenerCostoHacia(string destino) const {
    return tablaEnrutamiento.obtenerCostoHacia(destino);
}

vector<string> Router::obtenerCaminoHacia(string destino) const {
    return tablaEnrutamiento.obtenerCaminoHacia(destino);
}

string Router::obtenerSiguienteSaltoHacia(string destino) const {
    return tablaEnrutamiento.obtenerSiguienteSaltoHacia(destino);
}

bool Router::tieneRutaHacia(string destino) const {
    return tablaEnrutamiento.tieneRutaHacia(destino);
}

void Router::mostrarEstadisticas() const {
    cout << "\n=== Estadisticas del Router " << id << " ===" << endl;
    cout << "Vecinos directos: " << vecinos.size() << endl;
    cout << "Rutas conocidas: " << tablaEnrutamiento.obtenerNumeroEntradas() << endl;

    if (tablaEnrutamiento.obtenerNumeroEntradas() > 0) {
        cout << "Costo promedio: " << tablaEnrutamiento.obtenerCostoPromedio() << endl;

        string masCostosa = tablaEnrutamiento.obtenerRutaMasCostosa();
        if (!masCostosa.empty()) {
            cout << "Ruta mas costosa: " << masCostosa
                 << " (costo: " << tablaEnrutamiento.obtenerCostoHacia(masCostosa) << ")" << endl;
        }

        string masBarata = tablaEnrutamiento.obtenerRutaMasEconomica();
        if (!masBarata.empty()) {
            cout << "Ruta mas economica: " << masBarata
                 << " (costo: " << tablaEnrutamiento.obtenerCostoHacia(masBarata) << ")" << endl;
        }
    }

    cout << "===================================\n" << endl;
}
