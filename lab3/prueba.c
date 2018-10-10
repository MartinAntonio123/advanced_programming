#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
/**************
 * Martin Antonio Vivanco Palacios A01701167
 * Lab 3 programacion avanzada
 * ls recursive
 **************/
int copiar(char name, char *origin, char *destination, char program) {

	char filenameO[PATH_MAX + NAME_MAX + 1];
	sprintf(filenameO, "%s/%s", origin, name);
	char filenameD[PATH_MAX + NAME_MAX + 1];
	sprintf(filenameD, "%s/%s", destination, name);
	printf("Origen: %s \n", filenameO);
	printf("Destino: %s \n\n", filenameD);
	int fd_in, fd_out;

	if ((fd_in = open(filenameO, O_RDONLY)) < 0){ // abrir primer archivo modo lectura
		perror(program);
		return -1;
	}
	if ((fd_out = open(filenameD, O_WRONLY | O_TRUNC | O_CREAT, 0666)) < 0){ // abrir segundo archivo modo escritura
		perror(program);
		return -1;
	}
	char buffer[128]; // crear buffer de 128 bits
	ssize_t nbytes;
	while((nbytes = read(fd_in, buffer, sizeof(buffer))) != 0){
		write(fd_out, buffer, nbytes);
	}
	close(fd_in);
	close(fd_out);
	return 0;
}

void list(char *origin, char *destination, unsigned char recursivo, char *program){
	DIR* dir;
	struct dirent* direntry;
	struct stat info;
	char filename[PATH_MAX + NAME_MAX + 1];

	if ( (dir = opendir(origin)) == NULL ) {
		perror(program);
		exit(-1);
	}
	while ( (direntry = readdir(dir)) != NULL ) {
		if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {
			sprintf(filename, "%s/%s", origin, direntry->d_name);
			copiar(direntry->d_name, origin, destination, program);
		}
	}

	if (recursivo) {
		rewinddir(dir);
		while ( (direntry = readdir(dir)) != NULL ) {
			if (strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {

				sprintf(filename, "%s/%s", origin, direntry->d_name);
				lstat(filename, &info);
				if (S_ISDIR(info.st_mode)) {
					copiar(direntry->d_name, origin, destination, program);
				}
			}
		}
	}
	printf("\n");
}

int main(int argc, char* argv[]){

	if ((argc != 4) && (argc != 3))
	{
		printf("usage: copy origin destination [-r]\n");
		return -1;
	}
	unsigned char recursivo = 0;
	if (argc == 4)
	{
		if (strcmp(argv[3], "-r") != 0)
		{
			printf("usage: copy origin destination [-r]\n");
			return -1;
		}
		recursivo = 1;
	}
	char *origin, *destination;
	destination = argv[2];
 	origin = argv[1];
	list(origin, destination, recursivo, argv[0]);
	return 0;

}
