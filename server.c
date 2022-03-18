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
LogData array[991];

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
Filtering_Data Filtering_Datas;

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
Backup_Method_Ratio_Pie_Chart Backup_Method_Ratio_Pie_Charts;

// 홈 UI, 일별 총 백업 수를 보여주 데이터 구조체 선언
typedef struct Total_Backup_Count_LineChart
{
	// int Job_Status_Completed_Count;	// Total, Do not Need
	int _2022_02_08_Completed_Count;
	int _2022_02_09_Completed_Count;
	int _2022_02_10_Completed_Count;
	int _2022_02_11_Completed_Count;
	int _2022_02_12_Completed_Count;
	int _2022_02_13_Completed_Count;
	int _2022_02_14_Completed_Count;
	int _2022_02_15_Completed_Count;
} Total_Backup_Count_LineChart;
Total_Backup_Count_LineChart Total_Backup_Count_LineCharts;

// 파일통계화면 UI, 파일 관련 통계를 보여줄 데이터 구조체 선언 
typedef struct File_Statistics_PieChart
{
	int Total_Size;
	int Total_File_Size;
	int Total_Write_Size;
} File_Statistics_PieChart;
File_Statistics_PieChart File_Statistics_PieCharts;

// 파일통계화면 UI, 평균 경과시간을 보여주는 데이터 구조체 선언 
typedef struct Avg_Elapsed_Time_LineChart{
	int Avg_Elapsed_Times;
	int _2022_02_08_Avg_Elapsed_Times;
	int _2022_02_09_Avg_Elapsed_Times;
	int _2022_02_10_Avg_Elapsed_Times;
	int _2022_02_11_Avg_Elapsed_Times;
	int _2022_02_12_Avg_Elapsed_Times;
	int _2022_02_13_Avg_Elapsed_Times;
	int _2022_02_14_Avg_Elapsed_Times;
	int _2022_02_15_Avg_Elapsed_Times;
} Avg_Elapsed_Time_LineChart;
Avg_Elapsed_Time_LineChart Avg_Elapsed_Time_LineCharts;

// 파일통계화면 UI, 작업 종류를 보여주는 데이터 구조체 선언
typedef struct JobType_PieChart{
	int Total_Count;
	int File_Backup_Count;
	int Informix_Onbar_Backup_Count;
	int Mysql_Backup_Count;
	int Oracle_RMAN_Backup_Count;
	int Physical_Backup_Count;
	int Vm_Ware_Backup_Count;
} JobType_PieChart;
JobType_PieChart JobType_PieCharts;

// 에러 UI, 에러 비율을 보여주는 데이터 구조체 선언  
typedef struct Total_Error_Ratio_PieChart{
	int Total_Count;
	int Total_Completed_Count;
	int Total_Error_Count;
} Total_Error_Ratio_PieChart;
Total_Error_Ratio_PieChart Total_Error_Ratio_PieCharts;

