#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUF 1024


struct ListStudents {
      char  *Name;
      char  *MSSV;
      char  *Class;
      char   *K;
};


// Propotype
void printStruct (struct ListStudents student) {
      printf("\n%s\t%s\t%s\t%s", student.Name, student.MSSV, student.Class, student.K);
}
      // Ham de noi hai string
char* concat(const char *s1, const char *s2)
{
      char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator '\0'
      // in real code you would check for errors in malloc here
      strcpy(result, s1);
      strcat(result, s2);
      return result;
}
int convertToHEVC(const char* file_in, const char* file_out) {
      char* temp = concat("ffmpeg -y -loglevel quiet -i ", file_in);
      // -y là tự động override
      // -loglevel quiet: Tắt các thông báo gây đau đầu
      temp = concat(temp, " -c:v libx265 -x265-params crf=26:psy-rd=1 ");
      temp = concat(temp, file_out);
      return system(temp);
}

      // Ham de noi video
void playback(char* video_path){
    char* open_video_command = concat("ffplay -autoexit -loglevel quiet ", video_path);
    system(open_video_command);
}

double getDuration(char* video_path, char* name){
      char* temp = concat("ffprobe -i ", video_path);

      char * nameFile = concat(name, "_tmp.txt");
      temp = concat(temp, " -show_entries format=duration -v quiet -of csv=\"p=0\" >> ");
      temp = concat(temp, nameFile);
      system(temp);

      FILE * fp;
      char * line = NULL;
      size_t len = 0;
      fp = fopen(nameFile, "r");
      if (fp == NULL)
            exit(EXIT_FAILURE);
      while (getline(&line, &len, fp) != -1) {
        break;
      }
      fclose(fp);
      remove(nameFile);
      double duration = atof (line);
      if (line) free(line);
      //printf("** Duration of the video: %lf seconds \n", duration);
      return duration;
}


void displayCpuMemAndDynamicLib(int duration, pid_t cpid_1, pid_t cpid_2){

	char str1[15], str2[15];
	sprintf(str1, "%d", cpid_1);
      sprintf(str2, "%d", cpid_2);

	char* temp = concat("ps -p ",str1);
	temp = concat(temp, " -o pid,%cpu,%mem && ps -p ");
      temp = concat(temp, str2);
      temp = concat(temp, " -o pid,%cpu,%mem");
      for(int i=duration/3; i>0; i--){
            system(temp);
            system("echo \"\nThư viện liên kết động:\" && ldd a.out");
            sleep(3);
      }
}

void show_video_info(char* video_path) {
      struct stat video_info;
      int status = stat(video_path, &video_info);
      if ( status == 0){
            float video_size = (float)video_info.st_size/1048576.0;
            printf("** Size of the video: %.3f MB\n", video_size);
            printf("** Duration of the video: %.3lf seconds \n", getDuration(video_path, "Quyen"));

            // char str[15];
            // sprintf(str, "%d", pid); // Convert num to string
            // char *command = concat("kill ", str);
            // system(command);
      }
      //printf("** Size of the video: %.2f min\n", getDuration(video_path)/ 60.0);

}

