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

	// 파일통계화면 UI, 파일 관련 통계를 보여줄 데이터 구조체 선언 
	typedef struct File_Statistics_PieChart
	{
		int Total_Size;
		double Total_File_Size;
		double Total_Write_Size;
	}File_Statistics_PieChart;
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
	// 에러 UI, 에러 비율을 보여주는 데이터 구조체 선언  
	typedef struct Total_Error_Ratio_PieChart{
		int Total_Count;
		int Total_Completed_Count;
		int Total_Error_Count;
	} Total_Error_Ratio_PieChart;

	Total_Error_Ratio_PieChart Array_Error_Ratio_PieChart;

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

	// 파일 통계(사이즈) 위한 변수 선언

	
	// 평균 통합시간
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

	// 시,분,초 구분 위해
	for(int i=0;i<991;i++)
	{
		if (strcmp(array[i].End_Time,"2022-02-08")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			printf("h : %s\n",ptr); // h 배열에 저장 
			_02_08_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_08_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_08_s_array[i] = atoi(ptr);
			printf("h: %d, m: %d, s:%d\n", _02_08_h_array[i],_02_08_m_array[i],_02_08_s_array[i]);
			_02_08_count++;
		}
		if (strcmp(array[i].End_Time,"2022-02-09")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			printf("h : %s\n",ptr); // h 배열에 저장 
			_02_09_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_09_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_09_s_array[i] = atoi(ptr);
			printf("h: %d, m: %d, s:%d\n", _02_09_h_array[i],_02_09_m_array[i],_02_09_s_array[i]);
			_02_09_count++;
		}
		if (strcmp(array[i].End_Time,"2022-02-10")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			printf("h : %s\n",ptr); // h 배열에 저장 
			_02_10_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_10_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_10_s_array[i] = atoi(ptr);
			printf("h: %d, m: %d, s:%d\n", _02_10_h_array[i],_02_10_m_array[i],_02_10_s_array[i]);
			_02_10_count++;
		}
		if (strcmp(array[i].End_Time,"2022-02-11")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			printf("h : %s\n",ptr); // h 배열에 저장 
			_02_11_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_11_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_11_s_array[i] = atoi(ptr);
			printf("h: %d, m: %d, s:%d\n", _02_11_h_array[i],_02_11_m_array[i],_02_11_s_array[i]);
			_02_11_count++;
		}
		if (strcmp(array[i].End_Time,"2022-02-12")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			printf("h : %s\n",ptr); // h 배열에 저장 
			_02_12_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_12_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_12_s_array[i] = atoi(ptr);
			printf("h: %d, m: %d, s:%d\n", _02_12_h_array[i],_02_12_m_array[i],_02_12_s_array[i]);
			_02_12_count++;
		}
		if (strcmp(array[i].End_Time,"2022-02-13")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			printf("h : %s\n",ptr); // h 배열에 저장 
			_02_13_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_13_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_13_s_array[i] = atoi(ptr);
			printf("h: %d, m: %d, s:%d\n", _02_13_h_array[i],_02_13_m_array[i],_02_13_s_array[i]);
			_02_13_count++;
		}
		if (strcmp(array[i].End_Time,"2022-02-14")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			printf("h : %s\n",ptr); // h 배열에 저장 
			_02_14_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_14_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_14_s_array[i] = atoi(ptr);
			printf("h: %d, m: %d, s:%d\n", _02_14_h_array[i],_02_14_m_array[i],_02_14_s_array[i]);
			_02_14_count++;
		}
		if (strcmp(array[i].End_Time,"2022-02-15")==0)
		{
			char *ptr = strtok(array[i].Elapsed_Time,":");
			printf("h : %s\n",ptr); // h 배열에 저장 
			_02_15_h_array[i]=atoi(ptr);
			ptr = strtok(NULL,":");
			_02_15_m_array[i]= atoi(ptr);
			ptr = strtok(NULL,":");
			_02_15_s_array[i] = atoi(ptr);
			printf("h: %d, m: %d, s:%d\n", _02_15_h_array[i],_02_15_m_array[i],_02_15_s_array[i]);
			_02_15_count++;
		}

	}
	
	int t_f_c =0;
	int t_w_c =0;
	// GB, MB, KB 구분 위해 
	for(int i=0;i<991;i++)
	{
		char *f_ptr2 = strtok(array[i].Files_Size," ");
		printf("f_ptr2 : %s\n",f_ptr2);
		char *f_ptr3 = strtok(NULL," ");
		printf("f_ptr3 : %s\n",f_ptr3);
		if (strcmp(f_ptr3,"GB")==0)
		{
			t_f_c++;
			printf("%d f_ptr2 GB : %s\n",t_f_c,f_ptr2);
			File_Statistics_PieCharts.Total_File_Size += atof(f_ptr2);
		}
		char *w_ptr2 = strtok(array[i].Write_Size," ");
		printf("w_ptr2 : %s\n",w_ptr2);
		char *w_ptr3 = strtok(NULL," ");
		printf("w_ptr3 : %s\n",w_ptr3);
		if (strcmp(w_ptr3,"GB")==0)
		{
			File_Statistics_PieCharts.Total_Write_Size += atof(w_ptr2);
			t_w_c++;
		}
	}
	
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
	for (int i=0; i<991;i++)
	{
		//Avg_Elapsed_Time_LineCharts._2022_02_08_Avg_Elapsed_Times
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
	}
	
	Avg_Elapsed_Time_LineCharts._2022_02_08_Avg_Elapsed_Times = ((Sum_02_08_m+(Sum_02_08_s/60))/_02_08_count)*60;
	Avg_Elapsed_Time_LineCharts._2022_02_09_Avg_Elapsed_Times = ((Sum_02_09_m+(Sum_02_09_s/60))/_02_09_count)*60;
	Avg_Elapsed_Time_LineCharts._2022_02_10_Avg_Elapsed_Times = ((Sum_02_10_m+(Sum_02_10_s/60))/_02_10_count)*60;
	Avg_Elapsed_Time_LineCharts._2022_02_11_Avg_Elapsed_Times = ((Sum_02_11_m+(Sum_02_11_s/60))/_02_11_count)*60;
	Avg_Elapsed_Time_LineCharts._2022_02_12_Avg_Elapsed_Times = ((Sum_02_12_m+(Sum_02_12_s/60))/_02_12_count)*60;
	Avg_Elapsed_Time_LineCharts._2022_02_13_Avg_Elapsed_Times = ((Sum_02_13_m+(Sum_02_13_s/60))/_02_13_count)*60;
	Avg_Elapsed_Time_LineCharts._2022_02_14_Avg_Elapsed_Times = ((Sum_02_14_m+(Sum_02_14_s/60))/_02_14_count)*60;
	Avg_Elapsed_Time_LineCharts._2022_02_15_Avg_Elapsed_Times = ((Sum_02_15_m+(Sum_02_15_s/60))/_02_15_count)*60;

	printf("_02_09_count=%d\n",_02_09_count);
	printf("_02_10_count=%d\n",_02_10_count);
	printf("_02_11_count=%d\n",_02_11_count);
	printf("_02_12_count=%d\n",_02_12_count);
	printf("_02_13_count=%d\n",_02_13_count);
	printf("_02_14_count=%d\n",_02_14_count);
	printf("_02_15_count=%d\n",_02_15_count);

	printf("02-08 avg=%d\n", Avg_Elapsed_Time_LineCharts._2022_02_08_Avg_Elapsed_Times);
	printf("02-09 avg=%d\n", Avg_Elapsed_Time_LineCharts._2022_02_09_Avg_Elapsed_Times);
	printf("02-10 avg=%d\n", Avg_Elapsed_Time_LineCharts._2022_02_10_Avg_Elapsed_Times);
	printf("02-11 avg=%d\n", Avg_Elapsed_Time_LineCharts._2022_02_11_Avg_Elapsed_Times);
	printf("02-12 avg=%d\n", Avg_Elapsed_Time_LineCharts._2022_02_12_Avg_Elapsed_Times);
	printf("02-13 avg=%d\n", Avg_Elapsed_Time_LineCharts._2022_02_13_Avg_Elapsed_Times);
	printf("02-14 avg=%d\n", Avg_Elapsed_Time_LineCharts._2022_02_14_Avg_Elapsed_Times);
	printf("02-15 avg=%d\n", Avg_Elapsed_Time_LineCharts._2022_02_15_Avg_Elapsed_Times);
	
	printf("f_S = %f, w_s = %f\n",File_Statistics_PieCharts.Total_File_Size,File_Statistics_PieCharts.Total_Write_Size);
	printf("%d,%d\n",t_f_c,t_w_c);
	return 0;
	
}
