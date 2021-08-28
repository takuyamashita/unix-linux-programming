#include <ctype.h>

int strtovec(char *s, char **v, int max){
    int i, skip;

    if(max < 1 || v == 0)
	skip = 1;
    else
	skip = 0;

    i = 0;
    for(;;){
	if(!skip && i >= max -1){
	    v[i] = 0;
	    skip = 1;
	}

	while(*s != '\0' && isspace(*s)) s++;

	if(*s == '\0') break;

	if(!skip) v[i] = s;
	i++;

	while(*s != '\0' && !isspace(*s)) s++;
	if(*s == '\0') break;

	*s = '\0';
	s++;
    }

    if(!skip)
	v[i] = 0;
    i++;

    return i;
}
