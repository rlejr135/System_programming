#include "20141602.h"

int main()
{
	char input[300], inputcopy[300];
	char forcommand[20];
	char formnemonic[10];
	int dump_start, dump_end, dump_flag, fail_flag;
	int edit_address, edit_value, edit_flag;
	int fill_start, fill_end, fill_value,  fill_flag;
	int opcode_number;
	int i, j, k, l, now_input_num, assemble_flag, loader_flag;
	FILE *type_file;
	char type_filename_save[50];
	char hash_type[6];

	
	char *token_input[10];
	int tokennum;
	int tmp_program_address = 0;

	init();						// initalize.
	while(1)
	{
		tokennum = 0;
		
		for (i = 0 ; i < 10 ; i++)
				token_input[i] = NULL;
		for (i = 0 ; i < 300 ; i++)
			input[i] = '\0';

		for(i = 0 ; i < 10 ; i++)
			formnemonic[i] = 0;

		printf("sicsim> ");
		fgets(input, 260, stdin);		//input string. string length is not over 256
		
		if (input[0] == 0)
			continue;

		strcpy(inputcopy, input);
		now_input_num = 0, i=j=k=0;
		
		while(input[i++] != '\n');		//change string's last character to 0
		input[i-1] = 0;
		

		while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
			now_input_num++;
		while(input[now_input_num] != ' ' && input[now_input_num] != '\t' && input[now_input_num] != '\0')
			forcommand[j++] = input[now_input_num++];
		while(input[now_input_num] == ' ' || input[now_input_num] == '\t')		// delete void space
			now_input_num++;

		forcommand[j] = '\0';
	
		// for proj3, change how to process input string

		token_input[tokennum++] = strtok(inputcopy, " \n\0");

		while((token_input[tokennum++] = strtok(NULL, " \n\0")) != NULL){
			if (tokennum == 10)
					break;
		}

		////////////////////////////////////////////////////:





	//	while(input[now_input_num] == ' ' || input[now_input_num] == '\t')		// delete void space
	//		now_input_num++;

		// compare forcommand and command. if it is single command( h, dir... ) check whether last character is \0 or not
		if ((!strcmp(forcommand, "h") || !strcmp(forcommand, "help")) && (input[now_input_num] == '\0')) //
		{
			addhistory(input);
			help();
		}
		else if ((!strcmp(forcommand, "d") || !strcmp(forcommand, "dir")) && (input[now_input_num] == '\0'))
		{
			addhistory(input);
			dir();
		}
		else if ((!strcmp(forcommand, "q") || !strcmp(forcommand, "quit")) && (input[now_input_num] == '\0'))
		{
			quit();
			exit(1);
		}
		else if ((!strcmp(forcommand, "hi") || !strcmp(forcommand, "history")) && (input[now_input_num] == '\0'))
		{
			addhistory(input);
			history();
		}
		else if (!strcmp(forcommand, "du") || !strcmp(forcommand, "dump"))
		{
			dump_start = 0;
			dump_end=0;
			dump_flag = 0;
			fail_flag = 0;
			k=0;

			while(input[now_input_num] != '\0')		// divide each number 
			{
				k = 1;
				if (dump_flag >= 2)
					break;
				if (input[now_input_num] == ',') 	// divide first and second number when input[now_input_num] == ','
				{
					dump_flag++;
					now_input_num++;
					while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
						now_input_num++;
					continue;
				}

				if (dump_flag == 0) 			// when dump flag is 0, it is first number
				{
					if (input[now_input_num] >= '0' && input[now_input_num] <= '9')
						dump_start = dump_start * 16 + input[now_input_num] - '0';
					else if (input[now_input_num] >= 'a' && input[now_input_num] <= 'f')
						dump_start = dump_start *16 + input[now_input_num] - 87;
					else if (input[now_input_num] >= 'A' && input[now_input_num] <= 'F')
						dump_start = dump_start *16 + input[now_input_num] - 55;
					else if (input[now_input_num] == ' ' || input[now_input_num] == '\t') 	// if there is void space between numbers, set fail flag
					{
						while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
							now_input_num++;
						if (input[now_input_num] != ',' && input[now_input_num] != '\0')
							fail_flag = 1;
						continue;
					}
				}
				else if (dump_flag == 1)  		// when dump flag is 1, it is second number
				{
					if (input[now_input_num] >= '0' && input[now_input_num] <= '9') 
						dump_end = dump_end * 16 + input[now_input_num] - '0';
					else if (input[now_input_num] >= 'a' && input[now_input_num] <= 'f')
						dump_end = dump_end *16 + input[now_input_num] - 87;
					else if (input[now_input_num] >= 'A' && input[now_input_num] <= 'F')
						dump_end = dump_end *16 + input[now_input_num] - 55;
					else if (input[now_input_num] == ' ' || input[now_input_num] == '\t')
					{
						while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
							now_input_num++;
						if (input[now_input_num] != '\0')
							fail_flag = 1;
					}
					k = 2;
				}
				now_input_num++;

			}
			// check error
			// dump_flag = count of comma
			// k = operand's number
			// fail_flag -> strange string
			if (fail_flag == 1)
			{
				printf("error! you cannot write number like that!\n");
				continue;
			}
			if (dump_flag >= 2)
			{
				printf("error! too many comma!\n");
				continue;
			}

			if (dump_flag == 1 && k == 1)
			{
				printf("error! no second range!\n");
				continue;
			}
			if (dump_start > dump_end && dump_flag == 1)
			{
				printf("error! start > end!\n");
				continue;
			}

			if (k == 0)
			{
				if (dump_lasthistory == 0)
					dump_start = 0;
				else if (dump_lasthistory >= 0xFFFFF)
					dump_start = 0;
				else
					dump_start = dump_lasthistory+1;
				if (dump_start+159 <= 0xFFFFF)
					dump_end = dump_start + 159;
				else
					dump_end = 0xFFFFF;
			}
			if (k >= 1 && dump_flag == 0)
			{
				if (dump_start+159 <= 0xFFFFF)
					dump_end = dump_start + 159;
				else
					dump_end = 0xFFFFF;
			}
			if (dump_start < 0 || dump_start > 0xFFFFF)
			{
				printf("error! over boundary!\n");
				continue;
			}
			if (dump_end > 0xFFFFF || dump_end < 0)
			{
				printf("error! over boundary!\n");
				continue;
			}
				
			addhistory(input);

			if (dump_end >= 0xFFFFF)
				dump_end = 0xFFFFF;
			dump(dump_start, dump_end);
		}
		else if (!strcmp(forcommand, "edit") || !strcmp(forcommand, "e"))
		{
			edit_address = 0;
			edit_value=0;
			edit_flag = 0;
			fail_flag = 0;
			k=0;
			// divide number	
			while(input[now_input_num] != '\0')
			{
				if (k == 0)
					k = 1;
				if (edit_flag >= 2)
					break;
				if (input[now_input_num] == ',') 		// divide first and second number , if input[now_input_num] is ','
				{
					edit_flag++;
					now_input_num++;
					while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
						now_input_num++;

					continue;
				}

				if (edit_flag == 0) 				//when edit_flag is 0, it is address number
				{
					if (input[now_input_num] >= '0' && input[now_input_num] <= '9')
						edit_address = edit_address * 16 + input[now_input_num] - '0';
					else if (input[now_input_num] >= 'a' && input[now_input_num] <= 'f')
						edit_address = edit_address *16 + input[now_input_num] - 87;
					else if (input[now_input_num] >= 'A' && input[now_input_num] <= 'F')
						edit_address = edit_address *16 + input[now_input_num] - 55;
					else if (input[now_input_num] == ' ' || input[now_input_num] == '\t') // if there is void space between number, set fail flag
					{
						while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
							now_input_num++;
						if (input[now_input_num] != ',')
							fail_flag = 1;
						continue;
					}
				}
				else if (edit_flag == 1) 			//when edit_flag is 1, it is value number
				{
					if (input[now_input_num] >= '0' && input[now_input_num] <= '9')
						edit_value = edit_value * 16 + input[now_input_num] - '0';
					else if (input[now_input_num] >= 'a' && input[now_input_num] <= 'f')
						edit_value = edit_value *16 + input[now_input_num] - 87;
					else if (input[now_input_num] >= 'A' && input[now_input_num] <= 'F')
						edit_value = edit_value *16 + input[now_input_num] - 55;
					else if (input[now_input_num] == ' ' || input[now_input_num] == '\t')  // if there is void space between number, set fail flag
					{
						while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
							now_input_num++;
						if (input[now_input_num] != '\0')
							fail_flag = 1;
						continue;
					}
					k = 2;
				}
				now_input_num++;

			}
			// check error
			// edit_flag = count of comma
			// k = operand's number
			// fail_flag -> strange string	
			if (fail_flag == 1)
			{
				printf("error! you cannot write number like that!\n");
				continue;
			}
			if (edit_address < 0 || edit_address > 0xFFFFF)
			{
				printf("error! over boundary!\n");
				continue;
			}
			if (edit_value < 0 || edit_value > 0xff)
			{
				printf("error! odd input!\n");
				continue;
			}
		
			if (k == 0)
			{
				printf("error! no input!\n");
				continue;
			}
			if (k == 1)
			{
				printf("error! no value!\n");
				continue;
			}
			if (edit_flag >= 2)
			{
				printf("error! too many comma!\n");
				continue;
			}
			edit(edit_address, edit_value);
			addhistory(input);
		}
		else if (!strcmp(forcommand, "fill") || !strcmp(forcommand, "f"))
		{
			fill_start = fill_end = fill_value = 0;
			fail_flag = 0, fill_flag = 0;
			k=0;
			// divide number
			while(input[now_input_num] != '\0')
			{
				if (k == 0)
					k = 1;
				if (fill_flag >= 3)
					break;
				if (input[now_input_num] == ',') 		// divide number first, second, and third number , if input[now_input_num] is ','
				{
					fill_flag++;
					now_input_num++;
					while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
						now_input_num++;
					continue;
				}

				if (fill_flag == 0)				// if fill_flag is 0, it is start address
				{
					if (input[now_input_num] >= '0' && input[now_input_num] <= '9')
						fill_start = fill_start * 16 + input[now_input_num] - '0';
					else if (input[now_input_num] >= 'a' && input[now_input_num] <= 'f')
						fill_start = fill_start *16 + input[now_input_num] - 87;
					else if (input[now_input_num] >= 'A' && input[now_input_num] <= 'F')
						fill_start = fill_start *16 + input[now_input_num] - 55;
					else if (input[now_input_num] == ' ' || input[now_input_num] == '\t') //if there is void space between number, set fail flag
					{
						while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
							now_input_num++;
						if (input[now_input_num] != ',')
							fail_flag = 1;
						continue;
					}
				}
				else if (fill_flag == 1) 			//if fill_flag is 1, it is end address
				{
					if (input[now_input_num] >= '0' && input[now_input_num] <= '9')
						fill_end = fill_end * 16 + input[now_input_num] - '0';
					else if (input[now_input_num] >= 'a' && input[now_input_num] <= 'f')
						fill_end = fill_end *16 + input[now_input_num] - 87;
					else if (input[now_input_num] >= 'A' && input[now_input_num] <= 'F')
						fill_end = fill_end *16 + input[now_input_num] - 55;
					else if (input[now_input_num] == ' ' || input[now_input_num] == '\t') //if there is void space between number, set fail flag
					{
						while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
							now_input_num++;
						if (input[now_input_num] != ',')
							fail_flag = 1;
						continue;
					}
					k = 2;
				}
				else if (fill_flag == 2) 			//if fill_flag is 2, it is value
				{
					if (input[now_input_num] >= '0' && input[now_input_num] <= '9')
						fill_value = fill_value * 16 + input[now_input_num] - '0';
					else if (input[now_input_num] >= 'a' && input[now_input_num] <= 'f')
						fill_value = fill_value *16 + input[now_input_num] - 87;
					else if (input[now_input_num] >= 'A' && input[now_input_num] <= 'F')
						fill_value = fill_value *16 + input[now_input_num] - 55;
					else if (input[now_input_num] == ' ' || input[now_input_num] == '\t') // if there is void space between number, set fail flag
					{
						while(input[now_input_num] == ' ' || input[now_input_num] == '\t')
							now_input_num++;
						if (input[now_input_num] != '\0')
							fail_flag = 1;
						continue;
					}
					k = 3;
				}
				now_input_num++;
			}
			// check error
			// fill_flag = count of comma
			// k = operand's number
			// fail_flag -> strange string	
			if (fail_flag == 1)
			{
				printf("error! you cannot write number like that!\n");
				continue;
			}
			if (fill_flag < 2)
			{
				printf("error! we cannot distribute address and value!\n");
				continue;
			}
			if (fill_flag > 2)
			{
				printf("error! too many comma!\n");
				continue;
			}
			if (fill_start > fill_end)
			{
				printf("error! start > end!\n");
				continue;
			}
			if (fill_value < 0 || fill_value > 0xff)
			{
				printf("error! odd value!\n");
				continue;
			}
			if (fill_start < 0 || fill_start > 0xfffff)
			{
				printf("error! odd boundary!\n");
				continue;
			}
			if (fill_end > 0xfffff || fill_end < 0)
			{
				printf("error! odd boundary!\n");
				continue;
			}

			fill(fill_start, fill_end, fill_value);
			addhistory(input);
	
		}
		else if ((!strcmp(forcommand, "reset")) && (input[now_input_num] == '\0'))
		{
			// change all memory to 0
			for (i = 0 ; i < 1048677 ; i++)
				dump_list[i] = 0;
			addhistory(input);
		}
		else if (!strcmp(forcommand, "opcode"))
		{
			j = 0;

			// divide opcode and mnemonic			
			while(input[now_input_num] != ' ' && input[now_input_num] != '\t' && input[now_input_num] != '\0')
				formnemonic[j++] = input[now_input_num++];
			formnemonic[j] = '\0';
			opcode_number = find_hashnum(formnemonic, hash_type);
	
			if (opcode_number == -1)
			{
				printf("error! wrong code!\n");
				continue;
			}
				
			printf("opcode is %X\n", opcode_number);
			addhistory(input);
		}
		else if ((!strcmp(forcommand, "opcodelist")) && (input[now_input_num] == '\0'))
		{
			opcodelist();
			addhistory(input);
		}
		else if (!strcmp(forcommand, "type"))
		{
			j = 0;
			for (l = now_input_num ; l < strlen(input) ; l++)
			{
				if (input[l] == ' ' || input[l] == '\0' || input[l] == '\t')
					break;
				type_filename_save[j++] = input[l];
			}
			type_filename_save[j] = '\0';

			type_file = fopen(type_filename_save, "r");
			
			if(type_file == NULL)
			{
				printf("error! there is no file\n");
				continue;
			}

			type_filename(type_file);
			
			fclose(type_file);
			addhistory(input);
		}
		else if (!strcmp(forcommand, "assemble"))
		{
			j = 0;
			for (l = now_input_num ; l < strlen(input) ; l++)
			{
				if (input[l] == ' ' || input[l] == '\0' || input[l] == '\t')
					break;
				type_filename_save[j++] = input[l];
			}
			type_filename_save[j] = '\0';

			assemble_flag = 0;
			assemble_flag = assemble_file(type_filename_save);

			if (assemble_flag == -1)
				continue;

			addhistory(input);

		}
		else if ((!strcmp(forcommand, "symbol")) && (input[now_input_num] == '\0'))
		{
			symbol_display();
			addhistory(input);
		}
		else if (!strcmp(token_input[0], "progaddr"))
		{
			fail_flag = 0;
			tmp_program_address = 0;
			if (token_input[1] == NULL || token_input[2] != NULL)
				fail_flag = 1;

			for (l = 0 ; l < strlen(token_input[1]) ; l++){
				if (token_input[1][l] >= '0' && token_input[1][l] <= '9')
					tmp_program_address = 16*tmp_program_address + token_input[1][l]-'0';
				else if (token_input[1][l] >= 'A' && token_input[1][l] <= 'F')
					tmp_program_address = 16*tmp_program_address + token_input[1][l]-55;
				else if (token_input[1][l] >= 'a' && token_input[1][l] <= 'f')
					tmp_program_address = 16*tmp_program_address + token_input[1][l] - 87;
				else{
					fail_flag = 1;
					break;
				}
			}
			
			
			if (fail_flag == 1){
				printf("error! inapposite progaddr command!\n");
				continue;
			}

			program_address = tmp_program_address;
			addhistory(input);
		}
		else if (!strcmp(token_input[0], "loader"))
		{
			if (token_input[1] == NULL){
				printf("error! there is no filename!\n");
				continue;
			}

			if (token_input[4] != NULL){
				printf("error! too many file!\n");
				continue;
			}
			loader_flag = cmd_loader(token_input[1], token_input[2], token_input[3]);

			if(loader_flag == -1){
				printf("error! wrong filename!\n");
				continue;
			}
			if (loader_flag == -2)
				continue;
			
			addhistory(input);
		}
		else if((!strcmp(token_input[0], "run")) && (input[now_input_num] == '\0'))
		{
			cmd_run();
			addhistory(input);
		}
		else if (!(strcmp(token_input[0], "bp")))
		{
			fail_flag = 0;
			tmp_program_address = 0;
			if (token_input[2] != NULL)
				fail_flag = 1;
			if (fail_flag == 0 && token_input[1] != NULL && !strcmp(token_input[1], "clear")){
				cmd_bpclear();
				addhistory(input);
				continue;
			}
			else if (fail_flag == 0 && token_input[1] == NULL){
				cmd_bpprint();
				addhistory(input);
				continue;
			}
			else if (fail_flag == 0){
				for (l = 0 ; l < strlen(token_input[1]) ; l++){
					if (token_input[1][l] >= '0' && token_input[1][l] <= '9')
						tmp_program_address = 16*tmp_program_address + token_input[1][l]-'0';
					else if (token_input[1][l] >= 'A' && token_input[1][l] <= 'F')
						tmp_program_address = 16*tmp_program_address + token_input[1][l]-55;
					else if (token_input[1][l] >= 'a' && token_input[1][l] <= 'f')
						tmp_program_address = 16*tmp_program_address + token_input[1][l] - 87;
					else{
						fail_flag = 1;
						break;
					}
				}
			}

			if (fail_flag == 1){
				printf("error! inapposite bp command\n");
				continue;
			}
		
			if (cmd_bp(tmp_program_address) == -1)
				printf("already breakpoint in there\n");



			addhistory(input);
		}
		else
		{
			printf("error! inapposite command!\n");
		}
	}

}
void help()
{
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");
	printf("progaddr\n");
	printf("loader\n");
	printf("run\n");
	printf("bp\n");

}

