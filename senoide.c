#include <stdio.h>
#include <math.h>
#include <inttypes.h>

#define PI        3.141592654
//#define f_sinal   2500000 // 2.5e6 Hz
#define f_sinal   500000    // 500e3 Hz
#define f_amostra 20000000  // 20Msps
#define k         (2 * cos(2 * PI * ((float)f_sinal/(float)f_amostra)))
//#define n_elem    10 // 0.5e-6 Segundos
#define n_elem    1000 // 50e-6

int32_t n=0;
float y[n_elem];

FILE *arquivo;

int main()
{
 if((arquivo =  fopen("SinalSenoidal.dat","w")) == NULL)
 {
  printf("Erro ao abrir o arquivo.\r\n");
  return 0;
 }

 //Constante
 y[0]=0;
 fprintf(arquivo, "%f\r\n",y[n++]);

 // Constante
 y[1]=sin(2*PI*((float)f_sinal/(float)f_amostra));
 fprintf(arquivo,"%f\r\n",y[n++]);

 for (n=2; n < n_elem; n++)
 {
  y[n] = k*y[n-1]-y[n-2];
  fprintf(arquivo,"%f\r\n",y[n]);
 }
 fclose(arquivo);
 return 0;
}
