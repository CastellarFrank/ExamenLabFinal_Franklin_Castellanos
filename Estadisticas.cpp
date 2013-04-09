#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <list>
#include <cstdlib>
#include <sstream>

using namespace std;

string texto,archivoSalida;
map<char,int> characters;
int menorFrecuencia,mayorFrecuencia,cantidadLetras,cantidadNumeros,cantidadEspecial,suma;
list<char> menores;
list<char> mayores;
char*tipo;


bool cargarArchivo(string path){
    ifstream lectura(path.c_str());
    texto="";
    if(lectura.is_open()){
        while(lectura.good()){
            string leer;
            getline(lectura,leer);
            texto+=leer;
        }
        lectura.close();
        return true;
    }else{
        return false;
    }

}
void MenorMayorCharacters(){
    map<char,int>::iterator it;

    bool val=true;
    for(it=characters.begin();it!=characters.end();it++){
        if(val){
            mayorFrecuencia=(*it).second;
            menorFrecuencia=(*it).second;
            val=false;
        }
        if(menorFrecuencia>(*it).second)
            menorFrecuencia=(*it).second;

        if(mayorFrecuencia<(*it).second)
            mayorFrecuencia=(*it).second;
    }
    for(it=characters.begin();it!=characters.end();it++){
        if((*it).second==menorFrecuencia){
            menores.push_back((*it).first);
        }
        if((*it).second==mayorFrecuencia){
            mayores.push_back((*it).first);
        }
    }
}


void procesarTexto(char *){
    for(int i=0;i<(int)texto.length();i++){

        char temp=texto.at(i);
        if(strcmp(tipo,"-sd")==0){
            if(temp>='a' && temp<='z'){
                temp=toupper(temp);
                cantidadLetras++;
            }
        }else{
            if(temp>='A' && temp<='Z'){
                cantidadLetras++;
            }else if(temp>='0' && temp<='9'){
                int val=(temp-'0');
                suma+=val;
                cantidadNumeros++;
            }else if(temp>='a' && temp<='z'){
                temp=toupper(temp);
                cantidadLetras++;
            }else{
                cantidadEspecial++;
            }
        }
        characters[temp]++;
    }
}

void generarTexto(){
    archivoSalida="\n*Tipo Archivo: ";
    archivoSalida+=tipo;
    archivoSalida+="\n_ | _\n";
    archivoSalida+="\n*Frecuencia de Caracteres: \n";
    map<char,int>::iterator it;
    for(it=characters.begin();it!=characters.end();it++){
        archivoSalida+="\t";
        archivoSalida+="Caracter: ";
        archivoSalida+=(*it).first;
        archivoSalida+="  Frecuencia: ";
        stringstream out;
        out<<(*it).second;
        string temp=out.str();
        archivoSalida+=temp;
        archivoSalida+="\n";
    }
    archivoSalida+="_ | _\n";
    archivoSalida+="\n*Caracteres con la menor Frecuencia: \n";
    archivoSalida+="Frecuencias: ";
    stringstream out;
    out<<menorFrecuencia;
    archivoSalida+=out.str();
    archivoSalida+="\n";
    list<char>::iterator ite;
    for(ite=menores.begin();ite!=menores.end();ite++){
        archivoSalida+="\t";
        archivoSalida+=(*ite);
        archivoSalida+="\n";
    }
    archivoSalida+="_ | _\n";
    archivoSalida+="\n*Caracteres con la mayor Frecuencia: \n";
    archivoSalida+="Frecuencias: ";
    stringstream out4;
    out4<<mayorFrecuencia;
    archivoSalida+=out4.str();
    archivoSalida+="\n";
    for(ite=mayores.begin();ite!=mayores.end();ite++){
        archivoSalida+="\t";
        archivoSalida+=(*ite);
        archivoSalida+="\n";
    }

    if(strcmp(tipo,"-ld")==0){
        archivoSalida+="_ | _\n";
        archivoSalida+="\n*Cantidad Total de Letras: ";
        stringstream out;
        out<<cantidadLetras;
        archivoSalida+=out.str();
        archivoSalida+="\n_ | _\n";
        archivoSalida+="\n*Cantidad Total de Numeros: ";
        stringstream out2;
        out2<<cantidadNumeros;
        archivoSalida+=out2.str();
        archivoSalida+="\n_ | _\n";
        archivoSalida+="\n*Cantidad Total de Caracteres Especiales: ";
        stringstream out3;
        out3<<cantidadEspecial;
        archivoSalida+=out3.str();
        archivoSalida+="\n_ | _\n";
        archivoSalida+="\n*Suma Total de los Numeros: ";
        stringstream out4;
        out4<<suma;
        archivoSalida+=out4.str();
    }
    archivoSalida+="\n\n\n\t CopyRight By Industrias Record\n\n";
}

void guardarArchivo(string path){
    ofstream escritura(path.c_str());
    escritura<<archivoSalida;
    escritura.close();

}

int main(int argc, char *argv[])
{
    tipo=argv[1];
    if(argc!=4){
        cout<<"Error en la cantidad de parametros.\n";
        return 0;
    }else if(!(strcmp(tipo,"-sd")==0 || strcmp(tipo,"-ld")==0)){
        cout<<"El tipo de operacion ingresada es invalida.\n";
        return 0;
    }else if(!cargarArchivo(argv[2])){
        cout<<"No se pudo abrir el archivo.\n";
        return 0;
    }
    procesarTexto(argv[1]);
    MenorMayorCharacters();
    generarTexto();
    guardarArchivo(argv[3]);
    cout<<"La Ejecucion finalizo exitosamente.";
    return 0;
}

