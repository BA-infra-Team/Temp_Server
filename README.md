# Temp_Server
Temp_Server (미완성된 서버, 서버 코드 관련 연습 코드 등)  

## 환경설정  
가상환경 : VMWare WorkStation 15 Player  
OS : Linux Ubuntu 18.04.6 LTS  
Language : C   
## 파일 내용  
- server.c : 현재 진행중인 코드
- server2.c : 단순 리눅스 파일 전송 서버 예제
- readcsv.c : csv 파일 읽기 연습
- readcsv2.c : csv 파일 읽기 및 데이터 가공 연습 
- LogData.csv : 로그 수집기 대신 사용될 데이터 

## 특이사항 정리  
2022-02:27  
server / client 파일 주고 받기 예제, 통신 확인 

2022-03-03  
csv파일 읽기 완료  

2022-03-05  
데이터 전처리 시작  

2022-03-08  
기타 단순 데이터 가공 완료  

2022-03-10  
추후 평균처리시간(Avg_Elapsed_Time) 및 파일사이즈 (GB,MB,KB) 관련 데이터 가공 예정  

2022-03-16  
현재 서버 데이터 가공 쪽 평균처리시간 계산 시  
시간 결과에서 ex) 336/58 = 5.8 나오는데 5.8 0.8 * 60 = 48이 나옴.  
5.8 * 60 = 348 = 5:48 이런식으로 계산하는 방법이 추가되거나 고려해야함.  
1차 데이터 가공 완료 

2022-03-18  
VMware Bridge 설정 완료(window 클라이언트와 통신 확인)  

## 유용한 URL 정리 : 

Linux File Transfer S/C : https://ofstack.com/C++/9959/c-language-to-achieve-linux-socket-file-transfer-instances.html  
Vmware Network Adapter 설정 : https://lifegoesonme.tistory.com/388  

---
추후 필터링 및 로그인 기능 추가 예정  
상황에 따른 쓰레드 사용 고려 예정  
