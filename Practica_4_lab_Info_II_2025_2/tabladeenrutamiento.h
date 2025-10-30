#ifndef TABLADEENRUTAMIENTO_H
#define TABLADEENRUTAMIENTO_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

// Estructura para representar una entrada en la tabla
struct EntradaDeEnrutamiento {
    string destino;              // Router destino
    int costo;                   // Costo total para llegar
    vector<string> camino;       // Camino completo (secuencia de nodos)
    string siguienteSalto;       // Siguiente salto (primer router después del origen)

    EntradaDeEnrutamiento() : destino(""), costo(-1), siguienteSalto("") {}

    EntradaDeEnrutamiento(string dest, int c, vector<string> cam)
        : destino(dest), costo(c), camino(cam) {
        // El siguiente salto es el segundo nodo del camino (después del origen)
        if (camino.size() >= 2) {
            siguienteSalto = camino[1];
        } else {
            siguienteSalto = "";
        }
    }
};

class TablaDeEnrutamiento {
private:
    string idPropietario;                              // ID del router dueño de esta tabla
    map<string, EntradaDeEnrutamiento> entradas;       // Entradas de enrutamiento

public:
    // Constructores
    TablaDeEnrutamiento();
    TablaDeEnrutamiento(string idRouter);

    // Operaciones básicas
    void agregarEntrada(const EntradaDeEnrutamiento& entrada);
    void agregarEntrada(string destino, int costo, vector<string> camino);
    bool eliminarEntrada(string destino);
    void limpiar();

    // Consultas
    bool tieneRutaHacia(string destino) const;
    int obtenerCostoHacia(string destino) const;
    vector<string> obtenerCaminoHacia(string destino) const;
    string obtenerSiguienteSaltoHacia(string destino) const;
    EntradaDeEnrutamiento obtenerEntrada(string destino) const;

    // Getters
    string obtenerIdPropietario() const;
    int obtenerNumeroEntradas() const;
    map<string, EntradaDeEnrutamiento> obtenerTodasLasEntradas() const;

    // Actualización masiva
    void actualizarTabla(const map<string, EntradaDeEnrutamiento>& nuevasEntradas);

    // Visualización
    void mostrar() const;
    void mostrarCompacta() const;

    // Comparación (útil para detectar cambios)
    bool esIgualA(const TablaDeEnrutamiento& otra) const;

    // Análisis
    string obtenerRutaMasCostosa() const;
    string obtenerRutaMasEconomica() const;
    int obtenerCostoPromedio() const;
    vector<string> obtenerDestinos() const;
};

#endif