void dir()
{
	DIR * dir;
	struct dirent *direntp = NULL;
	struct stat file_info;
	int cnt = 0;

	dir = opendir(".");

	while((direntp = readdir(dir)) != NULL)				// check file until end 
	{
		stat(direntp->d_name, &file_info);

		if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
			continue;


		if (S_ISDIR(file_info.st_mode))				// if it is directory, add /
			printf("%24s/", direntp->d_name);
		else if (S_IXUSR & file_info.st_mode)			// if it is excution file, add *
			printf("%24s*", direntp->d_name);
		else							// else, print file name
			printf("%25s", direntp->d_name);
		cnt++;
		if(cnt >= 3)
		{
			cnt = 0;
			printf("\n");
		}

	}
	
	printf("\n");
	closedir(dir);
}

void quit()
{
	history_link  *step, *nexts;
	opcode_list *opstep, *opnextstep;
	symtab *sstep, *snextstep;
	modi *mstep, *mnextstep;
	breakpoint *bstep, *bnextstep;

	int i;

	step = start;

	while(step!=NULL)				// free all dynamic allocation
	{
		nexts = step->next;
		free(step);
		step = nexts;
	}
	for (i = 0 ; i < 20 ; i++)
	{
		opstep = opcode[i];
		while(opstep != NULL)
		{
			opnextstep = opstep->next;
			free(opstep);
			opstep = opnextstep;
		}
	}
	
	sstep = symbol_table;
	while (sstep!=NULL){
		snextstep = sstep->next; free(sstep);sstep = snextstep;
	}
	
	mstep = modified;
	while(mstep !=NULL){
		mnextstep = mstep->next; free(mstep); mstep = mnextstep;
	}
	
	bstep = first_breakpoint;
	while(bstep!= NULL){
		bnextstep = bstep->next; free(bstep); bstep = bnextstep;
	}

}


