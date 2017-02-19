#ifndef CRC16_HPP
#define CRC16_HPP

//Clase CRC16
//Carlos Arellano 
#include <iostream>
#include <fstream>
#include <bitset>
#define MAXT 40     //tamaño maximo de mensaje a trasmitir por trama
#define MAXMSJ 160 //tamaño maximo de un mensaje
using namespace std;
typedef string binario ;
class crc16{

public:
	static void enviar(string msj);
private:
	//Funciones propias
	static binario crc(binario bin_in);
	//Funciones genericas
	static binario msjTobin(string mensaje);
	static binario relleno_Bit(binario bin_in);
	static binario insercion_banderas(binario bin_in);
	static binario insertarCabecera(binario bin_in,int CE,int T,int CR,int control);
};

#endif 

