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
    bool error=false;
	if(c){
	    ifstream archivo2("canalcrc.txt");
	    if(archivo2.is_open()){
	        while (! archivo2.eof() &&!error){

	             buff=archivo2.get();
	             if(buff=='0'||buff=='1'){
                         tramas+=buff;
	               }else{
                        
                        if(buff!= EOF&&buff!='\n'){
                            error=true;
                            
                        }
                    
                 }
	        }
            if(!error){
	           crc16::recibir(tramas);
                }
	        archivo2.close();
            if(error){
                cout<<"Se ha detectado algun un error"<<endl;
                ofstream archivo("salidacrc.txt");  
                if(archivo.is_open()){
                    archivo<<"Se ha detectado algun un error"<<endl;
                    archivo.close();
                }else{
                    cout<<"Se ha detectado algun un error"<<endl;
                }
            }
	    }else{
            ofstream archivo("salidacrc.txt");  
            if(archivo.is_open()){
            archivo<<"Se ha detectado algun un error"<<endl;
            archivo.close();
            }else{
                cout<<"Se ha detectado algun un error"<<endl;
            }

            cout<<"Se ha detectado algun un error"<<endl;
        }
	}
	else{
	    ifstream archivo2("canalh.txt");
	    if(archivo2.is_open()){
	        while (! archivo2.eof() ){

	             buff=archivo2.get();
	             if(buff=='0'||buff=='1'){
                         tramas+=buff;
                   }else{
                        
                        if(buff!= EOF&&buff!='\n'){
                            error=true;
                            
                        }
                    
                 }
	        }
            if(!error){
	           hamming::recibir(tramas);
	        }
            archivo2.close();
            if(error){
                cout<<"Se ha detectado algun un error"<<endl;
                ofstream archivo("salidah.txt");
                if(archivo.is_open()){
                    archivo<<"Se ha detectado algun un error"<<endl;
                    archivo.close();
                }else{
                    cout<<"Se ha detectado algun un error"<<endl;
                }
            }
	    }else{
            ofstream archivo("salidah.txt");
            if(archivo.is_open()){
                archivo<<"Se ha detectado algun un error"<<endl;
                archivo.close();
            }else{
                cout<<"Se ha detectado algun un error"<<endl;
            }
            cout<<"Se ha detectado algun un error"<<endl;
        }

	}
}

void emisor(int c){
    ifstream archivo("entrada.txt");
    string mensaje="";
    int i,j=0;
    char buff;
    bool error=false;
    if(archivo.is_open()){
        i=0;
        while (!archivo.eof()&&!error)
        {

             buff=archivo.get();
             if((buff>=32&&buff<=126)/*||buff=='\n'*/){
                mensaje+=buff;
            }else{
                
                if(buff!= EOF&&buff!='\n' ){//solo se ecepta un salto de linea y fin de archivo
                   // cout<<buff<<endl;
                    error=true;
                }
                if(buff=='\n'){
                        j++;
                   }
                if(buff==EOF){i--;}
            }
            if(j>=2){error=true;}
            i++;
        }
        
        if(i-1>MAXMSJ){error=true;}
        if(!error){
            if(c){
                crc16::enviar(mensaje);
                }
            else{
                hamming::enviar(mensaje);
                }   
        }
        if(error){
            cout<<"Se ha detectado algun un error"<<endl;
        }
        archivo.close();
    }else{

            cout<<"Se ha detectado algun un error"<<endl;
        }
}
