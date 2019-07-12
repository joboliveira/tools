#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

int main(int argc, char **argv)
{
  FILE *pont_data_bin;
  FILE *pont_data_hex;

  typedef struct sc16q11 {
    int16_t q;
    int16_t i;
  }SC16Q11_T;

  if(argc <= 1)
	{
		printf("Entre com o comando:\r\t\t\t$ %s.exe <arquivo.bin> <arquivo.hex>\n",argv[0]);
		return 0;
	}

  SC16Q11_T ADIQ;
  pont_data_bin = fopen(argv[1],"r");
  if(pont_data_bin == NULL)
  {
    printf("Erro ao abrir o arquivo \"%s\", arquivo vazio!\n",argv[2]);
    return 1;
  }

  pont_data_hex = fopen("saida.hex","w");
  //if(pont_data_hex == NULL)
  //{
  //  printf("Erro ao abrir o arquivo \"%s\", arquivo vazio!\n",argv[3]);
  //  return 1;
  //}

  fread(&ADIQ, sizeof(SC16Q11_T),1, pont_data_bin);
  printf("Valor da Amostra Q hexadecimal 0x%08X %5d\n", ADIQ.q,ADIQ.q);

  int16_t endereco;

  printf(":04%04x00%08xFF\r\n",endereco,ADIQ.q);
  //fwrite(&ADIQ,sizeof(SC16Q11_T),1,pont_output_data);

  fclose(pont_data_bin);
  fclose(pont_data_hex);

  return 0;
}
