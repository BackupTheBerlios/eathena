#include <stdlib.h>

char filtered_query[2000];

char *get_query(char *inquery)
{
	memset(filtered_query, 0x0, 2000);
	sscanf(inquery, "GET %s %[$]", filtered_query);
	return(filtered_query);
}
