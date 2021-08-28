#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    DATELEN = 10,
};

int main(int argc, char *argv[]){
    int year, month, day;
    double total;
    char date[DATELEN + 1];
    FILE *fp;
    char line[1024];

    if(argc != 2){
	fprintf(stderr, "usage: %s <yyyy/mm/dd>\n", argv[0]);
    }

    if(sscanf(argv[1], "%d/%d/%d", &year, &month, &day) != 3){
	fprintf(stderr, "Please specify date as year/month/day\n");
    }

    if(year < 100){
	year += 2000;
    }
    snprintf(date, sizeof(date), "%04d/%02d/%02d", year, month, day);

    if((fp = fopen("acctbook.txt", "r")) == NULL){
	perror("fopen");
	exit(1);
    }

    total = 0.0;
    while(fgets(line, sizeof(line), fp) != NULL){
	double fee;
	if(strncmp(line, date, DATELEN) == 0){
	    sscanf(line, "%*s %lf", &fee);
	    total += fee;
	}
    }

    if(ferror(fp)){
	perror("fgets");
	exit(1);
    }

    printf("%s %.2f\n", date, total);
    fclose(fp);

    return 0;
}
