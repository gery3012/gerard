#include <iostream>
#include <fstream>
#include <cstring>

const int MAX_NOMBRE = 30;
const char* NOMBRE_ARCHIVO = "inventario.dat";

enum Categoria {
    ELECTRONICA,
    ROPA,
    ALIMENTOS
};

struct Producto {
    int codigo;
    char nombre[MAX_NOMBRE];
    double precio;
    int stock;
    Categoria categoria;
    bool activo;

    void mostrar() const {
        std::cout << "Código: " << codigo << ", Nombre: " << nombre
                  << ", Precio: " << precio << ", Stock: " << stock
                  << ", Categoría: " << categoria << ", Activo: " << (activo ? "Sí" : "No") << std::endl;
    }
};

void cargarProductos(std::fstream& archivo) {
    archivo.seekg(0, std::ios::beg);
    Producto p;
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.activo) {
            p.mostrar();
        }
    }
}

void agregarProducto(std::fstream& archivo) {
    Producto p;
    std::cout << "Ingrese código: ";
    std::cin >> p.codigo;
    std::cin.ignore(); // Limpiar el buffer
    std::cout << "Ingrese nombre: ";
    std::cin.getline(p.nombre, MAX_NOMBRE);
    std::cout << "Ingrese precio: ";
    std::cin >> p.precio;
    std::cout << "Ingrese stock: ";
    std::cin >> p.stock;
    std::cout << "Ingrese categoría (0: Electrónica, 1: Ropa, 2: Alimentos): ";
    int cat;
    std::cin >> cat;
    p.categoria = static_cast<Categoria>(cat);
    p.activo = true;

    archivo.seekp(0, std::ios::end);
    archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
}

void modificarProducto(std::fstream& archivo) {
    int codigo;
    std::cout << "Ingrese el código del producto a modificar: ";
    std::cin >> codigo;

    Producto p;
    archivo.seekg(0, std::ios::beg);
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigo && p.activo) {
            std::cout << "Producto encontrado:\n";
            p.mostrar();

            std::cout << "Ingrese nuevo nombre: ";
            std::cin.ignore();
            std::cin.getline(p.nombre, MAX_NOMBRE);
            std::cout << "Ingrese nuevo precio: ";
            std::cin >> p.precio;
            std::cout << "Ingrese nuevo stock: ";
            std::cin >> p.stock;
            std::cout << "Ingrese nueva categoría (0: Electrónica, 1: Ropa, 2: Alimentos): ";
            int cat;
            std::cin >> cat;
            p.categoria = static_cast<Categoria>(cat);

            // Regresar al inicio del registro para sobrescribir
            archivo.seekp(-static_cast<int>(sizeof(Producto)), std::ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
            return;
        }
    }
    std::cout << "Producto no encontrado.\n";
}

void consultarProducto(std::fstream& archivo) {
    int codigo;
    std::cout << "Ingrese el código del producto a consultar: ";
    std::cin >> codigo;

    Producto p;
    archivo.seekg(0, std::ios::beg);
    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigo && p.activo) {
            std::cout << "Producto encontrado:\n";
            p.mostrar();
            return;
        }
    }
    std::cout << "Producto no encontrado.\n";
}

int main() {
    std::fstream archivo(NOMBRE_ARCHIVO, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    
    if (!archivo) {
        // Si el archivo no existe, lo creamos
        archivo.clear(); // Limpia el estado del flujo
        archivo.open(NOMBRE_ARCHIVO, std::ios::out | std::ios::binary);
        archivo.close();
        archivo.open(NOMBRE_ARCHIVO, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
    }

    int opcion;
    do {
        std::cout << "\nMenu:\n";
        std::cout << "1. Cargar Productos\n";
        std::cout << "2. Agregar Producto\n";
        std::cout << "3. Modificar Producto\n";
        std::cout << "4. Consultar Producto\n";
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opción: ";
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                cargarProductos(archivo);
                break;
            case 2:
                agregarProducto(archivo);
                break;
            case 3:
                modificarProducto(archivo);
                break;
            case 4:
                consultarProducto(archivo);
                break;
            case 5:
                break;
            default:
                std::cout << "Opción no válida.\n";
                break;
        }
    } while (opcion != 5);

    archivo.close();
    return 0;
}
