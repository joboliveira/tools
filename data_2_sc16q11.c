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
	FILE *pont_out_data_i;
	FILE *pont_out_data_q;

	typedef struct sc16q11 {
		int16_t i;
		int16_t q;
	}SC16Q11;

	int16_t ptemp;
	char    ptemp_char[256];
	char    ptemp_char_pulse[256];
	char    ptemp_char_echo[256];
	int32_t line_number;
	int16_t data;
	int16_t data_i;
	int16_t data_q;
	int16_t data_pulse;
	int16_t data_echo;

	if(argc <= 1)
	{
		printf("Entre com o comando:\r\t\t\t$ %s.exe <bin> <number of channels 1|2> <input.bin> <output.csv>\n",argv[0]);
		printf("Entre com o comando:\r\t\t\t$ %s.exe <csv> <input.csv> <output.bin'>\n",argv[0]);
		printf("Ou:\r\t\t\t$ %s.exe <iq> <arquivo_i.txt> <arquivo_q.txt>\n",argv[0]);
		printf("Ou:\r\t\t\t$ %s.exe <sig2iq> <arquivo_de_entrada.txt>\n",argv[0]);
		printf("Ou:\r\t\t\t$ %s.exe <sig2qi> <arquivo_de_entrada.txt>\n",argv[0]);
		printf("Ou:\r\t\t\t$ %s.exe <sig2iq2ch> <arquivo_de_entrada_canal_1.txt>\r\n\t\t\t <arquivo_de_entrada_canal_1.txt>\n",argv[0]);
		printf("Entre com o comando:\r\t\t\t$ %s.exe <bin2fill> <samplerate> <prf> <teste.bin>\n",argv[0]);
		return 0;
	}
	//printf("Abrindo arquivo: %s\n",argv[1]);

	//SC16Q11 ADIQ;
	printf("Tamanho da estrutura SC16Q11 = %ld Bytes\n\n",sizeof(SC16Q11));

	//
	//argv[1] = tipo de conversão: "bin" usiliza como entrada um arquivo binário
	//argv[ ] = quantidade de canais presentes no arquivo binário de entrada.
	//argv[2] = arquivo de entrada, apenas como leitura
	//argv[3] = arqivos de saída do tipo csv
	//
	if (strcmp(argv[1],"bin") == 0)
	{
		SC16Q11 ADIQ;
		long   	flSize;
		int32_t channels;
		char *buffer;
		channels				 = atoi(argv[2]) == 2 ? 2 : 1;
		pont_data				 = fopen(argv[3],"r");
		pont_output_data = fopen(argv[4],"wb");
		buffer           = malloc(strlen(argv[4]) + 10 );

		memset(buffer,0,sizeof(buffer));
		strcpy(buffer,argv[4]);
		strcat(buffer,".I.txt");
		pont_data_i      = fopen(buffer,"wb");

		memset(buffer,0,sizeof(buffer));
		strcpy(buffer,argv[4]);
		strcat(buffer,".Q.txt");
		pont_data_q      = fopen(buffer,"wb");

		//FILE *FILE_temporario;


		if(pont_data == NULL)
		{
			printf("Erro ao abrir o arquivo de entrada, arquivo vazio!\n");
			return 1;
		}

		if(pont_output_data == NULL)
		{
			printf("Erro ao abrir o arquivo de saida, arquivo vazio!\n");
			return 1;
		}

		fseek(pont_data,0,SEEK_END);
		flSize = ftell(pont_data);
		rewind (pont_data);

		//FILE_temporario = malloc(flSize);

		//printf("Funcao para uso com apenas um canal\r\n");
		printf("Tamanho do arquivo de entrada %ld\r\n",flSize);
		printf("Quantidade de Amostras encontradas para cada canal: %ld",(flSize/(channels*4)));

		for(long count = 0; count < (flSize/4); ){

			if(channels==2){
				// channel 1
				fread(&ADIQ, sizeof(SC16Q11),1, pont_data);
				//printf("Channel 1: I = %5d, Q = %5d\r\n",ADIQ.i,ADIQ.q);
				fprintf(pont_output_data,"%d, %d",ADIQ.i,ADIQ.q);
				//fprintf(pont_data_i,"%d\r\n",ADIQ.i);
				//fprintf(pont_data_q,"%d\r\n",ADIQ.q);
				//fprintf(FILE_temporario,"%d,\t%d\r\n",ADIQ.i,ADIQ.q);
				// channel 2
				fread(&ADIQ, sizeof(SC16Q11),1, pont_data);
				//printf("Channel 2: I = %5d, Q = %5d\r\n",ADIQ.i,ADIQ.q);
				fprintf(pont_output_data," %d, %d\r\n",ADIQ.i,ADIQ.q);
				//fprintf(FILE_temporario,"%d,\t%d\r\n",ADIQ.i,ADIQ.q);
				count+=channels;
			}
			else{
				// channel 1
				fread(&ADIQ, sizeof(SC16Q11),1, pont_data);
				//printf("Channel 1: I = %5d, Q = %5d\r\n",ADIQ.i,ADIQ.q);
				fprintf(pont_output_data,"%d, %d\r\n",ADIQ.i,ADIQ.q);
				fprintf(pont_data_i,"%d\r\n",ADIQ.i);
				fprintf(pont_data_q,"%d\r\n",ADIQ.q);

				//fprintf(FILE_temporario,"%d,\t%d\r\n",ADIQ.i,ADIQ.q);
				count+=channels;
			}
		}

		//memcpy(pont_output_data,FILE_temporario,flSize);
		//fwrite(pont_output_data,1,flSize,FILE_temporario);

		//free(FILE_temporario);
		fclose(pont_data);
		fclose(pont_output_data);
		fclose(pont_data_i);
		fclose(pont_data_q);
	}
	if (strcmp(argv[1],"bin2fill") == 0)
	{
		SC16Q11	ADIQ;
		long   	flSize;
		size_t 	result;
		long   	samplerate;
		long   	prf;
		long   	samples;
		long    fcount;

		samplerate       = atol(argv[2]);
		prf              = atol(argv[3]);
		pont_data        = fopen(argv[4],"r");
		pont_output_data = fopen("saida.bin","w");
		//pont_out_data_i  = fopen("saida_I.bin","w");
		//pont_out_data_q  = fopen("saida_Q.bin","w");

		fseek(pont_data,0,SEEK_END);
		flSize = ftell(pont_data);
		rewind (pont_data);

		ADIQ.i =     0; // 0 com  0 Graus de defasagem
		ADIQ.q =     0;//-2047; // 0 com 90 Graus de defasagem

		printf("Tamanho do arquivo binário SC16Q11 = %ld Bytes\n\n",flSize);

		if(pont_data == NULL)
		{
			printf("Erro ao abrir o arquivo, arquivo vazio!\n");
			return 1;
		}

		//fcount = flSize;
		for (samples = 0 ; samples < (samplerate/prf); samples++)
		{
			if(samples < ((flSize/4)-1))
			{
				fread(&ADIQ, sizeof(SC16Q11),1, pont_data);
				fcount -= sizeof(SC16Q11);
			}
			else
			{
				ADIQ.i = 0;
				ADIQ.q = 0;//-2047; // 0 com 90 Graus de defasagem
			}
			fwrite(&ADIQ,sizeof(SC16Q11),1,pont_output_data);
		}
		fclose(pont_data);
		fclose(pont_output_data);
		//fclose(pont_out_data_i);
		//fclose(pont_out_data_q);

	}
	else if (strcmp(argv[1],"csv") == 0)
	{
		SC16Q11 ADIQ;
		pont_data = fopen(argv[2],"r");
		if(pont_data == NULL)
		{
			printf("Erro ao abrir o arquivo de entrada, arquivo vazio!\n");
			return 1;
		}
		pont_output_data = fopen("saida.bin","w");
		if(pont_output_data == NULL)
		{
			printf("Erro ao abrir o arquivode saída, arquivo vazio!\n");
			return 1;
		}
		//printf("Arquivo não suportado\r\n");

		line_number=1;
		while(fgets(ptemp_char,32,pont_data) != NULL)
		{
			data = atoi(ptemp_char);
			printf("Line number: %5d => %5d\r\n",line_number, data);
			line_number++;

			ADIQ.i = data;
			ADIQ.q = 0;

			fwrite(&ADIQ,sizeof(SC16Q11),1,pont_output_data);
		}
		fclose(pont_output_data);
		fclose(pont_data);
	}
	else if (strcmp(argv[1],"iq") == 0)
	{
		SC16Q11 ADIQ;
		printf("Convertendo arquivos I e q para binario IQ\r\n");
		pont_output_data = fopen("saida.bin","w");
		pont_out_data_i  = fopen("saida_I.bin","w");
		pont_out_data_q  = fopen("saida_Q.bin","w");
		pont_data_i      = fopen(argv[2],"r");
		pont_data_q      = fopen(argv[3],"r");

		line_number=1;
		while(fgets(ptemp_char,32,pont_data_i) != NULL)
		{
			data_i = atoi(ptemp_char);

			fgets(ptemp_char,32,pont_data_q);
			data_q = atoi(ptemp_char);

			ADIQ.i = data_i;
			ADIQ.q = data_q;

			printf("Line number: %5d =>\r\t\t\tI %0+6d\r\t\t\t\t, Q %0+6d\r\n",line_number, data_i, data_q);

			fwrite(&ADIQ,sizeof(SC16Q11),1,pont_output_data);
			fwrite(&ADIQ.i,sizeof(ADIQ.i),1,pont_out_data_i);
			fwrite(&ADIQ.q,sizeof(ADIQ.q),1,pont_out_data_q);

			line_number++;
		}
		fclose(pont_output_data);
		fclose(pont_data_i);
		fclose(pont_data_q);
		//fclose(pont_data);
		fclose(pont_out_data_i);
		fclose(pont_out_data_q);
	}
	else if (strcmp(argv[1],"sig2iq") == 0)
	{
		SC16Q11 ADIQ;
		printf("Convertendo arquivos de sinal para para binario IQ\r\n");
		pont_output_data = fopen("saida.bin","w");
		pont_data_i = fopen(argv[2],"r");
		//pont_data_q = fopen(argv[3],"r");

		SIGNAL_T data_temp;
    SIGNAL_T *p_data_temp;

    p_data_temp = &data_temp;

    printf("...\r\n");
    p_data_temp->amplitude =  1.0;
    p_data_temp->frequency = 60.0;
    p_data_temp->time      =  1.0;
    p_data_temp->phase     =  0.0;

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

			printf("Line number: %5d =>\r\t\t\tI %0+8d\r\t\t\t\t, Q %0+6d\r\n",line_number, data_i, data_q);

			fwrite(&ADIQ,sizeof(SC16Q11),1,pont_output_data);

			line_number++;
		}
		fclose(pont_output_data);
		fclose(pont_data_i);
		//fclose(pont_data_q);
		fclose(pont_data);
	}
	else if (strcmp(argv[1],"sig2qi") == 0)
	{
		SC16Q11 ADIQ;
		printf("Convertendo arquivos de sinal para para binario IQ\r\n");
		pont_output_data = fopen("saida.bin","w");
		pont_data_q = fopen(argv[2],"r");
		//pont_data_q = fopen(argv[3],"r");

		line_number=1;
		while(fgets(ptemp_char,32,pont_data_q) != NULL)
		{
			data_q = atoi(ptemp_char);

			//fgets(ptemp_char,32,pont_data_q);
			//data_q = atoi(ptemp_char);

			ADIQ.i = 0;      // Apenas para teste, trocar os nomes depois
			ADIQ.q = data_q; // Apenas para teste, trocar os nomes depois

			printf("Line number: %5d =>\r\t\t\tI %0+8d\r\t\t\t\t, Q %0+6d\r\n",line_number, data_i, data_q);

			fwrite(&ADIQ,sizeof(SC16Q11),1,pont_output_data);

			line_number++;
		}
		fclose(pont_output_data);
		fclose(pont_data_q);
		//fclose(pont_data_q);
		fclose(pont_data);
	}
	else if (strcmp(argv[1],"sig2iq2ch") == 0)
	{
		SC16Q11 ADIQ_CH1;
		SC16Q11 ADIQ_CH2;
		printf("Convertendo arquivos de sinal para para binario IQ\r\n");
		pont_output_data = fopen("saida.bin","w");
		pont_data_pulse  = fopen(argv[2],"r");
		pont_data_echo   = fopen(argv[3],"r");
		//pont_data_q    = fopen(argv[3],"r");

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

			fwrite(&ADIQ_CH1,sizeof(SC16Q11),1,pont_output_data);
			fwrite(&ADIQ_CH2,sizeof(SC16Q11),1,pont_output_data);

			line_number++;
		}
		fclose(pont_output_data);
		fclose(pont_data_i);
		//fclose(pont_data_q);
		fclose(pont_data);
	}


	//fclose(pont_data);

	//printf("Fechando arquivo: %s\n",argv[1]);

	return 0;
}
