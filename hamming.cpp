#include "hamming.hpp"


/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
void hamming::enviar(string msj){
    size_t i,n;
    string bufferMsj[MAXMSJ/MAXT];
    binario bufferB[MAXMSJ/MAXT*8];
    i=0;n=0;
	ofstream archivo("canalh.txt");

    if(archivo.is_open()){
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

	        bufferB[i]=codificadorHamming(bufferB[i]);
	        bufferB[i]=relleno_Bit(bufferB[i]);
	        bufferB[i]=insercion_banderas(bufferB[i]);
	        archivo/*<<"Trama Hamming: "*/<<bufferB[i]<<endl;
	    }

        cout<<"tramas Enviadas "<<endl;
        archivo.close();
    }else{
    	cout<<"Se ha detectado algun un error"<<endl;
    }
 }
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
void hamming::recibir(binario bin){
	size_t i,li,ls,j,a,b;
    int r;
    bool error=false;
    binario aux="";
    binario bufferB[MAXMSJ/MAXT*8];
    binario buffer2[MAXMSJ/MAXT*8];
    string bufferMsj;
    ofstream archivo("salidah.txt");
    
   
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
    if(j>16){error=true;}// maximo 16 tramas 
    i=0;
    while(!error&&i<j){// Verfificar que no hay error en las banderas 
        if(bufferB[i].size()<=30){
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
            
    
    if(!error){// decodificar hamming
        i=0;
        while(!error&&i<j){// decodificar hamming
        	// cout<<bufferB[i]<<endl;
        	// cout<<crc(bufferB[i])<<endl;
        	bufferB[i]=decodificadorHamming(bufferB[i],&r);
        	//~ cout<<"HOLA "<<r<<endl;
            if(r==-1){
                error=true;
            }
            i++;
        }
    }
   
       
    if(!error){// Cabecera, Que no hay desorden en las tramas y que hay trama final
        i=0;
        while(!error&&i<j){// 
            a=BitStringToInt(bufferB[i].substr(1,4));
            b=BitStringToInt(bufferB[i].substr(5,1));
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
                bufferMsj+=(char)(BitStringToInt(buffer2[i].substr(a,8)));
            }   
        }

        cout<<bufferMsj<<endl;
        if(archivo.is_open()){
            archivo<<bufferMsj;
            if(r==0){
            	cout<<"Se ha detectado y corregido un error"<<endl;
            	archivo<<endl<<"Se ha detectado y corregido un error"<<endl;
            }
            archivo.close();
        }else{
        	cout<<"Se ha detectado algun un error"<<endl;
        }
    }
    
    
    
    
    if(error){
        cout<<"Se ha detectado algun un error"<<endl;
        if(archivo.is_open()){
            archivo<<"Se ha detectado algun un error"<<endl;
            archivo.close();
        }else{
        	cout<<"Se ha detectado algun un error"<<endl;
        }
    }
}

/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/

    //inserta las banderas de inicio y fin a una trama
binario hamming::insercion_banderas(binario bin_in){
            return "01111110"+bin_in+"01111110";
            }
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
                //Hace relleno de bit a la trama poniendo un 0 cada 5 1
binario hamming::relleno_Bit(binario bin_in){
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

// Inserta la cabecera
binario hamming::insertarCabecera(binario bin_in,int CE,int T){

        return ("0"+bitset<4>(CE).to_string()+bitset<1>(T).to_string()+bin_in);
}
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
binario hamming::msjTobin(string mensaje){
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
binario hamming::codificadorHamming(binario bin){
    unsigned int i,j,k,m,n,p;
    int b[MAXMSJ],acum;
    binario s="";
    k=0;j=0;i=0;n=bin.size();//inicializacion
    
    while(k<=n){
        if(pow(2,j)==(i+1)){//si es potencia de 2 se omite el vector se rellena con 0
            j++;
            b[i]=0;
            }
        else{
            b[i]=(bin[k]=='1')?1:0;
            k++;
            }
        i++;
        }
    n+=j;
    j--;
    //~ cout<<j<<" "<<n<<endl;
    if(potencia2(n)){n--;}
    for(i=0;i<=j;i++){

        p=pow(2,i);
        k=p-1;acum=0;
        while(k<n){

                for(m=k;m<p+k&&m<n;m++){
                    if(m!=p-1){
                        acum+=b[m];

                    }
                    //~ cout<<m<<" ";
                }
                //~ cout<<endl;
                k=p+m;
            }
            //~ cout<<endl;
        if(acum%2!=0){
            b[p-1]=1;
            }
    }
    acum=0;
    for (i = 0; i < n; i++){
        acum+=b[i];
        s+=(b[i]+'0');
    }
    //~ cout<<s<<" "<<j<<endl;
    if(acum%2==0){
        b[n]=0;
        s+='0';
    }
    else{
        b[n]=1;
        s+='1';
        }
    //~ n++;

    //~ cout<<"Tamano: "<<n<<" | Bits de paridad: "<<j<<endl<<"Binario: ";
    //~ for(i = 0; i <n ; i++){
        //~ cout<<b[i];
    //~ }
    //~ cout<<endl;

    return s;
    }
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
bool hamming::potencia2(int n){
    int i=0;bool r=false;int p;
    p=pow(2,i);

    while(!r&&p<=n){
        if(n==p){r=true;}
        i++;p=pow(2,i);
        }
    return r;
    }

/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
binario hamming::decodificadorHamming(binario bin,int *r){
    unsigned int i,j,k,m,n,p,x;
    int b[MAXMSJ],acum;
    binario s="";
    char bp;
    k=0;i=0;//inicializacion

    bp=bin.at(bin.size()-1);
   	bin=bin.substr(0,bin.size()-1);
    n=bin.size();
    
    for (i = 0; i < n; ++i)
    {
    	b[i]=(bin[i]=='1')?1:0;
    }
    j=0;i=0;
    while(j<n){

        p=pow(2,i);
        k=p-1;acum=0;
        while(k<n){

                for(m=k;m<p+k&&m<n;m++){
                    // if(m!=p-1){
                        acum+=b[m];
                    // }
                    //~ cout<<m<<" ";
                }
                //~ cout<<endl;
                k=p+m;
            }
            //~ cout<<endl;
        if(acum%2==0){
            s.insert(0,"0");
            }
        else{
        	s.insert(0,"1");
        }
     i++;j=pow(2,i);
    }
    
    acum=0;
    for (i = 0; i < n; i++){
        acum+=b[i];
    }
    if(acum%2==0){
        j=0;
        }
    else{
    	j=1;
    }
    //termina la decodificacion
	// cout<<s<<" "<<j<<endl;
    x=BitStringToInt(s);
    if(x!=0){x--;}
  
    if((((int)j+'0')!=bp)&&x!=0){//paridad mala y hamming malo
    	
    	if(bin[x]=='1'){
    		bin.replace(x,1,"0");
    	}else{
    		bin.replace(x,1,"1");
    	}

    	*r=0;//Se repara un error
    }else{
    	if((((int)j+'0')==bp)&&x!=0){
    		*r=-1;//error doble
    	}
    	else{
    		*r=1;//sin error
    	}
    }
    if(*r!=-1){
    	s="";
    	for (i = 0; i < n; ++i)
    	{
    		if(!potencia2(i+1)){
    			s+=bin[i];
    		}
    	}
    }
    
    return s;
    }
    /*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
int hamming::BitStringToInt(string bin){
	size_t i,n=bin.size(),acum=0;
	for (i = 0; i < n; ++i)
	{
		if(bin[i]=='1'){
			acum+=pow(2,n-(i+1)) ;
		}
	}
	return acum;
}
