#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

int main()
{
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
		char Job_Status[50];
		char Job_Type[50];
		char Server[50];
		char Client[50];
		char Schedule[50];
		char Files[50];
	} Filtering_Data;
	Filtering_Data Filtering_Datas[991];


	// 에러 UI, 에러 비율을 보여주는 데이터 구조체 선언  
	typedef struct Total_Error_Ratio_PieChart{
		int Total_Count;
		int Total_Completed_Count;
		int Total_Error_Count;
	} Total_Error_Ratio_PieChart;

	Total_Error_Ratio_PieChart Array_Error_Ratio_PieChart;

	typedef struct ChartData{
		// 스케줄 별 개수 표시 (2022-03-24 추가)
		int Schedule_testsc1_Count;
		int Schedule_testsc2_Count;
		int Schedule_testsc3_Count;
		int Schedule_testsc4_Count;

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
	printf("Total_field_count=%d\n",i);
	for(int i=0;i<991;i++)
	{
		printf("%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s,%s\n",
		array[i].Job_ID,array[i].Job_Status, array[i].Error_Code, array[i].Job_Type, array[i].Backup_Method, array[i].Client
		, array[i].Job, array[i].Schedule, array[i].Files, array[i].Files_Size, array[i].Write_Size, array[i].Data_Transferred, array[i].Throughput,
		array[i].Start_Time, array[i].End_Time, array[i].Elapsed_Time, array[i].Server, array[i].Volume_Pool, array[i].Storage, array[i].Tape_Drive, array[i].Volume);
	}

	char Job_Status[300];
	char Schedule[300];
	char End_Time[300];
	char Job_Type[300];
	
	int VmWare_array_count = 0;

	// 파일 변수 선언
	FILE *_Filtering_Datas;
	char *CopyNameSource;
	char *CopyNameDest;

	for(int i=0;i<Total_row_count;i++)
	{
		CopyNameSource = array[i].Job_Status;
		if (strcmp(array[i].Job_Type,"Vmware Backup")==0)
		{
			// Vmware Backup으로만 구성 된 필터링 데이터 구조체 할당 
			strcpy(Filtering_Datas[VmWare_array_count].Job_Status,array[i].Job_Status);
			strcpy(Filtering_Datas[VmWare_array_count].Job_Type,array[i].Job_Type);
			strcpy(Filtering_Datas[VmWare_array_count].Server,array[i].Server);
			strcpy(Filtering_Datas[VmWare_array_count].Client,array[i].Client);
			strcpy(Filtering_Datas[VmWare_array_count].Schedule,array[i].Schedule);
			strcpy(Filtering_Datas[VmWare_array_count].Files,array[i].Files);
			VmWare_array_count++;
		}
	}
	// for문 실시간 탐색 및 구조체 정의 끝나고 파일 저장
	_Filtering_Datas = fopen("FilterData.dat","w");
	if (_Filtering_Datas == NULL)
	{
		fprintf(stderr, "\nError Opened Files\n");
		// exit(1);
	}
	// 필터링 구조체 내용을 파일에 저장한다. 
	fwrite(&VmWare_array_count, sizeof(int),1, _Filtering_Datas);
	fwrite(Filtering_Datas, sizeof(Filtering_Data), VmWare_array_count, _Filtering_Datas);
	if (fwrite != 0)
		printf("Contents to Filtering file Written Successfully !\n");
	else
		printf("Error Writing Filtering file !\n");
	// 파일을 닫아준다. 
	fclose(_Filtering_Datas);


	return 0;
}


