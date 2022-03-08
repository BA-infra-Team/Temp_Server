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

// 필터링 UI, 필터링에 사용될 데이터 구조체 선언 
typedef struct Filtering_Data
{
	int Total_Count;
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
} Filtering_Data;

// 홈 UI, 백업-메소드 비율을 보여주기 위한 데이터 구조체 선언 
typedef struct Backup_Method_Ratio_Pie_Chart
{
	int Total_Count;
	int Archive_Backup_Count;
	int Differential_Backup_Count;
	int Dump_Backup_Count;
	int Full_Backup_Count;
	int Incremental_Backup_Count;
	int Synthetic_Count;
} Backup_Method_Ratio_Pie_Chart;

// 홈 UI, 일별 총 백업 수를 보여주 데이터 구조체 선언
typedef struct Total_Backup_Count_LineChart
{
	// int Job_Status_Completed_Count;	// Total, Do not Need
	int 2022_02_08_Completed_Count;
	int 2022_02_09_Completed_Count;
	int 2022_02_10_Completed_Count;
	int 2022_02_11_Completed_Count;
	int 2022_02_12_Completed_Count;
	int 2022_02_13_Completed_Count;
	int 2022_02_14_Completed_Count;
	int 2022_02_15_Completed_Count;
} Total_Backup_Count_LineChart;

// 파일통계화면 UI, 파일 관련 통계를 보여줄 데이터 구조체 선언 
typedef struct File_Statistics_PieChart
{
	int Total_Size;
	int Total_File_Size;
	int Total_Write_Size;
} File_Statistics_PieChart;

// 파일통계화면 UI, 평균 경과시간을 보여주는 데이터 구조체 선언 
typedef struct Avg_Elapsed_Time_LineChart{
	int Avg_Elapsed_Times;
	int 2022_02_08_Avg_Elapsed_Times;
	int 2022_02_09_Avg_Elapsed_Times;
	int 2022_02_10_Avg_Elapsed_Times;
	int 2022_02_11_Avg_Elapsed_Times;
	int 2022_02_12_Avg_Elapsed_Times;
	int 2022_02_13_Avg_Elapsed_Times;
	int 2022_02_14_Avg_Elapsed_Times;
	int 2022_02_15_Avg_Elapsed_Times;
} Avg_Elapsed_Time_LineChart;

// 파일통계화면 UI, 작업 종류를 보여주는 데이터 구조체 선언
typedef struct JobType_Chart_PieChart{
	int Total_Count;
	int File_Backup_Count;
	int Informix_Onbar_Backup_Count;
	int Mysql_Backup_Count;
	int Oracle_RMAN_Backup_Count;
	int Physical_Backup_Count;
	int Vm_Ware_Backup_Count;
} JobType_Chart_PieChart;

// 에러 UI, 에러 비율을 보여주는 데이터 구조체 선언  
typedef struct Total_Error_Ratio_PieChart{
	int Total_Count;
	int Total_Completed_Count;
	int Total_Error_Count;
} Total_Error_Ratio_PieChart;

// 에러 UI, 작업 종류별 에러타입을 위한 데이터 구조체 선언 
typedef struct Error_Ratio_By_Job_Status_PieChart{
	int Total_Error_Count;
	int Partially_Completed_Count;
	int Suspended_Error_Count;
	int Failed_Error_Count;
	int Canceled_Error_Count;
} Error_Ratio_By_Job_Status_PieChart;

