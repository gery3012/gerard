#include <iostream>
#include <vector>
#include <iomanip>  // para std::setw
#include <stdexcept> // para excepciones
using namespace std;

class Matriz {
private:
    vector<vector<double>> datos;
    int filas, columnas;

    // Método auxiliar para obtener matriz menor (para determinante)
    Matriz submatriz(int fila_excl, int col_excl) const {
        Matriz sub(filas - 1, columnas - 1);
        for (int i = 0, subi = 0; i < filas; i++) {
            if (i == fila_excl) continue;
            for (int j = 0, subj = 0; j < columnas; j++) {
                if (j == col_excl) continue;
                sub.datos[subi][subj] = datos[i][j];
                subj++;
            }
            subi++;
        }
        return sub;
    }

public:
    Matriz(int f, int c) : filas(f), columnas(c) {
        if (f <= 0 || c <= 0)
            throw invalid_argument("Filas y columnas deben ser > 0");
        datos.resize(filas, vector<double>(columnas, 0));
    }

    // Constructor para inicializar con vector
    Matriz(const vector<vector<double>>& v) : datos(v), filas(v.size()), columnas(v.empty() ? 0 : v[0].size()) {
        for (const auto& fila : v)
            if ((int)fila.size() != columnas)
                throw invalid_argument("Todas las filas deben tener la misma cantidad de columnas");
    }

    // Acceso para modificar o leer elementos
    double& operator()(int f, int c) {
        if (f < 0 || f >= filas || c < 0 || c >= columnas)
            throw out_of_range("Indice fuera de rango");
        return datos[f][c];
    }

    double operator()(int f, int c) const {
        if (f < 0 || f >= filas || c < 0 || c >= columnas)
            throw out_of_range("Indice fuera de rango");
        return datos[f][c];
    }

    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }

    // Imprime la matriz en formato tabular con alineación
    void imprimir() const {
        // Para alinear, primero calculamos el ancho máximo de cada columna
        vector<int> anchos(columnas, 0);
        for (int j = 0; j < columnas; j++) {
            for (int i = 0; i < filas; i++) {
                // Convertimos número a string para medir longitud
                string s = to_string(datos[i][j]);
                // Ajustamos precisión y quitamos ceros innecesarios
                size_t pos = s.find('.');
                if (pos != string::npos) {
                    // Limitar a 4 decimales
                    s = s.substr(0, pos + 5);
                    // Remover ceros finales
                    while (s.back() == '0') s.pop_back();
                    if (s.back() == '.') s.pop_back();
                }
                if ((int)s.size() > anchos[j]) anchos[j] = s.size();
            }
        }
        // Imprimir con setw
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) {
                cout << setw(anchos[j]) << fixed << setprecision(4) << datos[i][j] << " ";
            }
            cout << "\n";
        }
    }

    // Retorna la transpuesta
    Matriz transpuesta() const {
        Matriz t(columnas, filas);
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                t.datos[j][i] = datos[i][j];
        return t;
    }

    // Calcula determinante por expansión de cofactores recursiva
    double determinante() const {
        if (filas != columnas)
            throw logic_error("Determinante solo definido para matrices cuadradas");

        if (filas == 1) return datos[0][0];
        if (filas == 2)
            return datos[0][0]*datos[1][1] - datos[0][1]*datos[1][0];

        double det = 0;
        for (int c = 0; c < columnas; c++) {
            double signo = (c % 2 == 0) ? 1 : -1;
            det += signo * datos[0][c] * submatriz(0, c).determinante();
        }
        return det;
    }
    // Suma dos matrices de mismas dimensiones
    Matriz suma(const Matriz& otra) const {
        if (filas != otra.filas || columnas != otra.columnas)
            throw logic_error("Dimensiones incompatibles para suma");
        Matriz res(filas, columnas);
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                res.datos[i][j] = datos[i][j] + otra.datos[i][j];
        return res;
    }

    // Multiplica dos matrices compatibles
    Matriz multiplicacion(const Matriz& otra) const {
        if (columnas != otra.filas)
            throw logic_error("Dimensiones incompatibles para multiplicacion");
        Matriz res(filas, otra.columnas);
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < otra.columnas; j++) {
                double suma = 0;
                for (int k = 0; k < columnas; k++)
                    suma += datos[i][k] * otra.datos[k][j];
                res.datos[i][j] = suma;
            }
        }
        return res;
    }

    // Compara si dos matrices son idénticas
    bool esIgual(const Matriz& otra) const {
        if (filas != otra.filas || columnas != otra.columnas) return false;
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                if (datos[i][j] != otra.datos[i][j]) return false;
        return true;
    }
};

int main() {
    try {
        vector<vector<double>> v1{{1,2,3},{4,5,6},{7,8,9}};
        Matriz A(v1);

        cout << "Matriz A:\n";
        A.imprimir();

        cout << "\nTranspuesta de A:\n";
        Matriz At = A.transpuesta();
        At.imprimir();

        cout << "\nDeterminante de A: " << A.determinante() << "\n";

        vector<vector<double>> v2{{9,8,7},{6,5,4},{3,2,1}};
        Matriz B(v2);

        cout << "\nMatriz B:\n";
        B.imprimir();

        cout << "\nSuma A + B:\n";
        Matriz S = A.suma(B);
        S.imprimir();

        cout << "\nMultiplicacion A * B:\n";
        Matriz M = A.multiplicacion(B);
        M.imprimir();

        cout << "\nA es igual a B? " << (A.esIgual(B) ? "Sí" : "No") << "\n";

    } catch(const exception& e){
        cerr << "Error: " << e.what() << '\n';
    }
}
