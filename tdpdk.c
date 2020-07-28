
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* split string into tokens */
int
rte_strsplit(char *string, int stringlen,
	     char **tokens, int maxtokens, char delim)
{
	int i, tok = 0;
	int tokstart = 1; /* first token is right at start of string */

	if (string == NULL || tokens == NULL)
		goto einval_error;

	for (i = 0; i < stringlen; i++) {
		if (string[i] == '\0' || tok >= maxtokens)
			break;
		if (tokstart) {
			tokstart = 0;
			tokens[tok++] = &string[i];
		}
		if (string[i] == delim) {
			string[i] = '\0';
			tokstart = 1;
		}
	}
	return tok;

einval_error:
	return -1;
}


eal_parse_socket_mem(char *socket_mem)
{
	char * arg[10];
	char *end;
	int arg_num, i, len;
	uint64_t total_mem = 0;

	len = strnlen(socket_mem, 100);
	if (len == 100) {
		printf("--socket-mem is too long\n");
		return -1;
	}

	/* all other error cases will be caught later */
	if (!isdigit(socket_mem[len-1])){
		printf("is not digit\n");
		return -1;
	}

	/* split the optarg into separate socket values */
	arg_num = rte_strsplit(socket_mem, len,
			arg, 10, ',');

	/* if split failed, or 0 arguments */
	if (arg_num <= 0){
		printf("argv_num < 0\n");
		return -1;}


	/* parse each defined socket option */
	for (i = 0; i < arg_num; i++) {
		end = NULL;
		int tmp_value = strtoull(arg[i], &end, 10);
printf("av:%d tmp_value:%d\n",arg[i][0],tmp_value);
		/* check for invalid input */
		if ((arg[i][0] == '\0') || (end == NULL) || (*end != '\0')){
				printf("xxxx\n");
				return -1;
			}
	}

	return 0;
}



int main(int argc, char *argv[])
{
	eal_parse_socket_mem(argv[1]);
}