int main(int argc, char **argv)
{
	// LogData.csv 파일을 읽는다
	FILE *the_file = fopen("/home/ubuntu/Workspace/LogData.csv","r");
	if (the_file == NULL)
	{
		perror("Unable to open the file.");
		exit(1);
	}
	// csv 파일 읽기 위한 변수 선언 및 정의
	int row_count = 0;
	int total_field_count = 0;
	int field_count = 0;
	char line[300];
	LogData array[991];
	int Total_row_count = 0;

	// csv 파일 끝까지 읽기
	while (fgets(line, sizeof(line), the_file))
	{
		field_count = 0;
		row_count++;
		// csv 파일의 첫 줄은 Column 이름이기 때문에 생략
		if(row_count==1)
			continue;당
		char *field = strtok(line, ",");
		
		// 구조체에 각각의 값들을 할당 
		while (field)
		{
			if(field_count==0)
				strcpy(array[Total_row_count].Job_ID, field);
			if(field_count==1)
				strcpy(array[Total_row_count].Job_Status, field);
			if(field_count==2)
				strcpy(array[Total_row_count].Error_Code, field);
			if(field_count==3)
				strcpy(array[Total_row_count].Job_Type, field);
			if(field_count==4)
				strcpy(array[Total_row_count].Backup_Method, field);
			if(field_count==5)
				strcpy(array[Total_row_count].Client, field);
			if(field_count==6)
				strcpy(array[Total_row_count].Job, field);
			if(field_count==7)
				strcpy(array[Total_row_count].Schedule, field);
			if(field_count==8)
				strcpy(array[Total_row_count].Files, field);
			if(field_count==9)
				strcpy(array[Total_row_count].Files_Size, field);
			if(field_count==10)
				strcpy(array[Total_row_count].Write_Size, field);
			if(field_count==11)
				strcpy(array[Total_row_count].Data_Transferred, field);
			if(field_count==12)
				strcpy(array[Total_row_count].Throughput, field);
			if(field_count==13)
				strcpy(array[Total_row_count].Start_Time, field);
			if(field_count==14)
				strncpy(array[Total_row_count].End_Time, field,10);
				array[Total_row_count].End_Time[10] = '\0';	// NULL 값 추가 
			if(field_count==15)
				strcpy(array[Total_row_count].Elapsed_Time, field);
			if(field_count==16)
				strcpy(array[Total_row_count].Server, field);
			if(field_count==17)
				strcpy(array[Total_row_count].Volume_Pool, field);
			if(field_count==18)
				strcpy(array[Total_row_count].Storage, field);
			if(field_count==19)
				strcpy(array[Total_row_count].Tape_Drive, field);
			if(field_count==20)
				strcpy(array[Total_row_count].Volume, field);
			
			field = strtok(NULL,",");
			field_count++;
		}
		Total_row_count++;
	}
	fclose(the_file);
	
	// 데이터 가공 (추후 쓰레드 추가 고려 예정) 
	// 백업 메소드 통계 데이터 가공을 위한 변수 선언 (Home UI)
	char Backup_Method[300];
	// 작업 완료 수 통계, 에러 통계 데이터 가공을 위한 변수 선언 (Home UI, Error UI)
	char Job_Status[300];
	char End_Date[300];
	// 작업 종류 통계 데이터 가공을 위한 변수 선언 (Statistics UI)
	char Job_Type[300];
	

	for(int i=0;i<Total_row_count;i++)						
	{
		// 에러 File_Statistics_PieChart.Total_File_Size += array[i].File_Size;
		strcpy(Backup_Method,array[i].Backup_Method); // 백업 메소드 통계 데이터 가공을 위한 변수 값 복사 (Home UI)
		strcpy(Job_Status,array[i].Job_Status); // 작업완료 수 통계 데이터 가공을 위한 변수 값 복사 (Home UI), 에러 통계 데이터 가공을 위한 변수 값 복사 (Error UI)
		strcpy(End_Date,array[i].End_Date);	// 완료일자 계산 위한 변수 값 복사 
		strcpy(Job_Type,array[i].Job_Type); // 작업 종류 통계 데이터 가공을 위한 변수 값 복사 (Statistics UI)
		
		// 백업 메소드 통계 (Home UI)
		if (strcmp(Backup_Method,"Archive Backup")==0)
		{
			Backup_Method_Ratio_Pie_Chart.Archive_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Differential Backup")==0)
		{
			Backup_Method_Ratio_Pie_Chart.Differential_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Dump Backup")==0)
		{
			Backup_Method_Ratio_Pie_Chart.Dump_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Full Backup")==0)
		{
			Backup_Method_Ratio_Pie_Chart.Full_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Incremental Backup")==0)
		{
			Backup_Method_Ratio_Pie_Chart.Incremental_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Synthetic")==0)
		{
			Backup_Method_Ratio_Pie_Chart.Synthetic_Count++;
		}

		// 작업 완료 수, 에러 통계 데이터 가공 (Home UI, Error UI)
		if (strcmp(Job_Status,"Canceled")==0)
		{
			Total_Error_Ratio_PieChart.Total_Error_Count++;	// Error UI
			Error_Ratio_By_Job_Status_PieChart.Canceled_Error_Count++; // Error UI
			
		}
		if (strcmp(Job_Status,"Completed")==0)
		{
			Total_Backup_Count_LineChart.Job_Status_Completed_Count++;	// Home UI
			Total_Error_Ratio_PieChart.Total_Completed_Count++;	// Error UI
			if (strcmp(End_Date,"2022-02-08")==0)
				Total_Backup_Count_LineChart.2022_02_08_Completed_Count++;
			if (strcmp(End_Date,"2022-02-09")==0)
				Total_Backup_Count_LineChart.2022_02_09_Completed_Count++;
			if (strcmp(End_Date,"2022-02-10")==0)
				Total_Backup_Count_LineChart.2022_02_10_Completed_Count++;
			if (strcmp(End_Date,"2022-02-11")==0)
				Total_Backup_Count_LineChart.2022_02_11_Completed_Count++;
			if (strcmp(End_Date,"2022-02-12")==0)
				Total_Backup_Count_LineChart.2022_02_12_Completed_Count++;
			if (strcmp(End_Date,"2022-02-13")==0)
				Total_Backup_Count_LineChart.2022_02_13_Completed_Count++;
			if (strcmp(End_Date,"2022-02-14")==0)
				Total_Backup_Count_LineChart.2022_02_14_Completed_Count++;
			if (strcmp(End_Date,"2022-02-15")==0)
				Total_Backup_Count_LineChart.2022_02_15_Completed_Count++;
		}
		if (strcmp(Job_Status,"Failed")==0)
		{
			Total_Error_Ratio_PieChart.Total_Error_Count++;	// Error UI
			Error_Ratio_By_Job_Status_PieChart.Failed_Error_Count++;
		}
		if (strcmp(Job_Status,"Partially Completed")==0)
		{
			Total_Error_Ratio_PieChart.Total_Error_Count++;	// Error UI
			Error_Ratio_By_Job_Status_PieChart.Partially_Completed_Count++;
		}
		if (strcmp(Job_Status,"Suspended")==0)
		{
			Total_Error_Ratio_PieChart.Total_Error_Count++;	// Error UI
			Error_Ratio_By_Job_Status_PieChart.Suspended_Error_Count++;	// Error UI
		}
		
		// 작업 종류 수 통계 위한 데이터 가공 (Statistics UI)
		if (strcmp(Job_Type,"File Backup")==0)
		{
			JobType_Chart_PieChart.File_Backup_Count++;
		}
		if (strcmp(Job_Type,"Informix Onbar Backup")==0)
		{
			JobType_Chart_PieChart.Informix_Onbar_Backup_Count++;
		}
		if (strcmp(Job_Type,"Mysql Backup")==0)
		{
			JobType_Chart_PieChart.Mysql_Backup_Count++;
		}
		if (strcmp(Job_Type,"Oracle RMAN Backup")==0)
		{
			JobType_Chart_PieChart.Oracle_RMAN_Backup_Count++;
		}
		if (strcmp(Job_Type,"Physical Backup")==0)
		{
			JobType_Chart_PieChart.Physical_Backup_Count++;
		}
		if (strcmp(Job_Type,"Vmware Backup")==0)
		{
			JobType_Chart_PieChart.Vm_Ware_Backup_Count++;
		}
	}
	Backup_Method_Ratio_Pie_Chart.Total_Count = Total_row_count;
	Total_Error_Ratio_PieChart.Total_Count = Total_row_count;
	JobType_Chart_PieChart.Total_Count = Total_row_count;
	Error_Ratio_By_Job_Status_PieChart.Total_Count = Total_Error_Ratio_PieChart.Total_Error_Count;

	// 가공한 데이터 저장 (.dat)


	// 소켓 통신 시작 
	//Set a socket address structure server_addr, representing the server Internet address, port
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr)); //Set all the contents of a section of memory to 0
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
	// time_t now;
	FILE *stream;
	//Create a streaming protocol (TCP)socket for the Internet, with server_socket representing the server socket
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
	if( server_socket < 0)
	{
		printf("Create Socket Failed!");
		exit(1);
	}
	//Connect the socket to the socket address structure
	if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
	{
		printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT);
		exit(1);
	}
	//Server_socket is used for listening
	if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
	{
		printf("Server Listen Failed!");
		exit(1);
	}
	while (1) //The server side runs all the time
	{
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);
		int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
		if ( new_server_socket < 0)
		{
			printf("Server Accept Failed!n");
			break;
		}
		char buffer[BUFFER_SIZE];
		bzero(buffer, BUFFER_SIZE);
		strcpy(buffer,"Hello,World!  From the server! ");
		strcat(buffer,"n"); //C string concatenation
		send(new_server_socket,buffer,BUFFER_SIZE,0);
		bzero(buffer,BUFFER_SIZE);
		//Receives information sent by the client to the buffer
		length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
		if (length < 0)
		{
			printf("Server Recieve Data Failed!n");
			exit(1);
		}
		printf("n%s",buffer);
		if((stream = fopen("data1","r"))==NULL)
		{
			printf("The file 'data1' was not opened! n");
			exit(1);
		}
		else
		{
			printf("The file 'filename' was opened! n");
		}
		bzero(buffer,BUFFER_SIZE);
		int lengsize = 0;
		while((lengsize = fread(buffer,1,1024,stream)) > 0)
		{
			printf("lengsize = %dn",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File is Failedn");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream))
		{
			printf("The file 'data' was not closed! n");
			exit(1);    
		}
		//Close the connection to the client
		close(new_server_socket);    
	}
	//Close the socket for listening
	close(server_socket);
	return 0;
}
