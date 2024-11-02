#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "GrafoML.h"
#include "Coordenada.h"

void recuperarDatos(std::string nombreArchivoEntrada, std::vector<GrafoML<Coordenada, double>>& grafos);
double calcularDistanciaEuclidiana(Coordenada c1, Coordenada c2);
void escribirArchivoSalida(std::string nombreArchivoSalida, std::vector<GrafoML<Coordenada, double>>& grafos);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " archivo_entrada.txt archivo_salida.txt" << std::endl;
        return 1;
    }

    std::string nombreArchivoEntrada = argv[1];
    std::string nombreArchivoSalida = argv[2];

    // Vector de grafos para almacenar los datos
    std::vector<GrafoML<Coordenada, double>> grafos;

    recuperarDatos(nombreArchivoEntrada, grafos);

    std::cout << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < grafos.size(); i++) {
        std::cout << "Circuito [" << i + 1 << "]" << std::endl;
        grafos[i].imprimir();
    }

    double x = 0;
    double y = 0;

    Coordenada coordenadaInicial(x, y);

    std::cout << std::endl;
    for (int i = 0; i < grafos.size(); i++) {
        std::cout << "Camino de circuito para: [" << i + 1 << "]" << std::endl;
        grafos[i].encontrarCaminoHamiltonConCostoMinimo();
        std::cout << std::endl;
    }

    // Llamar a la función para escribir el archivo de salida
    escribirArchivoSalida(nombreArchivoSalida, grafos);

    return 0;
}

void recuperarDatos(std::string nombreArchivoEntrada, std::vector<GrafoML<Coordenada, double>>& grafos) {
    std::ifstream archivoEntrada(nombreArchivoEntrada);

    if (!archivoEntrada.is_open()) {
        std::cerr << "No fue posible abrir el archivo de entrada" << std::endl;
        return;
    } else {
        std::cout << "El archivo de entrada se abrió con éxito" << std::endl;
    }

    int numCircuitos;
    archivoEntrada >> numCircuitos;
    grafos.resize(numCircuitos);

    for (int i = 0; i < numCircuitos; ++i) {
        int numAgujeros;
        archivoEntrada >> numAgujeros;

        GrafoML<Coordenada, double> grafo;

        // Vector temporal para almacenar coordenadas del circuito actual
        std::vector<Coordenada> agujeros;

        for (int j = 0; j < numAgujeros + 1; ++j) {
            if (j == 0) {
                double x = 0, y = 0;
                Coordenada coordInicial(x, y);
                agujeros.push_back(coordInicial);
                grafo.insertarVertice(coordInicial);
            }

            double x, y;
            archivoEntrada >> x >> y;
            Coordenada coord(x, y);

            agujeros.push_back(coord);
            grafo.insertarVertice(coord);  // Insertar Coordenada como vértice

            for (int k = 0; k < j; ++k) {
                double costo = calcularDistanciaEuclidiana(agujeros[k], coord); // Calcular la distancia
                grafo.insertarAristaNoDirigida(agujeros[k], coord, costo); // Insertar arista entre el nuevo vértice y los anteriores
            }
        }

        grafos[i] = grafo;
    }

    archivoEntrada.close();
    std::cout << "\nDatos cargados exitosamente." << std::endl;
}

double calcularDistanciaEuclidiana(Coordenada c1, Coordenada c2) {
    double deltaX = c1.obtenerX(); 
    double deltaY = c1.obtenerY(); 
    double deltaX2 = c2.obtenerX(); 
    double deltaY2 = c2.obtenerY(); 
    
    return std::sqrt((deltaX - deltaX2) * (deltaX - deltaX2) + (deltaY - deltaY2) * (deltaY - deltaY2));
}

void escribirArchivoSalida(std::string nombreArchivoSalida, std::vector<GrafoML<Coordenada, double>>& grafos) {
    std::ofstream archivoSalida(nombreArchivoSalida);
    if (!archivoSalida.is_open()) {
        std::cerr << "No fue posible abrir el archivo de salida" << std::endl;
        return;
    }

    archivoSalida << grafos.size() << std::endl;

    for (int i = 0; i < grafos.size(); ++i) {
        grafos[i].encontrarCaminoHamiltonConCostoMinimo();
        int numAgujeros = grafos[i].cantVertices() - 1;  // Excluir el punto inicial (0,0)
        archivoSalida << numAgujeros << std::endl;

        auto vertices = grafos[i].getVertices();

        for (auto coord : vertices) {  // Evitamos el `const` al copiar `coord`
            archivoSalida << coord.obtenerX() << " " << coord.obtenerY() << std::endl;
        }

        // Crear un vector de índices que representa el orden de visita de los vértices
        std::vector<int> orden;
        for (int j = 0; j < vertices.size(); ++j) {
            orden.push_back(j);  // Agregamos cada índice al vector de orden
        }

        double distanciaTotal = grafos[i].calcularCostoRecorrido(orden);
        archivoSalida << "Distancia total recorrida: " << distanciaTotal << " mm" << std::endl;
    }

    archivoSalida.close();
    std::cout << "Archivo de salida generado exitosamente." << std::endl;
}
