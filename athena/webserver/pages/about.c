void generate_about(int sock_in, char *query, char *ip)
{
	web_send(sock_in, "<title>About</title>\n");
	web_send(sock_in, "<center>eAthena Web Server!</center>\n");	
}
