////////////////////////////////////
//Server code
///////////////////////////////////
//This file is the code for the server
#include <netinet/in.h>  // for sockaddr_in
#include <sys/types.h>  // for socket
#include <sys/socket.h>  // for socket
#include <stdio.h>    // for printf
#include <stdlib.h>    // for exit
#include <string.h>    // for bzero
#include <time.h>        //for time_t and time
#include <sys/stat.h>	// for get the file size
#include <math.h> // for calculating 
#define HELLO_WORLD_SERVER_PORT 7754
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024

// csv 파일을 읽기 위한 구조체 선언 
typedef struct LogData
{
	char Job_ID[50];
	char Job_Status[50];
	char Error_Code[50];
	char Job_Type[50];
	char Backup_Method[50];
	char Client[50];
	char Job[50];
	char Schedule[50];
	char Files[50];
	char Files_Size[50];
	char Write_Size[50];
	char Data_Transferred[50];
	char Throughput[50];
	char Start_Time[50];
	char End_Time[50];
	char Elapsed_Time[50];
	char Server[50];
	char Volume_Pool[50];
	char Storage[50];
	char Tape_Drive[50];
	char Volume[50];
} LogData;
LogData array[991];

// 필터링 UI, 필터링에 사용될 데이터 구조체 선언 
typedef struct Filtering_Data
{
	char Job_Status[50];
	char Job_Type[50];
	char Server[50];
	char Client[50];
	char Schedule[50];
	char Files[50];
} Filtering_Data;
Filtering_Data Filtering_Datas[991];


int main(int argc, char **argv)
{
	FILE *the_file = fopen("/home/ubuntu/Workspace/LogData.csv","r");
	if (the_file == NULL)
	{
		perror("Unable to open the file.");
		exit(1);
	}


	int row_count =0;
	int Total_row_count=0;
	int field_count=0;
	char line[300];
	char Temp[300];
	LogData array[991];

	int i=0;
	
	// File_Size 와 Write_Size 위한 변수 
	int t_f_c =0;
	int t_w_c =0;
	int t_d_c =0;

	while (fgets(line, sizeof(line), the_file))
	{
		field_count = 0;
		row_count++;
		if(row_count==1)
			continue;
		char *field = strtok(line, ",");

		while (field)
		{
			if(field_count==0)
				strcpy(array[i].Job_ID, field);
			if(field_count==1)
				strcpy(array[i].Job_Status, field);
			if(field_count==2)
				strcpy(array[i].Error_Code, field);
			if(field_count==3)
				strcpy(array[i].Job_Type, field);
			if(field_count==4)
				strcpy(array[i].Backup_Method, field);
			if(field_count==5)
				strcpy(array[i].Client, field);
			if(field_count==6)
				strcpy(array[i].Job, field);
			if(field_count==7)
				strcpy(array[i].Schedule, field);
			if(field_count==8)
				strcpy(array[i].Files, field);
			if(field_count==9)
				strcpy(array[i].Files_Size, field);
			if(field_count==10)
				strcpy(array[i].Write_Size, field);
			if(field_count==11)
				strcpy(array[i].Data_Transferred, field);
			if(field_count==12)
				strcpy(array[i].Throughput, field);
			if(field_count==13)
				strcpy(array[i].Start_Time, field);
			if(field_count==14)
					strncpy(array[i].End_Time, field,10);
					array[i].End_Time[10] = '\0';
			if(field_count==15)
				strcpy(array[i].Elapsed_Time, field);
			if(field_count==16)
				strcpy(array[i].Server, field);
			if(field_count==17)
				strcpy(array[i].Volume_Pool, field);
			if(field_count==18)
				strcpy(array[i].Storage, field);
			if(field_count==19)
				strcpy(array[i].Tape_Drive, field);
			if(field_count==20)
				strcpy(array[i].Volume, field);
				
			field = strtok(NULL,",");
			field_count++;
		}
		i++;
	}
	fclose(the_file);

	File *fpt;
	fpt = fopen("MyFile.csv","w+");
	
	// Filtering Data에 할당하기 
	for(int i=0;i<Total_row_count;i++)
	{
		char Filter_Job_Type[300];
		printf("Filtering start\n");
		// Vmware Backup으로만 구성 된 필터링 데이터 구조체 할당 

		strcpy(Filter_Job_Type,array[i].Job_Type); // 작업 종류 통계 데이터 가공을 위한 변수 값 복사 (Statistics UI)
		printf("Filter_Job_Type = %s\n",Filter_Job_Type);
		if (strcmp(Filter_Job_Type,"Vmware Backup")==0)
		{
			printf("vmwarebackup hello\n");
			strcpy(Filtering_Datas[VmWare_array_count].Job_Status,array[i].Job_Status);
			strcpy(Filtering_Datas[VmWare_array_count].Job_Type,array[i].Job_Type);
			strcpy(Filtering_Datas[VmWare_array_count].Server,array[i].Server);
			strcpy(Filtering_Datas[VmWare_array_count].Client,array[i].Client);
			strcpy(Filtering_Datas[VmWare_array_count].Schedule,array[i].Schedule);
			strcpy(Filtering_Datas[VmWare_array_count].Files,array[i].Files);
			VmWare_array_count++;
			printf("VmWare_array_count = %d\n",VmWare_array_count);
		}
	}
	for (int i=0; i<VmWare_array_count,i++)
	{
		//
		fprintf(fpt,"%s,%s,%s,%s,%s,%s\n", Filtering_Datas[i].Job_Status,Filtering_Datas[i].Job_Type,
		Filtering_Datas[i].Server,Filtering_Datas[i].Client,
		Filtering_Datas[i].Schedule,Filtering_Datas[i].Files);
	}
	fclose(fpt);
	return 0;
}
