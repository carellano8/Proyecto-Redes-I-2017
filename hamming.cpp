#include "hamming.hpp"


/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
void hamming::enviar(string msj){
    size_t i,n;
    string bufferMsj[MAXMSJ/MAXT];
    binario bufferB[MAXMSJ/MAXT*8];
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

        bufferB[i]=codificadorHamming(bufferB[i]);
        bufferB[i]=relleno_Bit(bufferB[i]);
        bufferB[i]=insercion_banderas(bufferB[i]);
        cout/*<<"Trama Hamming: "*/<<bufferB[i]<<endl;
    }


    }
/*------------------------------------------------------------------------------------------------*/
///----------------------------------------------------------------------------------------------///
/*------------------------------------------------------------------------------------------------*/
void hamming::recibir(binario bin){
    size_t i,li,ls,j;
    // binario bufferTramas[MAXMSJ/MAXT];
    binario bufferB[MAXMSJ/MAXT*8];
     i=0;j=0;
    while(i<bin.size()){

        li=bin.find("01111110",i);
        ls=bin.find("01111110",i+8);
        bufferB[j]=bin.substr(li,(ls+8)-li);

        cout<<"Inferior "<<li<<" Superior "<<ls<<" Distancia "<<(ls+8)-li<<endl;
        cout<<bufferB[j]<<endl;
    j++;
    i=ls+8;;
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
    if(potencia2(n)){n--;}
    for(i=0;i<j;i++){

        p=pow(2,i);
        k=p-1;acum=0;
        while(k<n){

                for(m=k;m<p+k&&m<n;m++){
                    if(m!=p-1){
                        acum+=b[m];

                    }
                }
                k=p+m;
            }
        if(acum%2!=0){
            b[p-1]=1;
            }
    }
    acum=0;
    for (i = 0; i < n; i++){
        acum+=b[i];
        s+=(b[i]+'0');
    }

    if(acum%2==0){
        b[n]=0;
        s+='0';
    }
    else{
        b[n]=1;
        s+='1';
        }
    n++;

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