// 에러 UI, 작업 종류별 에러타입을 위한 데이터 구조체 선언 
typedef struct Error_Ratio_By_Job_Status_PieChart{
	int Total_Error_Count;
	int Partially_Completed_Count;
	int Suspended_Error_Count;
	int Failed_Error_Count;
	int Canceled_Error_Count;
} Error_Ratio_By_Job_Status_PieChart;
Error_Ratio_By_Job_Status_PieChart Error_Ratio_By_Job_Status_PieCharts;

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
	// LogData array[991];
	int Total_row_count = 0;

	// csv 파일 끝까지 읽기
	while (fgets(line, sizeof(line), the_file))
	{
		field_count = 0;
		row_count++;
		// csv 파일의 첫 줄은 Column 이름이기 때문에 생략
		if(row_count==1)
			continue;
		// ,를 기준으로 문자열을 자르는 변수 선언 
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
			// 다음 문자열을 자른 후 포인터 반환
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
	char End_Time[300];
	// 작업 종류 통계 데이터 가공을 위한 변수 선언 (Statistics UI)
	char Job_Type[300];
	// 평균 처리시간 위한 변수 선언 
	int _02_08_h_array[991];
	int _02_08_m_array[991];
	int _02_08_s_array[991];
	
	int _02_09_h_array[991];
	int _02_09_m_array[991];
	int _02_09_s_array[991];
	
	int _02_10_h_array[991];
	int _02_10_m_array[991];
	int _02_10_s_array[991];
	
	int _02_11_h_array[991];
	int _02_11_m_array[991];
	int _02_11_s_array[991];
	
	int _02_12_h_array[991];
	int _02_12_m_array[991];
	int _02_12_s_array[991];
	
	int _02_13_h_array[991];
	int _02_13_m_array[991];
	int _02_13_s_array[991];
	
	int _02_14_h_array[991];
	int _02_14_m_array[991];
	int _02_14_s_array[991];
	
	int _02_15_h_array[991];
	int _02_15_m_array[991];
	int _02_15_s_array[991];
	
	int _02_08_count = 0;
	int _02_09_count = 0;
	int _02_10_count = 0;
	int _02_11_count = 0;
	int _02_12_count = 0;
	int _02_13_count = 0;
	int _02_14_count = 0;
	int _02_15_count = 0;

	// total file count, total write count
	// File_Size 와 Write_Size 위한 변수 
	int t_f_c =0;
	int t_w_c =0;
	
	for(int i=0;i<Total_row_count;i++)						
	{
		// 에러 File_Statistics_PieChart.Total_File_Size += array[i].File_Size;
		strcpy(Backup_Method,array[i].Backup_Method); // 백업 메소드 통계 데이터 가공을 위한 변수 값 복사 (Home UI)
		strcpy(Job_Status,array[i].Job_Status); // 작업완료 수 통계 데이터 가공을 위한 변수 값 복사 (Home UI), 에러 통계 데이터 가공을 위한 변수 값 복사 (Error UI)
		strcpy(End_Time,array[i].End_Time);	// 완료일자 계산 위한 변수 값 복사 
		strcpy(Job_Type,array[i].Job_Type); // 작업 종류 통계 데이터 가공을 위한 변수 값 복사 (Statistics UI)
		
		// 백업 메소드 통계 (Home UI)
		if (strcmp(Backup_Method,"Archive Backup")==0)
		{
			Backup_Method_Ratio_Pie_Charts.Archive_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Differential Backup")==0)
		{
			Backup_Method_Ratio_Pie_Charts.Differential_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Dump Backup")==0)
		{
			Backup_Method_Ratio_Pie_Charts.Dump_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Full Backup")==0)
		{
			Backup_Method_Ratio_Pie_Charts.Full_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Incremental Backup")==0)
		{
			Backup_Method_Ratio_Pie_Charts.Incremental_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Synthetic")==0)
		{
			Backup_Method_Ratio_Pie_Charts.Synthetic_Count++;
		}

		// 작업 완료 수, 에러 통계 데이터 가공 (Home UI, Error UI)
		if (strcmp(Job_Status,"Canceled")==0)
		{
			Total_Error_Ratio_PieCharts.Total_Error_Count++;	// Error UI
			Error_Ratio_By_Job_Status_PieCharts.Canceled_Error_Count++; // Error UI
			
		}
		if (strcmp(Job_Status,"Completed")==0)
		{
			// Total_Backup_Count_LineCharts.Job_Status_Completed_Count++;	// Home UI
			Total_Error_Ratio_PieCharts.Total_Completed_Count++;	// Error UI
			if (strcmp(End_Time,"2022-02-08")==0)
				Total_Backup_Count_LineCharts._2022_02_08_Completed_Count++;
			if (strcmp(End_Time,"2022-02-09")==0)
				Total_Backup_Count_LineCharts._2022_02_09_Completed_Count++;
			if (strcmp(End_Time,"2022-02-10")==0)
				Total_Backup_Count_LineCharts._2022_02_10_Completed_Count++;
			if (strcmp(End_Time,"2022-02-11")==0)
				Total_Backup_Count_LineCharts._2022_02_11_Completed_Count++;
			if (strcmp(End_Time,"2022-02-12")==0)
				Total_Backup_Count_LineCharts._2022_02_12_Completed_Count++;
			if (strcmp(End_Time,"2022-02-13")==0)
				Total_Backup_Count_LineCharts._2022_02_13_Completed_Count++;
			if (strcmp(End_Time,"2022-02-14")==0)
				Total_Backup_Count_LineCharts._2022_02_14_Completed_Count++;
			if (strcmp(End_Time,"2022-02-15")==0)
				Total_Backup_Count_LineCharts._2022_02_15_Completed_Count++;
		}
		if (strcmp(Job_Status,"Failed")==0)
		{
			Total_Error_Ratio_PieCharts.Total_Error_Count++;	// Error UI
			Error_Ratio_By_Job_Status_PieCharts.Failed_Error_Count++;
		}
		if (strcmp(Job_Status,"Partially Completed")==0)
		{
			Total_Error_Ratio_PieCharts.Total_Error_Count++;	// Error UI
			Error_Ratio_By_Job_Status_PieCharts.Partially_Completed_Count++;
		}
		if (strcmp(Job_Status,"Suspended")==0)
		{
			Total_Error_Ratio_PieCharts.Total_Error_Count++;	// Error UI
			Error_Ratio_By_Job_Status_PieCharts.Suspended_Error_Count++;	// Error UI
		}
		
		// 작업 종류 수 통계 위한 데이터 가공 (Statistics UI)
		if (strcmp(Job_Type,"File Backup")==0)
		{
			JobType_PieCharts.File_Backup_Count++;
		}
		if (strcmp(Job_Type,"Informix Onbar Backup")==0)
		{
			JobType_PieCharts.Informix_Onbar_Backup_Count++;
		}
		if (strcmp(Job_Type,"Mysql Backup")==0)
		{
			JobType_PieCharts.Mysql_Backup_Count++;
		}
		if (strcmp(Job_Type,"Oracle RMAN Backup")==0)
		{
			JobType_PieCharts.Oracle_RMAN_Backup_Count++;
		}
		if (strcmp(Job_Type,"Physical Backup")==0)
		{
			JobType_PieCharts.Physical_Backup_Count++;
		}
		if (strcmp(Job_Type,"Vmware Backup")==0)
		{
			JobType_PieCharts.Vm_Ware_Backup_Count++;
		}
		
		// 일별 평균처리시간 위한 데이터 가공 (Statistics UI)
		if (strcmp(End_Time,"2022-02-08")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			_02_08_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_08_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_08_s_array[i] = atoi(ptr);
			_02_08_count++;
		}
		if (strcmp(End_Time,"2022-02-09")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			_02_09_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_09_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_09_s_array[i] = atoi(ptr);
			_02_09_count++;
		}
		if (strcmp(End_Time,"2022-02-10")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			_02_10_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_10_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_10_s_array[i] = atoi(ptr);
			_02_10_count++;
		}
		if (strcmp(End_Time,"2022-02-11")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			_02_11_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_11_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_11_s_array[i] = atoi(ptr);
			_02_11_count++;
		}
		if (strcmp(End_Time,"2022-02-12")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			_02_12_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_12_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_12_s_array[i] = atoi(ptr);
			_02_12_count++;
		}
		if (strcmp(End_Time,"2022-02-13")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			_02_13_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_13_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_13_s_array[i] = atoi(ptr);
			_02_13_count++;
		}
		if (strcmp(End_Time,"2022-02-14")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			_02_14_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_14_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_14_s_array[i] = atoi(ptr);
			_02_14_count++;
		}
		if (strcmp(End_Time,"2022-02-15")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			ptr = strtok(NULL,":");
			_02_15_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_15_s_array[i] = atoi(ptr);
			_02_15_count++;
		}
		
		// GB, MB, KB 구분 위한 데이터 가공 (Statistics UI)
		char *f_ptr2 = strtok(array[i].Files_Size," ");
		char *f_ptr3 = strtok(NULL," ");
		
		// GB만 추출해서 저장
		if (strcmp(f_ptr3,"GB")==0)
		{
			t_f_c++;
			File_Statistics_PieCharts.Total_File_Size += atof(f_ptr2);
		}
		char *w_ptr2 = strtok(array[i].Write_Size," ");
		char *w_ptr3 = strtok(NULL," ");

		if (strcmp(w_ptr3,"GB")==0)
		{
			File_Statistics_PieCharts.Total_Write_Size += atof(w_ptr2);
			t_w_c++;
		}
	}
	Backup_Method_Ratio_Pie_Charts.Total_Count = Total_row_count;
	Total_Error_Ratio_PieCharts.Total_Count = Total_row_count;
	JobType_PieCharts.Total_Count = Total_row_count;
	Error_Ratio_By_Job_Status_PieCharts.Total_Error_Count = Total_Error_Ratio_PieCharts.Total_Error_Count;

	// 가공한 데이터 저장 (.dat)
	// 파일 변수 선언
	FILE *_Filtering_Data;
	FILE *_BackupMethod_PieChart;
	FILE *_BackupCount_LineChart;
	FILE *_FileStatistics_PieChart;
	FILE *_Avg_Elapsed_LineChart;
	FILE *_JobType_PieChart;
	FILE *_ErrorRatio_PieChart;
	FILE *_ErrorRatio_By_JobType_PieChart;
	
	// 파일에 저장하기 위한 파일을 엽니다. 
	_BackupMethod_PieChart = fopen("BackupMethod_PieChart.dat","w");
	_BackupCount_LineChart = fopen("BackupCount_LineChart.dat","w");
	_FileStatistics_PieChart = fopen("FileStatistics_PieChart.dat","w");
	_Avg_Elapsed_LineChart = fopen("Avg_Elapsed_LineChart.dat","w");
	_JobType_PieChart = fopen("JobType_PieChart.dat","w");
	_ErrorRatio_PieChart = fopen("ErrorRatio_PieChart.dat","w");
	_ErrorRatio_By_JobType_PieChart = fopen("ErrorRatio_By_JobType_PieChart.dat","w");
	
	if (_BackupMethod_PieChart == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}
	if (_BackupCount_LineChart == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}
	if (_FileStatistics_PieChart == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}
	if (_Avg_Elapsed_LineChart == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}
	if (_JobType_PieChart == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}
	if (_ErrorRatio_PieChart == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}
	if (_ErrorRatio_By_JobType_PieChart == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}
	// 구조체 내용을 파일에 저장한다. 
	fwrite(&Backup_Method_Ratio_Pie_Charts, sizeof(struct Backup_Method_Ratio_Pie_Chart), 1, _BackupMethod_PieChart);
	fwrite(&Total_Backup_Count_LineCharts, sizeof(struct Total_Backup_Count_LineChart), 1, _BackupCount_LineChart);
	fwrite(&File_Statistics_PieCharts, sizeof(struct File_Statistics_PieChart), 1, _FileStatistics_PieChart);
	fwrite(&Avg_Elapsed_Time_LineCharts, sizeof(struct Avg_Elapsed_Time_LineChart), 1, _Avg_Elapsed_LineChart);
	fwrite(&JobType_PieCharts, sizeof(struct JobType_PieChart), 1, _JobType_PieChart);
	fwrite(&Total_Error_Ratio_PieCharts, sizeof(struct Total_Error_Ratio_PieChart), 1, _ErrorRatio_PieChart);
	fwrite(&Error_Ratio_By_Job_Status_PieCharts, sizeof(struct Error_Ratio_By_Job_Status_PieChart), 1, _ErrorRatio_By_JobType_PieChart);
	
	if (fwrite != 0)
		printf("Contents to file Written Successfully !\n");
	else
		printf("Error Writing file !\n");
	// 파일을 닫아준다. 
	fclose(_BackupMethod_PieChart);
	fclose(_BackupCount_LineChart);
	fclose(_FileStatistics_PieChart);
	fclose(_Avg_Elapsed_LineChart);
	fclose(_JobType_PieChart);
	fclose(_ErrorRatio_PieChart);
	fclose(_ErrorRatio_By_JobType_PieChart);
	
	// 소켓 통신 시작 
	// 소켓 구조체를 선언하고 IP 주소 및 포트를 설정 
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr)); // 모든 내용을 0으로 초기화 
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
	// time_t now;
	FILE *stream;	// For Backup_Method Struct
	FILE *stream1;	// For BackupCount Struct
	FILE *stream2;	// For FileStatistics Struct
	FILE *stream3;	// For Avg_Elapsed Struct
	FILE *stream4;	// For JobType Struct
	FILE *stream5;	// For ErrorRatio Struct
	FILE *stream6;	// For ErrorRatio_By_JobType Struct
	
	// 서버 소켓을 나타내는 server_socket을 사용하여 인터넷용 스트리밍 프로토콜(TCP) 소켓을 만듭니다.
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
	if( server_socket < 0)
	{
		printf("Create Socket Failed!\n");
		exit(1);
	}
	// 할당 된 소켓 주소에 소켓을 연결합니다. 
	if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
	{
		printf("Server Bind Port : %d Failed!\n", HELLO_WORLD_SERVER_PORT);
		exit(1);
	}
	// Server_socket은 수신에 사용됩니다.
	if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
	{
		printf("Server Listen Failed!\n");
		exit(1);
	}
	while (1) // 서버측을 항상 가동 시킵니다.
	{
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);
		int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
		if ( new_server_socket < 0)
		{
			printf("Server Accept Failed!\n");
			break;
		}
		char buffer[BUFFER_SIZE];
		bzero(buffer, BUFFER_SIZE);
		strcpy(buffer,"Hello,World!  From the server! ");
		strcat(buffer,"\n"); // C 문자열 연결
		send(new_server_socket,buffer,BUFFER_SIZE,0);
		bzero(buffer,BUFFER_SIZE);
		// 클라이언트가 버퍼로 보낸 정보 수신
		length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
		if (length < 0)
		{
			printf("Server Recieve Data Failed!\n");
			exit(1);
		}
		printf("n%s",buffer);
		
		// For Backup_Method Struct
		if((stream = fopen("BackupMethod_PieChart.dat","r"))==NULL)
		{
			printf("The file 'BackupMethod_PieChart.dat' was not opened! \n");
			exit(1);
		}
		else
		{
			printf("The file 'BackupMethod_PieChart.dat' was opened! \n");
		}
		bzero(buffer,BUFFER_SIZE);
		int lengsize = 0;
		while((lengsize = fread(buffer,1,1024,stream)) > 0)
		{
			printf("lengsize = %d\n",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File(BackupMethod_PieChart.dat) is Failed\n");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream))
		{
			printf("The file 'BackupMethod_PieChart.dat' was not closed! \n");
			exit(1);    
		}
		
		// For BackupCount Struct
		if((stream1 = fopen("BackupCount_LineChart.dat","r"))==NULL)
		{
			printf("The file 'BackupCount_LineChart.dat' was not opened! \n");
			exit(1);
		}
		else
		{
			printf("The file 'BackupCount_LineChart.dat' was opened! \n");
		}
		bzero(buffer,BUFFER_SIZE);
		lengsize = 0;
		while((lengsize = fread(buffer,1,1024,stream1)) > 0)
		{
			printf("lengsize = %d\n",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File(BackupCount_LineChart) is Failed\n");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream1))
		{
			printf("The file 'BackupCount_LineChart.dat' was not closed! \n");
			exit(1);    
		}
		
		// For FileStatistics Struct
		if((stream2 = fopen("FileStatistics_PieChart.dat","r"))==NULL)
		{
			printf("The file 'FileStatistics_PieChart.dat' was not opened! \n");
			exit(1);
		}
		else
		{
			printf("The file 'FileStatistics_PieChart.dat' was opened! \n");
		}
		bzero(buffer,BUFFER_SIZE);
		lengsize = 0;
		while((lengsize = fread(buffer,1,1024,stream2)) > 0)
		{
			printf("lengsize = %d\n",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File(FileStatistics_PieChart.dat) is Failed\n");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream2))
		{
			printf("The file 'FileStatistics_PieChart.dat' was not closed! \n");
			exit(1);    
		}
		
		// For Avg_Elapsed Struct
		if((stream3 = fopen("Avg_Elapsed_LineChart.dat","r"))==NULL)
		{
			printf("The file 'Avg_Elapsed_LineChart.dat' was not opened! \n");
			exit(1);
		}
		else
		{
			printf("The file 'Avg_Elapsed_LineChart.dat' was opened! \n");
		}
		bzero(buffer,BUFFER_SIZE);
		lengsize = 0;
		while((lengsize = fread(buffer,1,1024,stream3)) > 0)
		{
			printf("lengsize = %d\n",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File(Avg_Elapsed_LineChart.dat) is Failed\n");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream3))
		{
			printf("The file 'Avg_Elapsed_LineChart.dat' was not closed! \n");
			exit(1);    
		}
		
		// For JobType Struct
		if((stream4 = fopen("JobType_PieChart.dat","r"))==NULL)
		{
			printf("The file 'JobType_PieChart.dat' was not opened! \n");
			exit(1);
		}
		else
		{
			printf("The file 'JobType_PieChart.dat' was opened! \n");
		}
		bzero(buffer,BUFFER_SIZE);
		lengsize = 0;
		while((lengsize = fread(buffer,1,1024,stream4)) > 0)
		{
			printf("lengsize = %d\n",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File(JobType_PieChart.dat) is Failed\n");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream4))
		{
			printf("The file 'JobType_PieChart.dat' was not closed! \n");
			exit(1);    
		}
		
		// For ErrorRatio Struct
		if((stream5 = fopen("ErrorRatio_PieChart.dat","r"))==NULL)
		{
			printf("The file 'ErrorRatio_PieChart.dat' was not opened! \n");
			exit(1);
		}
		else
		{
			printf("The file 'ErrorRatio_PieChart.dat' was opened! \n");
		}
		bzero(buffer,BUFFER_SIZE);
		lengsize = 0;
		while((lengsize = fread(buffer,1,1024,stream5)) > 0)
		{
			printf("lengsize = %d\n",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File(ErrorRatio_PieChart.dat) is Failed\n");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream5))
		{
			printf("The file 'ErrorRatio_PieChart.dat' was not closed! \n");
			exit(1);    
		}
		
		// For ErrorRatio_By_JobType Struct
		if((stream6 = fopen("ErrorRatio_By_JobType_PieChart.dat","r"))==NULL)
		{
			printf("The file 'ErrorRatio_By_JobType_PieChart.dat' was not opened! \n");
			exit(1);
		}
		else
		{
			printf("The file 'ErrorRatio_By_JobType_PieChart.dat' was opened! \n");
		}
		bzero(buffer,BUFFER_SIZE);
		lengsize = 0;
		while((lengsize = fread(buffer,1,1024,stream6)) > 0)
		{
			printf("lengsize = %d\n",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File(ErrorRatio_By_JobType_PieChart.dat) is Failed\n");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream6))
		{
			printf("The file 'ErrorRatio_By_JobType_PieChart.dat' was not closed! \n");
			exit(1);    
		}
		
		// 클라이언트와의 연결을 종료합니다.
		close(new_server_socket);    
	}
	// 수신을 위한 서버소켓을 종료합니다. 
	close(server_socket);
	return 0;
}
