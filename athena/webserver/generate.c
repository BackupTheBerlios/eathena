
void generate_page(int sock_in, char *query, char *ip)
{
	char *page = get_param(query, 0);


	//To make this simple, we will have a bunch of if statements
	//that then shoot out data off into functions.

	
	//The 'index'
	if ( strcmp(page, "/") == 0 )
		generate_notdone(sock_in, query, ip);


	//About page:
	if ( strcmp(page, "/about.html") == 0 )
		generate_about(sock_in, query, ip);

}