void addhistory(char *input)
{
	history_link *new;
	new = (history_link*)malloc(sizeof(history_link));
	new->num = count++;
	strcpy(new->s, input);
	new->next = NULL;
	if (start == NULL)					// add input string to history linked list 
	{
		start = new;
		end = start;
	}
	else
	{
		end->next = new;
		end = end->next;
	}
}
void history()
{
	history_link *step;
	step = start;

	// print history start to lasthistory
	while(step != NULL)					// print all history 
	{
		printf("%d\t%s\n", step->num, step->s);
		step = step->next;
	}
}
void dump(int start, int end)
{
	int start_line, end_line, now_line;
	int i;
	now_line = start_line = start/16;
	end_line = end/16;

	// print dump start to end
	for (; now_line <= end_line ; now_line++)
	{
		printf("%05X ", now_line*16);
		for (i = now_line*16 ; i < (now_line+1) * 16 ; i++)		// print one line
		{
			if (i < start || i > end)
				printf("   ");
			else	
				printf(" %02X", dump_list[i]);
		}
		printf(" ; ");

		for (i = now_line*16 ; i < (now_line+1) * 16 ; i++)		// print character each dump memory
		{
			if (i < start || i > end || (int)dump_list[i] < 0x20 || (int)dump_list[i] > 0x7E)
				printf(".");
			else
				printf("%c", dump_list[i]);

		}



		printf("\n");
	}
	dump_lasthistory = end;
}
void edit(int address, int value)			// edit address 
{
	dump_list[address] = value;
}
void fill(int start, int end, int value)		// fill address
{
	int i;
	for (i = start ; i <= end ; i++)
		dump_list[i] = value;
}
int add_hashnum(char *token)
{
	// make hash table's number
	int i, cnt = 0;
	for (i = 0 ; i < strlen(token) ; i++)
		cnt += token[i];

	return cnt%20;
}
void opcodelist()
{
	int i;
	opcode_list *step;

	// print opcodelist.
	for (i = 0 ; i < MAX_HASH_TABLE_NUM ; i++)
	{
		printf("%d : ", i);
		step = opcode[i];
		if (step != NULL) // if it is first
		{
			printf("[%s,%X]", step->opname, step->opnum);
			step = step->next;
		}
		else 		// if it is no opocde in hash table number 
			printf(" empty ");
		while(step != NULL)
		{
			printf(" -> [%s,%X]", step->opname, step->opnum);
			step = step->next;
		}
		printf("\n");
	}
}
void init()					// 
{
	FILE *fp;
	char tmpcode[10], tmpsymbol[10];
	int tmpcodenum, hashnum, i;
	opcode_list *tmpopcode, *hashstep;

	
	// initalize program.
	// open opcode.txt file, and make hash talbe
	modified = NULL;
	symbol_table = NULL;
	for (i = 0 ; i < MAX_HASH_TABLE_NUM ; i++){
		opcode[i] = NULL;
	}
	fp = fopen("opcode.txt", "r");
	
	while(fscanf(fp, "%x %s %s", &tmpcodenum, tmpcode, tmpsymbol) != EOF)
	{
		tmpopcode = (opcode_list*)malloc(sizeof(opcode_list));
		tmpopcode->opnum = tmpcodenum;
		strcpy(tmpopcode->opname, tmpcode);
		strcpy(tmpopcode->symbol, tmpsymbol);
		tmpopcode->next = NULL;
		hashnum = add_hashnum(tmpopcode->opname);
		if (opcode[hashnum] == NULL)
			opcode[hashnum] = tmpopcode;
		else
		{
			hashstep = opcode[hashnum];
			while(hashstep->next != NULL)
				hashstep = hashstep->next;
			hashstep->next = tmpopcode;
		}
	}

	fclose(fp);
	start = NULL;

	for (i = 0 ; i < MAX_DUMP_LIST_NUM ; i++)
		dump_list[i] = 0;


	first_breakpoint = NULL;
	etable[0] = NULL;
	etable[1] = NULL;
	etable[2] = NULL;
}
int find_hashnum(char *input_opcode, char *symbol)
{
	opcode_list *find_code;
	int i, flag = 0, num = -1;

	// for opcode. find opcode's hash table's number
	for (i = 0 ; i < 20 ; i++)
	{
		find_code = opcode[i];
		while(find_code != NULL)
		{
			if (!strcmp(find_code->opname, input_opcode))
			{
				flag = 1;
				num = find_code->opnum;
				strcpy(symbol, find_code->symbol);
				break;
			}
			find_code = find_code->next;
		}
		if (flag)
			break;
	}
	return num;
}
void find_opcode_symbol(int opnum, char*symbol){
	opcode_list *find_code;
	int i, flag = 0;

	// for opcode. find opcode's name
	for (i = 0 ; i < 20 ; i++)
	{
		find_code = opcode[i];
		while(find_code != NULL)
		{
			if (opnum == find_code->opnum)
			{
				flag = 1;
				strcpy(symbol, find_code->symbol);
				break;
			}
			find_code = find_code->next;
		}
		if (flag)
			break;
	}
}
void type_filename(FILE *fp)
{
	char tmp;

	while(fscanf(fp, "%c", &tmp) != EOF)
		printf("%c", tmp);
}
int assemble_file(char* fp_filename)
{
	int line_number = 0;
	int LOCCTR = 0;
	int ex_LOCCTR = 0;
	int next_LOCCTR = 0;
	int base_address;
	int i, j, k;
	int opcode_number;
	int variable_number;
	int format4_flag = 0;
	int operand_address;
	int start_address, end_address;		// for obj file, start address and end address
	int obj_newlineflag = 0;
	int obj_linelen;					// for obj file, save one line's code length
	unsigned int disp;					// display
	unsigned int A_ad, B_ad, C_ad, D_ad; 	// ABCD -> object code
	

	int  numberflag,  labelflag, commaflag, xflag, formatflag;

	char line_input_by_asm[100];
	char output_at_nextfile[100];
	char *front_filename;
	char *tmp_input[7];
	char hash_type[6];
	char output_at_objfile[100];
	char tmpchar;						// for trash, or imediate char
	char asm_code_name[10];

	modi *modistep, *moditmp;			//for modified, 
	FILE *fp_asm, *fp_lst, *fp_obj;		// each FILE pointer

	free_symbol_table();
	
	if (modified != NULL) free(modified);
	modified = NULL;
	fp_asm = fopen(fp_filename, "r");

	if (fp_asm==NULL){					// if there is no file
		   printf("error! there is no file!\n");
		   return -2;
	}

	front_filename = strtok(fp_filename, ".");
	front_filename = strcat(front_filename, ".llst");	
	fp_lst = fopen(front_filename, "w");

	front_filename = strtok(front_filename, ".");
	front_filename = strcat(front_filename, ".obj");	
	fp_obj = fopen(front_filename, "w");

	front_filename = strtok(front_filename, ".");

	// pass 1 start
	i = 0;
	
	fgets(line_input_by_asm, 100, fp_asm);
	line_input_by_asm[strlen(line_input_by_asm) - 1] = 0;
	strcpy(output_at_nextfile, line_input_by_asm);

	tmp_input[i++] = strtok(line_input_by_asm, " \t\n\0");
	while((tmp_input[i++] = strtok(NULL, " \t\n\0")) != NULL);

	j = 0;
	if (!strcmp(tmp_input[1], "START")){ 			// if it is start
		while (tmp_input[2][j] != '\0')
			LOCCTR = LOCCTR *10 + tmp_input[2][j++] - '0';
		
		strcpy(asm_code_name, tmp_input[0]);
		start_address = LOCCTR;
		line_number += 5;
		fprintf(fp_lst, "%d\t%04X\t%s\n", line_number, LOCCTR, output_at_nextfile);
		ex_LOCCTR = LOCCTR;
	}
	else if(!strcmp(tmp_input[0], "START")){		// if it is start.
		while (tmp_input[1][j] != '\0')
			LOCCTR = LOCCTR *10 + tmp_input[1][j++] - '0';
		
		strcpy(asm_code_name, "DENAME");
		start_address = LOCCTR;
		line_number += 5;
		fprintf(fp_lst, "%d\t%04X\t%s\n", line_number, LOCCTR, output_at_nextfile);
		ex_LOCCTR = LOCCTR;
		
	}
	else{									// if first line is not START.
		strcpy(asm_code_name, "DENAME");
		fclose(fp_asm);
		front_filename = strcat(front_filename, ".asm");
		fp_asm = fopen(front_filename, "r");
		front_filename = strtok(front_filename, ".");
		start_address = ex_LOCCTR = LOCCTR = 0;
	}
	while (fgets(line_input_by_asm, 100, fp_asm) != NULL)	// read new line
	{
		line_input_by_asm[strlen(line_input_by_asm) - 1] = 0;
		strcpy(output_at_nextfile, line_input_by_asm);
		i = 0;
		while(i < 7)
			tmp_input[i++] = NULL;
		i = 0;
		if (line_input_by_asm[0] != '.')	// if it is not a comment
		{
			i = 0;
			tmp_input[i++] = strtok(line_input_by_asm, " \t\n\0");
			if(tmp_input[0] == NULL) continue;
			while((tmp_input[i++] = strtok(NULL, " \t\n\0") ) != NULL);
			j = 0;
			if (line_input_by_asm[0] == ' ') // it means there is no label
			{
				tmp_input[5] = tmp_input[4];
				tmp_input[4] = tmp_input[3];
				tmp_input[3] = tmp_input[2];
				tmp_input[2] = tmp_input[1];
				tmp_input[1] = tmp_input[0];
				tmp_input[0] = NULL;
			}

			if (!strcmp(tmp_input[1], "END")) // if end, break pass1
			{
				line_number+=5;
				fprintf(fp_lst, "%d\t\t%s\n", line_number, output_at_nextfile);
				break;
			}
			if (!strcmp(tmp_input[1], "BASE")) // if base, continue
			{
				//add_symbol_table(tmp_input[1], LOCCTR);
				line_number+=5;
				fprintf(fp_lst, "%d\t\t%s\n", line_number, output_at_nextfile);
				continue;
			}
			
			if (tmp_input[1][0] == '+')		//for format4
			{
				format4_flag = 1;
				tmp_input[1] = tmp_input[1] + 1;
			}
					

			opcode_number = find_hashnum(tmp_input[1], hash_type);
			if (opcode_number == -1)			// if it is not a opcode
			{
				variable_number = 0;
				if (!strcmp(tmp_input[1], "WORD")) // word takes 3 byte
				{
					LOCCTR += 3;
				}
				else if (!strcmp(tmp_input[1], "RESW"))	// resw reserve the indicated number of words for a data area
				{
					k = 0;
					while (tmp_input[2][k] >= '0' && tmp_input[2][k] <= '9')
					{
						variable_number = variable_number * 10 + tmp_input[2][k] - '0';
						k++;
					}
					if (tmp_input[2][k] != '\0')
					{
						assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! wrong variable decarlation!\n", 1, front_filename);
						return -1;
					}
					LOCCTR += variable_number * 3;
				}
				else if (!strcmp(tmp_input[1], "RESB"))  // resb reserve the indicated number of words for a data area
				{
					k = 0;
					while (tmp_input[2][k] >= '0' && tmp_input[2][k] <= '9')
					{
						variable_number = variable_number * 10 + tmp_input[2][k] - '0';
						k++;
					}
					if (tmp_input[2][k] != '\0')
					{
						assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! wrong variable decarlation!\n", 1, front_filename);
						return -1;
					}
					LOCCTR += variable_number;
				
				}
				else if (!strcmp(tmp_input[1], "BYTE")) // byte reserve hex or char constant.
				{
					k = 0;
					j = 0;
					if (tmp_input[2][k] == 'C')
					{
						k++;
						if (tmp_input[2][k++] == '\'')
						{
							while(tmp_input[2][k] != '\'')
							{
								if (tmp_input[2][k] == '\0')
								{
									assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! wrong variable declaration\n", 1, front_filename);
									return -1;
								}
								k++;
								j++;
							}
						}
						else
						{
							assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! wrong type of variable!\n",1, front_filename);
							return -1;
						}
						LOCCTR += j;
					}
					else if (tmp_input[2][k] == 'X')
					{
						k++;
						if (tmp_input[2][k++] == '\'')
						{
							while(tmp_input[2][k] != '\'')
							{
								if (tmp_input[2][k] == '\0')
								{
									assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! wrong variable declaration\n", 1, front_filename);
									return -1;
								}
								k++;
								j++;
							}
						}
						else
						{
							assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! wrong type of variable!\n", 1, front_filename);
							return -1;
						}
						LOCCTR += j/2;
					}
					else
					{
						assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! wrong type of variable!\n", 1, front_filename);
						return -1;
					}
				}
				else			// it is not opcode and variable declaration
				{
					assemble_error(fp_asm, fp_lst, fp_obj, line_number, "wrong opcode in here!\n", 1, front_filename);
					return -1;
				}
			}
			else				// if it is opcode, find format and add LOCCTR
			{
				if (hash_type[0] == '1')
					LOCCTR += 1;
				else if (hash_type[0] == '2')
					LOCCTR += 2;
				else if (format4_flag == 1)
					LOCCTR += 4;
				else
					LOCCTR += 3;
			}
			
				//find another symbol
			line_number+=5;
			fprintf(fp_lst, "%d\t%04X\t%s\n", line_number, ex_LOCCTR, output_at_nextfile);		// add at immediate file
				
			if (tmp_input[0] != NULL){ // label in there
				if (find_symbol_table(tmp_input[0]) != -1){
					assemble_error(fp_asm, fp_lst, fp_obj, line_number, "same symbol in here!\n", 1, front_filename);
					return -1;
				}
				else {
					add_symbol_table(tmp_input[0], ex_LOCCTR);
				}
			}
		
		}
		else		// if it is comment, add line at immediate file directly
		{
			line_number+=5;
			fprintf(fp_lst, "%d\t%s\n", line_number, output_at_nextfile);
	
		}
		format4_flag = 0;
		ex_LOCCTR = LOCCTR;
	}

	end_address = LOCCTR;
	fclose(fp_lst);
	fclose(fp_asm);



	// for pass2, make file and initialize

	fp_asm = NULL;
	fp_lst = NULL;

	front_filename = strcat(front_filename, ".lst");	
	fp_lst = fopen(front_filename, "w");

	front_filename = strtok(front_filename, ".");
	front_filename = strcat(front_filename, ".llst");
	fp_asm = fopen(front_filename, "r");

	front_filename = strtok(front_filename, ".");

	line_number = LOCCTR = ex_LOCCTR = i = j = k = 0;
	format4_flag = 0;
	base_address = -1;	
	
	// pass 2 start
	i = 0;
	line_number = 0;
	fgets(line_input_by_asm, 100, fp_asm);
	line_input_by_asm[strlen(line_input_by_asm)-1] = 0;
	strcpy(output_at_nextfile, line_input_by_asm);

	tmp_input[i++] = strtok(line_input_by_asm, " \t\n\0");
	while((tmp_input[i++] = strtok(NULL, " \t\n\0")) != NULL);
	line_number += 5;
	fprintf(fp_lst, "%s\n", output_at_nextfile);
	fprintf(fp_obj, "H%-6s%06X%06X\n", asm_code_name, start_address, end_address);
	j = 0, k = 0;
	
	while((tmp_input[1][j] >= '0' && tmp_input[1][j] <= '9') ||(tmp_input[1][j] >= 'A' && tmp_input[1][j] <= 'F')){
		if (tmp_input[1][j] <= '9')
			k = k * 16 + tmp_input[1][j] - '0';
		else
			k = k*16 + tmp_input[1][j] - 'A' + 10;
		j++;
	}
	LOCCTR = k;


	obj_newlineflag = 0;
	sprintf(output_at_objfile, "T%06X", k);		// for next obj line
	obj_linelen = 0;
	
	while(fgets(line_input_by_asm, 100, fp_asm) != NULL){
		line_number += 5;
		operand_address = 0;
		numberflag = labelflag = commaflag = xflag = formatflag = 0;
		next_LOCCTR = disp = 0;
		line_input_by_asm[strlen(line_input_by_asm)-1] = 0;
		format4_flag = 0;
		strcpy(output_at_nextfile, line_input_by_asm);

		A_ad = B_ad = C_ad = D_ad = 0;		// address initailize
		i = 0;

		while(i<7) tmp_input[i++] = NULL;		// initailize
		i = 0;
		tmp_input[i++] = strtok(line_input_by_asm, " \t\n\0");
		while(i < 7 && ((tmp_input[i++] = strtok(NULL, " \t\n\0")) != NULL));
	
		if (tmp_input[1][0] == '.') {
			fprintf(fp_lst, "%s\n", output_at_nextfile);
			continue;
		}
		// input and seperate. all word

		if (!strcmp(tmp_input[1], "END")){
			   fprintf(fp_lst, "%s\n", output_at_nextfile);
			   break;
		}		// if it is a end, break
		if (!strcmp(tmp_input[1], "BASE")){		// if it is base, add base address and continue

			   if (tmp_input[2][0] == '#' || tmp_input[2][0] == '@'){
					 tmp_input[2] = tmp_input[2] + 1;
			   }
			   if (find_symbol_table(tmp_input[2]) == -1){
					 assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! wrong base symbol!\n",2,front_filename);
					 return -1;
			   }
			   else{
					 base_address = find_symbol_table(tmp_input[2]);
			   }

			   fprintf(fp_lst, "%s\n", output_at_nextfile);
	 		   continue;
		}
		if (tmp_input[1][0] == '.'){
			   fprintf(fp_lst,"%s\n", output_at_nextfile);
			   continue;
		}		// if it is a comment
		if (tmp_input[2][0] == '+'){
			   format4_flag = 1;
			   tmp_input[2] = tmp_input[2] + 1;
		}
		if (tmp_input[3] != NULL && tmp_input[3][0] == '+'){
			format4_flag = 1;
			tmp_input[3] = tmp_input[3] + 1;
		}
		if (find_hashnum(tmp_input[2], hash_type) != -1 ){
			tmp_input[6] = tmp_input[5];
			tmp_input[5] = tmp_input[4];
			tmp_input[4] = tmp_input[3];
			tmp_input[3] = tmp_input[2];
			tmp_input[2] = NULL;
		}			// if tmp_input[2] is opcode, it hasn't label
		
		opcode_number = find_hashnum(tmp_input[3], hash_type); //find optab for opcode
		if (opcode_number != -1){		// it is command
			if ((tmp_input[4] != NULL && tmp_input[4][strlen(tmp_input[4])-1] == ',') || (tmp_input[5] != NULL && (tmp_input[5][0] == ',' || !strcmp(tmp_input[5], ","))) || (tmp_input[4] != NULL && (tmp_input[4][strlen(tmp_input[4])-2] == ',' || tmp_input[4][strlen(tmp_input[4])-3] == ','))){
				   commaflag = 1;

				   if (tmp_input[5] != NULL && !strcmp(tmp_input[5], ",")){
						tmp_input[5] = tmp_input[6];
				   }
				   else if (tmp_input[5] != NULL && tmp_input[5][0] == ','){
						   tmp_input[5] = tmp_input[5] + 1;
				   }
				   else{
						if (tmp_input[4][strlen(tmp_input[4])-1] == ',')
						   tmp_input[4][strlen(tmp_input[4])-1] = 0;
						else{
							tmp_input[6] = strchr(tmp_input[4], ',');
							tmp_input[4] = strtok(tmp_input[4], ",");
							tmp_input[5] = tmp_input[6]+1;
						}
				   }
			}		// if it has comma
			
			if (hash_type[0] == '1'){
				   formatflag = 1;
			}
			else if (!strcmp(hash_type, "2")){
				   formatflag = 2;
			}
			else
				   formatflag = 3;		//find hash type and set flag

			j = 0, k = 0;
			while((tmp_input[1][j] >= '0' && tmp_input[1][j] <= '9') ||(tmp_input[1][j] >= 'A' && tmp_input[1][j] <= 'F')){
				   if (tmp_input[1][j] <= '9')
					  k = k * 16 + tmp_input[1][j] - '0';
				   else
					k = k*16 + tmp_input[1][j] - 'A' + 10;
				   j++;
			}
			LOCCTR = k;		//set LOCCTR

		 	if (format4_flag == 1) next_LOCCTR  = LOCCTR + 4;
		  	else next_LOCCTR = LOCCTR + 3;

			if (formatflag == 1){		// if it is format 1 
				A_ad = opcode_number;
				sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t\t%02X", A_ad);
				if (obj_linelen+1 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){		// if it is over max length, print obj file and make new line string
					sprintf(output_at_objfile+9+(obj_linelen*2),"%02X", A_ad);
					obj_linelen += 1;
				}
				else{
					tmpchar = output_at_objfile[9];
					sprintf(output_at_objfile+7,"%02X", obj_linelen);
					output_at_objfile[9] = tmpchar;
					fprintf(fp_obj, "%s\n", output_at_objfile);
					strcpy(output_at_objfile, "");
					sprintf(output_at_objfile, "T%06X  %02X", LOCCTR, A_ad);
					obj_linelen = 1;
					obj_newlineflag = 0;
				}
			}
			else if (formatflag == 2){		// if it is format 2, find register number
				j = 4;
				for (; j<6 ; j++){
				   if (tmp_input[j] == NULL && j == 5){
						 B_ad = B_ad*16;
				   }
				   else if (!strcmp(tmp_input[j], "A")){
						if (j == 4) B_ad = 0;
						else B_ad = B_ad*16+0;
				   }
				   else if (!strcmp(tmp_input[j], "X")){
						 if (j==4)  B_ad = 1;
						 else B_ad = B_ad*16+1;
				   }
				    else if (!strcmp(tmp_input[j], "L")){
						 if (j==4) B_ad = 2;
						 else B_ad = B_ad*16+2;
				   }
				    else if (!strcmp(tmp_input[j], "B")){
						 if (j==4) B_ad = 3;
						 else B_ad = B_ad*16+3;
				   }
				    else if (!strcmp(tmp_input[j], "S")){
						 if (j==4) B_ad = 4;
						 else B_ad = B_ad*16+4;
				   }
				    else if (!strcmp(tmp_input[j], "T")){
						 if (j==4) B_ad = 5;
						 else B_ad = B_ad*16+5;
				   }
				    else if (!strcmp(tmp_input[j], "F")){
						 if (j==4) B_ad = 6;
						 else B_ad = B_ad*16+6;
				   }
				    else if (!strcmp(tmp_input[j], "PC")){
						 if (j==4) B_ad = 8;
						 else B_ad = B_ad*16+8;
				   }
				    else if (!strcmp(tmp_input[j], "SW")){
						 if (j==4) B_ad = 9;
						 else B_ad = B_ad*16+9;
				   }
				}
				A_ad = opcode_number;
				if (tmp_input[5] == NULL)
					sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t\t%02X%02X", A_ad,B_ad);
				else
					sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t%02X%02X", A_ad,B_ad);

				if (obj_linelen+2 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){		// if it is over max length, print obj file and make new line string
					sprintf(output_at_objfile+9+(obj_linelen*2),"%02X%02X", A_ad,B_ad);
					obj_linelen += 2;
				}
				else{
					tmpchar = output_at_objfile[9];
					sprintf(output_at_objfile+7,"%02X", obj_linelen);
					output_at_objfile[9] = tmpchar;
					fprintf(fp_obj, "%s\n", output_at_objfile);
					strcpy(output_at_objfile, "");
					sprintf(output_at_objfile, "T%06X  %02X%02X", LOCCTR, A_ad,B_ad);
					obj_linelen = 2;
					obj_newlineflag = 0;
				}
			}
			else{// if it is format 4 or 3

				if (!strcmp(tmp_input[3], "JSUB")){
					   modistep = (modi*)malloc(sizeof(modi));
					   modistep->next = NULL;
					   modistep->address = LOCCTR+1;
					   if (format4_flag == 1) modistep->hbyte = 5;
					   else modistep->hbyte = 4;

					   if (modified == NULL){ 
							 modified = modistep;
					   }
					   else{
					   		moditmp = modified;
							while(moditmp->next!= NULL){
								   moditmp = moditmp->next;
							}
							moditmp->next = modistep;
					   }
				}

				if (tmp_input[4] == NULL && !strcmp(tmp_input[3], "RSUB")){
					   A_ad += 3;
					   B_ad = 0;
					   C_ad = 0;
					   A_ad += opcode_number;
				sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t\t%02X%02X%02X", A_ad,B_ad,C_ad);
					if (obj_linelen+3 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){		// if it is over max length, print obj file and make new line string
						sprintf(output_at_objfile+9+(obj_linelen*2),"%02X%02X%02X", A_ad,B_ad,C_ad);
						obj_linelen += 3;
					}
					else{
						sprintf(output_at_objfile+7,"%02X", obj_linelen);
						fprintf(fp_obj, "%s\n", output_at_objfile);
						strcpy(output_at_objfile, "");
						sprintf(output_at_objfile, "T%06X  %02X%02X%02X", LOCCTR, A_ad,B_ad,C_ad);
						obj_linelen = 3;
						obj_newlineflag = 0;
					}
				}
				else{
				
					   if (tmp_input[4][strlen(tmp_input[4])-1] == ','){
							 commaflag = 1;
							 tmp_input[4][strlen(tmp_input[4])-1] = 0;
					   }

					   if (commaflag == 1){				// if it has X, set X flag to 1
							 if (!strcmp(tmp_input[5], "X")){
								   B_ad += 128;
							 }
					   }
						 if (tmp_input[4][0] == '#'){   // direct, ni = 01
							 j = 1;
							 operand_address = 0;
							 if (tmp_input[4][j] >= '0' && tmp_input[4][j] <= '9'){
							   while (tmp_input[4][j] >= '0' && tmp_input[4][j] <= '9'){
									 operand_address = operand_address * 10 + tmp_input[4][j] - '0';
									 j++;
							   }
							   numberflag = 1;
						   }
						   else{
								 operand_address = find_symbol_table(tmp_input[4]+1);
								 numberflag = 2;
						   }
						   A_ad = 1;
					   }
					   else if (tmp_input[4][0] == '@'){ // pointer, ni = 10
							 operand_address = find_symbol_table(tmp_input[4]+1);
							 A_ad = 2;
					   }
					   else if (find_symbol_table(tmp_input[4]) != -1){ // it has symbol,
						   operand_address = find_symbol_table(tmp_input[4]); // find symtab for symbol
						   A_ad = 3;
					   }
					   else{ // it hasn't symbol
						   assemble_error(fp_asm, fp_lst, fp_obj, line_number, "error! undefined symbol!\n",2,front_filename);
						   return -1;	
					   }
				   // do, assemble the object code instruction
					   A_ad = opcode_number + A_ad;

					   if (format4_flag == 0){		// if it is format 4

						   if (numberflag == 1 ){
							   disp = operand_address;
							   if (disp < 4096){
									 B_ad += ((disp%4096)/256);
									 C_ad = disp%256;
									 sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t%02X%02X%02X", A_ad,B_ad,C_ad);
								}
							   else{
								 B_ad += 16;
								 B_ad += operand_address/65536;
								 C_ad = (operand_address%65536)/256;
								 D_ad = (operand_address%256);
								sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t%02X%02X%02X%02X", A_ad,B_ad,C_ad, D_ad);
								if (obj_linelen+4 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){
									sprintf(output_at_objfile+9+(obj_linelen*2),"%02X%02X%02X%02X", A_ad,B_ad,C_ad,D_ad);
									obj_linelen += 4;
								}
								else{
									tmpchar = output_at_objfile[9];
									sprintf(output_at_objfile+7,"%02X", obj_linelen);
									output_at_objfile[9] = tmpchar;
									fprintf(fp_obj, "%s\n", output_at_objfile);
									strcpy(output_at_objfile, "");
									sprintf(output_at_objfile, "T%06X  %02X%02X%02X%02X", LOCCTR, A_ad,B_ad,C_ad,D_ad);
									obj_linelen = 4;
									obj_newlineflag=0;
								}
						   		format4_flag = 1;
	
							   }
						   }
						   else if ((operand_address - next_LOCCTR < 2048) && (operand_address - next_LOCCTR > -2048)){		// if use pc relative
							   B_ad += 32;
							   disp = operand_address - next_LOCCTR;
							   B_ad += ((disp%4096)/256);
							   C_ad = disp%256;
			    		       sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t%02X%02X%02X", A_ad,B_ad,C_ad);
						   }
						   else if ((operand_address - base_address < 4096) && operand_address - base_address >= 0 && base_address != -1){		// if use base relative
							   B_ad += 64;
							   disp = operand_address - base_address;
					   		   B_ad += ((disp%4096)/256);
							   C_ad = disp%256;
					           sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t%02X%02X%02X", A_ad,B_ad,C_ad);
						   }
						   else{
								 B_ad += 16;
								 B_ad += operand_address/65536;
								 C_ad = (operand_address%65536)/256;
								 D_ad = (operand_address%256);
								sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t%02X%02X%02X%02X", A_ad,B_ad,C_ad, D_ad);
								if (obj_linelen+4 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){
									sprintf(output_at_objfile+9+(obj_linelen*2),"%02X%02X%02X%02X", A_ad,B_ad,C_ad,D_ad);
									obj_linelen += 4;
								}
								else{
									tmpchar = output_at_objfile[9];
									sprintf(output_at_objfile+7,"%02X", obj_linelen);
									output_at_objfile[9] = tmpchar;
									fprintf(fp_obj, "%s\n", output_at_objfile);
									strcpy(output_at_objfile, "");
									sprintf(output_at_objfile, "T%06X  %02X%02X%02X%02X", LOCCTR, A_ad,B_ad,C_ad,D_ad);
									obj_linelen = 4;
									obj_newlineflag=0;
								}
						   		format4_flag = 1;
						   }
						   
						   if (format4_flag == 0){
								   if (obj_linelen+3 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){
										sprintf(output_at_objfile+9+(obj_linelen*2),"%02X%02X%02X", A_ad,B_ad,C_ad);
									   obj_linelen += 3;
								   }
								   else{
									   tmpchar = output_at_objfile[9];
									   sprintf(output_at_objfile+7,"%02X", obj_linelen);
										output_at_objfile[9] = tmpchar;
										fprintf(fp_obj, "%s\n", output_at_objfile);
										strcpy(output_at_objfile, "");
										sprintf(output_at_objfile, "T%06X  %02X%02X%02X", LOCCTR, A_ad,B_ad,C_ad);
										obj_linelen = 3;
										obj_newlineflag = 0;
								   }
							 }
		 			  }
					  else if (format4_flag == 1){
						 B_ad += 16;
						 B_ad += operand_address/65536;
						 C_ad = (operand_address%65536)/256;
						 D_ad = (operand_address%256);
						sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t%02X%02X%02X%02X", A_ad,B_ad,C_ad, D_ad);
						if (obj_linelen+4 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){
							sprintf(output_at_objfile+9+(obj_linelen*2),"%02X%02X%02X%02X", A_ad,B_ad,C_ad,D_ad);
							obj_linelen += 4;
						}
						else{
							tmpchar = output_at_objfile[9];
							sprintf(output_at_objfile+7,"%02X", obj_linelen);
							output_at_objfile[9] = tmpchar;
							fprintf(fp_obj, "%s\n", output_at_objfile);
							strcpy(output_at_objfile, "");
							sprintf(output_at_objfile, "T%06X  %02X%02X%02X%02X", LOCCTR, A_ad,B_ad,C_ad,D_ad);
							obj_linelen = 4;
							obj_newlineflag=0;
						}				   
					}
				}
			}
			fprintf(fp_lst,"%s\n",output_at_nextfile);
		}
		else if (!strcmp(tmp_input[3], "BYTE") || !strcmp(tmp_input[3], "WORD")){// it is variable declartion
			disp = 0;
			if (!strcmp(tmp_input[3], "BYTE")){
				if(tmp_input[4][0] == 'C'){
					j = 2;
					while(tmp_input[4][j] != '\''){
						   disp = disp*256 + tmp_input[4][j];
						   j++;
					}
					sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t%02X", disp);
					if (obj_linelen+j-2 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){
						sprintf(output_at_objfile+9+(obj_linelen*2),"%X", disp);
						obj_linelen += j-2;
					}
					else{
						tmpchar = output_at_objfile[9];
						sprintf(output_at_objfile+7,"%02X", obj_linelen);
						output_at_objfile[9] = tmpchar;
						fprintf(fp_obj, "%s\n", output_at_objfile);
						strcpy(output_at_objfile, "");
						sprintf(output_at_objfile, "T%06X  %X", LOCCTR, disp);
						obj_linelen = j-2;
						obj_newlineflag=0;
					}				   
				}
				else if (tmp_input[4][0] == 'X'){
					j = 2;
					disp = 0;
					if ((tmp_input[4][j] >= '0' && tmp_input[4][j] <= '9') || (tmp_input[4][j] >= 'A' && tmp_input[4][j] <= 'F')){
						sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t\t");
						while(tmp_input[4][j] != '\''){
							if (tmp_input[4][j] <= '9'){
								disp = disp * 16 + tmp_input[4][j] - '0';
							}			   
							else{
								disp = disp * 16 +tmp_input[4][j] - 'A' + 10;
							}
							j++;
						}
						sprintf(output_at_nextfile+strlen(output_at_nextfile), "%02X",disp);
						if (obj_linelen+(j-2)/2 <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){
							sprintf(output_at_objfile+9+(obj_linelen*2),"%X", disp);
							obj_linelen += (j-2)/2;
						}
						else{
							tmpchar = output_at_objfile[9];
							sprintf(output_at_objfile+7,"%02X", obj_linelen);
							output_at_objfile[9] = tmpchar;
							fprintf(fp_obj, "%s\n", output_at_objfile);
							strcpy(output_at_objfile, "");
							sprintf(output_at_objfile, "T%06X  %X", LOCCTR, disp);
							obj_linelen = (j-2)/2;
							obj_newlineflag=0;
						}				   
		   
					}
				}
			}
			else{ 	// if it is a word
				j = 10;
				disp = 0;
				if (tmp_input[4][j] >= '0' && tmp_input[4][j] <= '9'){
					sprintf(output_at_nextfile+strlen(output_at_nextfile), "\t");
					while(tmp_input[4][j] != '\0' && tmp_input[4][j] != '\n'){
						disp = disp * 10 + tmp_input[4][j] - '0';
						j++;
					}
					sprintf(output_at_nextfile+strlen(output_at_nextfile), "%02X",disp);
					if (obj_linelen+j <= MAX_OBJLINE_LENGTH && obj_newlineflag == 0){
						sprintf(output_at_objfile+9+(obj_linelen*2),"%X", disp);
						obj_linelen += j;
					}
					else{
						tmpchar = output_at_objfile[9];
						sprintf(output_at_objfile+7,"%02X", obj_linelen);
						output_at_objfile[9] = tmpchar;
						fprintf(fp_obj, "%s\n", output_at_objfile);
						strcpy(output_at_objfile, "");
						sprintf(output_at_objfile, "T%06X  %X", LOCCTR, disp);
						obj_linelen = j;
						obj_newlineflag=0;
					}				   
		   
				}			
			}
			fprintf(fp_lst, "%s\n", output_at_nextfile);
	  	}
		else {	// if it is resw or resb
			   obj_newlineflag = 1;
		}

	}
	tmpchar = output_at_objfile[9];
	sprintf(output_at_objfile+7,"%02X", obj_linelen);
	output_at_objfile[9] = tmpchar;
	fprintf(fp_obj, "%s\n", output_at_objfile);
	strcpy(output_at_objfile, "");


	i = 0, j = 0;

	moditmp = modified;

	while(moditmp != NULL){
		   fprintf(fp_obj, "M%06X%02X\n", moditmp->address, moditmp->hbyte);
		   moditmp = moditmp->next;
	}

	fprintf(fp_obj,"E%06X\n", start_address);
				
	fclose(fp_asm);
	fclose(fp_lst);
	fclose(fp_obj);
	
	front_filename = strcat(front_filename, ".llst");
	remove(front_filename);
	front_filename = strtok(front_filename, ".");
	printf("output file : [%s.lst], [%s.obj]\n", front_filename, front_filename);
	return 0;
}
void assemble_error(FILE *fp_asm, FILE *fp_lst, FILE*fp_obj, int error_line, char *error_msg, int num, char *filename)		// print error line and message, close file and remove file
{
	free_symbol_table();
	fclose(fp_asm);
	fclose(fp_lst);
	fclose(fp_obj);

	
     filename = strcat(filename,".llst");
     remove(filename);
     filename = strtok(filename, ".");
     filename = strcat(filename,".obj");
     remove(filename);
	
	if (num == 2){
		filename = strtok(filename, ".");
	     filename = strcat(filename,".lst");
	     remove(filename);
	}
	filename = strtok(filename, ".");

	printf("error : line number %d\n%s", error_line, error_msg);
}
int add_symbol_table(char *symbolname, int address)		// add symbol table, descending order
{
	symtab *tmp, *tmpstep, *exstep;

	tmp = (symtab*)malloc(sizeof(symtab));
	strcpy(tmp->symbol, symbolname);
	tmp->address = address;
	tmp->next = NULL;

	if (symbol_table == NULL){
		   symbol_table = tmp;
	}
	else{
		tmpstep = symbol_table;
		while(tmpstep != NULL){
			if (!strcmp(tmp->symbol, tmpstep->symbol)){
				   return -1;
			}
			else if (strcmp(tmp->symbol, tmpstep->symbol) > 0){
				if (tmpstep == symbol_table){
					   tmp->next = symbol_table;
					   symbol_table = tmp;
					   return 0;
				}
				else{
					   tmp->next = tmpstep;
					   exstep->next = tmp;
					   return 0;
				}
			}
			else if (tmpstep->next == NULL){
				   tmpstep->next = tmp;
				   return 0;
			}
			else	{
				exstep = tmpstep;
				tmpstep = tmpstep->next;
			}
		}
	}
	return 0;
}
int find_symbol_table(char *name)		// find symbol table and return address
{
	symtab *find;
	int num = -1;

 	 find = symbol_table;
	  while(find != NULL)
	  {
		  if (!strcmp(find->symbol, name)) {
			  return find->address;
		  }
		  find = find->next;
	  }
	return num;
}
void free_symbol_table()			//free symbol table
{
	symtab *symbolstep, *nextsymbolstep;

	symbolstep = symbol_table;
	while(symbolstep != NULL){
		nextsymbolstep = symbolstep->next;
		free(symbolstep);
		symbolstep = nextsymbolstep;
	}
	symbol_table = NULL;
}
void symbol_display()				//display symbol table for symbol command
{
	symtab *step;
	step = symbol_table;
	
	while (step != NULL){
		   printf("\t%s\t%04X", step->symbol, step->address);
		   step = step->next;
		   if (step != NULL)
				   printf("\n");
	}
}

