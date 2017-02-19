//Clase CRC16
//Carlos Arellano 
#include "crc16.hpp"

/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/	
void crc16::enviar(string msj){
	size_t i,n;
	string bufferMsj[MAXMSJ/MAXT];
	binario bufferB[MAXMSJ/MAXT];
	i=0;n=0;
	while(i<msj.size()){
		bufferMsj[n]=msj.substr(i,MAXT);
		i+=MAXT;n++;
	}

	for (i = 0; i < n; ++i){
		bufferB[i]=msjTobin(bufferMsj[i]);
		if(i==n-1){
			bufferB[i]=insertarCabecera(bufferB[i],i,1,0,0);// Inserta el bit de fin de trasmision demensaje
		}else{
			bufferB[i]=insertarCabecera(bufferB[i],i,0,0,0);
		}
		bufferB[i]=crc(bufferB[i]);
		bufferB[i]=relleno_Bit(bufferB[i]);
		bufferB[i]=insercion_banderas(bufferB[i]);
		cout<<"Trama: "<<bufferB[i]<<endl;
	}

	
	}
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/	
	// calculo de CRC16 de una trama
binario crc16::crc(binario bin_in){
		//Polinomio genrador x16+x15+x2+1
		binario crc16="11000000000000101";//polinomio generador
		binario bin_out=bin_in+"0000000000000000", resto="", aux="";//copia de la trama inicial los r bit 0 del grado del polinomio
		//resto de la divicion xor, auxiliar
		size_t i,crcLong=crc16.size();//variable de for y longitud de el polinomio
		
		for (i = 0; i <bin_out.size()-crcLong+1 ; i++)
		{
			if (bin_out[i]=='1')
			{
				aux=bin_out.substr(i,crcLong);
				
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
				
			}
			
		}
		return bin_in+bin_out.substr(bin_in.size());
		}
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/		
	//inserta las banderas de inicio y fin a una trama
binario crc16::insercion_banderas(binario bin_in){
			binario bin_out="01111110";
			bin_out+=bin_in;
			return bin_out+"01111110";
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
binario crc16::insertarCabecera(binario bin_in,int CE,int T,int CR,int control){

		return ("0"+bitset<2>(CE).to_string()+bitset<1>(T).to_string()+bitset<2>(CR).to_string()+bitset<2>(control).to_string()+bin_in);
}

