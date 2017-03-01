#ifndef HAMMING_HPP
#define HAMMING_HPP

#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#define MAXT 125     //tamaño maximo de mensaje a trasmitir por trama
#define MAXMSJ 2000 //tamaño maximo de un mensaje
using namespace std;
typedef string binario ;

class hamming{
public:
    static void enviar(string msj);
    static void recibir(binario bin);
private:
    //Funciones propias
    static binario codificadorHamming(binario bin);
    static bool potencia2(int n);
    //Funciones genericas
    static binario msjTobin(string mensaje);
    static binario relleno_Bit(binario bin_in);
    static binario insercion_banderas(binario bin_in);
    static binario insertarCabecera(binario bin_in,int CE,int T);
};


#endif
