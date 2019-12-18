#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_DUMP_LIST_NUM 1048677
#define MAX_OPCODE_NAME_LENGTH 10
#define MAX_HASH_TABLE_NUM 20
#define MAX_OBJLINE_LENGTH 0x1D


// for register number
#define A 0
#define X 1
#define L 2
#define B 3
#define S 4
#define T 5
#define PC 8

// for sw
#define GREAT 1
#define LESS -1
#define EQUAL 0


typedef struct historylink
{
	int num;
	char s[200];
	struct historylink *next;
}history_link;				//save history

typedef struct opcodelist
{
	int opnum;
	char opname[MAX_OPCODE_NAME_LENGTH];
	char symbol[10];
	struct opcodelist *next;
}opcode_list;				//save opcode 

history_link *start, *end;		//for history link
opcode_list *opcode[20];		//fpr opcode hash table

int dump_lasthistory = 0;
int count = 1;

unsigned char dump_list[MAX_DUMP_LIST_NUM-1];	// dump. 1mb


typedef struct _symtab
{
	char symbol[15];
	int address;
	struct _symtab *next;
}symtab;

symtab *symbol_table;

typedef struct _modi
{
	   int address;
	   int hbyte;
	   struct _modi *next;
}modi;

modi *modified, *modilast;



int program_address = 0;
int CSADDR;

typedef struct _bp{
	int bpnum;
	struct _bp *next;
}breakpoint;

breakpoint *first_breakpoint, *cur_bp;

typedef struct _estab{
	char symname[10];
	int addr;
	int length;
	int count;
	struct _estab *next;
}ESTAB;

ESTAB *etable[3];

int run_start_addr;				// start program address	
int run_cur_addr;				// now program address
int prog_end_addr;				// end program address
int stop_prog_addr;				// stop program address(breakpoint)

unsigned int reg[16];			//register
int prog_run_flag;

// function for command

void init();			//for initalize
void help();			//for help command
void dir();			//for dir command
void history();			//for history command
void dump(int, int);		//for dump command
void edit();			//for edit command
void fill(int, int, int);	//for fill command
void quit(); 			//for quit command
void addhistory(char*);		//if typed command is valuable, add history's linked list
void opcodelist();		//for opcodelist command
int add_hashnum(char*);		//for hash table, allocate opcode's hash table number
int find_hashnum(char*, char*);	//for hash table, find where opcode's number is
void find_opcode_symbol(int, char*);

void type_filename(FILE*);	// for type command
int assemble_file(char*);	// for assemble command, pass 1 and pass 2
void assemble_error(FILE*, FILE*, FILE*, int, char*, int, char*);	// for assemble error. print error line and error message
int add_symbol_table(char*, int);	// add symbol table
int find_symbol_table(char*);		// find symbol table and return address
void free_symbol_table();			// free symbol table for assemble another file
void symbol_display();				// symbol display for symbol command


int cmd_bp(int);					// return -1 when already has breakpoint there
void cmd_bpclear();					// for bpclear
void cmd_bpprint();					// for bpprint
int cmd_loader(char *, char *, char *);	//for loader
int hextodec(char*, int);				// for change hex to dec
int findetab(char*, int, int*);			// if find same etab symbol, return -1 and addr
void error_loader(FILE*, FILE*, FILE*, int, char *);		//for loading error
void cmd_run();												// for run command
void disassemble(int, char, int, int, unsigned int*, unsigned int*);	//for run, disassemble
