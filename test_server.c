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
#define HELLO_WORLD_SERVER_PORT 7756
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024

// Login Info
typedef struct Login
{
	char UserName[20];
	char Password[20]; 
}Login;
Login Login_Data;

// get the file size 
struct stat st;
// File_Information
#pragma pack(push, 1)    // 1바이트 크기로 정렬
typedef struct File_Info
{
	int FileNameLen;
	char File_Name[15]; 
	int File_Size;
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
	char Job_Status[50];
	char Job_Type[50];
	char Server[50];
	char Client[50];
	char Schedule[50];
	char Files[50];
} Filtering_Data;
//Filtering_Data Filtering_Datas[991];

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
	int File_Statistics_PieChart_Total_Data_Transferred;	// (2022-03-24 수정 )
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
	// 03-30 추가 
	int Error_02_08_Count;
	int Error_02_09_Count;
	int Error_02_10_Count;
	int Error_02_11_Count;
	int Error_02_12_Count;
	int Error_02_13_Count;
	int Error_02_14_Count;
	int Error_02_15_Count;

	// 에러 UI, 작업 종류별 에러타입을 위한 데이터 구조체 선언 
	int Error_Ratio_By_Job_Status_PieChart_Total_Error_Count;
	int Error_Ratio_By_Job_Status_PieChart_Partially_Completed_Count;
	int Error_Ratio_By_Job_Status_PieChart_Suspended_Error_Count;
	int Error_Ratio_By_Job_Status_PieChart_Failed_Error_Count;
	int Error_Ratio_By_Job_Status_PieChart_Canceled_Error_Count;

	// 스케줄 별 개수 표시 (2022-03-24 추가)
	int Schedule_testsc1_Count;
	int Schedule_testsc2_Count;
	int Schedule_testsc3_Count;
	int Schedule_testsc4_Count;

	// 총 데이터 백업한 파일 개수 (2022-03-24 추가)
	int Total_Files_Count;

	// 일별 파일 사이즈 (2022-03-24 추가)
	int Total_File_Size_LineChart_2022_02_08_Count;
	int Total_File_Size_LineChart_2022_02_09_Count;
	int Total_File_Size_LineChart_2022_02_10_Count;
	int Total_File_Size_LineChart_2022_02_11_Count;
	int Total_File_Size_LineChart_2022_02_12_Count;
	int Total_File_Size_LineChart_2022_02_13_Count;
	int Total_File_Size_LineChart_2022_02_14_Count;
	int Total_File_Size_LineChart_2022_02_15_Count;

	// 일별 저장 사이즈 (2022-03-24 추가)
	int Total_Write_Size_LineChart_2022_02_08_Count;
	int Total_Write_Size_LineChart_2022_02_09_Count;
	int Total_Write_Size_LineChart_2022_02_10_Count;
	int Total_Write_Size_LineChart_2022_02_11_Count;
	int Total_Write_Size_LineChart_2022_02_12_Count;
	int Total_Write_Size_LineChart_2022_02_13_Count;
	int Total_Write_Size_LineChart_2022_02_14_Count;
	int Total_Write_Size_LineChart_2022_02_15_Count;

	// 일별 전송 사이즈 (2022-03-24 추가)
	int Total_Data_Transferred_LineChart_2022_02_08_Count;
	int Total_Data_Transferred_LineChart_2022_02_09_Count;
	int Total_Data_Transferred_LineChart_2022_02_10_Count;
	int Total_Data_Transferred_LineChart_2022_02_11_Count;
	int Total_Data_Transferred_LineChart_2022_02_12_Count;
	int Total_Data_Transferred_LineChart_2022_02_13_Count;
	int Total_Data_Transferred_LineChart_2022_02_14_Count;
	int Total_Data_Transferred_LineChart_2022_02_15_Count;
} ChartData;
ChartData ChartDatas;

