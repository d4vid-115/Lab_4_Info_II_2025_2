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
    map<string, EntradaDeEnrutamiento> calcularCaminosMasCortos(string idOrigen);

public:
    // Constructor
    Red();

    // Gestión de enrutadores
    void agregarRouter(string idRouter);
    void eliminarRouter(string idRouter);
    bool existeRouter(string idRouter) const;

    // Gestión de enlaces
    void agregarEnlace(string router1, string router2, int costo);
    void eliminarEnlace(string router1, string router2);
    void actualizarCostoEnlace(string router1, string router2, int nuevoCosto);

    // Actualización de tablas de enrutamiento
    void actualizarTodasLasTablas();

    // Consultas
    int obtenerCostoEntre(string origen, string destino) const;
    vector<string> obtenerCaminoEntre(string origen, string destino) const;
    void mostrarRed() const;
    void mostrarTablaRouter(string idRouter) const;

    // Carga y guardado
    void cargarDesdeArchivo(string nombreArchivo);
    void guardarEnArchivo(string nombreArchivo) const;

    // Generación aleatoria
    void generarRedAleatoria(int numRouters, int numEnlaces);

    // Métodos adicionales
    bool routerTieneRutaHacia(string origen, string destino) const;
    string obtenerSiguienteSalto(string origen, string destino) const;
    void mostrarEstadisticasRouter(string idRouter) const;
    TablaDeEnrutamiento obtenerTablaRouter(string idRouter) const;
    void compararTablasRouters(string router1, string router2) const;
};

#endif // RED_H
