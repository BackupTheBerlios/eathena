#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strlib.h"

//-----------------------------------------------
// string lib.
// I change this function because of the bad memory management of it
// It returns a new alloced string, you have to free it when not used anymore
// Devil.
unsigned char* jstrescape (unsigned char* pt) {
	unsigned char * ptr;
	int i, j, cnt;

	for (i=0,cnt=0; pt[i]; i++)
		if (pt[i] == '\'')
			cnt++;
	ptr = malloc(strlen(pt) + cnt + 1);
	for (i=0,j=0; pt[i]; i++) {
		switch (pt[i]) {
			case '\'':
				ptr[j++] = '\\';
				ptr[j++] = pt[i];
				break;
			case '\\':
				ptr[j++] = '\\';
				ptr[j++] = pt[i];
				break;
			default:
				ptr[j++] = pt[i];
		}
	}
	ptr[j++] = '\0';
	free(pt);
	return (ptr);
}

unsigned char* jstrescapecpy (unsigned char* pt,unsigned char* spt) {
	//copy from here
	int i =0, j=0;
	
	while (spt[i] != '\0') {
		switch (spt[i]) {
			case '\'':
				pt[j++] = '\\';
				pt[j++] = spt[i++];
				break;
			case '\\':
				pt[j++] = '\\';
				pt[j++] = spt[i++];
				break;
			default:
				pt[j++] = spt[i++];
		}
	}
	pt[j++] = '\0';
	return (unsigned char*) &pt[0];
}
int jmemescapecpy (unsigned char* pt,unsigned char* spt, int size) {
	//copy from here
	int i =0, j=0;
	
	while (i < size) {
		switch (spt[i]) {
			case '\'':
				pt[j++] = '\\';
				pt[j++] = spt[i++];
				break;
			case '\\':
				pt[j++] = '\\';
				pt[j++] = spt[i++];
				break;
			default:
				pt[j++] = spt[i++];
		}
	}
	// copy size is 0 ~ (j-1)
	return j;
}
