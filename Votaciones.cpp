#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <limits> 
using namespace std;

struct Opcion {
    string nombre;
    int votos;
};

struct Votacion {
    int id;
    string nombre;
    vector<Opcion> opciones;
};

vector<Votacion> cargarVotaciones(const string& nombreArchivo);
void guardarVotaciones(const string& nombreArchivo, const vector<Votacion>& votaciones);
void mostrarVotaciones(const vector<Votacion>& votaciones);
void votar(vector<Votacion>& votaciones, int idVotacion, const string& nombreOpcion);
void agregarOpcion(vector<Votacion>& votaciones, int idVotacion, const string& nombreOpcion);
void eliminarOpcion(vector<Votacion>& votaciones, int idVotacion, const string& nombreOpcion);

map<string, string> cargarUsuarios(const string& nombreArchivo);
void guardarUsuarios(const string& nombreArchivo, const map<string, string>& usuarios);
bool registrarUsuario(map<string, string>& usuarios, const string& nombreArchivo);
bool iniciarSesion(const map<string, string>& usuarios);

int main() {
    const string archivoUsuarios = "usuarios.txt";
    const string archivoVotaciones = "votaciones.txt";

    map<string, string> usuarios = cargarUsuarios(archivoUsuarios);
    vector<Votacion> votaciones = cargarVotaciones(archivoVotaciones);

    int opcion;
    do {
        cout << "Menu de Autenticacion:\n";
        cout << "1. Registrarse\n";
        cout << "2. Iniciar sesion\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the input buffer

        if (opcion == 1) {
            if (registrarUsuario(usuarios, archivoUsuarios)) {
                cout << "Registro exitoso!\n";
            } else {
                cout << "Error en el registro. El usuario ya existe.\n";
            }
        } else if (opcion == 2) {
            if (iniciarSesion(usuarios)) {
                cout << "Inicio de sesion exitoso!\n";
                break;
            } else {
                cout << "Usuario o contrasena incorrectos.\n";
            }
        }
    } while (opcion != 3);

    if (opcion == 3) {
        return 0;
    }

    do {
        cout << "Menu de Votaciones:\n";
        cout << "1. Mostrar votaciones\n";
        cout << "2. Votar\n";
        cout << "3. Agregar opcion\n";
        cout << "4. Eliminar opcion\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the input buffer

        if (opcion == 1) {
            mostrarVotaciones(votaciones);
        } else if (opcion == 2) {
            int idVotacion;
            string nombreOpcion;
            cout << "ID de la votacion: ";
            cin >> idVotacion;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the input buffer
            cout << "Nombre de la opcion: ";
            getline(cin, nombreOpcion);
            votar(votaciones, idVotacion, nombreOpcion);
            guardarVotaciones(archivoVotaciones, votaciones);
        } else if (opcion == 3) {
            int idVotacion;
            string nombreOpcion;
            cout << "ID de la votacion: ";
            cin >> idVotacion;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the input buffer
            cout << "Nombre de la nueva opcion: ";
            getline(cin, nombreOpcion);
            agregarOpcion(votaciones, idVotacion, nombreOpcion);
            guardarVotaciones(archivoVotaciones, votaciones);
        } else if (opcion == 4) {
            int idVotacion;
            string nombreOpcion;
            cout << "ID de la votacion: ";
            cin >> idVotacion;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the input buffer
            cout << "Nombre de la opcion a eliminar: ";
            getline(cin, nombreOpcion);
            eliminarOpcion(votaciones, idVotacion, nombreOpcion);
            guardarVotaciones(archivoVotaciones, votaciones);
        }
    } while (opcion != 5);

    return 0;
}

vector<Votacion> cargarVotaciones(const string& nombreArchivo) {
    vector<Votacion> votaciones;
    ifstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return votaciones;
    }
    string linea;
    Votacion votacionActual;
    int maxId = 0;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        if (linea.find(':') == string::npos) {
            if (!votacionActual.nombre.empty()) {
                votaciones.push_back(votacionActual);
                votacionActual = Votacion();
            }
            istringstream ss(linea);
            ss >> votacionActual.id;
            ss.ignore(1); // Ignore the space after ID
            getline(ss, votacionActual.nombre);
            maxId = max(maxId, votacionActual.id);
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

    // Ensure unique IDs for new votations
    for (size_t i = 0; i < votaciones.size(); ++i) {
        if (votaciones[i].id == 0) {
            votaciones[i].id = ++maxId;
        }
    }

    return votaciones;
}

