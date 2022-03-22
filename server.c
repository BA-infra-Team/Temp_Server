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
#define HELLO_WORLD_SERVER_PORT 7754
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024

// get the file size 
struct stat st;
// File_Information
#pragma pack(push, 1)    // 1바이트 크기로 정렬
typedef struct File_Info
{
	int FileNameLen;
	char File_Name[15]; 
	//int File_Size;
}File_Info;
File_Info File_Infos;
#pragma pack(pop)        // 정렬 설정을 이전 상태(기본값)로 되돌림

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
typedef struct ChartData
{
	// 홈 UI, 백업-메소드 비율을 보여주기 위한 데이터 구조체 선언 
	int Backup_Method_Ratio_Pie_Chart_Total_Count;
	int Backup_Method_Ratio_Pie_Chart_Archive_Backup_Count;
	int Backup_Method_Ratio_Pie_Chart_Differential_Backup_Count;
	int Backup_Method_Ratio_Pie_Chart_Dump_Backup_Count;
	int Backup_Method_Ratio_Pie_Chart_Full_Backup_Count;
	int Backup_Method_Ratio_Pie_Chart_Incremental_Backup_Count;
	int Backup_Method_Ratio_Pie_Chart_Synthetic_Count;

	// 홈 UI, 일별 총 백업 수를 보여주 데이터 구조체 선언
	int Total_Backup_Count_LineChart_2022_02_08_Completed_Count;
	int Total_Backup_Count_LineChart_2022_02_09_Completed_Count;
	int Total_Backup_Count_LineChart_2022_02_10_Completed_Count;
	int Total_Backup_Count_LineChart_2022_02_11_Completed_Count;
	int Total_Backup_Count_LineChart_2022_02_12_Completed_Count;
	int Total_Backup_Count_LineChart_2022_02_13_Completed_Count;
	int Total_Backup_Count_LineChart_2022_02_14_Completed_Count;
	int Total_Backup_Count_LineChart_2022_02_15_Completed_Count;

	// 파일통계화면 UI, 파일 관련 통계를 보여줄 데이터 구조체 선언 
	int File_Statistics_PieChart_Total_Size;
	int File_Statistics_PieChart_Total_File_Size;
	int File_Statistics_PieChart_Total_Write_Size;

	// 파일통계화면 UI, 평균 경과시간을 보여주는 데이터 구조체 선언 
	int Avg_Elapsed_Time_LineChart_2022_02_08_Avg_Elapsed_Times;
	int Avg_Elapsed_Time_LineChart_2022_02_09_Avg_Elapsed_Times;
	int Avg_Elapsed_Time_LineChart_2022_02_10_Avg_Elapsed_Times;
	int Avg_Elapsed_Time_LineChart_2022_02_11_Avg_Elapsed_Times;
	int Avg_Elapsed_Time_LineChart_2022_02_12_Avg_Elapsed_Times;
	int Avg_Elapsed_Time_LineChart_2022_02_13_Avg_Elapsed_Times;
	int Avg_Elapsed_Time_LineChart_2022_02_14_Avg_Elapsed_Times;
	int Avg_Elapsed_Time_LineChart_2022_02_15_Avg_Elapsed_Times;

	// 파일통계화면 UI, 작업 종류를 보여주는 데이터 구조체 선언
	int JobType_PieChart_Total_Count;
	int JobType_PieChart_File_Backup_Count;
	int JobType_PieChart_Informix_Onbar_Backup_Count;
	int JobType_PieChart_Mysql_Backup_Count;
	int JobType_PieChart_Oracle_RMAN_Backup_Count;
	int JobType_PieChart_Physical_Backup_Count;
	int JobType_PieChart_Vm_Ware_Backup_Count;

	// 에러 UI, 에러 비율을 보여주는 데이터 구조체 선언
	int Total_Error_Ratio_PieChart_Total_Count;
	int Total_Error_Ratio_PieChart_Total_Completed_Count;
	int Total_Error_Ratio_PieChart_Total_Error_Count;

	// 에러 UI, 작업 종류별 에러타입을 위한 데이터 구조체 선언 
	int Error_Ratio_By_Job_Status_PieChart_Total_Error_Count;
	int Error_Ratio_By_Job_Status_PieChart_Partially_Completed_Count;
	int Error_Ratio_By_Job_Status_PieChart_Suspended_Error_Count;
	int Error_Ratio_By_Job_Status_PieChart_Failed_Error_Count;
	int Error_Ratio_By_Job_Status_PieChart_Canceled_Error_Count;
} ChartData;
ChartData ChartDatas;

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
	
	// 데이터 가공 
	// 백업 메소드 통계 데이터 가공을 위한 변수 선언 (Home UI)
	char Backup_Method[300];
	// 작업 완료 수 통계, 에러 통계 데이터 가공을 위한 변수 선언 (Home UI, Error UI)
	char Job_Status[300];
	char End_Time[300];
	// 작업 종류 통계 데이터 가공을 위한 변수 선언 (Statistics UI)
	char Job_Type[300];
	// 평균 처리시간 위한 변수 선언 
	int _02_08_h_array[991] = { 0, };
	int _02_08_m_array[991] = { 0, };
	int _02_08_s_array[991] = { 0, };
	
	int _02_09_h_array[991] = { 0, };
	int _02_09_m_array[991] = { 0, };
	int _02_09_s_array[991] = { 0, };
	
	int _02_10_h_array[991] = { 0, };
	int _02_10_m_array[991] = { 0, };
	int _02_10_s_array[991] = { 0, };
	
	int _02_11_h_array[991] = { 0, };
	int _02_11_m_array[991] = { 0, };
	int _02_11_s_array[991] = { 0, };
	
	int _02_12_h_array[991] = { 0, };
	int _02_12_m_array[991] = { 0, };
	int _02_12_s_array[991] = { 0, };
	
	int _02_13_h_array[991] = { 0, };
	int _02_13_m_array[991] = { 0, };
	int _02_13_s_array[991] = { 0, };
	
	int _02_14_h_array[991] = { 0, };
	int _02_14_m_array[991] = { 0, };
	int _02_14_s_array[991] = { 0, };
	
	int _02_15_h_array[991] = { 0, };
	int _02_15_m_array[991] = { 0, };
	int _02_15_s_array[991] = { 0, };
	
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

	// h m s 총 더한 변수
	double Sum_02_08_m = 0;
	double Sum_02_08_s = 0;
	double Sum_02_09_m = 0;
	double Sum_02_09_s = 0;
	double Sum_02_10_m = 0;
	double Sum_02_10_s = 0;
	double Sum_02_11_m = 0;
	double Sum_02_11_s = 0;
	double Sum_02_12_m = 0;
	double Sum_02_12_s = 0;
	double Sum_02_13_m = 0;
	double Sum_02_13_s = 0;
	double Sum_02_14_m = 0;
	double Sum_02_14_s = 0;
	double Sum_02_15_m = 0;
	double Sum_02_15_s = 0;
	
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
			ChartDatas.Backup_Method_Ratio_Pie_Chart_Archive_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Differential Backup")==0)
		{
			ChartDatas.Backup_Method_Ratio_Pie_Chart_Differential_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Dump Backup")==0)
		{
			ChartDatas.Backup_Method_Ratio_Pie_Chart_Dump_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Full Backup")==0)
		{
			ChartDatas.Backup_Method_Ratio_Pie_Chart_Full_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Incremental Backup")==0)
		{
			ChartDatas.Backup_Method_Ratio_Pie_Chart_Incremental_Backup_Count++;
		}
		if (strcmp(Backup_Method,"Synthetic")==0)
		{
			ChartDatas.Backup_Method_Ratio_Pie_Chart_Synthetic_Count++;
		}

		// 작업 완료 수, 에러 통계 데이터 가공 (Home UI, Error UI)
		if (strcmp(Job_Status,"Canceled")==0)
		{
			ChartDatas.Total_Error_Ratio_PieChart_Total_Error_Count++;	// Error UI
			ChartDatas.Error_Ratio_By_Job_Status_PieChart_Canceled_Error_Count++; // Error UI
			
		}
		if (strcmp(Job_Status,"Completed")==0)
		{
			// ChartDatas.Total_Backup_Count_LineChart_Job_Status_Completed_Count++;	// Home UI
			ChartDatas.Total_Error_Ratio_PieChart_Total_Completed_Count++;	// Error UI
			if (strcmp(End_Time,"2022-02-08")==0)
				ChartDatas.Total_Backup_Count_LineChart_2022_02_08_Completed_Count++;
			if (strcmp(End_Time,"2022-02-09")==0)
				ChartDatas.Total_Backup_Count_LineChart_2022_02_09_Completed_Count++;
			if (strcmp(End_Time,"2022-02-10")==0)
				ChartDatas.Total_Backup_Count_LineChart_2022_02_10_Completed_Count++;
			if (strcmp(End_Time,"2022-02-11")==0)
				ChartDatas.Total_Backup_Count_LineChart_2022_02_11_Completed_Count++;
			if (strcmp(End_Time,"2022-02-12")==0)
				ChartDatas.Total_Backup_Count_LineChart_2022_02_12_Completed_Count++;
			if (strcmp(End_Time,"2022-02-13")==0)
				ChartDatas.Total_Backup_Count_LineChart_2022_02_13_Completed_Count++;
			if (strcmp(End_Time,"2022-02-14")==0)
				ChartDatas.Total_Backup_Count_LineChart_2022_02_14_Completed_Count++;
			if (strcmp(End_Time,"2022-02-15")==0)
				ChartDatas.Total_Backup_Count_LineChart_2022_02_15_Completed_Count++;
		}
		if (strcmp(Job_Status,"Failed")==0)
		{
			ChartDatas.Total_Error_Ratio_PieChart_Total_Error_Count++;	// Error UI
			ChartDatas.Error_Ratio_By_Job_Status_PieChart_Failed_Error_Count++;
		}
		if (strcmp(Job_Status,"Partially Completed")==0)
		{
			ChartDatas.Total_Error_Ratio_PieChart_Total_Error_Count++;	// Error UI
			ChartDatas.Error_Ratio_By_Job_Status_PieChart_Partially_Completed_Count++;
		}
		if (strcmp(Job_Status,"Suspended")==0)
		{
			ChartDatas.Total_Error_Ratio_PieChart_Total_Error_Count++;	// Error UI
			ChartDatas.Error_Ratio_By_Job_Status_PieChart_Suspended_Error_Count++;	// Error UI
		}
		
		// 작업 종류 수 통계 위한 데이터 가공 (Statistics UI)
		if (strcmp(Job_Type,"File Backup")==0)
		{
			ChartDatas.JobType_PieChart_File_Backup_Count++;
		}
		if (strcmp(Job_Type,"Informix Onbar Backup")==0)
		{
			ChartDatas.JobType_PieChart_Informix_Onbar_Backup_Count++;
		}
		if (strcmp(Job_Type,"Mysql Backup")==0)
		{
			ChartDatas.JobType_PieChart_Mysql_Backup_Count++;
		}
		if (strcmp(Job_Type,"Oracle RMAN Backup")==0)
		{
			ChartDatas.JobType_PieChart_Oracle_RMAN_Backup_Count++;
		}
		if (strcmp(Job_Type,"Physical Backup")==0)
		{
			ChartDatas.JobType_PieChart_Physical_Backup_Count++;
		}
		if (strcmp(Job_Type,"Vmware Backup")==0)
		{
			ChartDatas.JobType_PieChart_Vm_Ware_Backup_Count++;
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
			_02_15_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_15_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_15_s_array[i] = atoi(ptr);
			_02_15_count++;
		}

		// 평균 처리시간 계산위해 총 더한 값들 
		Sum_02_08_m += _02_08_m_array[i];
		Sum_02_08_s += _02_08_s_array[i];
		Sum_02_09_m += _02_09_m_array[i];
		Sum_02_09_s += _02_09_s_array[i];
		Sum_02_10_m += _02_10_m_array[i];
		Sum_02_10_s += _02_10_s_array[i];
		Sum_02_11_m += _02_11_m_array[i];
		Sum_02_11_s += _02_11_s_array[i];
		Sum_02_12_m += _02_12_m_array[i];
		Sum_02_12_s += _02_12_s_array[i];
		Sum_02_13_m += _02_13_m_array[i];
		Sum_02_13_s += _02_13_s_array[i];
		Sum_02_14_m += _02_14_m_array[i];
		Sum_02_14_s += _02_14_s_array[i];
		Sum_02_15_m += _02_15_m_array[i];
		Sum_02_15_s += _02_15_s_array[i];

		// GB, MB, KB 구분 위한 데이터 가공 (Statistics UI)
		char *f_ptr2 = strtok(array[i].Files_Size," ");
		char *f_ptr3 = strtok(NULL," ");
		
		// GB만 추출해서 저장
		if (strcmp(f_ptr3,"GB")==0)
		{
			t_f_c++;
			ChartDatas.File_Statistics_PieChart_Total_File_Size += atof(f_ptr2);
		}
		char *w_ptr2 = strtok(array[i].Write_Size," ");
		char *w_ptr3 = strtok(NULL," ");

		if (strcmp(w_ptr3,"GB")==0)
		{
			ChartDatas.File_Statistics_PieChart_Total_Write_Size += atof(w_ptr2);
			t_w_c++;
		}
	}
	ChartDatas.Avg_Elapsed_Time_LineChart_2022_02_08_Avg_Elapsed_Times = ((Sum_02_08_m+(Sum_02_08_s/60))/_02_08_count)*60;
	ChartDatas.Avg_Elapsed_Time_LineChart_2022_02_09_Avg_Elapsed_Times = ((Sum_02_09_m+(Sum_02_09_s/60))/_02_09_count)*60;
	ChartDatas.Avg_Elapsed_Time_LineChart_2022_02_10_Avg_Elapsed_Times = ((Sum_02_10_m+(Sum_02_10_s/60))/_02_10_count)*60;
	ChartDatas.Avg_Elapsed_Time_LineChart_2022_02_11_Avg_Elapsed_Times = ((Sum_02_11_m+(Sum_02_11_s/60))/_02_11_count)*60;
	ChartDatas.Avg_Elapsed_Time_LineChart_2022_02_12_Avg_Elapsed_Times = ((Sum_02_12_m+(Sum_02_12_s/60))/_02_12_count)*60;
	ChartDatas.Avg_Elapsed_Time_LineChart_2022_02_13_Avg_Elapsed_Times = ((Sum_02_13_m+(Sum_02_13_s/60))/_02_13_count)*60;
	ChartDatas.Avg_Elapsed_Time_LineChart_2022_02_14_Avg_Elapsed_Times = ((Sum_02_14_m+(Sum_02_14_s/60))/_02_14_count)*60;
	ChartDatas.Avg_Elapsed_Time_LineChart_2022_02_15_Avg_Elapsed_Times = ((Sum_02_15_m+(Sum_02_15_s/60))/_02_15_count)*60;
	
	ChartDatas.Backup_Method_Ratio_Pie_Chart_Total_Count = Total_row_count;
	ChartDatas.Total_Error_Ratio_PieChart_Total_Count = Total_row_count;
	ChartDatas.JobType_PieChart_Total_Count = Total_row_count;
	ChartDatas.Error_Ratio_By_Job_Status_PieChart_Total_Error_Count = ChartDatas.Total_Error_Ratio_PieChart_Total_Error_Count;
	ChartDatas.File_Statistics_PieChart_Total_Size = ChartDatas.File_Statistics_PieChart_Total_File_Size + ChartDatas.File_Statistics_PieChart_Total_Write_Size;

	// 가공한 데이터 저장 (.dat)
	// 파일 변수 선언
	FILE *_Filtering_Datas;
	FILE *_ChartDatas;
	
	// 파일에 저장하기 위한 파일을 엽니다. 
	//_Filtering_Datas = fopen("FilterData.dat","w");
	_ChartDatas = fopen("ChartDatas.dat","w");

	
	//if (_Filtering_Datas == NULL)
	//{
	//	fprintf(stderr, "\nError Opened Files\n");
	//	exit(1);
	//}

	if (_ChartDatas == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}

	// 구조체 내용을 파일에 저장한다. 
	//fwrite(&Filtering_Datas, sizeof(struct Filtering_Data), 1, _Filtering_Datas);
	fwrite(&ChartDatas, sizeof(struct ChartData), 1, _ChartDatas);

	if (fwrite != 0)
		printf("Contents to file Written Successfully !\n");
	else
		printf("Error Writing file !\n");

	// 파일을 닫아준다. 
	//fclose(_Filtering_Datas);
	fclose(_ChartDatas);

	// 소켓 통신 시작 
	// 소켓 구조체를 선언하고 IP 주소 및 포트를 설정 
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr)); // 모든 내용을 0으로 초기화 
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
	// time_t now;
	FILE *stream;	// For Chart Struct 
	FILE *stream1;	// For Filtering Struct

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
		printf("%s\n",buffer);
		
		// For Chart Data
		if((stream = fopen("ChartDatas.dat","r"))==NULL)
		{
			printf("The file 'ChartDatas.dat' was not opened!\n");
			// exit(1);
		}
		else
		{
			printf("The file 'ChartDatas.dat' was opened!\n");
		}
		bzero(buffer,BUFFER_SIZE);
		int lengsize = 0;

		// 파일 구조체 정보 먼저 전송
		strcpy(File_Infos.File_Name,"ChartDatas.dat");
		File_Infos.FileNameLen = strlen(File_Infos.File_Name);
		
		send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);

		while((lengsize = fread(buffer,1,1024,stream)) > 0)
		{
			printf("lengsize = %d\n",lengsize);
			if(send(new_server_socket,buffer,lengsize,0)<0)
			{
				printf("Send File(ChartDatas.dat) is Failed\n");
				break;
			}
			bzero(buffer, BUFFER_SIZE);
		}
		if(fclose(stream))
		{
			printf("The file 'ChartDatas.dat' was not closed! \n");
			// exit(1);    
		}
		
		// For Filtering data
		//if((stream1 = fopen("FilterData.dat","r"))==NULL)
		//{
		//	printf("The file 'FilterData.dat' was not opened! \n");
		//	exit(1);
		//}
		//else
		//{
		//	printf("The file 'FilterData.dat' was opened! \n");
		//}
		//bzero(buffer,BUFFER_SIZE);
		//lengsize = 0;

		// 파일 구조체 정보 먼저 전송
		//strcpy(File_Infos.File_Name,"FilterData.dat");
		//File_Infos.FileNameLen = strlen(File_Infos.File_Name);
		//send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);

		//while((lengsize = fread(buffer,1,1024,stream1)) > 0)
		//{
		//	printf("lengsize = %d\n",lengsize);
		//	if(send(new_server_socket,buffer,lengsize,0)<0)
		//	{
		//		printf("Send File(FilterData.dat) is Failed\n");
		//		break;
		//	}
		//	bzero(buffer, BUFFER_SIZE);
		//}
		//if(fclose(stream1))
		//{
		//	printf("The file 'FilterData.dat' was not closed! \n");
			// exit(1);    
		//}
		
		// 클라이언트와의 연결을 종료합니다.
		close(new_server_socket);    
	}
	// 수신을 위한 서버소켓을 종료합니다. 
	close(server_socket);
	return 0;
}