int cmd_bp(int address){			// make bp point, like priority queue
	
	breakpoint *tmp, *step, *bstep;
	
	tmp = (breakpoint*)malloc(sizeof(breakpoint));
	
	tmp->bpnum = address;
	tmp->next = NULL;

	step = first_breakpoint;

	if (step == NULL){
		first_breakpoint = tmp;
	}
	else{
		bstep = step;
		
		if (step->bpnum > address){
			tmp->next = step;
			first_breakpoint = tmp; 
		}
		else{
			step = step->next;

			while(1){
				if (step == NULL){
					bstep->next = tmp;
					break;
				}
				else if (step->bpnum < address){
					step = step->next;
					bstep = bstep->next;
					continue;
				}
				else if (step->bpnum == address){
					free(tmp);
					return -1;
				}
				else if (step->bpnum > address){
					tmp->next = step;
					bstep->next = tmp;
					break;
				}
			}
		}
	}
	cur_bp = first_breakpoint;
	printf("\t\t[ok] create breakpoint %04X\n", address);
	return 1;
}

void cmd_bpclear(){				//delete all bp
	breakpoint *step, *bstep;

	step = first_breakpoint;

	
	if (step == NULL){
		printf("there is no breakpoint!\n");
		return;
	}

	bstep = step->next;

	while (bstep != NULL){
		free(step);
		step = bstep;
		bstep = bstep->next;
	}
	free(step);
	first_breakpoint = NULL;

	printf("\t\t[ok] clear all breakpoints\n");

	return;
}
void cmd_bpprint(){				//print bp
	breakpoint *step;
	step = first_breakpoint;
	printf("\t\tbreakpoint\n");
	printf("\t\t----------\n");
	while (step != NULL){
		printf("\t\t%4x\n", step->bpnum);
		step = step->next;
	}

}

