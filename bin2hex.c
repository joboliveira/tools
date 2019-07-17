#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "color.h"
//
//Black:   \u001b[30m.
//Red:     \u001b[31m.
//Green:   \u001b[32m.
//Yellow:  \u001b[33m.
//Blue:    \u001b[34m.
//Magenta: \u001b[35m.
//Cyan:    \u001b[36m.
//White:   \u001b[37m.
//

int main(int argc, char **argv)
{
  FILE *pont_data_bin;
  FILE *pont_data_hex;

  typedef struct sc16q11 {
    int16_t i;
    int16_t q;
  }SC16Q11_T;

  static char bin2hex[] = "0123456789ABCDEF";

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

  uint16_t endereco;
  uint8_t  data;
  char     dataH;
  char     dataL;
  long     size;
  uint8_t  crc;   // For use with Intel Hexfile
  #define SIZE_LINE 256
  char     hex_line[SIZE_LINE];

  endereco = 0;

  fseek (pont_data_bin, 0, SEEK_END);   // non-portable
  size=ftell (pont_data_bin);
  if(size > 65536)
  {
    printf("Arquivo excedeu o tamanho de 65536 Bytes\r\n");
    fclose(pont_data_bin);
    fclose(pont_data_hex);
    return 1;
  }

  fseek (pont_data_bin, 0, SEEK_SET);   // non-portable

  printf("Tamanho do arquivo de entrada em Bytes %ld\r\n",size);

  for (endereco ; endereco < size ; endereco++)
  {
    fread(&data, sizeof(data),1, pont_data_bin);
    dataH = bin2hex[(data >> 4) & 0x0F];
    dataL = bin2hex[(data >> 0) & 0x0F];
    crc = 0xff - 0x01 - (uint8_t)((endereco >> 8) & 0xFF) - (uint8_t)((endereco >> 0) & 0xFF) - data + 0x01;
    printf(GREEN_TEXT(":") RED_TEXT("01") GREEN_TEXT("%04X") RED_TEXT("00") GREEN_TEXT("%C") GREEN_TEXT("%C") YELLOW_TEXT("%02X") "\r\n",endereco,dataH,dataL,crc);
    snprintf(hex_line,SIZE_LINE,":" "01" "%04X" "00" "%C" "%C" "%02X" "\n",endereco,dataH,dataL,crc);
    //printf("%s",hex_line);
    fprintf(pont_data_hex,"%s",hex_line);
  }
  printf(GREEN_TEXT(":") RED_TEXT("00") GREEN_TEXT("0000") RED_TEXT("01") YELLOW_TEXT("FF") "\r\n");
  snprintf(hex_line,SIZE_LINE,":00000001FF" "\n");
  //printf("%s",hex_line);
  fprintf(pont_data_hex,"%s",hex_line);

  //fwrite(&ADIQ,sizeof(SC16Q11_T),1,pont_output_data);

  fclose(pont_data_bin);
  fclose(pont_data_hex);

  return 0;
}
