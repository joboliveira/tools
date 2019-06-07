#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
                FILE *pont_data;
               
                typedef struct sc16q11 {
                               int16_t q;
                               int16_t i;
                }SC16Q11;
               
                printf("Entre com o comando %s teste.bin\n",argv[0]);
                
                //printf("Abrindo arquivo: %s\n",argv[1]);
                pont_data = fopen(argv[1],"r");
            
                if(pont_data == NULL)
                {
                    printf("Erro ao abrir o arquivo, arquivo vazio!\n");
                    return 1;
                }
                           
                SC16Q11 ADIQ;
               
                printf("Tamanho da estrutura SC16Q11 = %d Bytes\n\n",sizeof(SC16Q11));
               
                fread(&ADIQ, sizeof(SC16Q11),1, pont_data);
                printf("Valor da Amostra Q hexadecimal 0x%08X %+5d\n", ADIQ.q,ADIQ.q);
                printf("Valor da Amostra I hexadecimal 0x%08X %+5d\n", ADIQ.i,ADIQ.i);

                fread(&ADIQ, sizeof(SC16Q11),1, pont_data);
                printf("Valor da Amostra Q hexadecimal 0x%08X %+5d\n", ADIQ.q,ADIQ.q);
                printf("Valor da Amostra I hexadecimal 0x%08X %+5d\n", ADIQ.i,ADIQ.i);
    
                fclose(pont_data);
                //printf("Fechando arquivo: %s\n",argv[1]);

                return 0;
}
