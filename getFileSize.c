#include <sys/stat.h>


int main()
{
	struct stat st;
	char filename[50];

	strcpy(filename,"FilterData.csv");

	stat(filename, &st);
	int size = st.st_size;
	printf("%s\n",filename);
	printf("%d\n",size);

	return 0;
}