int main () {
      printf("\nProject Operating System\n");

      int fd[2], nbytes;
      pid_t cpid[7];
      char readbuffer[100];

      // Pipe thu 2: ket noi child process 2 voi parent process
      int fd_2[2];

      // Pipe thu 3: ket noi child process 2 voi child process 3
      int fd_3[2];

      // Pipe thu 4: ket noi child process 3 voi child process 1
      int fd_4[2];

      // Pipe thu 5: ket noi child process 1 voi parent process
      int fd_5[2];

      // Pipe thu 6: ket noi child process 1 voi child process 4
      int fd_6[2];

      // Pipe thu 7: ket noi child process 4 voi child process 3
      int fd_7[2];

      // Pipe thu 8: ket noi child process 3 voi parent process
      int fd_8[2];

      // Pipe thu 9: ket noi child process 3 voi child process 5
      int fd_9[2];

      // Pipe thu 10: ket noi child process 4 voi child process 6
      int fd_10[2];

      // Pipe thu 11: ket noi child process 5 voi child process 4
      int fd_11[2];

      // Pipe thu 12: ket noi child process 4 voi parent process
      int fd_12[2];

      // Pipe thu 13: ket noi child process 5 voi child process 7
      int fd_13[2];

      // Pipe thu 14: ket noi child process 6 voi child process 5
      int fd_14[2];

      // Pipe thu 15: ket noi child process 5 voi parent process
      int fd_15[2];

      // Pipe thu 16: ket noi child process 7 voi child process 6
      int fd_16[2];

      // Pipe thu 17: ket noi child process 6 voi parent process 
      int fd_17[2];

      pipe(fd);
      pipe(fd_2);
      pipe(fd_3);
      pipe(fd_4);
      pipe(fd_5);
      pipe(fd_6);
      pipe(fd_7);
      pipe(fd_8);
      pipe(fd_9);
      pipe(fd_10);
      pipe(fd_11);
      pipe(fd_12);
      pipe(fd_13);
      pipe(fd_14);
      pipe(fd_15);
      pipe(fd_16);
      pipe(fd_17);

      // Nhiệm vụ 1:
      //  Chương trình chính (gắn với tiến trình cha mẹ) hiện ra màn hình họ tên, số hiệu sinh viên, lớp, khóa của tất cả các thành viên.

      {
            printf("\nDanh sách sinh viên làm bài tập lớn:\n");
            printf("\nHọ và tên\tMSSV\t\tLớp\t\tKhóa");
            struct ListStudents Quyen = {"Ngô Văn Quyền", "20153086", "Điện Tử 08", "K60"}; printStruct(Quyen);
            struct ListStudents Son1 = {"Ngô Tùng Sơn","20153200", "Điện tử 10", "K60"}; printStruct(Son1);
            struct ListStudents Quan1 = {"Phan Hữu Quân","20153040","Điện tử 03", "K60"}; printStruct(Quan1);
            struct ListStudents Thanh = {"Lê Đức Thành","20153378","Điện tử 03", "K60"}; printStruct(Thanh);
            struct ListStudents Son2 = {"Lê Thanh Sơn", "20143805", "Điện tử 05", "K59"}; printStruct(Son2);
            struct ListStudents Quan2 = {"Trần Hải Quân", "20153048", "Điện tử 10", "K60"}; printStruct(Quan2);
            struct ListStudents Son3 = {"Hoàng Văn Sơn", "20153189", "Điện tử 05", "K60"}; printStruct(Son3);
            printf("\n\n\n");
      }
      sleep(6);

      // Bắt đầu quá trình
      cpid[0] = fork();
      if (cpid[0] == 0){ 
            printf("\n\n\n************************************************************\n");
            printf("P1 with pid = %d from parent with pid = %d\n",getpid(),getppid());
            printf("************************************************************\n\n");

            // BAT DAU MA HOA 
            char* output_video = "Quyen/test_after_compression.mp4";
            convertToHEVC("Quyen/TheVideoOfQuyen.mp4",output_video);
            
            // GUI YEU CAU CHO CP2 DE PLAYBACK
            char* video_path = output_video;
            close(fd[0]);	//chi write, k read
            write(fd[1],video_path,(strlen(video_path)+1));
            close(fd[1]);

            // NHAN YEU CAU TU CP3 DE PLAYBACK
            close(fd_4[1]);     //chi read, k write
            nbytes = read(fd_4[0], readbuffer, sizeof(readbuffer));
            close(fd_4[0]);
            if (nbytes != -1){
                  // GUI YEU CAU CHO PARENT DE HIEN THI %CPU VA %MEM
                  close(fd_5[0]);
                  write(fd_5[1], readbuffer, (strlen(readbuffer)+1));
                  close(fd_5[1]);
                  // playback
                  playback (readbuffer);
            }
            
            // HIEN THI THONG TIN VIDEO
            show_video_info(readbuffer);

            // Gửi tín hiệu yêu cầu child process 4 mã hóa video kế tiếp
            close(fd_6[0]);     //chi write, k read
            write(fd_6[1],"signal",(strlen("signal")+1));
            close(fd_6[1]);

            exit(0);
      }


      cpid[1] = fork();
      if (cpid[1] == 0){

            // NHAN YEU CAU TU CP1 DE PLAYBACK
            close(fd[1]);	//chi read, k write
            nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
            close(fd[0]);
            printf("\n\n\n************************************************************\n");
            printf("P2 with pid = %d from parent with pid = %d\n",getpid(),getppid());
            printf("************************************************************\n\n");
            if (nbytes != -1){
                  // GUI YEU CAU CHO PARENT DE HIEN THI %CPU VA %MEM
                  close(fd_2[0]);
			write(fd_2[1], readbuffer, (strlen(readbuffer)+1));
                  close(fd_2[1]);
                  // playback
                  playback (readbuffer);
            }
            // HIEN THI THONG TIN VIDEO
            show_video_info(readbuffer);
            printf("Mời sinh viên tiếp theo convert video của mình !\n\n");

            // GUI YEU CAU CHO CP3 DE MA HOA
            close(fd_3[0]);     //chi write, k read
            write(fd_3[1],"signal",(strlen("signal")+1));
            close(fd_3[1]);
            exit(0);
      }

      cpid[2] = fork();
      if (cpid[2] == 0){

            // NHAN YEU CAU TU CP2 DE MA HOA
            close(fd_3[1]);     //chi read, k write
            nbytes = read(fd_3[0], readbuffer, sizeof(readbuffer));
            close(fd_3[0]);
            printf("\n\n\n************************************************************\n");            
            printf("P3 with pid = %d from parent with pid = %d\n",getpid(),getppid());
            printf("************************************************************\n\n");
            char* output_video = "Son/test_after_compression.mp4";
            convertToHEVC("Son/TheVideoOfSon.mp4",output_video);


             // GUI YEU CAU CHO CP3 DE PLAYBACK           
            char* video_path = output_video;
            close(fd_4[0]);     //chi write, k read
            write(fd_4[1],video_path,(strlen(video_path)+1));
            close(fd_4[1]);

            // NHAN YEU CAU TU CP4 DE PLAYBACK
            close(fd_7[1]);     //chi read, k write
            nbytes = read(fd_7[0], readbuffer, sizeof(readbuffer));
            close(fd_7[0]);
            if (nbytes != -1){
                  // GUI YEU CAU CHO PARENT DE HIEN THI %CPU VA %MEM
                  close(fd_8[0]);
                  write(fd_8[1], readbuffer, (strlen(readbuffer)+1));
                  close(fd_8[1]);
                  // playback
                  playback (readbuffer);
            }
            // HIEN THI THONG TIN VIDEO
            show_video_info(readbuffer);
            printf("Mời sinh viên tiếp theo convert video của mình !\n\n");

            // GUI YEU CAU CHO CP5 DE MA HOA
            close(fd_9[0]);     //chi write, k read
            write(fd_9[1],"signal",(strlen("signal")+1));
            close(fd_9[1]);

            exit(0);
      }

      cpid[3] = fork();
      if (cpid[3] == 0){

            // NHAN YEU CAU TU CP1 DE MA HOA
            close(fd_6[1]);     //chi read, k write
            nbytes = read(fd_6[0], readbuffer, sizeof(readbuffer));
            close(fd_6[0]);
            printf("\n\n\n************************************************************\n");
            printf("P4 with pid = %d from parent with pid = %d\n",getpid(),getppid());
            printf("************************************************************\n\n");
            char* output_video = "Quan1/test_after_compression.mp4";
            convertToHEVC("Quan1/TheVideoOfQuan1.mp4",output_video);
            

            // GUI YEU CAU CHO CP3 DE PLAYBACK
            char* video_path = output_video;
            close(fd_7[0]);     //chi write, k read
            write(fd_7[1],video_path,(strlen(video_path)+1));
            close(fd_7[1]);

            // NHAN YEU CAU TU CP5 DE PLAYBACK
            close(fd_11[1]);     //chi read, k write
            nbytes = read(fd_11[0], readbuffer, sizeof(readbuffer));
            close(fd_11[0]);
            if (nbytes != -1){
                  // GUI YEU CAU CHO PARENT DE HIEN THI %CPU VA %MEM
                  close(fd_12[0]);
                  write(fd_12[1], readbuffer, (strlen(readbuffer)+1));
                  close(fd_12[1]);
                  // playback
                  playback (readbuffer);
            }
            // HIEN THI THONG TIN VIDEO
            show_video_info(readbuffer);
            printf("Mời sinh viên tiếp theo convert video của mình !\n\n");

            // // GUI YEU CAU CHO CP6 DE MA HOA
            close(fd_10[0]);     //chi write, k read
            write(fd_10[1],"signal",(strlen("signal")+1));
            close(fd_10[1]);

            exit(0);
      }

      cpid[4] = fork();
      if (cpid[4] == 0){

            // NHAN YEU CAU TU CP3 DE MA HOA
            close(fd_9[1]);     //chi read, k write
            nbytes = read(fd_9[0], readbuffer, sizeof(readbuffer));
            close(fd_9[0]);
            printf("\n\n\n************************************************************\n");
            printf("P5 with pid = %d from parent with pid = %d\n",getpid(),getppid());
            printf("************************************************************\n\n");
            char* output_video = "Quan2/test_after_compression.mp4";
            convertToHEVC("Quan2/TheVideoOfQuan2.mp4",output_video);
            

            // GUI YEU CAU CHO CP4 DE PLAYBACK
            char* video_path = output_video;
            close(fd_11[0]);     //chi write, k read
            write(fd_11[1],video_path,(strlen(video_path)+1));
            close(fd_11[1]);

            // NHAN YEU CAU TU CP6 DE PLAYBACK
            close(fd_14[1]);     //chi read, k write
            nbytes = read(fd_14[0], readbuffer, sizeof(readbuffer));
            close(fd_14[0]);

            if (nbytes != -1){
                  // GUI YEU CAU CHO PARENT DE HIEN THI %CPU VA %MEM
                  close(fd_15[0]);
                  write(fd_15[1], readbuffer, (strlen(readbuffer)+1));
                  close(fd_15[1]);
                  // playback
                  playback (readbuffer);
            }
            // HIEN THI THONG TIN VIDEO
            show_video_info(readbuffer);
            printf("Mời sinh viên tiếp theo convert video của mình !\n\n");

            // GUI YEU CAU CHO CP7 DE MA HOA
            close(fd_13[0]);     //chi write, k read
            write(fd_13[1],"signal",(strlen("signal")+1));
            close(fd_13[1]);

            exit(0);
      }


      cpid[5] = fork();
      if (cpid[5] == 0){

            // NHAN YEU CAU TU CP4 DE MA HOA
            close(fd_10[1]);     //chi read, k write
            nbytes = read(fd_10[0], readbuffer, sizeof(readbuffer));
            close(fd_10[0]);
            printf("\n\n\n************************************************************\n");
            printf("P6 with pid = %d from parent with pid = %d\n",getpid(),getppid());
            printf("************************************************************\n\n");
            char* output_video = "Son2/test_after_compression.mp4";
            convertToHEVC("Son2/TheVideoOfSon2.mp4",output_video);
            

            // GUI YEU CAU CHO CP5 DE PLAYBACK
            char* video_path = output_video;
            close(fd_14[0]);     //chi write, k read
            write(fd_14[1],video_path,(strlen(video_path)+1));
            close(fd_14[1]);

            // NHAN YEU CAU TU CP7 DE PLAYBACK
            close(fd_16[1]);     //chi read, k write
            nbytes = read(fd_16[0], readbuffer, sizeof(readbuffer));
            close(fd_16[0]);
    
            if (nbytes != -1){
                  // GUI YEU CAU CHO PARENT DE HIEN THI %CPU VA %MEM
                  close(fd_17[0]);
                  write(fd_17[1], readbuffer, (strlen(readbuffer)+1));
                  close(fd_17[1]);
                  // playback
                  playback (readbuffer);
            }
            // HIEN THI THONG TIN VIDEO
            show_video_info(readbuffer);
            printf("Mời sinh viên tiếp theo convert video của mình !\n\n");

            exit(0);
      }

      cpid[6] = fork();
      if (cpid[6] == 0){

            // NHAN YEU CAU TU CP5 DE MA HOA
            close(fd_13[1]);     //chi read, k write
            nbytes = read(fd_13[0], readbuffer, sizeof(readbuffer));
            close(fd_13[0]);
            printf("\n\n\n************************************************************\n");
            printf("P7 with pid = %d from parent with pid = %d\n",getpid(),getppid());
            printf("************************************************************\n\n");
            char* output_video = "Thanh/test_after_compression.mp4";
            convertToHEVC("Thanh/TheVideoOfThanh.mp4",output_video);
            

            // GUI YEU CAU CHO CP6 DE PLAYBACK
            char* video_path = output_video;
            close(fd_16[0]);     //chi write, k read
            write(fd_16[1],video_path,(strlen(video_path)+1));
            close(fd_16[1]);

            exit(0);
      }



      if (cpid[0] != 0  &&  cpid[1] != 0){

            // PARENT PROCESS

            // NHAN YEU CAU TU CP2 DE HIEN THI %CPU, %MEM CUA CAC CHILD PROCESS
            close(fd_2[1]);
            nbytes = read(fd_2[0], readbuffer, sizeof(readbuffer));
            close(fd_2[0]);
            if (nbytes != -1){//viec nhan string thanh cong
                  int duration = getDuration(readbuffer, "pQuyen");
                  displayCpuMemAndDynamicLib(duration, cpid[0], cpid[1]);
            }

            // NHAN YEU CAU TU CP1 DE HIEN THI %CPU, %MEM CUA CAC CHILD PROCESS
            close(fd_5[1]);     //chi read, k write
            nbytes = read(fd_5[0], readbuffer, sizeof(readbuffer));
            close(fd_5[0]);
            if (nbytes != -1){//viec nhan string thanh cong
                  int duration = getDuration(readbuffer, "pSon");
                  displayCpuMemAndDynamicLib(duration, cpid[2], cpid[0]);
            }


            // NHAN YEU CAU TU CP3 DE HIEN THI %CPU, %MEM CUA CAC CHILD PROCESS
            close(fd_8[1]);     //chi read, k write
            nbytes = read(fd_8[0], readbuffer, sizeof(readbuffer));
            close(fd_8[0]);
            if (nbytes != -1){//viec nhan string thanh cong
                  int duration = getDuration(readbuffer, "pQuan1");
                  displayCpuMemAndDynamicLib(duration, cpid[3], cpid[2]);
            }


            // NHAN YEU CAU TU CP4 DE HIEN THI %CPU, %MEM CUA CAC CHILD PROCESS
            close(fd_12[1]);     //chi read, k write
            nbytes = read(fd_12[0], readbuffer, sizeof(readbuffer));
            close(fd_12[0]);
            if (nbytes != -1){//viec nhan string thanh cong
                  int duration = getDuration(readbuffer, "pQuan2");
                  displayCpuMemAndDynamicLib(duration, cpid[4], cpid[3]);
            }

            // NHAN YEU CAU TU CP5 DE HIEN THI %CPU, %MEM CUA CAC CHILD PROCESS
            close(fd_15[1]);     //chi read, k write
            nbytes = read(fd_15[0], readbuffer, sizeof(readbuffer));
            close(fd_15[0]);
            if (nbytes != -1){//viec nhan string thanh cong
                  int duration = getDuration(readbuffer, "pSon2");
                  displayCpuMemAndDynamicLib(duration, cpid[5], cpid[4]);
            }

            // NHAN YEU CAU TU CP5 DE HIEN THI %CPU, %MEM CUA CAC CHILD PROCESS
            close(fd_17[1]);     //chi read, k write
            nbytes = read(fd_17[0], readbuffer, sizeof(readbuffer));
            close(fd_17[0]);
            if (nbytes != -1){//viec nhan string thanh cong
                  int duration = getDuration(readbuffer, "Thanh2");
                  displayCpuMemAndDynamicLib(duration, cpid[6], cpid[5]);
            }


            // HIEN THI % DONG GOP CUA CAC THANH VIEN TRONG BAI TAP LON
            {
            printf("\nDanh sách sinh viên làm bài tập lớn:\n");
            printf("\nHọ và tên\tMSSV\t\tPhần trăm đóng góp");
	    printf("Ngô Văn Quyền\t20153086\t40%");
	    printf("Ngô Tùng Sơn\t20153200\t40%");
	    printf("Phan Hữu Quân\t20153040\t16%");
	    printf("Lê Đức Thành\t20153378\t1%");
	    printf("Lê Thanh Sơn\t20143805\t1%");
	    printf("Trần Hải Quân\t20153048\t1%");
	    printf("Hoàng Văn Sơn\t20153189\t1%");

            printf("\n\n\n");
            }


            while (wait(NULL) > 0);
      }

      return 0;
}
      // Giải thích wait()
      // Nếu chưa có tiến trình con nào kết thúc ---> Tiếp tục chờ
      // Nếu có một tiến trình con kết thúc wait(NULL) trả về PID của tiến trình con đã kết thúc chính là giá trị dương
      // Khi này lại tiếp tục chờ các tiến trình con khác (Lặp lại vòng while())
      // .....
      // Khi mà không còn tiến trình con nào wait(NULL) sẽ trả về -1 và như thế kết thúc quá trình chờ đợi của tiến trình cha.
      // Khi có lỗi wait() cũng trả về -1
