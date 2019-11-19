#include "nemu.h"
#include "cpu/reg.h"
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
	HEX = 261

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
	{"-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"[$][a-zA-Z]{2, 3}", REG},
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
				printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;
				if(substr_len>=32){
					printf("substr is too long at position %d\n%s\n%*.s^\n", position, e, position, "");
					return false;
				}
				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
				if (rules[i].token_type != NOTYPE) {
					//for (int i = 0; i < substr_len; i++)
					strncpy(tokens[nr_token].str,substr_start,substr_len);
					tokens[nr_token].str[substr_len] = '\0';
				}
				switch (rules[i].token_type)
				{
					case NOTYPE:break;
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
	bool flag = true;

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
			printf("parentheses error!");
			assert(0);
		}
	}

}

uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}

	printf("\nPlease implement expr at expr.c\n");
	assert(0);

	return 0;
}
