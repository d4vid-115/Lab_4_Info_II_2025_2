#ifndef ROUTER_H
#define ROUTER_H

#include "tabladeenrutamiento.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

class Router {
private:
    string id;                              // Identificador del enrutador
    map<string, int> vecinos;               // Vecinos directos y sus costos
    TablaDeEnrutamiento tablaEnrutamiento;  // Tabla de enrutamiento (objeto, NO clase)

public:
    // Constructores
    Router();
    Router(string idRouter);

    // Getters
    string obtenerId() const;
    map<string, int> obtenerVecinos() const;
    TablaDeEnrutamiento obtenerTablaEnrutamiento() const;
    TablaDeEnrutamiento* obtenerPunteroTablaEnrutamiento();

    // Métodos para gestionar vecinos
    void agregarVecino(string idVecino, int costo);
    void eliminarVecino(string idVecino);
    void actualizarCostoVecino(string idVecino, int nuevoCosto);
    bool esVecino(string idRouter) const;

    // Métodos para la tabla de enrutamiento
    void actualizarTablaEnrutamiento(const map<string, EntradaDeEnrutamiento>& nuevaTabla);
    void mostrarTablaEnrutamiento() const;

    // Consultas de enrutamiento (delegan a TablaDeEnrutamiento)
    int obtenerCostoHacia(string destino) const;
    vector<string> obtenerCaminoHacia(string destino) const;
    string obtenerSiguienteSaltoHacia(string destino) const;
    bool tieneRutaHacia(string destino) const;

    // Análisis
    void mostrarEstadisticas() const;
};

#endif
