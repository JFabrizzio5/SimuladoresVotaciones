#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Opcion {
    string nombre;
    int votos;
};

struct Votacion {
    string nombre;
    vector<Opcion> opciones;
};

vector<Votacion> cargarVotaciones(const string& nombreArchivo) {
    vector<Votacion> votaciones;
    ifstream archivo(nombreArchivo.c_str());  // Usar c_str() para convertir a const char*
    string linea;
    Votacion votacionActual;

    while (getline(archivo, linea)) {
        if (linea.empty()) {
            continue;
        }
        if (linea.find(':') == string::npos) {
            if (!votacionActual.nombre.empty()) {
                votaciones.push_back(votacionActual);
                votacionActual = Votacion();
            }
            votacionActual.nombre = linea;
        } else {
            istringstream ss(linea);
            Opcion opcion;
            getline(ss, opcion.nombre, ':');
            ss >> opcion.votos;
            votacionActual.opciones.push_back(opcion);
        }
    }
    if (!votacionActual.nombre.empty()) {
        votaciones.push_back(votacionActual);
    }

    return votaciones;
}

void guardarVotaciones(const string& nombreArchivo, const vector<Votacion>& votaciones) {
    ofstream archivo(nombreArchivo.c_str());  // Usar c_str() para convertir a const char*
    for (size_t i = 0; i < votaciones.size(); ++i) {
        archivo << votaciones[i].nombre << "\n";
        for (size_t j = 0; j < votaciones[i].opciones.size(); ++j) {
            archivo << votaciones[i].opciones[j].nombre << ": " << votaciones[i].opciones[j].votos << "\n";
        }
    }
}

void mostrarVotaciones(const vector<Votacion>& votaciones) {
    for (size_t i = 0; i < votaciones.size(); ++i) {
        cout << "Votacion: " << votaciones[i].nombre << "\n";
        for (size_t j = 0; j < votaciones[i].opciones.size(); ++j) {
            cout << "  " << votaciones[i].opciones[j].nombre << ": " << votaciones[i].opciones[j].votos << "\n";
        }
    }
}

void votar(vector<Votacion>& votaciones, const string& nombreVotacion, const string& nombreOpcion) {
    for (size_t i = 0; i < votaciones.size(); ++i) {
        if (votaciones[i].nombre == nombreVotacion) {
            for (size_t j = 0; j < votaciones[i].opciones.size(); ++j) {
                if (votaciones[i].opciones[j].nombre == nombreOpcion) {
                    votaciones[i].opciones[j].votos++;
                    return;
                }
            }
            Opcion nuevaOpcion = {nombreOpcion, 1};
            votaciones[i].opciones.push_back(nuevaOpcion);
            return;
        }
    }
    Opcion nuevaOpcion = {nombreOpcion, 1};
    Votacion nuevaVotacion;
    nuevaVotacion.nombre = nombreVotacion;
    nuevaVotacion.opciones.push_back(nuevaOpcion);
    votaciones.push_back(nuevaVotacion);
}

void agregarOpcion(vector<Votacion>& votaciones, const string& nombreVotacion, const string& nombreOpcion) {
    for (size_t i = 0; i < votaciones.size(); ++i) {
        if (votaciones[i].nombre == nombreVotacion) {
            Opcion nuevaOpcion = {nombreOpcion, 0};
            votaciones[i].opciones.push_back(nuevaOpcion);
            return;
        }
    }
    Opcion nuevaOpcion = {nombreOpcion, 0};
    Votacion nuevaVotacion;
    nuevaVotacion.nombre = nombreVotacion;
    nuevaVotacion.opciones.push_back(nuevaOpcion);
    votaciones.push_back(nuevaVotacion);
}

struct EliminarOpcion {
    string nombreOpcion;
    EliminarOpcion(const string& nombre) : nombreOpcion(nombre) {}
    bool operator()(const Opcion& opcion) {
        return opcion.nombre == nombreOpcion;
    }
};

void eliminarOpcion(vector<Votacion>& votaciones, const string& nombreVotacion, const string& nombreOpcion) {
    for (size_t i = 0; i < votaciones.size(); ++i) {
        if (votaciones[i].nombre == nombreVotacion) {
            votaciones[i].opciones.erase(
                remove_if(votaciones[i].opciones.begin(), votaciones[i].opciones.end(), EliminarOpcion(nombreOpcion)),
                votaciones[i].opciones.end()
            );
            return;
        }
    }
}

int main() {
    const string nombreArchivo = "votaciones.txt";
    vector<Votacion> votaciones = cargarVotaciones(nombreArchivo);

    int opcion;
    do {
        cout << "Menu de Votaciones:\n";
        cout << "1. Mostrar votaciones\n";
        cout << "2. Votar\n";
        cout << "3. Agregar opcion\n";
        cout << "4. Eliminar opcion\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            mostrarVotaciones(votaciones);
        } else if (opcion == 2) {
            string nombreVotacion, nombreOpcion;
            cout << "Nombre de la votacion: ";
            cin >> nombreVotacion;
            cout << "Nombre de la opcion: ";
            cin >> nombreOpcion;
            votar(votaciones, nombreVotacion, nombreOpcion);
            guardarVotaciones(nombreArchivo, votaciones);
        } else if (opcion == 3) {
            string nombreVotacion, nombreOpcion;
            cout << "Nombre de la votacion: ";
            cin >> nombreVotacion;
            cout << "Nombre de la nueva opcion: ";
            cin >> nombreOpcion;
            agregarOpcion(votaciones, nombreVotacion, nombreOpcion);
            guardarVotaciones(nombreArchivo, votaciones);
        } else if (opcion == 4) {
            string nombreVotacion, nombreOpcion;
            cout << "Nombre de la votacion: ";
            cin >> nombreVotacion;
            cout << "Nombre de la opcion a eliminar: ";
            cin >> nombreOpcion;
            eliminarOpcion(votaciones, nombreVotacion, nombreOpcion);
            guardarVotaciones(nombreArchivo, votaciones);
        }
    } while (opcion != 5);

    return 0;
}