int cmd_loader(char *filename_1, char *filename_2, char *filename_3){
	
	int all_filenum = 0;
	int csaddr, execsaddr;
	int i, k, l;							//for loop
	int cslength;
	int returnaddr;
	int T_start_addr;
	int T_len;
	int M_addr, M_mod, M_div;
	unsigned int tmp_dump;
	int total_len = 0;
	
	int *ref_code;


	FILE *fp[3];
	char input_string[100];
	char tmp_symname[10];
	char tmp_addr[10];


	ESTAB *etmp, *estep;

//	if (etable[0] != NULL){
		for (i = 0 ; i < 3 ; i++){
			estep = etable[i];
			while(estep!= NULL){
				etmp = estep->next;
				free(estep);
				estep = etmp;
			}
			etable[i] = NULL;
		}
//	}

	csaddr = program_address;

	if (filename_1 != NULL){
		all_filenum++;
		if ((fp[0] = fopen(filename_1, "r")) == NULL)
			return -1;
		if (filename_2 != NULL){
			all_filenum++;
			if ((fp[1] = fopen(filename_2, "r")) == NULL){
				fclose(fp[0]);
				return -1;
			}
			if (filename_3 != NULL){
				all_filenum++;
				if ((fp[2] = fopen(filename_3, "r")) == NULL){
					fclose(fp[0]);
					fclose(fp[1]);
					return -1;	
				}
			}
		}
	}

	////// pass 1

	for (i = 0 ; i < all_filenum ; i++){	
		fgets(input_string, 100, fp[i]);
		memcpy(tmp_symname, input_string + 1, 6);
		memcpy(tmp_addr, input_string+13, 6);
		etmp = (ESTAB*)malloc(sizeof(ESTAB));
		strcpy(etmp->symname, tmp_symname);
		for (k = 0 ; k < 7 ; k++){
			if (k == 6){
				etmp->symname[k] = 0;
				break;
			}
			if (etmp->symname[k] == ' ' || etmp->symname[k] == '\t' || etmp->symname[k] == '\n'){
				etmp->symname[k] = 0;
				break;
			}
		}
		
		etmp->length = hextodec(tmp_addr, 6);
		etmp->addr = csaddr;

		if (findetab(etmp->symname, all_filenum, &returnaddr) == -1){
			error_loader(fp[0], fp[1], fp[2], all_filenum, "same progname in there!\n");
			return -2;
		}

		etable[i] = etmp;


		while(1){
			fgets(input_string, 100, fp[i]);
			if (input_string[0] == 'E')			// if E, end
				break;
			if (input_string[0] == 'D'){		// if D, it is extern def, add etable
				for (l = 1 ; l < strlen(input_string)-1; l += 12){
					memcpy(tmp_symname, input_string+l, 6);
					memcpy(tmp_addr, input_string+l+6, 6);
					etmp = (ESTAB*)malloc(sizeof(ESTAB));
					strcpy(etmp->symname, tmp_symname);
					for (k = 0 ; k < 7 ; k++){
						if (k == 6){
							etmp->symname[k] = 0;
							break;
						}
						if (etmp->symname[k] == ' ' || etmp->symname[k] == '\t' || etmp->symname[k] == '\n'){
							etmp->symname[k] = 0;
							break;
						}
					}
					etmp->addr = csaddr + hextodec(tmp_addr, 6);
					etmp->length = 0;
					etmp->count = 0;
	
					if (findetab(etmp->symname, all_filenum, &returnaddr) == -1){
						error_loader(fp[0], fp[1], fp[2], all_filenum, "same symb in there!\n");
						return -2;
					}

					estep = etable[i];
					while (estep->next != NULL)
						estep = estep->next;
					estep->next = etmp;
					etable[i]->count++;

				}
			}
		}
		csaddr += etable[i]->length;
	}

	//////pass2

	execsaddr = csaddr = program_address;

	if(all_filenum > 0){
		fclose(fp[0]);
		fp[0] = fopen(filename_1, "r");
		if (all_filenum > 1){
			fclose(fp[1]);
			fp[1] = fopen(filename_2, "r");
			if (all_filenum > 2){
				fclose(fp[2]);
				fp[2] = fopen(filename_3, "r");
			}
		}
	}

	for (i = 0 ; i < all_filenum ; i++){					// loop
		fgets(input_string, 100, fp[i]);
		cslength = etable[i]->length;
		ref_code = (int*)malloc(sizeof(int)*100);
		for (k = 0 ; k < 100 ; k++)
			ref_code[k] = -1;

		ref_code[1] = etable[i]->addr;
		while(1){
			fgets(input_string, 100, fp[i]);
			if (input_string[0] == 'E')			// if finish
				break;
			if (input_string[0] == '.')			// if comment
				continue;
			else if (input_string[0] == 'R'){	// if R
				for (k = 1 ; k < strlen(input_string)-1 ; k+=8){
					memcpy(tmp_addr, input_string+k, 2);
					memcpy(tmp_symname, input_string+k+2, 6);
					for (l = 0 ; l < 7 ; l++){
						if (l == 6){
							tmp_symname[l] = 0;
							break;
						}
						if (tmp_symname[l] == ' ' || tmp_symname[l] == '\t' || tmp_symname[l] == '\n'){
							tmp_symname[l] = 0;
							break;
						}
					}
					if(findetab(tmp_symname, all_filenum, &returnaddr) == 0){
						error_loader(fp[0], fp[1], fp[2], all_filenum, "there is no ref symbol!\n");
						return -2;
					}
					ref_code[hextodec(tmp_addr, 2)] = returnaddr;
				}
			}
			else if (input_string[0] == 'T'){		// if T, edit dump
				memcpy(tmp_addr, input_string+1, 6);
				T_start_addr = hextodec(tmp_addr, 6)+ref_code[1];
				memcpy(tmp_addr, input_string+7, 2);
				T_len = hextodec(tmp_addr, 2);

				for (k = 0 ; k < T_len ; k++){
					memcpy(tmp_addr, input_string+9 + k*2, 2);
					dump_list[T_start_addr+k] = hextodec(tmp_addr, 2);
				}
			}
			else if (input_string[0] == 'M'){		// if M, modify dump
				memcpy(tmp_addr, input_string+1, 6);
				T_start_addr = hextodec(tmp_addr, 6) + ref_code[1];
				memcpy(tmp_addr, input_string+7, 2);
				T_len = hextodec(tmp_addr, 2);
				memcpy(tmp_addr, input_string+10, 2);
				M_addr = ref_code[hextodec(tmp_addr, 2)];
				T_len = (T_len+1) / 2;
				if (ref_code[hextodec(tmp_addr, 2)] == -1 ){
					error_loader(fp[0], fp[1], fp[2], all_filenum, "error! there is wrong modify!\n");
					return -2;
				}

				tmp_dump = 0;

				for (k = 0 ; k < T_len ; k++)
					tmp_dump = (tmp_dump<<8) + dump_list[T_start_addr + k];

				if (input_string[9] == '+')
					tmp_dump = tmp_dump + (unsigned int)M_addr;
				else
					tmp_dump = tmp_dump - (unsigned int)M_addr;

				M_mod = 256;
				M_div = 1;
				for (k = 0 ; k < T_len ; k++){
					dump_list[T_start_addr + T_len-k-1] = (tmp_dump%M_mod)/M_div;
					M_mod *= 256;
					M_div *= 256;
				}

			}

		}
		if (input_string[1] >= '0' && input_string[1] <= '9'){
			memcpy(tmp_addr, input_string+1, 6);
			execsaddr = csaddr+ hextodec(tmp_addr,6);
		}
		csaddr += cslength;
		free(ref_code);
	}
	run_start_addr = execsaddr;
	printf("\t\tcontrol\t\tsymbol\t\taddress\t\tlength\n");
	printf("\t\tsection\t\tname\n");
	printf("\t\t---------------------------------------------------------\n");

	for (i = 0 ; i < all_filenum ; i++){
		estep = etable[i];
		printf("\t\t%s\t\t\t\t%04X\t\t%04X\n", estep->symname, estep->addr, estep->length);
		total_len += estep->length;
		estep = estep->next;
		l = 0;
		while(estep!= NULL && etable[i]->count > l){
			l++;
			printf("\t\t\t\t%s\t\t%04X\n", estep->symname, estep->addr);
			estep = estep->next;
		}
	}

	printf("\t\t---------------------------------------------------------\n");
	printf("\t\t\t\t\t\ttotal length\t%04x\n", total_len);
	
	prog_end_addr = etable[0]->addr + total_len;
	run_cur_addr = run_start_addr;
	stop_prog_addr = prog_end_addr;
	cur_bp = first_breakpoint;

	for (k = 0 ; k < 16 ; k++)
		reg[k] = 0;
	prog_run_flag = 0;

	//jump to location given by execaddr

	return 0;
}
int findetab(char *name, int filenum, int *forreturn){
	int i, l;
	ESTAB *step;

	for (i = 0 ; i < filenum ; i++){				//find etable
		step = etable[i];
		l = 0;
		while (step != NULL && l < etable[i]->count){
			if (!strcmp(step->symname, name)){
				*forreturn = step->addr;
				return -1;
			}
			if(step->next != NULL)
				step = step->next;
			else
				break;
		}
	}
	return 0;
}
void error_loader(FILE *fp1, FILE *fp2, FILE *fp3, int fpnum, char *errmsg){		// if loader error
	
	if (fpnum > 0){
		fclose(fp1);
		if (fpnum > 1){
			fclose(fp2);
			if (fpnum > 2)
				fclose(fp3);
		}
	}

	printf("%s\n", errmsg);
}
int hextodec(char*str, int maxnum){
	int l;
	int tmpnum = 0;

	for (l = 0 ; l < maxnum ; l++){
		if (str[l] >= '0' && str[l] <= '9')
			tmpnum = 16*tmpnum + str[l]-'0';
		else if (str[l] >= 'A' && str[l] <= 'F')
			tmpnum = 16*tmpnum + str[l]-55;
		else if (str[l] >= 'a' && str[l] <= 'f')
			tmpnum = 16*tmpnum + str[l] - 87;
		else{
			return -1;
		}
	}
	return tmpnum;
}
void cmd_run(){
	unsigned char tmp_opcode;
	int ni;						// for ni
	int i;						//for loop
	int cur_obj_code;			// cur code
	unsigned int address, value;
	unsigned char a,b;			//for divide objcode
	char opsym[10];				//for op format

	int formatnum;
	int bpflag = 0;
	int cc = -2;				// for current stat. sw

	if (prog_run_flag == 0){
		reg[L] = prog_end_addr;
		reg[PC] = run_start_addr;
		prog_run_flag = 1;
	}


	if (cur_bp != NULL){
		if (prog_end_addr > cur_bp->bpnum){
			stop_prog_addr = cur_bp->bpnum;
			cur_bp = cur_bp->next;
			bpflag = 1;
		}
		else
			stop_prog_addr = prog_end_addr;
	}
	else
		stop_prog_addr = prog_end_addr;
	
	while(reg[PC] < stop_prog_addr){				//loop when meet bp or end program addr
		a = dump_list[reg[PC]];
		tmp_opcode = (a & 0xFC);
		ni = (a & 0x03);
		find_opcode_symbol((unsigned int)tmp_opcode, opsym);
		if (opsym[0] == '1')
			formatnum = 1;
		else if (opsym[0] == '2')
			formatnum = 2;
		else if (opsym[0] == '3')
			formatnum = 3;
		
		if (formatnum == 3){
			b = dump_list[reg[PC]+1];
			if (((b%32)/16) == 1)
				formatnum = 4;
		}
		cur_obj_code = a;
		if (formatnum > 1){
			cur_obj_code = (cur_obj_code<<8) + dump_list[reg[PC]+1];
			if (formatnum > 2){
				cur_obj_code = (cur_obj_code<<8) + dump_list[reg[PC]+2];
				if (formatnum > 3){
					cur_obj_code = (cur_obj_code<<8) + dump_list[reg[PC]+3];
				}
			}
		}

		run_cur_addr += formatnum;
		reg[PC] += formatnum;

		//printf("%d %06X %02X %06X\n", formatnum, reg[PC], tmp_opcode, stop_prog_addr);

		if (a == 0 || a == 0xFC)
			continue;
		else if (tmp_opcode == 0x00){		//LDA
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
			reg[A] = value;
		}
		else if (tmp_opcode == 0x04){		//LDX	
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[X] = value;
		}
		else if (tmp_opcode == 0x74){		//LDT
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[T] = value;
		}
		else if (tmp_opcode == 0x18){		//ADD
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[A] = reg[A]+value;
		}
		else if (tmp_opcode == 0x58){		//ADDF
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0x90){		//ADDR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[value] = reg[value] + reg[address];
		}
		else if (tmp_opcode == 0x40){		//AND
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[A] = reg[A] & value;
		}
		else if (tmp_opcode == 0xB4){		//CLEAR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[address] = 0;
		}	
		else if (tmp_opcode == 0x28){		//COMP
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			if(reg[A] > value)
				cc = GREAT;
			else if (reg[A] == value)
				cc = EQUAL;
			else
				cc = LESS;
		}
		else if (tmp_opcode == 0x88){		//COMPF
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0xA0){		//COMPR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			if(reg[address] > reg[value])
				cc = GREAT;
			else if (reg[address] == reg[value])
				cc = EQUAL;
			else
				cc = LESS;
		}
		else if (tmp_opcode == 0x24){		//DIV
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[A] = reg[A] / value;
		}
		else if (tmp_opcode == 0x64){		//DIVF
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0x9C){		//DIVR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[value] = reg[value] / reg[address];
		}	
		else if (tmp_opcode == 0x3C){		//J
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[PC] = address;
		}
		else if (tmp_opcode == 0x30){		//JEQ
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
			if(cc == EQUAL)
				reg[PC] = address;
		}
		else if (tmp_opcode == 0x34){		//JGT
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			if(cc == GREAT)
				reg[PC] = address;
		}
		else if (tmp_opcode == 0x38){		//JLT
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			if (cc == LESS)
				reg[PC] = address;
		}	
		else if (tmp_opcode == 0x48){		//JSUB
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[L] = reg[PC];
			reg[PC] = address;
		}
		else if (tmp_opcode == 0x68){		//LDB
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[B] = value;
		}
		else if (tmp_opcode == 0x50){		//LDCH
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[A] = reg[A] - (reg[A]%0x100) + dump_list[address] ;
		}
		else if (tmp_opcode == 0x70){		//LDF
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0x08){		//LDL
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[L] = value;
		}	
		else if (tmp_opcode == 0x6C){		//LDS
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[S] = value;
		}	
		else if (tmp_opcode == 0xD0){		//LPS
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0x20){		//MUL
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[A] = reg[A] * value;
		}	
		else if (tmp_opcode == 0x60){		//MULF
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0x98){		//MULR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[value] *= reg[address];
		}	
		else if (tmp_opcode == 0x44){		//OR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[A] = reg[A] | value;
		}	
		else if (tmp_opcode == 0xD8){		//RD
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			
		}	
		else if (tmp_opcode == 0xAC){		//RMO
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[value] = reg[address];
		}	
		else if (tmp_opcode == 0x4C){		//RSUB
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[PC] = reg[L];
		}	
		else if (tmp_opcode == 0xA4){		//SHIFTL
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			for (i = 0 ; i < value ; i++){
				reg[address] = reg[address]<< 1;
				reg[address] = reg[address] - (reg[address]%2) + reg[address] %0x10000000 / 0x1000000 % 2;
			}
		}	
		else if (tmp_opcode == 0xA8){		//SHIFTR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			for (i = 0 ; i < value ; i++){
				reg[address] = reg[address] % 0x1000000 + ((reg[address] % 2) * 0x1000000);
				reg[address] = reg[address]>>1;
			}
		}	
		else if (tmp_opcode == 0xF0){		//SIO
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
		}	
		else if (tmp_opcode == 0xEC){		//SSK
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
				
		}	
		else if (tmp_opcode == 0x0C){		//STA
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			dump_list[address] = reg[A] / 0x10000;
			dump_list[address + 1] = (reg[A] % 0x10000) / 0x100;
			dump_list[address + 2] = (reg[A] % 0x100);
		}	
		else if (tmp_opcode == 0x78){		//STB
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			dump_list[address] = reg[B] / 0x10000;
			dump_list[address + 1] = (reg[B] % 0x10000) / 0x100;
			dump_list[address + 2] = (reg[B] % 0x100);	
		}	
		else if (tmp_opcode == 0x54){		//STCH
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			dump_list[address] = reg[A] % 0x100;
		}	
		else if (tmp_opcode == 0xD4){		//STI
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0x14){		//STL
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
			//printf("[%06X][%06X]", address, value);
			dump_list[address] = reg[L] / 0x10000;
			dump_list[address + 1] = (reg[L] % 0x10000) / 0x100;
			dump_list[address + 2] = (reg[L] % 0x100);	
		}	
		else if (tmp_opcode == 0x7C){		//STS
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			dump_list[address] = reg[S] / 0x10000;
			dump_list[address + 1] = (reg[S] % 0x10000) / 0x100;
			dump_list[address + 2] = (reg[S] % 0x100);	
		}	
		else if (tmp_opcode == 0xE8){		//STSW
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0x84){		//STT
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			dump_list[address] = reg[T] / 0x10000;
			dump_list[address + 1] = (reg[T] % 0x10000) / 0x100;
			dump_list[address + 2] = (reg[T] % 0x100);	
		}	
		else if (tmp_opcode == 0x10){		//STX
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			dump_list[address] = reg[X] / 0x10000;
			dump_list[address + 1] = (reg[X] % 0x10000) / 0x100;
			dump_list[address + 2] = (reg[X] % 0x100);		
		}	
		else if (tmp_opcode == 0x1C){		//SUB
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[A] -= value;
		}
		else if (tmp_opcode == 0x5C){		//SUBF
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}	
		else if (tmp_opcode == 0x94){		//SUBR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[value] -= reg[address];
		}
		else if (tmp_opcode == 0xB0){		//SVC
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);
		}
		else if (tmp_opcode == 0xE0){		//TD
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			cc = GREAT;
		}	
		else if (tmp_opcode == 0x2C){		//TIX
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[X] += 1;
			if (reg[X] > value)
				cc = GREAT;
			else if (reg[X] == value)
				cc = EQUAL;
			else
				cc = LESS;
		}	
		else if (tmp_opcode == 0xB8){		//TIXR
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	
			reg[X] += 1;
			if (reg[X] > reg[address])
				cc = GREAT;
			else if (reg[X] == reg[address])
				cc = EQUAL;
			else
				cc = LESS;
		}	
		else if (tmp_opcode == 0xDC){		//WD
			disassemble(cur_obj_code, tmp_opcode, formatnum, ni, &address, &value);	

		}
		else{
			reg[PC]++;
		}

	}					//print register
	printf("\t\t A : %06X\t X : %06X\n", reg[A], reg[X]);
	printf("\t\t L : %06X\t PC : %06X\n", reg[L], reg[PC]);
	printf("\t\t B : %06X\t S : %06X\n", reg[B], reg[S]);
	printf("\t\t T : %06X\n", reg[T]);

	if (bpflag == 0){
		printf("\t\tEnd program.\n");
		for (i = 0 ; i < 16 ; i++)
			reg[i] = 0;
		prog_run_flag = 0;
		run_cur_addr = program_address;
		reg[PC] = program_address;
		cur_bp = first_breakpoint;
	}
	else{
		printf("\t\tStop at checkpoint[%04X]\n", stop_prog_addr);
	}



}
void disassemble(int allcode, char objcode,int formatnum, int ni, unsigned int* first_para, unsigned int *second_para){
	unsigned int target_address = 0;
	int disp = 0, forni2 = 0;
	char xbpe;

	if (formatnum == 2){			// first para -> first register num, second para -> second register num
		disp = allcode % 0x100;
		*first_para = disp/0x10;
		*second_para = disp%0x10;
	}
	else if (formatnum == 3){			// first para -> address, second para -> value

		if (ni == 0){
			disp = allcode % 0x8000;
			if ((disp & 0x00000800) == 0x00000800)
				disp = disp | 0xFFFFF000;
			if ((allcode & 0x008000) == 0x008000)
				disp += reg[X];
			*first_para = disp;
			*second_para = dump_list[disp] * 0x10000;
			*second_para += dump_list[disp+1] * 0x100;
			*second_para += dump_list[disp+2];
		}
		else{
			xbpe = (allcode % 0x10000) / 0x1000;			// add x, b, p
			if ((xbpe&0x08) == 0x08)
				target_address += reg[X];
			if ((xbpe&0x04) == 0x04)
				target_address += reg[B];
			if ((xbpe&0x02) == 0x02)
				target_address += reg[PC];
			if ((xbpe&0x06) == 0x00)
				target_address += program_address;
			disp = allcode % 0x1000;
			if ((disp & 0x00000800) == 0x00000800)			// if disp is minus
				disp = disp | 0xFFFFF000;
	
			target_address += disp;
			if (ni == 3){									// n = 1, i = 1
				*first_para = target_address;
				*second_para = dump_list[target_address] * 0x10000;
				*second_para += dump_list[target_address+1] * 0x100;
				*second_para += dump_list[target_address+2];
			}
			else if (ni == 2){								// n = 1, i = 0
				forni2 = dump_list[target_address] * 0x10000;
				forni2 += dump_list[target_address+1] * 0x100;
				forni2 += dump_list[target_address+2];

				*first_para = forni2;

				*second_para = dump_list[forni2] * 0x10000;
				*second_para += dump_list[forni2+1] * 0x100;
				*second_para += dump_list[forni2+2];
			}
			else if (ni == 1){								// n = 0 , i = 1
				*first_para = target_address;
				*second_para = target_address;
			}
		}

	}
	else if (formatnum == 4){			//first para -> address, second para -> value
		target_address = 0;
		if ((allcode & 0x00800000) == 0x00800000)			//add x
			target_address += reg[X];

		target_address += (allcode % 0x100000) + program_address;
		if (ni == 3){									// n = 1, i = 1
			*first_para = target_address;
			*second_para = dump_list[target_address] * 0x1000000;
			*second_para += dump_list[target_address+1] * 0x10000;
			*second_para += dump_list[target_address+2] * 0x100;
			*second_para += dump_list[target_address+3];
		}
		else if (ni == 2){								// n = 1, i = 0
			forni2 = dump_list[target_address] * 0x1000000;
			forni2 += dump_list[target_address+1] * 0x10000;
			forni2 += dump_list[target_address+2] * 0x100;
			forni2 += dump_list[target_address+3];

			*first_para = forni2;

			*second_para = dump_list[forni2] * 0x1000000;
			*second_para += dump_list[forni2+1] * 0x10000;
			*second_para += dump_list[forni2+2] * 0x100;
			*second_para += dump_list[forni2+3];
		}
		else if (ni == 1){								// n = 0, i = 1
			*first_para = target_address;
			*second_para = target_address;
		}
	}
}
