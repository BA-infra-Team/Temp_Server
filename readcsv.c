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

	for(int i=0;i<991;i++)
	{
		strcpy(Job_Status,array[i].Job_Status);
		strcpy(Schedule,array[i].Schedule);

		// GB, MB, KB 구분 위한 데이터 가공 (Statistics UI)
		// GB만 추출해서 저장
		// 파일 사이즈(File_Size) 가공 
		char *f_ptr2 = strtok(array[i].Files_Size," ");
		char *f_ptr3 = strtok(NULL," ");
		
		// 저장 사이즈(Write_Size) 가공 
		if (strcmp(f_ptr3,"GB")==0)
		{
			t_f_c++;
			//ChartDatas.File_Statistics_PieChart_Total_File_Size += atof(f_ptr2);
		}
		char *w_ptr2 = strtok(array[i].Write_Size," ");
		char *w_ptr3 = strtok(NULL," ");

		if (strcmp(w_ptr3,"GB")==0)
		{
			//ChartDatas.File_Statistics_PieChart_Total_Write_Size += atof(w_ptr2);
			t_w_c++;
		}
		// 전송 크기(DataTransferred_Size) 가공
		char *d_ptr2 = strtok(array[i].Data_Transferred," ");
		char *d_ptr3 = strtok(NULL," ");
		if (strcmp(d_ptr3,"GB")==0)
		{
			//ChartDatas.File_Statistics_PieChart_Total_Data_Transferred += atof(d_ptr2);
			t_w_c++;
		}

		if (strcmp(Job_Status,"Canceled")==0)
		{
			Array_Error_Ratio_PieChart.Total_Error_Count++;
		}
		if (strcmp(Job_Status,"Completed")==0)
		{
			Array_Error_Ratio_PieChart.Total_Completed_Count++;
			if (strcmp(End_Time,"2022-02-08")==0)
				ChartDatas.Total_File_Size_LineChart_2022_02_08_Count += atof(f_ptr2);
				ChartDatas.Total_Write_Size_LineChart_2022_02_08_Count += atof(w_ptr2);
				ChartDatas.Total_Data_Transferred_LineChart_2022_02_08_Count += atof(d_ptr2);
				printf("f_ptr2 = %s\n",f_ptr2);
				printf("w_ptr2 = %s\n",w_ptr2);
				printf("d_ptr2 = %s\n",d_ptr2);
				printf("Total_File_Size_LineChart_2022_02_08_Count = %d\n",ChartDatas.Total_File_Size_LineChart_2022_02_08_Count);
				printf("Total_Write_Size_LineChart_2022_02_08_Count = %d\n",ChartDatas.Total_Write_Size_LineChart_2022_02_08_Count);
				printf("Total_Data_Transferred_LineChart_2022_02_08_Count = %d\n",ChartDatas.Total_Data_Transferred_LineChart_2022_02_08_Count);
			if (strcmp(End_Time,"2022-02-09")==0)
				ChartDatas.Total_File_Size_LineChart_2022_02_09_Count += atof(f_ptr2);
				ChartDatas.Total_Write_Size_LineChart_2022_02_09_Count += atof(w_ptr2);
				ChartDatas.Total_Data_Transferred_LineChart_2022_02_09_Count += atof(d_ptr2);
			if (strcmp(End_Time,"2022-02-10")==0)
				ChartDatas.Total_File_Size_LineChart_2022_02_10_Count += atof(f_ptr2);
				ChartDatas.Total_Write_Size_LineChart_2022_02_10_Count += atof(w_ptr2);
				ChartDatas.Total_Data_Transferred_LineChart_2022_02_10_Count += atof(d_ptr2);
			if (strcmp(End_Time,"2022-02-11")==0)
				ChartDatas.Total_File_Size_LineChart_2022_02_11_Count += atof(f_ptr2);
				ChartDatas.Total_Write_Size_LineChart_2022_02_11_Count += atof(w_ptr2);
				ChartDatas.Total_Data_Transferred_LineChart_2022_02_11_Count += atof(d_ptr2);
			if (strcmp(End_Time,"2022-02-12")==0)
				ChartDatas.Total_File_Size_LineChart_2022_02_12_Count += atof(f_ptr2);
				ChartDatas.Total_Write_Size_LineChart_2022_02_12_Count += atof(w_ptr2);
				ChartDatas.Total_Data_Transferred_LineChart_2022_02_12_Count += atof(d_ptr2);
			if (strcmp(End_Time,"2022-02-13")==0)
				ChartDatas.Total_File_Size_LineChart_2022_02_13_Count += atof(f_ptr2);
				ChartDatas.Total_Write_Size_LineChart_2022_02_13_Count += atof(w_ptr2);
				ChartDatas.Total_Data_Transferred_LineChart_2022_02_13_Count += atof(d_ptr2);
			if (strcmp(End_Time,"2022-02-14")==0)
				ChartDatas.Total_File_Size_LineChart_2022_02_14_Count += atof(f_ptr2);
				ChartDatas.Total_Write_Size_LineChart_2022_02_14_Count += atof(w_ptr2);
				ChartDatas.Total_Data_Transferred_LineChart_2022_02_14_Count += atof(d_ptr2);
			if (strcmp(End_Time,"2022-02-15")==0)
				ChartDatas.Total_File_Size_LineChart_2022_02_15_Count += atof(f_ptr2);
				ChartDatas.Total_Write_Size_LineChart_2022_02_15_Count += atof(w_ptr2);
				ChartDatas.Total_Data_Transferred_LineChart_2022_02_15_Count += atof(d_ptr2);
		}
		if (strcmp(Job_Status,"Failed")==0)
		{
			Array_Error_Ratio_PieChart.Total_Error_Count++;
		}
		if (strcmp(Job_Status,"Partially Completed")==0)
		{
			Array_Error_Ratio_PieChart.Total_Completed_Count++;
		}
		if (strcmp(Job_Status,"Suspended")==0)
		{
			Array_Error_Ratio_PieChart.Total_Error_Count++;
		}
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
	}
	Array_Error_Ratio_PieChart.Total_Count = Array_Error_Ratio_PieChart.Total_Completed_Count+Array_Error_Ratio_PieChart.Total_Error_Count;
	printf("%d,%d,%d\n",Array_Error_Ratio_PieChart.Total_Count,Array_Error_Ratio_PieChart.Total_Completed_Count,Array_Error_Ratio_PieChart.Total_Error_Count);

	printf("testsc1 %d\n",ChartDatas.Schedule_testsc1_Count);
	printf("testsc2 %d\n",ChartDatas.Schedule_testsc2_Count);
	printf("testsc3 %d\n",ChartDatas.Schedule_testsc3_Count);
	printf("testsc4 %d\n",ChartDatas.Schedule_testsc4_Count);

	printf("Total_File_Size_LineChart_2022_02_08_Count %d\n",ChartDatas.Total_File_Size_LineChart_2022_02_08_Count);
	printf("Total_Write_Size_LineChart_2022_02_08_Count %d\n",ChartDatas.Total_Write_Size_LineChart_2022_02_08_Count);
	printf("Total_Data_Transferred_LineChart_2022_02_08_Count %d\n",ChartDatas.Total_Data_Transferred_LineChart_2022_02_08_Count);
	return 0;
}


