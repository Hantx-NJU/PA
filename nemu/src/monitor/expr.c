#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/instr.h"
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
	HEX = 261,
	NEG = 262,
	DER = 263

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
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"\\$e[a,b,c,d]x", REG},
	{"\\$e[s,b]p", REG},
	{"\\$e[s,d]i", REG},
	{"[a-zA-Z_][a-zA-Z0-9_]+", SYMB},
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
				//int len =substr_len > 31 ? 31 : substr_len;
				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;
				if(substr_len>=32){
					printf("substr is too long at position %d\n%s\n%*.s^\n", position, e, position, "");
					return false;
				}
				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				//if (rules[i].token_type != NOTYPE) {
					//for (int i = 0; i < substr_len; i++)
					
				//}
				switch (rules[i].token_type)
				{
					case NOTYPE:break;
					case NUM:
					case HEX:
					case SYMB:
					case REG:
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len] = '\0';
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

bool check_parentheses(int s, int e)
{
	int count = 0;
	
	if(tokens[s].type!='(')
		return false;

	for(int i = s;i<e;++i)
	{
		if(tokens[i].type=='(')
			++count;
		else if(tokens[i].type == ')')
		{
			--count;
		}
		if(count == 0)	return false;
		if(count < 0)	{
			printf("1:parentheses error!");
			assert(0);
		}
	}
	if(count!=1||tokens[e].type!=')'){
		printf("2:parentheses error!");
			assert(0);
	}
	else{
		return true;
	}
}
extern uint32_t look_up_symtab(char * sym, bool * success);

uint32_t eval(int s, int e) 
{
	//printf("s=%d,e=%d\n",s,e);
	if (s > e) {
		printf("eval error!");
		assert(0);
	}
	
	else if(s == e) { 
		//printf("atoi=%d\n",atoi(tokens[s].str));
		if (tokens[s].type == NUM)
		{
			uint32_t num;
			sscanf(tokens[s].str,"%d",&num);
			return num;
		}
		else if (tokens[s].type == REG) {
			printf("str=%s\n",tokens[s].str);
			if ((strcmp(tokens[s].str, "$eax") == 0))
					return cpu.eax;

			else if (strcmp(tokens[s].str, "edx") == 0)
				return cpu.edx;
			else if (strcmp(tokens[s].str, "ecx") == 0)
				return cpu.ecx;
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

		}
		else if (tokens[s].type == HEX) {
			uint32_t i;
			sscanf(tokens[s].str, "%x", &i);
			return i;
		}
		else if (tokens[s].type == SYMB) {
			bool success= true;
			//printf("4");
			return look_up_symtab(tokens[s].str, &success);
		}
		
	}
	else if(check_parentheses(s, e) == true) {
	return eval(s + 1, e - 1); 
	}
else {
int pm = -1, mul = -1, c = 0, i;
		for (i =s; i < e; i++) {
			if (tokens[i].type == '(')
				c++;
			else if (tokens[i].type == ')')
				c--;
			if (c == 0) {
				//printf("-=%d,now=%d",'-',tokens[i].type);
				if (tokens[i].type == '+' || tokens[i].type == '-')
					pm = i;
				else if (tokens[i].type == '*' || tokens[i].type == '/')
					mul = i;
			}
		}
//printf("pm=%d,mul=%d",pm,mul);
		if (pm != -1 || mul != -1) {
			int op;
			
			if (pm != -1)
				op = pm;
			else
				op = mul;
			//printf("op.pos=%d\n",op);
			int val1 = eval(s, op - 1);
			int val2 = eval(op + 1, e);
			//printf("val1=%d,val2=%d\n",val1,val2);
			switch(tokens[op].type) {
				case '+': return val1 + val2;
				case '-': /*printf("return here%d\n",val1-val2);*/return val1 - val2;
				case '*': return val1 * val2;
				case '/': return val1 / val2;
				default: assert(0);
			}
		} 
		else {
			if (tokens[s].type == NEG)
				return (-eval(s + 1, e));
			else if (tokens[s].type == DER)
				return vaddr_read(eval(s + 1, e), SREG_DS, 4);
			else
				{
					printf("type=%d,str=%s\n",tokens[s].type,tokens[s].str);
					assert(0);

				}
		}
	}
	
	return 0;
}

uint32_t expr(char *e, bool *success)
{
	*success = true;
	//printf("1");
	if (!make_token(e))
	{
		printf("3");
		*success = false;
		return 0;
	}
//printf("2");
	for (int i = 0; i< nr_token; ++i) {
		if (tokens[i].type == '-') {
			if (i == 0)
				tokens[i].type = NEG;
			else if ((tokens[i-1].type != ')') && (tokens[i-1].type != NUM ) && (tokens[i-1].type != HEX))
				tokens[i].type = NEG;
		}
		else if (tokens[i].type == '*') {
			if (i == 0)
				tokens[i].type = DER;
			else if ((tokens[i-1].type != ')') && (tokens[i-1].type != NUM) && (tokens[i-1].type != HEX))
				tokens[i].type = DER;
		}
	}

	return eval(0, nr_token-1);
	//printf("\nPlease implement expr at expr.c\n");
	//assert(0);

}
