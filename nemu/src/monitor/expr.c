#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum
{
	NOTYPE = 256,
	EQ = 257,
	NUM = 258,
	REG = 259,
	SYMB = 260,
	NEG = 261,
	DER = 262,
	HEX = 263

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"\\+", '+'},
	{"==", EQ},
	{"[0-9]+", NUM},
	{"-",'-'},
	{"-",NEG},
	{"\\*",'*'},
	{"\\*",DER},
	{"\\(",'('},
	{"\\)",')'},
	{"[$][a-zA-Z]{2,3}",REG},
	{"[a-zA-Z_][0-9a-zA-Z_]+", SYMB},
	{"[0][xX][0-9a-fA-F]+", HEX}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

void init_token()
{
	for(int i=0;i<32;++i)
	{
		tokens[i].str[0]='\0';
		tokens[i].type=NOTYPE;
	}
}
static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				if(rules[i].token_type!=NOTYPE)
				{
					for(int i=0;i<substr_len;++i)
					{
						tokens[nr_token].str[i]=*(position+e+i);
						tokens[nr_token].str[substr_len]='\0';
					}
				}

				printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type)
				{
				default:
					tokens[nr_token].type = rules[i].token_type;
					nr_token++;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

extern uint32_t look_up_symtab(char * sym, bool * success);

static bool  check_parentheses(int e,int s,bool *success)
{
	int c = 0;
	bool flag = true;
	for (int i = s; i < e; i++) {
		if (tokens[i].type == '(')
			c++;
		else if (tokens[i].type == ')') {
			c--;
			flag = false;
		}
		if (c < 0)
			*success = false;
	}
	if (tokens[e].type == ')')
		c--;
	if (c != 0)
		*success = false;
	return flag;
}

uint32_t eval(int e,int s, bool *success)
{
	if (s > e) {
		*success = false;
		return 0;
	}
	else if (s == e) {
		if (tokens[s].type == NUM)
			return atoi(tokens[s].str);
		else if (tokens[s].type == REG) {
			if ((strcmp(tokens[s].str, "$eax") == 0) || (strcmp(tokens[s].str, "$EAX") == 0))
					return cpu.eax;
			else if (strcmp(tokens[s].str, "ecx") == 0)
				return cpu.ecx;
			else if (strcmp(tokens[s].str, "edx") == 0)
				return cpu.edx;
			else if (strcmp(tokens[s].str, "ebx") == 0)
				return cpu.ebx;
			else if (strcmp(tokens[s].str, "esp") == 0)
				return cpu.esp;
			else if (strcmp(tokens[s].str, "ebp") == 0)
				return cpu.ebp;
			else if (strcmp(tokens[s].str, "esi") == 0)
				return cpu.esi;
			else if (strcmp(tokens[s].str, "edi") == 0)
				return cpu.edi;
			else
				*success = false;
		}
		else if (tokens[s].type == SYMB) {
			return look_up_symtab(tokens[s].str, success);
		}
		else if (tokens[s].type == HEX) {
			uint32_t i;
			sscanf(tokens[s].str, "%x", &i);
			return i;
		}
	}
	else if (check_parentheses(e, s, success) == true)
		return eval(e - 1 ,s + 1, success);
	else {
		int pm = -1, mul = -1, c = 0, i;
		for (i = s; i < e; i++) {
			if (tokens[i].type == '(')
				c++;
			else if (tokens[i].type == ')')
				c--;
			if (c == 0) {
				if (tokens[i].type == '+' || tokens[i].type == '-')
					pm = i;
				else if (tokens[i].type == '*')
					mul = i;
			}
		}
		if (pm != -1 || mul != -1) {
			int op;
			if (pm != -1)
				op = pm;
			else
				op = mul;
			int val1 = eval(op - 1, s, success);
			int val2 = eval(e, op + 1,  success);
			switch(tokens[op].type) {
				case '+': return val1 + val2;
				case '-': return val1 - val2;
				case '*': return val1 * val2;
				default: assert(0);
			}
		}
		else {
			if (tokens[s].type == NEG)
				return (-eval(e, s + 1, success));
			else if (tokens[s].type == DER)
				return vaddr_read(eval(s + 1, e, success), SREG_DS, 4);
			else
				assert(0);
		}
	}
	return 0;
}

uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}

	for(int i=0;i<32;++i)
	{
		if (tokens[i].type == '*') {
			if (i == 0)
				tokens[i].type = DER;
			else if ((tokens[i].type != ')') || (tokens[i].type != NUM) || (tokens[i].type != HEX))
				tokens[i].type = DER;
		}
		else if (tokens[i].type == '-') {
			if (i == 0)
				tokens[i].type = NEG;
			else if ((tokens[i-1].type != ')') || (tokens[i].type != NUM ) || (tokens[i].type != HEX))
				tokens[i].type = NEG;
		}
	}

		return eval(0, nr_token, success);
}
