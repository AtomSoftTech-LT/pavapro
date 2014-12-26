#define OK 1
#define P_ERR 2
#define B_ERR 3
#define RD_ERR 4
#define K7_ERR 5
#define K12_ERR 6
#define RR_ERR 7
#define K8_ERR 8
#define S_ERR 9
#define K_ERR 9

char get_token (char * source, char *dest, int actual);
char is_delimiter(unsigned char chr);
char xtoi_j(const char* xs, unsigned int* result);
unsigned char convert_to_8_number (char * string);
unsigned int convert_to_16_number (char * string);

void opcode_table (void);

char opcode_p_b (char *str, char actual, unsigned int skeleton, unsigned int * opcode);
char opcode_Rd (char *str, char actual, unsigned int skeleton, unsigned int * opcode);
char opcode_k7 (char *str, char actual, unsigned int skeleton, unsigned int * opcode);
char opcode_k12 (char *str, char actual, unsigned int skeleton, unsigned int * opcode);
char opcode_Rd_Rr (char *str, char actual, unsigned int skeleton, unsigned int * opcode);
char opcode_Rd_k8 (char *str, char actual, unsigned int skeleton, unsigned int * opcode, unsigned char cpl);
char opcode_s (char *str, char actual, unsigned int skeleton, unsigned int * opcode);
char opcode_out (char *str, char actual, unsigned int * opcode);
char opcode_in (char *str, char actual, unsigned int * opcode);
char popcode_org (char *str, char actual);

int op_sbi (char * line, char pos);
int op_cbi (char * line, char pos);
int op_sbic (char * line, char pos);
int op_sbis (char * line, char pos);

