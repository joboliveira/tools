#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>

/*
*
*   Observa��es Importantes.
*
*  Para um pulso de 0.5us é preciso de uma senoide / cossenoide com frequ�ncia de 2.000MHz
*  Para um pulso de 1.0us é preciso de uma senoide / cossenoide com frequ�ncia de 1.000MHz
*  Para um pulso de 2.0us é preciso de uma senoide / cossenoide com frequ�ncia de 0.500MHz
*
*  Ou, por simplifica��o, usamos a maior restri��o, embora o n�mero de amostras de um ciclo possa deteriorar o sinal.
*  0.5us teremos 10 samples de um sinal de 2.0MHz, o que equivale a um ciclo completo do sinal.
*  1.0us teremos 20 samples de um sinal de 2.0MHz, o que equivale a dois ciclos completos do sinal.
*  2.0us teremos 40 samples de um sinal de 2.0MHz, o que equivale a dois ciclos completos do sinal.
*/
#define PI  3.141592654
float 		k;
float 		f_sinal;
float 		f_amostra;


int32_t n=0;
long 	n_elem;
long  n_carrier;
float 	*y;
char	FileName_Float[256];
char	FileName_Int[256];
char	FileName_HDF[256];


FILE *arquivoFloat;
FILE *arquivoInt;

int main(int argc, char **argv)
{
 if(argc <= 5){
  printf("type senoide <sin|cos> <freq_signal> <Sampling_rate> <samples> <n_carriers>\r\n");
  return -1;
 }
 strcpy(FileName_Float,argv[1]);
 strcat(FileName_Float,"_");
 strcat(FileName_Float,argv[2]);
 strcat(FileName_Float,"_");
 strcat(FileName_Float,argv[3]);
 strcat(FileName_Float,"_");
 strcat(FileName_Float,argv[4]);
 strcat(FileName_Float,"_Float.txt");

 strcpy(FileName_Int,argv[1]);
 strcat(FileName_Int,"_");
 strcat(FileName_Int,argv[2]);
 strcat(FileName_Int,"_");
 strcat(FileName_Int,argv[3]);
 strcat(FileName_Int,"_");
 strcat(FileName_Int,argv[4]);
 strcat(FileName_Int,"_Int.txt");

 printf("Gerador de onda senoidal em arquivo\r\n");
 printf("Arquivo de saida: %s\r\n",FileName_Float);
 printf("Arquivo de saida: %s\r\n", FileName_Int);
 if((arquivoFloat = fopen(FileName_Float,"w")) == NULL)
 //if((arquivoFloat = fopen("SinalSenoidalFloat.txt","w")) == NULL)
 {
  printf("Erro ao abrir o arquivo.\r\n");
  return -1;
 }

 f_sinal    = atof(argv[2]);
 f_amostra	= atof(argv[3]);
 //k          = (2 * cos(2 * PI * (f_sinal/f_amostra)));
 n_elem     = atol(argv[4]);
 n_carrier  = atol(argv[5]);
 y          = (float *) malloc(n_elem * sizeof(float) * n_carrier);

 if((arquivoInt =  fopen(FileName_Int,"w")) == NULL)
 {
  printf("Erro ao abrir o arquivo.\r\n");
  free(y);
  return -1;
 }

// Constante
for(long carrier=0; carrier < n_carrier; carrier++)
{
  k = (2 * cos(2*PI*(f_sinal*(carrier+1)/f_amostra)));
 if (strcmp(argv[1],"sin") == 0)
 {
	 y[carrier + 0]=sin(0);
	 y[carrier + 1]=sin(2*PI*((float)f_sinal*(carrier+1)/(float)f_amostra));
 }
 else if (strcmp(argv[1],"cos") == 0)
 {
	 y[carrier + 0]=-cos(0);
	 y[carrier + 1]=-cos(2*PI*((float)f_sinal*(carrier+1)/(float)f_amostra));
 } else
 {
	 free(y);
	 return -1;
 }


 fprintf(arquivoFloat, "%f\r\n",y[carrier + 0]);
 fprintf(arquivoFloat, "%f\r\n",y[carrier + 1]);

 fprintf(arquivoInt, "%d\r\n",(int16_t)(y[carrier + 0]*2047));
 fprintf(arquivoInt, "%d\r\n",(int16_t)(y[carrier + 1]*2047));

 for (n=2; n < n_elem; n++)
 {
   //for(carrier=0; carrier < num_carrier; carrier++)
   //{
     y[carrier + n] = k*y[carrier + (n-1)]-y[carrier + (n-2)];
     fprintf(arquivoFloat,"%f\r\n",y[carrier + n]);
     fprintf(arquivoInt,"%d\r\n",(int16_t)(y[carrier + n]*2047));
   //}
 }
}
 fclose(arquivoFloat);
 fclose(arquivoInt);
 free(y);
 return 0;
}
