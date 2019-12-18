#include "20141602.h"

int main()
{
	char input[300];
	char forcommand[20];
	char formnemonic[10];
	int dump_start, dump_end, dump_flag, fail_flag;
	int edit_address, edit_value, edit_flag;
	int fill_start, fill_end, fill_value,  fill_flag;
	int opcode_number;
	int i, j, k, l, now_input_num, assemble_flag;
	FILE *type_file;
	char type_filename_save[50];
	char hash_type[6];

	init();						// initalize.
	while(1)
	{
		for (i = 0 ; i < 300 ; i++)
			input[i] = '\0';

		for(i = 0 ; i < 10 ; i++)
			formnemonic[i] = 0;

		printf("sicsim> ");
		fgets(input, 260, stdin);		//input string. string length is not over 256
		
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
		
		while(input[now_input_num] == ' ' || input[now_input_num] == '\t')		// delete void space
			now_input_num++;

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
