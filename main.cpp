/*
 *
 */
#include "crc16.hpp"
#include "hamming.hpp"
using namespace std;
int main(int argc, char **argv)
{
	string mensaje;
	binario a,b;
	
	char buffer[160];
	cout<<"ingrese el mensaje a enviar"<<endl;
	cin.getline(buffer,160,'\n');
	mensaje.append(buffer);
	crc16::enviar(mensaje);
	hamming::enviar(mensaje);
	return 0;
}

