void generate_sample(int sock_in, char *query, char *ip)
{
	web_send(sock_in, "<title>SAMPLE</title>\n");
	web_send(sock_in, "<center>THIS IS A SAMPLE PAGE!</center>\n");	
}
