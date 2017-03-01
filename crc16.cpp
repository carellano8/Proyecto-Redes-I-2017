//Clase CRC16
//Carlos Arellano
#include "crc16.hpp"

/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
void crc16::enviar(string msj){
    size_t i,n;
    string bufferMsj[MAXMSJ/MAXT];
    binario bufferB[MAXMSJ/MAXT*8];
    ofstream archivo("canalcrc.txt");

    if(archivo.is_open()){
        i=0;n=0;
        while(i<msj.size()){
            bufferMsj[n]=msj.substr(i,MAXT);
            i+=MAXT;n++;
        }

        for (i = 0; i < n; ++i){
            bufferB[i]=msjTobin(bufferMsj[i]);
            if(i==n-1){
                bufferB[i]=insertarCabecera(bufferB[i],i,1);// Inserta el bit de fin de trasmision demensaje
            }else{
                bufferB[i]=insertarCabecera(bufferB[i],i,0);
            }
            bufferB[i]=bufferB[i]+crc(bufferB[i]+"0000000000000000");
            bufferB[i]=relleno_Bit(bufferB[i]);
            bufferB[i]=insercion_banderas(bufferB[i]);
            //cout<<"Tamano: "<<bufferB[i].size()<<endl;
            archivo/*<<"Trama: "*/<<bufferB[i]<<endl;
        }
        cout<<"tramas Enviadas "<<endl;
        archivo.close();
    }
    

}

/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
void crc16::recibir(binario bin){
    size_t i,li,ls,j,a,b;
    bool error=false;
    binario aux="";
    binario bufferB[MAXMSJ/MAXT*8];
    binario buffer2[MAXMSJ/MAXT*8];
    string bufferMsj;
    ofstream archivo("salidacrc.txt");
    
   
     i=0;j=0;
    while(i<bin.size()){

        li=bin.find("01111110",i);
        ls=bin.find("01111110",i+8);
        bufferB[j]=bin.substr(li,(ls+8)-li);

        //~ cout<<"Inferior "<<li<<" Superior "<<ls<<" Distancia "<<(ls+8)-li<<endl;
        //~ cout<<bufferB[j]<<endl;
        j++;
        i=ls+8;;
    }

    i=0;
    while(!error&&i<j){// Verfificar que no hay error en las banderas 
        if(bufferB[i].size()<30){
            error=true;
        }
        if(!error){
            bufferB[i]=bufferB[i].substr(8,bufferB[i].size()-16);//quita las banderas 
        }
        i++;
    }
   	if(!error){// Quitar relleno de bits
        for (i = 0; i < j; i++)
        {
        	b=0;aux="";a=0;
        	while(a < bufferB[i].size())
        	{
        		if(bufferB[i].at(a)=='1'){
                    b++;
                }else{
                    b=0;
                    }
                aux+=bufferB[i].at(a);		
                if(b==5){
                a++;b=0;
                	}
            a++;
        	}
        	
        	bufferB[i]=aux;

       	}
    }
            
    
    if(!error){// CRC16
        i=0;
        while(!error&&i<j){// Verfificar CRC16
        	// cout<<bufferB[i]<<endl;
        	// cout<<crc(bufferB[i])<<endl;
            if(crc(bufferB[i])!="0000000000000000"){
                error=true;
            }else{
                bufferB[i]=bufferB[i].substr(0,bufferB[i].size()-16);
            }
            i++;
        }
    }
   
       
    if(!error){// Cabecera Que no hay desorden en las tramas y que hay trama final
        i=0;
        while(!error&&i<j){// Verfificar CRC16
            a=stoi(bufferB[i].substr(1,4), nullptr, 2);
            b=stoi(bufferB[i].substr(5,1), nullptr , 2);
            buffer2[a]=bufferB[i].substr(6);
            //cout<<a<<" "<<b<<" "<<buffer2[a]<<endl;
            i++;
        }
        if(!b){
            error=true;
        }
    }
    if(!error){
        bufferMsj="";
        for (i = 0; i <j ; i++)
        {
            for (a = 0; a <buffer2[i].size() ; a+=8)
            {
                bufferMsj+=(char)(stoi(buffer2[i].substr(a,8), nullptr, 2));
            }   
        }

        cout<<bufferMsj<<endl;
        if(archivo.is_open()){
            archivo<<bufferMsj;
            archivo.close();
        }
    }
    
    
    
    
    if(error){
        cout<<"Hay un error"<<endl;
        if(archivo.is_open()){
            archivo<<"Hay un error"<<endl;
            archivo.close();
        }
    }
}

/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
    // calculo de CRC16 de una trama
binario crc16::crc(binario bin_in){
        //Polinomio genrador x16+x15+x2+1
        binario crc16="11000000000000101";//polinomio generador
        binario bin_out=bin_in, resto="", aux="";//copia de la trama inicial los r bit 0 del grado del polinomio
        //resto de la divicion xor, auxiliar
        size_t i,crcLong=crc16.size();//variable de for y longitud de el polinomio

        for (i = 0; i <bin_out.size()-crcLong+1 ; i++)
        {
            if (bin_out[i]=='1')
            {
                aux=bin_out.substr(i,crcLong);
                // cout<<"aux "<<aux<<endl;
                resto="";
                for (size_t j = 0; j < crcLong; j++) {
                           if(aux[j]==crc16[j]){
                             resto=resto+'0';
                           }
                           else{
                             resto=resto+'1';
                           }
                }
                bin_out=bin_out.substr(0,i)+resto+bin_out.substr(i+crcLong);
            // cout<<"bin "<<bin_out<<endl;
            }

        }
      
        return bin_out.substr(bin_in.size()-16);
        }
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
    //inserta las banderas de inicio y fin a una trama
binario crc16::insercion_banderas(binario bin_in){
            return "01111110"+bin_in+"01111110";
            }
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
    //Hace relleno de bit a la trama poniendo un 0 cada 5 1
binario crc16::relleno_Bit(binario bin_in){
            int contador=0;
            size_t i;
            binario bin_out="";
            for (i = 0; i < bin_in.size(); i++){
                if(bin_in[i]=='1'){
                    contador++;
                    }else{
                        contador=0;
                        }
                    bin_out+=bin_in[i];
                    if(contador==5){
                        bin_out+="0";
                        contador=0;
                    }
            }
            return bin_out;

        }
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
// Convierte el mensaje en binario transformando cada caracter a binario de 8bits tomados de la tabla ascii
binario crc16::msjTobin(string mensaje){
            binario bin="";
            size_t i;
            for (i = 0; i < mensaje.size(); i++)
            {
                //cout<<bitset<8>(mensaje.c_str()[i]).to_string()<<endl;
                bin+=bitset<8>(mensaje[i]).to_string();
            }


            return bin;
        }
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
// Inserta la cabecera
binario crc16::insertarCabecera(binario bin_in,int CE,int T){

        return ("0"+bitset<4>(CE).to_string()+bitset<1>(T).to_string()+bin_in);
}