void guardarVotaciones(const string& nombreArchivo, const vector<Votacion>& votaciones) {
    ofstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return;
    }
    for (size_t i = 0; i < votaciones.size(); ++i) {
        archivo << votaciones[i].id << " " << votaciones[i].nombre << "\n";
        for (size_t j = 0; j < votaciones[i].opciones.size(); ++j) {
            archivo << votaciones[i].opciones[j].nombre << ": " << votaciones[i].opciones[j].votos << "\n";
        }
    }
}

void mostrarVotaciones(const vector<Votacion>& votaciones) {
    for (size_t i = 0; i < votaciones.size(); ++i) {
        cout << "ID: " << votaciones[i].id << " - Votacion: " << votaciones[i].nombre << "\n";
        for (size_t j = 0; j < votaciones[i].opciones.size(); ++j) {
            cout << "  " << votaciones[i].opciones[j].nombre << ": " << votaciones[i].opciones[j].votos << "\n";
        }
    }
}

void votar(vector<Votacion>& votaciones, int idVotacion, const string& nombreOpcion) {
    for (size_t i = 0; i < votaciones.size(); ++i) {
        if (votaciones[i].id == idVotacion) {
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
}

void agregarOpcion(vector<Votacion>& votaciones, int idVotacion, const string& nombreOpcion) {
    for (size_t i = 0; i < votaciones.size(); ++i) {
        if (votaciones[i].id == idVotacion) {
            Opcion nuevaOpcion = {nombreOpcion, 0};
            votaciones[i].opciones.push_back(nuevaOpcion);
            return;
        }
    }
    Opcion nuevaOpcion = {nombreOpcion, 0};
    Votacion nuevaVotacion;
    nuevaVotacion.id = idVotacion;
    nuevaVotacion.nombre = "Votacion " + idVotacion;
    nuevaVotacion.opciones.push_back(nuevaOpcion);
    votaciones.push_back(nuevaVotacion);
}

void eliminarOpcion(vector<Votacion>& votaciones, int idVotacion, const string& nombreOpcion) {
    for (size_t i = 0; i < votaciones.size(); ++i) {
        if (votaciones[i].id == idVotacion) {
            for (size_t j = 0; j < votaciones[i].opciones.size(); ++j) {
                if (votaciones[i].opciones[j].nombre == nombreOpcion) {
                    votaciones[i].opciones.erase(votaciones[i].opciones.begin() + j);
                    return;
                }
            }
        }
    }
}

map<string, string> cargarUsuarios(const string& nombreArchivo) {
    map<string, string> usuarios;
    ifstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return usuarios;
    }
    string linea;
    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string usuario, contrasena;
        ss >> usuario >> contrasena;
        usuarios[usuario] = contrasena;
    }
    return usuarios;
}

void guardarUsuarios(const string& nombreArchivo, const map<string, string>& usuarios) {
    ofstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo " << nombreArchivo << endl;
        return;
    }
    for (map<string, string>::const_iterator it = usuarios.begin(); it != usuarios.end(); ++it) {
        archivo << it->first << " " << it->second << "\n";
    }
}

bool registrarUsuario(map<string, string>& usuarios, const string& nombreArchivo) {
    string usuario, contrasena;
    cout << "Ingrese nombre de usuario: ";
    cin >> usuario;
    cout << "Ingrese contrasena: ";
    cin >> contrasena;
    if (usuarios.find(usuario) != usuarios.end()) {
        return false; // Usuario ya existe
    }
    usuarios[usuario] = contrasena;
    guardarUsuarios(nombreArchivo, usuarios);
    return true;
}

bool iniciarSesion(const map<string, string>& usuarios) {
    string usuario, contrasena;
    cout << "Ingrese nombre de usuario: ";
    cin >> usuario;
    cout << "Ingrese contrasena: ";
    cin >> contrasena;
    map<string, string>::const_iterator it = usuarios.find(usuario);
    if (it != usuarios.end() && it->second == contrasena) {
        return true;
    }
    return false;
}

