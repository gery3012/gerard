#include <iostream>
#include <vector>
#include <iomanip>

class Matriz {
private:
    std::vector<std::vector<double>> elementos;
    int filas;
    int columnas;

public:
    Matriz(int f, int c) : filas(f), columnas(c) {
        elementos.resize(f, std::vector<double>(c, 0));
    }

    void setElemento(int f, int c, double valor) {
        elementos[f][c] = valor;
    }

    void imprimir() const {
        for (const auto& fila : elementos) {
            for (const auto& elem : fila) {
                std::cout << std::setw(10) << elem; // Alineación de los números
            }
            std::cout << std::endl;
        }
    }

    Matriz transpuesta() const {
        Matriz result(columnas, filas);
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                result.setElemento(j, i, elementos[i][j]);
            }
        }
        return result;
    }

    double determinante() const {
        if (filas != columnas) throw std::invalid_argument("No se puede calcular el determinante de una matriz no cuadrada.");
        return calcularDeterminante(elementos);
    }

    Matriz suma(const Matriz& otra) const {
        if (filas != otra.filas || columnas != otra.columnas) throw std::invalid_argument("Las matrices deben tener las mismas dimensiones para sumar.");
        Matriz result(filas, columnas);
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                result.setElemento(i, j, elementos[i][j] + otra.elementos[i][j]);
            }
        }
        return result;
    }

    Matriz multiplicacion(const Matriz& otra) const {
        if (columnas != otra.filas) throw std::invalid_argument("Las matrices no son compatibles para multiplicar.");
        Matriz result(filas, otra.columnas);
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < otra.columnas; ++j) {
                for (int k = 0; k < columnas; ++k) {
                    result.setElemento(i, j, result.elementos[i][j] + elementos[i][k] * otra.elementos[k][j]);
                }
            }
        }
        return result;
    }

    bool esIgual(const Matriz& otra) const {
        if (filas != otra.filas || columnas != otra.columnas) return false;
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                if (elementos[i][j] != otra.elementos[i][j]) return false;
            }
        }
        return true;
    }

private:
    double calcularDeterminante(const std::vector<std::vector<double>>& m) const {
        int n = m.size();
        if (n == 1) return m[0][0];

        double det = 0;
        for (int p = 0; p < n; p++) {
            std::vector<std::vector<double>> submatriz;
            for (int i = 1; i < n; i++) {
                std::vector<double> fila;
                for (int j = 0; j < n; j++) {
                    if (j != p) {
                        fila.push_back(m[i][j]);
                    }
                }
                submatriz.push_back(fila);
            }
            det += (p % 2 == 0 ? 1 : -1) * m[0][p] * calcularDeterminante(submatriz);
        }
        return det;
    }
};

int main() {
    
    Matriz A(2, 2);
    A.setElemento(0, 0, 1);
    A.setElemento(0, 1, 2);
    A.setElemento(1, 0, 3);
    A.setElemento(1, 1, 4);

    std::cout << "Matriz A:" << std::endl;
    A.imprimir();

    std::cout << "\nTranspuesta de A:" << std::endl;
    Matriz At = A.transpuesta();
    At.imprimir();

    std::cout << "\nDeterminante de A: " << A.determinante() << std::endl;

    Matriz B(2, 2);
    B.setElemento(0, 0, 5);
    B.setElemento(0, 1, 6);
    B.setElemento(1, 0, 7);
    B.setElemento(1, 1, 8);
    
    std::cout << "\nMatriz B:" << std::endl;
    B.imprimir();

    std::cout << "\nSuma A + B:" << std::endl;
    Matriz C = A.suma(B);
    C.imprimir();

    std::cout << "\nMultiplicación A * B:" << std::endl;
    Matriz D = A.multiplicacion(B);
    D.imprimir();

    std::cout << "\n¿A es igual a B? " << (A.esIgual(B) ? "Sí" : "No") << std::endl;

    return 0;
}