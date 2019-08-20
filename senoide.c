#include <stdio.h>
#include <math.h>
#include <inttypes.h>


/*
*
*   Observações Importantes.
*
*  Para um pulso de 0.5us é preciso de uma senoide / cossenoide com frequência de 2.000MHz
*  Para um pulso de 1.0us é preciso de uma senoide / cossenoide com frequência de 1.000MHz
*  Para um pulso de 2.0us é preciso de uma senoide / cossenoide com frequência de 0.500MHz
*
*  Ou, por simplificação, usamos a maior restrição, embora o número de amostras de um ciclo possa deteriorar o sinal.
*  0.5us teremos 10 samples de um sinal de 2.0MHz, o que equivale a um ciclo completo do sinal.
*  1.0us teremos 20 samples de um sinal de 2.0MHz, o que equivale a dois ciclos completos do sinal.
*  2.0us teremos 40 samples de um sinal de 2.0MHz, o que equivale a dois ciclos completos do sinal.
*/
#define PI        3.141592654
//#define f_sinal   2500000 // 2.5e6 Hz
//#define f_sinal   500000  // 500e3 Hz
#define f_sinal   1000000   //   1e6 Hz
#define f_amostra 20000000  //   20Msps
#define k         (2 * cos(2 * PI * ((float)f_sinal/(float)f_amostra)))
//#define k         (2 * sin(2 * PI * ((float)f_sinal/(float)f_amostra)))
//#define n_elem    10 // 0.5e-6 Segundos
//#define n_elem    1000 // 50e-6 Segundos
#define n_elem    20  // 1e-6 Segundos

int32_t n=0;
float y[n_elem];

FILE *arquivoFloat;
FILE *arquivoInt;

int main()
{
 printf("Gerador de onda senoidal em arquivo\r\n");
 printf("Arquivo de saida: SinalSenoidalFloat.txt\r\n");
 printf("Arquivo de saida: SinalSenoidalInt.txt\r\n");
 if((arquivoFloat =  fopen("SinalSenoidalFloat.txt","w")) == NULL)
 {
  printf("Erro ao abrir o arquivo.\r\n");
  return 0;
 }

 if((arquivoInt =  fopen("SinalSenoidalInt.txt","w")) == NULL)
 {
  printf("Erro ao abrir o arquivo.\r\n");
  return 0;
 }

 //Constante
 //y[0]=0;
 y[0]=1.0;
 fprintf(arquivoFloat, "%f\r\n",y[n++]);
 fprintf(arquivoInt, "%d\r\n",(int16_t)(y[n++]*2047));

// Constante
//y[1]=sin(2*PI*((float)f_sinal/(float)f_amostra));
y[1]=cos(2*PI*((float)f_sinal/(float)f_amostra));
fprintf(arquivoFloat,"%f\r\n",y[1]);
fprintf(arquivoInt,"%d\r\n",(int16_t)(y[1]*2047));

 for (n=2; n < n_elem; n++)
 {
  y[n] = k*y[n-1]-y[n-2];
  fprintf(arquivoFloat,"%f\r\n",y[n]);
  fprintf(arquivoInt,"%d\r\n",(int16_t)(y[n]*2047));
 }
 fclose(arquivoFloat);
 fclose(arquivoInt);
 return 0;
}