int main(int argc, char **argv)
{
	int Filter_array_count = 0;
	Filtering_Data Filtering_Datas[991] = { 0 };
	char UserName[20] = "BA_infra";
	char Password[20] = "BA_infra123";
	strcpy(Login_Data.UserName,UserName);
	strcpy(Login_Data.Password,Password);
	 
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
	// 스케줄 데이터 가공 위한 변수 선언 
	char Schedule[300];
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
	// File_Size 와 Write_Size DataTransferred 위한 변수 
	int t_f_c = 0;
	int t_w_c = 0;
	int t_d_c = 0;

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
	
	// 소수점 Write, File_Size, Data_Transferred 처리 위한 변수 
	float float_Total_File_Size;
	float float_Total_Write_Size;
	float float_Total_Data_Transferred;	// (2022-03-24 수정 )

	// 일별 파일 사이즈 (2022-03-24 추가)
	float Total_File_Size_LineChart_2022_02_08_Count;
	float Total_File_Size_LineChart_2022_02_09_Count;
	float Total_File_Size_LineChart_2022_02_10_Count;
	float Total_File_Size_LineChart_2022_02_11_Count;
	float Total_File_Size_LineChart_2022_02_12_Count;
	float Total_File_Size_LineChart_2022_02_13_Count;
	float Total_File_Size_LineChart_2022_02_14_Count;
	float Total_File_Size_LineChart_2022_02_15_Count;

	// 일별 저장 사이즈 (2022-03-24 추가)
	float Total_Write_Size_LineChart_2022_02_08_Count;
	float Total_Write_Size_LineChart_2022_02_09_Count;
	float Total_Write_Size_LineChart_2022_02_10_Count;
	float Total_Write_Size_LineChart_2022_02_11_Count;
	float Total_Write_Size_LineChart_2022_02_12_Count;
	float Total_Write_Size_LineChart_2022_02_13_Count;
	float Total_Write_Size_LineChart_2022_02_14_Count;
	float Total_Write_Size_LineChart_2022_02_15_Count;

	// 일별 전송 사이즈 (2022-03-24 추가)
	float Total_Data_Transferred_LineChart_2022_02_08_Count;
	float Total_Data_Transferred_LineChart_2022_02_09_Count;
	float Total_Data_Transferred_LineChart_2022_02_10_Count;
	float Total_Data_Transferred_LineChart_2022_02_11_Count;
	float Total_Data_Transferred_LineChart_2022_02_12_Count;
	float Total_Data_Transferred_LineChart_2022_02_13_Count;
	float Total_Data_Transferred_LineChart_2022_02_14_Count;
	float Total_Data_Transferred_LineChart_2022_02_15_Count;


	for(int i=0;i<Total_row_count;i++)						
	{
		strcpy(Backup_Method,array[i].Backup_Method); // 백업 메소드 통계 데이터 가공을 위한 변수 값 복사 (Home UI)
		strcpy(Job_Status,array[i].Job_Status); // 작업완료 수 통계 데이터 가공을 위한 변수 값 복사 (Home UI), 에러 통계 데이터 가공을 위한 변수 값 복사 (Error UI)
		strcpy(End_Time,array[i].End_Time);	// 완료일자 계산 위한 변수 값 복사 
		strcpy(Job_Type,array[i].Job_Type); // 작업 종류 통계 데이터 가공을 위한 변수 값 복사 (Statistics UI)
		strcpy(Schedule,array[i].Schedule); // 스케줄 데이터 가공을 위한 변수 값 복사 
		
		// GB, MB, KB 구분 위한 데이터 가공 (Statistics UI)
		// GB만 추출해서 저장
		// 파일 사이즈(File_Size) 가공 
		char *f_ptr2 = strtok(array[i].Files_Size," ");
		char *f_ptr3 = strtok(NULL," ");
		
		if (strcmp(f_ptr3,"GB")==0)
		{
			t_f_c++;
			float_Total_File_Size += atof(f_ptr2);
		}
		char *w_ptr2 = strtok(array[i].Write_Size," ");
		char *w_ptr3 = strtok(NULL," ");

		// 저장 사이즈(Write_Size) 가공 
		if (strcmp(w_ptr3,"GB")==0)
		{
			float_Total_Write_Size += atof(w_ptr2);
			t_w_c++;
		}
		// 전송 크기(DataTransferred_Size) 가공
		char *d_ptr2 = strtok(array[i].Data_Transferred," ");
		char *d_ptr3 = strtok(NULL," ");
		if (strcmp(d_ptr3,"GB")==0)
		{
			float_Total_Data_Transferred += atof(d_ptr2);
			t_d_c++;
		}

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
			if (strcmp(Job_Status,"Completed")!=0)
			{
				ChartDatas.Error_02_08_Count++;
			}
			if (strcmp(f_ptr3,"GB")==0)
			{
				Total_File_Size_LineChart_2022_02_08_Count += atof(f_ptr2);
			}
			if (strcmp(w_ptr3,"GB")==0)
			{
				Total_Write_Size_LineChart_2022_02_08_Count += atof(w_ptr2);

			}
			if (strcmp(d_ptr3,"GB")==0)
			{
				Total_Data_Transferred_LineChart_2022_02_08_Count += atof(d_ptr2);
			}
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
			if (strcmp(Job_Status,"Completed")!=0)
			{
				ChartDatas.Error_02_09_Count++;
			}
			if (strcmp(f_ptr3,"GB")==0)
			{
				Total_File_Size_LineChart_2022_02_09_Count += atof(f_ptr2);
			}
			if (strcmp(w_ptr3,"GB")==0)
			{
				Total_Write_Size_LineChart_2022_02_09_Count += atof(w_ptr2);

			}
			if (strcmp(d_ptr3,"GB")==0)
			{
				Total_Data_Transferred_LineChart_2022_02_09_Count += atof(d_ptr2);
			}
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
			if (strcmp(Job_Status,"Completed")!=0)
			{
				ChartDatas.Error_02_10_Count++;
			}
			if (strcmp(f_ptr3,"GB")==0)
			{
				Total_File_Size_LineChart_2022_02_10_Count += atof(f_ptr2);
			}
			if (strcmp(w_ptr3,"GB")==0)
			{
				Total_Write_Size_LineChart_2022_02_10_Count += atof(w_ptr2);

			}
			if (strcmp(d_ptr3,"GB")==0)
			{
				Total_Data_Transferred_LineChart_2022_02_10_Count += atof(d_ptr2);
			}
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
			if (strcmp(Job_Status,"Completed")!=0)
			{
				ChartDatas.Error_02_11_Count++;
			}
			if (strcmp(f_ptr3,"GB")==0)
			{
				Total_File_Size_LineChart_2022_02_11_Count += atof(f_ptr2);
			}
			if (strcmp(w_ptr3,"GB")==0)
			{
				Total_Write_Size_LineChart_2022_02_11_Count += atof(w_ptr2);

			}
			if (strcmp(d_ptr3,"GB")==0)
			{
				Total_Data_Transferred_LineChart_2022_02_11_Count += atof(d_ptr2);
			}
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
			if (strcmp(Job_Status,"Completed")!=0)
			{
				ChartDatas.Error_02_12_Count++;
			}
			if (strcmp(f_ptr3,"GB")==0)
			{
				Total_File_Size_LineChart_2022_02_12_Count += atof(f_ptr2);
			}
			if (strcmp(w_ptr3,"GB")==0)
			{
				Total_Write_Size_LineChart_2022_02_12_Count += atof(w_ptr2);

			}
			if (strcmp(d_ptr3,"GB")==0)
			{
				Total_Data_Transferred_LineChart_2022_02_12_Count += atof(d_ptr2);
			}
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
			if (strcmp(Job_Status,"Completed")!=0)
			{
				ChartDatas.Error_02_13_Count++;
			}
			if (strcmp(f_ptr3,"GB")==0)
			{
				Total_File_Size_LineChart_2022_02_13_Count += atof(f_ptr2);
			}
			if (strcmp(w_ptr3,"GB")==0)
			{
				Total_Write_Size_LineChart_2022_02_13_Count += atof(w_ptr2);

			}
			if (strcmp(d_ptr3,"GB")==0)
			{
				Total_Data_Transferred_LineChart_2022_02_13_Count += atof(d_ptr2);
			}
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
			if (strcmp(Job_Status,"Completed")!=0)
			{
				ChartDatas.Error_02_14_Count++;
			}
			if (strcmp(f_ptr3,"GB")==0)
			{
				Total_File_Size_LineChart_2022_02_14_Count += atof(f_ptr2);
			}
			if (strcmp(w_ptr3,"GB")==0)
			{
				Total_Write_Size_LineChart_2022_02_14_Count += atof(w_ptr2);

			}
			if (strcmp(d_ptr3,"GB")==0)
			{
				Total_Data_Transferred_LineChart_2022_02_14_Count += atof(d_ptr2);
			}
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
			if (strcmp(Job_Status,"Completed")!=0)
			{
				ChartDatas.Error_02_15_Count++;
			}
			if (strcmp(f_ptr3,"GB")==0)
			{
				Total_File_Size_LineChart_2022_02_15_Count += atof(f_ptr2);
			}
			if (strcmp(w_ptr3,"GB")==0)
			{
				Total_Write_Size_LineChart_2022_02_15_Count += atof(w_ptr2);

			}
			if (strcmp(d_ptr3,"GB")==0)
			{
				Total_Data_Transferred_LineChart_2022_02_15_Count += atof(d_ptr2);
			}
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

		// schedule 데이터 가공
		if (strcmp(Schedule,"testsc_1")==0)
		{
			ChartDatas.Schedule_testsc1_Count++;
		}
		if (strcmp(Schedule,"testsc_2")==0)
		{
			ChartDatas.Schedule_testsc2_Count++;
		}
		if (strcmp(Schedule,"testsc_3")==0)
		{
			ChartDatas.Schedule_testsc3_Count++;
		}
		if (strcmp(Schedule,"testsc_4")==0)
		{
			ChartDatas.Schedule_testsc4_Count++;
		}
		// 파일 총 개수 가공
		ChartDatas.Total_Files_Count += atoi(array[i].Files);
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

	ChartDatas.File_Statistics_PieChart_Total_File_Size = (int)float_Total_File_Size; 
	ChartDatas.File_Statistics_PieChart_Total_Write_Size = (int)float_Total_Write_Size;
	ChartDatas.File_Statistics_PieChart_Total_Data_Transferred= (int)float_Total_Data_Transferred;

	// 일별 파일 사이즈 (2022-03-24 추가)
	ChartDatas.Total_File_Size_LineChart_2022_02_08_Count = (int)Total_File_Size_LineChart_2022_02_08_Count;
	ChartDatas.Total_File_Size_LineChart_2022_02_09_Count = (int)Total_File_Size_LineChart_2022_02_09_Count;
	ChartDatas.Total_File_Size_LineChart_2022_02_10_Count = (int)Total_File_Size_LineChart_2022_02_10_Count;
	ChartDatas.Total_File_Size_LineChart_2022_02_11_Count = (int)Total_File_Size_LineChart_2022_02_11_Count;
	ChartDatas.Total_File_Size_LineChart_2022_02_12_Count = (int)Total_File_Size_LineChart_2022_02_12_Count;
	ChartDatas.Total_File_Size_LineChart_2022_02_13_Count = (int)Total_File_Size_LineChart_2022_02_13_Count;
	ChartDatas.Total_File_Size_LineChart_2022_02_14_Count = (int)Total_File_Size_LineChart_2022_02_14_Count;
	ChartDatas.Total_File_Size_LineChart_2022_02_15_Count = (int)Total_File_Size_LineChart_2022_02_15_Count;

	// 일별 저장 사이즈 (2022-03-24 추가)
	ChartDatas.Total_Write_Size_LineChart_2022_02_08_Count = (int)Total_Write_Size_LineChart_2022_02_08_Count;
	ChartDatas.Total_Write_Size_LineChart_2022_02_09_Count = (int)Total_Write_Size_LineChart_2022_02_09_Count;
	ChartDatas.Total_Write_Size_LineChart_2022_02_10_Count = (int)Total_Write_Size_LineChart_2022_02_10_Count;
	ChartDatas.Total_Write_Size_LineChart_2022_02_11_Count = (int)Total_Write_Size_LineChart_2022_02_11_Count;
	ChartDatas.Total_Write_Size_LineChart_2022_02_12_Count = (int)Total_Write_Size_LineChart_2022_02_12_Count;
	ChartDatas.Total_Write_Size_LineChart_2022_02_13_Count = (int)Total_Write_Size_LineChart_2022_02_13_Count;
	ChartDatas.Total_Write_Size_LineChart_2022_02_14_Count = (int)Total_Write_Size_LineChart_2022_02_14_Count;
	ChartDatas.Total_Write_Size_LineChart_2022_02_15_Count = (int)Total_Write_Size_LineChart_2022_02_15_Count;

	// 일별 전송 사이즈 (2022-03-24 추가)
	ChartDatas.Total_Data_Transferred_LineChart_2022_02_08_Count = (int)Total_Data_Transferred_LineChart_2022_02_08_Count;
	ChartDatas.Total_Data_Transferred_LineChart_2022_02_09_Count = (int)Total_Data_Transferred_LineChart_2022_02_09_Count;
	ChartDatas.Total_Data_Transferred_LineChart_2022_02_10_Count = (int)Total_Data_Transferred_LineChart_2022_02_10_Count;
	ChartDatas.Total_Data_Transferred_LineChart_2022_02_11_Count = (int)Total_Data_Transferred_LineChart_2022_02_11_Count;
	ChartDatas.Total_Data_Transferred_LineChart_2022_02_12_Count = (int)Total_Data_Transferred_LineChart_2022_02_12_Count;
	ChartDatas.Total_Data_Transferred_LineChart_2022_02_13_Count = (int)Total_Data_Transferred_LineChart_2022_02_13_Count;
	ChartDatas.Total_Data_Transferred_LineChart_2022_02_14_Count = (int)Total_Data_Transferred_LineChart_2022_02_14_Count;
	ChartDatas.Total_Data_Transferred_LineChart_2022_02_15_Count = (int)Total_Data_Transferred_LineChart_2022_02_15_Count;

	// 가공한 데이터 저장 (.dat)
	// 파일 변수 선언
	FILE *_Filtering_Datas;
	FILE *_ChartDatas;
	
	// 파일에 저장하기 위한 파일을 엽니다. 
	_ChartDatas = fopen("ChartDatas.dat","w");

	if (_ChartDatas == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		exit(1);
	}

	// 구조체 내용을 파일에 저장한다. 
	fwrite(&ChartDatas, sizeof(struct ChartData), 1, _ChartDatas);

	if (fwrite != 0)
		printf("Contents to file Written Successfully !\n");
	else
		printf("Error Writing file !\n");

	// 파일을 닫아준다. 
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
	FILE *Filterstream;	// For Filtering Struct

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
		printf("new_server_socket = %d\n",new_server_socket);
		if ( new_server_socket < 0)
		{
			printf("Server Accept Failed!\n");
			//break;
		}
		char buffer[BUFFER_SIZE];
		bzero(buffer, BUFFER_SIZE);
		strcpy(buffer,"Hello,World! From the server! ");
		strcat(buffer,"\n"); // C 문자열 연결
		send(new_server_socket,buffer,BUFFER_SIZE,0);
		
		bzero(buffer,BUFFER_SIZE);
		// 클라이언트가 버퍼로 보낸 정보 수신
		length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
		if (length < 0)
		{
			printf("Server Recieve Data Failed!\n");
			// exit(1);
		}
		printf("%s\n",buffer);	// 처음이 Connected with Client 인데 없애자 
		

		//////////////////////////////////////////////////////
		// 로그인 처리 
		if (strcmp(buffer,"Login")==0)
		{
			bzero(buffer,BUFFER_SIZE);
			// 로그인 아이디 및 비밀번호 전송 받는다.
			length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
			if (length < 0)
			{
				printf("Server Recieve Login ID Data Failed!\n");
				//exit(1);
			}
			// ID Check 
			// ID, Password 한번에 체크할지 여부 고민 
			if (strcmp(buffer, UserName)==0)
			{
				printf("ID is Correct!\n");
				bzero(buffer,BUFFER_SIZE);
				
				// Password Check
				length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
				if (length < 0)
				{
					printf("Server Recieve Login Password Data Failed!\n");
					//exit(1);
				}
				else if (strcmp(buffer, Password)==0)
				{
					printf("Password is Correct!\n");
					strcpy(buffer,"Login_Success");
					strcat(buffer,"\n"); // C 문자열 연결
					send(new_server_socket,buffer,BUFFER_SIZE,0);
					bzero(buffer,BUFFER_SIZE);
					bzero(buffer,BUFFER_SIZE);
					//exit(1);
				}
				else
				{
					printf("Password is UnMatching\n");
				}
			}
			else 
			{
				printf("ID is UnMatching\n");
			}
		}
		////////////////////////////////////////////////////////// 


		//////////////////////////////////////////////////////////
		// Filtering 처리
		if (strcmp(buffer,"Filtering_Data")==0)
		{
			_Filtering_Datas = fopen("FilterData.csv","w");
			int Filter_array_count = 0;

			printf("Filtering_Data Enter!\n");
			bzero(buffer,BUFFER_SIZE);
			
			usleep(100000);	// 0.5 초 기다리기  ,이건 0.1초
			length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
			if (length < 0)
			{
				printf("Server Recieve Filtering Data Failed!\n");
				//exit(1);
			}

			/////////////////////////////////////////////////////////////////
			// 검색 기준이 File Backup 이라면 			
			if (strcmp(buffer, "File Backup")==0)
			{
				printf("검색 기준은 File Backup 입니다.\n");
				for(int i=0;i<Total_row_count;i++)
				{
					// Job Type == File Backup
					if (strcmp(array[i].Job_Type,"File Backup")==0)
					{
						strcpy(Filtering_Datas[Filter_array_count].Job_Status,array[i].Job_Status);
						strcpy(Filtering_Datas[Filter_array_count].Job_Type,array[i].Job_Type);
						strcpy(Filtering_Datas[Filter_array_count].Server,array[i].Server);
						strcpy(Filtering_Datas[Filter_array_count].Client,array[i].Client);
						strcpy(Filtering_Datas[Filter_array_count].Schedule,array[i].Schedule);
						strcpy(Filtering_Datas[Filter_array_count].Files,array[i].Files);
						Filter_array_count++;
					}
				}

				for (int i=0; i<Filter_array_count; i++)
				{
					fprintf(_Filtering_Datas,"%s,%s,%s,%s,%s,%s\n", Filtering_Datas[i].Job_Status,Filtering_Datas[i].Job_Type,
					Filtering_Datas[i].Server,Filtering_Datas[i].Client,
					Filtering_Datas[i].Schedule,Filtering_Datas[i].Files);
				}
				fclose(_Filtering_Datas);

				if((Filterstream = fopen("FilterData.csv","r"))==NULL)
				{
					printf("The file 'FilterData.csv' was not opened!\n");
					// exit(1);
				}
				else
				{
					printf("The file 'FilterData.csv' was opened!\n");
				}
				bzero(buffer,BUFFER_SIZE);
				int lengsize = 0;

				// 파일 구조체 정보 할당 
				strcpy(File_Infos.File_Name,"FilterData.csv");
				File_Infos.FileNameLen = strlen(File_Infos.File_Name);
				char filename[50];
				strcpy(filename,"FilterData.csv");
				stat(filename, &st);
				int size = st.st_size;
				File_Infos.File_Size = size;
				printf("file size = %d\n",File_Infos.File_Size);
				// 파일 구조체 정보 먼저 전송
				send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);

				// 파일 내용 전송 
				while((lengsize = fread(buffer,1,1024,Filterstream)) > 0)
				{
					printf("lengsize = %d\n",lengsize);
					if(send(new_server_socket,buffer,lengsize,0)<0)
					{
						printf("Send File(FilterData.csv) is Failed\n");
						// break;
					}
					bzero(buffer, BUFFER_SIZE);
				}
				if(fclose(Filterstream))
				{
					printf("The file 'FilterData.csv' was not closed! \n");
					// exit(1);    
				}
			}
			/////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////
			// 검색 기준이 Informix Onbar Backup 이라면 			
			else if (strcmp(buffer, "Informix Onbar Backup")==0)
			{
				printf("검색 기준은 Informix Onbar Backup 입니다.\n");
				for(int i=0;i<Total_row_count;i++)
				{
					// Job Type == File Backup
					if (strcmp(array[i].Job_Type,"Informix Onbar Backup")==0)
					{
						strcpy(Filtering_Datas[Filter_array_count].Job_Status,array[i].Job_Status);
						strcpy(Filtering_Datas[Filter_array_count].Job_Type,array[i].Job_Type);
						strcpy(Filtering_Datas[Filter_array_count].Server,array[i].Server);
						strcpy(Filtering_Datas[Filter_array_count].Client,array[i].Client);
						strcpy(Filtering_Datas[Filter_array_count].Schedule,array[i].Schedule);
						strcpy(Filtering_Datas[Filter_array_count].Files,array[i].Files);
						Filter_array_count++;
					}
				}

				for (int i=0; i<Filter_array_count; i++)
				{
					fprintf(_Filtering_Datas,"%s,%s,%s,%s,%s,%s\n", Filtering_Datas[i].Job_Status,Filtering_Datas[i].Job_Type,
					Filtering_Datas[i].Server,Filtering_Datas[i].Client,
					Filtering_Datas[i].Schedule,Filtering_Datas[i].Files);
				}
				fclose(_Filtering_Datas);

				if((Filterstream = fopen("FilterData.csv","r"))==NULL)
				{
					printf("The file 'FilterData.csv' was not opened!\n");
					// exit(1);
				}
				else
				{
					printf("The file 'FilterData.csv' was opened!\n");
				}
				bzero(buffer,BUFFER_SIZE);
				int lengsize = 0;

				// 파일 구조체 정보 할당 
				strcpy(File_Infos.File_Name,"FilterData.csv");
				File_Infos.FileNameLen = strlen(File_Infos.File_Name);
				char filename[50];
				strcpy(filename,"FilterData.csv");
				stat(filename, &st);
				int size = st.st_size;
				File_Infos.File_Size = size;
				printf("file size = %d\n",File_Infos.File_Size);
				// 파일 구조체 정보 먼저 전송
				send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);

				// 파일 내용 전송 
				while((lengsize = fread(buffer,1,1024,Filterstream)) > 0)
				{
					printf("lengsize = %d\n",lengsize);
					if(send(new_server_socket,buffer,lengsize,0)<0)
					{
						printf("Send File(FilterData.csv) is Failed\n");
						// break;
					}
					bzero(buffer, BUFFER_SIZE);
				}
				if(fclose(Filterstream))
				{
					printf("The file 'FilterData.csv' was not closed! \n");
					// exit(1);    
				}
			}
			/////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////
			// 검색 기준이 Mysql Backup 이라면 			
			else if (strcmp(buffer, "Mysql Backup")==0)
			{
				printf("검색 기준은 Mysql Backup 입니다.\n");
				for(int i=0;i<Total_row_count;i++)
				{
					// Job Type == File Backup
					if (strcmp(array[i].Job_Type,"Mysql Backup")==0)
					{
						strcpy(Filtering_Datas[Filter_array_count].Job_Status,array[i].Job_Status);
						strcpy(Filtering_Datas[Filter_array_count].Job_Type,array[i].Job_Type);
						strcpy(Filtering_Datas[Filter_array_count].Server,array[i].Server);
						strcpy(Filtering_Datas[Filter_array_count].Client,array[i].Client);
						strcpy(Filtering_Datas[Filter_array_count].Schedule,array[i].Schedule);
						strcpy(Filtering_Datas[Filter_array_count].Files,array[i].Files);
						Filter_array_count++;
					}
				}

				for (int i=0; i<Filter_array_count; i++)
				{
					fprintf(_Filtering_Datas,"%s,%s,%s,%s,%s,%s\n", Filtering_Datas[i].Job_Status,Filtering_Datas[i].Job_Type,
					Filtering_Datas[i].Server,Filtering_Datas[i].Client,
					Filtering_Datas[i].Schedule,Filtering_Datas[i].Files);
				}
				fclose(_Filtering_Datas);

				if((Filterstream = fopen("FilterData.csv","r"))==NULL)
				{
					printf("The file 'FilterData.csv' was not opened!\n");
					// exit(1);
				}
				else
				{
					printf("The file 'FilterData.csv' was opened!\n");
				}
				bzero(buffer,BUFFER_SIZE);
				int lengsize = 0;

				// 파일 구조체 정보 할당 
				strcpy(File_Infos.File_Name,"FilterData.csv");
				File_Infos.FileNameLen = strlen(File_Infos.File_Name);
				char filename[50];
				strcpy(filename,"FilterData.csv");
				stat(filename, &st);
				int size = st.st_size;
				File_Infos.File_Size = size;
				printf("file size = %d\n",File_Infos.File_Size);
				// 파일 구조체 정보 먼저 전송
				send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);

				// 파일 내용 전송 
				while((lengsize = fread(buffer,1,1024,Filterstream)) > 0)
				{
					printf("lengsize = %d\n",lengsize);
					if(send(new_server_socket,buffer,lengsize,0)<0)
					{
						printf("Send File(FilterData.csv) is Failed\n");
						// break;
					}
					bzero(buffer, BUFFER_SIZE);
				}
				if(fclose(Filterstream))
				{
					printf("The file 'FilterData.csv' was not closed! \n");
					// exit(1);    
				}
			}
			/////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////
			// 검색 기준이 Oracle RMAN Backup 이라면 			
			else if (strcmp(buffer, "Oracle RMAN Backup")==0)
			{
				printf("검색 기준은 Oracle RMAN Backup 입니다.\n");
				for(int i=0;i<Total_row_count;i++)
				{
					// Job Type == File Backup
					if (strcmp(array[i].Job_Type,"Oracle RMAN Backup")==0)
					{
						strcpy(Filtering_Datas[Filter_array_count].Job_Status,array[i].Job_Status);
						strcpy(Filtering_Datas[Filter_array_count].Job_Type,array[i].Job_Type);
						strcpy(Filtering_Datas[Filter_array_count].Server,array[i].Server);
						strcpy(Filtering_Datas[Filter_array_count].Client,array[i].Client);
						strcpy(Filtering_Datas[Filter_array_count].Schedule,array[i].Schedule);
						strcpy(Filtering_Datas[Filter_array_count].Files,array[i].Files);
						Filter_array_count++;
					}
				}

				for (int i=0; i<Filter_array_count; i++)
				{
					fprintf(_Filtering_Datas,"%s,%s,%s,%s,%s,%s\n", Filtering_Datas[i].Job_Status,Filtering_Datas[i].Job_Type,
					Filtering_Datas[i].Server,Filtering_Datas[i].Client,
					Filtering_Datas[i].Schedule,Filtering_Datas[i].Files);
				}
				fclose(_Filtering_Datas);

				if((Filterstream = fopen("FilterData.csv","r"))==NULL)
				{
					printf("The file 'FilterData.csv' was not opened!\n");
					// exit(1);
				}
				else
				{
					printf("The file 'FilterData.csv' was opened!\n");
				}
				bzero(buffer,BUFFER_SIZE);
				int lengsize = 0;

				// 파일 구조체 정보 할당 
				strcpy(File_Infos.File_Name,"FilterData.csv");
				File_Infos.FileNameLen = strlen(File_Infos.File_Name);
				char filename[50];
				strcpy(filename,"FilterData.csv");
				stat(filename, &st);
				int size = st.st_size;
				File_Infos.File_Size = size;
				printf("file size = %d\n",File_Infos.File_Size);
				// 파일 구조체 정보 먼저 전송
				send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);

				// 파일 내용 전송 
				while((lengsize = fread(buffer,1,1024,Filterstream)) > 0)
				{
					printf("lengsize = %d\n",lengsize);
					if(send(new_server_socket,buffer,lengsize,0)<0)
					{
						printf("Send File(FilterData.csv) is Failed\n");
						// break;
					}
					bzero(buffer, BUFFER_SIZE);
				}
				if(fclose(Filterstream))
				{
					printf("The file 'FilterData.csv' was not closed! \n");
					// exit(1);    
				}
			}
			/////////////////////////////////////////////////////////////
			
			/////////////////////////////////////////////////////////////
			// 검색 기준이 Physical Backup 이라면 			
			else if (strcmp(buffer, "Physical Backup")==0)
			{
				printf("검색 기준은 Physical Backup 입니다.\n");
				for(int i=0;i<Total_row_count;i++)
				{
					// Job Type == File Backup
					if (strcmp(array[i].Job_Type,"Physical Backup")==0)
					{
						strcpy(Filtering_Datas[Filter_array_count].Job_Status,array[i].Job_Status);
						strcpy(Filtering_Datas[Filter_array_count].Job_Type,array[i].Job_Type);
						strcpy(Filtering_Datas[Filter_array_count].Server,array[i].Server);
						strcpy(Filtering_Datas[Filter_array_count].Client,array[i].Client);
						strcpy(Filtering_Datas[Filter_array_count].Schedule,array[i].Schedule);
						strcpy(Filtering_Datas[Filter_array_count].Files,array[i].Files);
						Filter_array_count++;
					}
				}

				for (int i=0; i<Filter_array_count; i++)
				{
					fprintf(_Filtering_Datas,"%s,%s,%s,%s,%s,%s\n", Filtering_Datas[i].Job_Status,Filtering_Datas[i].Job_Type,
					Filtering_Datas[i].Server,Filtering_Datas[i].Client,
					Filtering_Datas[i].Schedule,Filtering_Datas[i].Files);
				}
				fclose(_Filtering_Datas);

				if((Filterstream = fopen("FilterData.csv","r"))==NULL)
				{
					printf("The file 'FilterData.csv' was not opened!\n");
					// exit(1);
				}
				else
				{
					printf("The file 'FilterData.csv' was opened!\n");
				}
				bzero(buffer,BUFFER_SIZE);
				int lengsize = 0;

				// 파일 구조체 정보 할당 
				strcpy(File_Infos.File_Name,"FilterData.csv");
				File_Infos.FileNameLen = strlen(File_Infos.File_Name);
				char filename[50];
				strcpy(filename,"FilterData.csv");
				stat(filename, &st);
				int size = st.st_size;
				File_Infos.File_Size = size;
				printf("file size = %d\n",File_Infos.File_Size);
				// 파일 구조체 정보 먼저 전송
				send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);

				// 파일 내용 전송 
				while((lengsize = fread(buffer,1,1024,Filterstream)) > 0)
				{
					printf("lengsize = %d\n",lengsize);
					if(send(new_server_socket,buffer,lengsize,0)<0)
					{
						printf("Send File(FilterData.csv) is Failed\n");
						// break;
					}
					bzero(buffer, BUFFER_SIZE);
				}
				if(fclose(Filterstream))
				{
					printf("The file 'FilterData.csv' was not closed! \n");
					// exit(1);    
				}
			}
			/////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////
			// 검색 기준이 Vmware Backup 이라면 			
			else if (strcmp(buffer, "Vmware Backup")==0)
			{
				printf("검색 기준은 Vmware Backup 입니다.\n");
				for(int i=0;i<Total_row_count;i++)
				{
					// Job Type == File Backup
					if (strcmp(array[i].Job_Type,"Vmware Backup")==0)
					{
						strcpy(Filtering_Datas[Filter_array_count].Job_Status,array[i].Job_Status);
						strcpy(Filtering_Datas[Filter_array_count].Job_Type,array[i].Job_Type);
						strcpy(Filtering_Datas[Filter_array_count].Server,array[i].Server);
						strcpy(Filtering_Datas[Filter_array_count].Client,array[i].Client);
						strcpy(Filtering_Datas[Filter_array_count].Schedule,array[i].Schedule);
						strcpy(Filtering_Datas[Filter_array_count].Files,array[i].Files);
						Filter_array_count++;
					}
				}

				for (int i=0; i<Filter_array_count; i++)
				{
					fprintf(_Filtering_Datas,"%s,%s,%s,%s,%s,%s\n", Filtering_Datas[i].Job_Status,Filtering_Datas[i].Job_Type,
					Filtering_Datas[i].Server,Filtering_Datas[i].Client,
					Filtering_Datas[i].Schedule,Filtering_Datas[i].Files);
				}
				fclose(_Filtering_Datas);

				if((Filterstream = fopen("FilterData.csv","r"))==NULL)
				{
					printf("The file 'FilterData.csv' was not opened!\n");
					// exit(1);
				}
				else
				{
					printf("The file 'FilterData.csv' was opened!\n");
				}
				bzero(buffer,BUFFER_SIZE);
				int lengsize = 0;

				// 파일 구조체 정보 할당 
				strcpy(File_Infos.File_Name,"FilterData.csv");
				File_Infos.FileNameLen = strlen(File_Infos.File_Name);
				char filename[50];
				strcpy(filename,"FilterData.csv");
				stat(filename, &st);
				int size = st.st_size;
				File_Infos.File_Size = size;
				printf("file size = %d\n",File_Infos.File_Size);
				// 파일 구조체 정보 먼저 전송
				send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);

				// 파일 내용 전송 
				while((lengsize = fread(buffer,1,1024,Filterstream)) > 0)
				{
					printf("lengsize = %d\n",lengsize);
					if(send(new_server_socket,buffer,lengsize,0)<0)
					{
						printf("Send File(FilterData.csv) is Failed\n");
						// break;
					}
					bzero(buffer, BUFFER_SIZE);
				}
				if(fclose(Filterstream))
				{
					printf("The file 'FilterData.csv' was not closed! \n");
					// exit(1);    
				}
			}
			else
			{
				// 검색기준이 서버에 없는 경우
				printf("해당 검색기준이 지금 서버 코드에 없습니다\n"); 
			}
			/////////////////////////////////////////////////////////////

		}		

		// msg 수신 내용과 상관없이 ChartData 열고 보내기 
		// For Chart Data

		if (strcmp(buffer,"ChartData")==0)
		{
			// msg 수신 내용과 상관없이 ChartData 열고 보내기 
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

			// 파일 구조체 정보 할당 
			strcpy(File_Infos.File_Name,"ChartDatas.dat");
			File_Infos.FileNameLen = strlen(File_Infos.File_Name);
			char filename[50];
			strcpy(filename,"ChartDatas.dat");
			stat(filename, &st);
			int size = st.st_size;
			File_Infos.File_Size = size;
			// 파일 구조체 정보 먼저 전송
			send(new_server_socket,(char*)&File_Infos,sizeof(File_Infos),0);
			bzero(buffer,BUFFER_SIZE);

			// 파일 내용 전송 
			while((lengsize = fread(buffer,1,1024,stream)) > 0)
			{
				printf("lengsize = %d\n",lengsize);
				if(send(new_server_socket,buffer,lengsize,0)<0)
				{
					printf("Send File(ChartDatas.dat) is Failed\n");
					// break;
				}
				bzero(buffer, BUFFER_SIZE);
			}
			if(fclose(stream))
			{
				printf("The file 'ChartDatas.dat' was not closed! \n");
				// exit(1);    
			}
		}
		
		
		// 클라이언트와의 연결을 종료합니다.
		close(new_server_socket);    
		printf("Client Connection Closed\n");
	}
	// 수신을 위한 서버소켓을 종료합니다. 
	close(server_socket);
	return 0;
}
