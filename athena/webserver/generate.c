
void generate_page(int sock_in, char *query, char *ip)
{
	char *page = get_param(query, 0);


	//To make this simple, we will have a bunch of if statements
	//that then shoot out data off into functions.

	//About page:
	if ( strcmp(page, "/about.html") == 0 )
		generate_about(sock_in, query, ip);

}
