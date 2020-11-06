
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

typedef struct
{
	byte *cs;
	byte *ds;
	word cs_base;
	word ds_base;
	dword cs_len;
	dword ds_len;
	dword cs_max;
	dword ds_max;
} cmd_desc;

void zero_cmd (cmd_desc *cmdfile);
void free_cmd (cmd_desc *cmdfile);
char *alloc_cmd(cmd_desc *cmdfile, long cs_len, long ds_len);

char *load_cmd(char *filename, cmd_desc *cmdfile);
char *save_cmd(char *filename, cmd_desc *cmdfile, int absolute);

word peekw(byte *base, word a);
void pokew(byte *base, word a, word v);

