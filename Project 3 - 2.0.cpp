#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Publicacion {
    string titulo; int anio;
    Publicacion(string t,int a):titulo(t),anio(a){}
    virtual ~Publicacion(){}
    virtual void mostrar()=0;
    static bool valTexto(const string &s){return !s.empty();}
    static bool valAnio(int a){return a>=1500 && a<=2025;}
};

struct Libro:Publicacion {
    string autor; int paginas;
    Libro(string t,int a,string au,int p):Publicacion(t,a),autor(au),paginas(p){}
    void mostrar() override {
        cout<<"Libro: "<<titulo<<", "<<anio<<", "<<autor<<", "<<paginas<<"p\n";
    }
};

struct Revista:Publicacion {
    int edicion; string mes;
    Revista(string t,int a,int e,string m):Publicacion(t,a),edicion(e),mes(m){}
    void mostrar() override {
        cout<<"Revista: "<<titulo<<", "<<anio<<", Ed. "<<edicion<<", "<<mes<<"\n";
    }
};

struct Periodico:Publicacion {
    string fecha; int edicion;
    Periodico(string t,int a,string f,int e):Publicacion(t,a),fecha(f),edicion(e){}
    void mostrar() override {
        cout<<"Periódico: "<<titulo<<", "<<anio<<", "<<fecha<<", Ed. "<<edicion<<"\n";
    }
};

int main(){
    vector<Publicacion*> cat;
    int op;
    do{
        cout<<"1-Agregar Libro 2-Revista 3-Periódico 4-Mostrar 5-Eliminar 6-Salir\n> ";
        cin>>op; cin.ignore();
        if(op==1){
            string t,a,au; int an,p;
            cout<<"Título: "; getline(cin,t);
            cout<<"Año: "; cin>>an; cin.ignore();
            cout<<"Autor: "; getline(cin,au);
            cout<<"Páginas: "; cin>>p; cin.ignore();
            if(Publicacion::valTexto(t)&&Publicacion::valAnio(an)&&Publicacion::valTexto(au)&&p>0)
                cat.push_back(new Libro(t,an,au,p));
            else cout<<"Datos inválidos\n";
        }
        else if(op==2){
            string t,m; int an,e;
            cout<<"Título: "; getline(cin,t);
            cout<<"Año: "; cin>>an;
            cout<<"Edición: "; cin>>e; cin.ignore();
            cout<<"Mes: "; getline(cin,m);
            if(Publicacion::valTexto(t)&&Publicacion::valAnio(an)&&e>0&&Publicacion::valTexto(m))
                cat.push_back(new Revista(t,an,e,m));
            else cout<<"Datos inválidos\n";
        }
        else if(op==3){
            string t,f; int an,e;
            cout<<"Título: "; getline(cin,t);
            cout<<"Año: "; cin>>an;
            cout<<"Fecha: "; cin.ignore(); getline(cin,f);
            cout<<"Edición: "; cin>>e; cin.ignore();
            if(Publicacion::valTexto(t)&&Publicacion::valAnio(an)&&e>0&&Publicacion::valTexto(f))
                cat.push_back(new Periodico(t,an,f,e));
            else cout<<"Datos inválidos\n";
        }
        else if(op==4){
            for(auto p:cat) p->mostrar();
        }
        else if(op==5){
            string t; cout<<"Título a eliminar: "; getline(cin,t);
            for(auto it=cat.begin(); it!=cat.end(); ++it){
                if((*it)->titulo==t){
                    delete *it; cat.erase(it);
                    cout<<"Eliminado\n"; break;
                }
            }
        }
    }while(op!=6);
    for(auto p:cat) delete p;
}
