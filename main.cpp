/*
 *
 */
#include "crc16.hpp"
#include "hamming.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void receptor(int c);
void emisor(int c);


int main(int argc, char **argv)
{

    int r;
    ////////////////////////////////////////////////////////
    do{
        cout<<"Ingrese ( 1 ) Para emisor o ( 0 ) Para receptor:"<<endl<<"-->";
        cin>>r;
        if(!(r==0 || r==1)){
            cout<<"Ingrese una opcion valida"<<endl;
        }
    }while(!(r==0 || r==1) );
    ////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////
    if(r){
        do{
            cout<<"Ingrese ( 1 ) Para CRC16 o ( 0 ) Para Hamming:"<<endl<<"-->";
            cin>>r;
            if(!(r==0 || r==1)){
                cout<<"Ingrese una opcion valida"<<endl;
            }
        }while(!(r==0 || r==1) );
        emisor(r);

    }
    else{
        do{
            cout<<"Ingrese ( 1 ) Para CRC16 o ( 0 ) Para Hamming:"<<endl<<"-->";
            cin>>r;
            if(!(r==0 || r==1)){
                cout<<"Ingrese una opcion valida"<<endl;
            }
        }while(!(r==0 || r==1) );
        receptor(r);
    }
    ////////////////////////////////////////////////////////
    return 0;
}

void receptor(int c){
    string tramas="";
    char buff;
    ifstream archivo2("canalcrc.txt");
    if(archivo2.is_open()){
        while (! archivo2.eof() )
        {

             buff=archivo2.get();
             if(buff=='0'||buff=='1'){
                tramas+=buff;}
        }
        if(c){
            crc16::recibir(tramas);
        }
        else{
            hamming::recibir(tramas);
        }
    archivo2.close();
    }
}
void emisor(int c){
    ifstream archivo("entrada.txt");
    string mensaje="";

    char buff;
    if(archivo.is_open()){

        while (! archivo.eof() )
        {

             buff=archivo.get();
             if(buff>=32&&buff<=126){
                mensaje+=buff;}
        }

        if(c){
            crc16::enviar(mensaje);
            }
        else{
            hamming::enviar(mensaje);
        }
        archivo.close();
    }
}
