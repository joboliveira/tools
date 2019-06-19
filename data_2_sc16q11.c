#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <signal_generator.h>


int main(int argc, char **argv)
{
	FILE *pont_data;
	FILE *pont_output_data;
	FILE *pont_data_i;
	FILE *pont_data_q;
	FILE *pont_data_pulse;
	FILE *pont_data_echo;

	typedef struct sc16q11 {
		int16_t q;
		int16_t i;
	}SC16Q11_T;

	int16_t ptemp;
	char ptemp_char[256];
	char ptemp_char_pulse[256];
	char ptemp_char_echo[256];
	int32_t line_number;
	int16_t data;
	int16_t data_i;
	int16_t data_q;
	int16_t data_pulse;
	int16_t data_echo;

	if(argc <= 1)
	{
		printf("Entre com o comando:\r\t\t\t$ %s.exe <bin|csv> <teste.txt>\n",argv[0]);
		printf("Ou:\r\t\t\t$ %s.exe <iq> <arquivo_i.txt> <arquivo_q.txt>\n",argv[0]);
		printf("Ou:\r\t\t\t$ %s.exe <sig2iq> <arquivo_de_entrada.txt>\n",argv[0]);
		printf("Ou:\r\t\t\t$ %s.exe <sig2iq2ch> <arquivo_de_entrada_canal_1.txt> <arquivo_de_entrada_canal_1.txt>\n",argv[0]);
		return 0;
	}
	//printf("Abrindo arquivo: %s\n",argv[1]);


//	SC16Q11_T ADIQ;

	printf("Tamanho da estrutura SC16Q11_T = %d Bytes\n\n",sizeof(SC16Q11_T));

	if (strcmp(argv[1],"bin") == 0)
	{
		SC16Q11_T ADIQ;
		pont_data = fopen(argv[2],"r");
		if(pont_data == NULL)
		{
			printf("Erro ao abrir o arquivo, arquivo vazio!\n");
			return 1;
		}
		fread(&ADIQ, sizeof(SC16Q11_T),1, pont_data);
		printf("Valor da Amostra Q hexadecimal 0x%08X %5d\n", ADIQ.q,ADIQ.q);
		printf("Valor da Amostra I hexadecimal 0x%08X %5d\n", ADIQ.i,ADIQ.i);

		fread(&ADIQ, sizeof(SC16Q11_T),1, pont_data);
		printf("Valor da Amostra Q hexadecimal 0x%08X %5d\n", ADIQ.q,ADIQ.q);
		printf("Valor da Amostra I hexadecimal 0x%08X %5d\n", ADIQ.i,ADIQ.i);
	}
	else if (strcmp(argv[1],"csv") == 0)
	{
		SC16Q11_T ADIQ;
		pont_data = fopen(argv[2],"r");
		if(pont_data == NULL)
		{
			printf("Erro ao abrir o arquivo, arquivo vazio!\n");
			return 1;
		}
		pont_output_data = fopen("saida.bin","w");
		printf("Arquivo não suportado\r\n");
		line_number=1;
		while(fgets(ptemp_char,32,pont_data) != NULL)
		{
			data = atoi(ptemp_char);
			printf("Line number: %5d => %5d\r\n",line_number, data);
			line_number++;

			ADIQ.i = data;
			ADIQ.q = 0;

			fwrite(&ADIQ,sizeof(SC16Q11_T),1,pont_output_data);
		}
		fclose(pont_output_data);
	}
	else if (strcmp(argv[1],"iq") == 0)
	{
		SC16Q11_T ADIQ;
		printf("Convertendo arquivos I e q para binario IQ\r\n");
		pont_output_data = fopen("saida.bin","w");
		pont_data_i = fopen(argv[2],"r");
		pont_data_q = fopen(argv[3],"r");

		line_number=1;
		while(fgets(ptemp_char,32,pont_data_i) != NULL)
		{
			data_i = atoi(ptemp_char);

			fgets(ptemp_char,32,pont_data_q);
			data_q = atoi(ptemp_char);

			ADIQ.i = data_i;
			ADIQ.q = data_q;

			printf("Line number: %5d =>\r\t\t\tI %0+8d\r\t\t\t\t, Q %0+6d\r\n",line_number, data_i, data_q);

			fwrite(&ADIQ,sizeof(SC16Q11_T),1,pont_output_data);

			line_number++;
		}
		fclose(pont_output_data);
		fclose(pont_data_i);
		fclose(pont_data_q);
	}
	else if (strcmp(argv[1],"sig2iq") == 0)
	{
		SC16Q11_T ADIQ;
		printf("Convertendo arquivos de sinal para para binario IQ\r\n");
		pont_output_data = fopen("saida.bin","w");
		pont_data_i = fopen(argv[2],"r");
		//pont_data_q = fopen(argv[3],"r");

    SIGNAL_T data_temp;
    SIGNAL_T *p_data_temp;

    p_data_temp = &data_temp;

    printf("...\r\n");
    p_data_temp->amplitude = 1.0;
    p_data_temp->frequency = 60.0;
    p_data_temp->time = 1.0;
    p_data_temp->phase = 0.0;

    senoidal(p_data_temp);

    printf("amplitude\r\t\t\t= %F\r\nfrequency\r\t\t\t= %F\r\ntime\r\t\t\t= %F\r\nphase\r\t\t\t= %F\r\n",
          p_data_temp->amplitude,
          p_data_temp->frequency,
          p_data_temp->time,
          p_data_temp->phase);

    printf("Resultado da funcao\r\t\t\t= %F\r\n\n",p_data_temp->output);

		line_number=1;
		while(fgets(ptemp_char,32,pont_data_i) != NULL)
		{
			data_i = atoi(ptemp_char);

			//fgets(ptemp_char,32,pont_data_q);
			//data_q = atoi(ptemp_char);

			ADIQ.i = data_i;
			ADIQ.q = 0;

			printf("Line number: %5d =>\r\t\t\tI %0+6d\r\t\t\t\t\t, Q %0+6d\r\n",line_number, data_i, data_q);

			fwrite(&ADIQ,sizeof(SC16Q11_T),1,pont_output_data);

			line_number++;
		}
		fclose(pont_output_data);
		fclose(pont_data_i);
		//fclose(pont_data_q);
	}
	else if (strcmp(argv[1],"sig2iq2ch") == 0)
	{
		SC16Q11_T ADIQ_CH1;
		SC16Q11_T ADIQ_CH2;
		printf("Convertendo arquivos de sinal para para binario IQ\r\n");
		pont_output_data = fopen("saida.bin","w");
		pont_data_pulse = fopen(argv[2],"r");
		pont_data_echo = fopen(argv[3],"r");
		//pont_data_q = fopen(argv[3],"r");

		line_number=1;
		//while((fgets(ptemp_char_pulse,32,pont_data_pulse) != NULL) || (fgets(ptemp_char_echo,32,pont_data_echo) != NULL))
		while((fgets(ptemp_char_pulse,32,pont_data_pulse) != NULL) && (fgets(ptemp_char_echo,32,pont_data_echo) != NULL))
		{
			data_pulse = atoi(ptemp_char_pulse);
			data_echo  = atoi(ptemp_char_echo);

			//fgets(ptemp_char,32,pont_data_q);
			//data_q = atoi(ptemp_char);

			ADIQ_CH1.i = data_pulse;
			ADIQ_CH1.q = 0;

			ADIQ_CH2.i = data_echo;
			ADIQ_CH2.q = 0;

			printf("Line number: %5d =>\r\t\t\tCH1: I %0+6d\r\t\t\t\t\t, Q %0+6d\r\t\t\t\t\t\t\t CH2: I %0+6d\r\t\t\t\t\t\t\t\t\t, Q %0+6d\r\n"    ,line_number, data_pulse, 0, data_echo, 0);

			fwrite(&ADIQ_CH1,sizeof(SC16Q11_T),1,pont_output_data);
			fwrite(&ADIQ_CH2,sizeof(SC16Q11_T),1,pont_output_data);

			line_number++;
		}
		fclose(pont_output_data);
		fclose(pont_data_i);
		//fclose(pont_data_q);
	}


	fclose(pont_data);

	//printf("Fechando arquivo: %s\n",argv[1]);

	return 0;
}  
