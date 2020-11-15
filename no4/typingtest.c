#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define TEXTSIZE 600

int main(void) {
	time_t tStart, tEnd;
	double elapsed_time, accuracy;
	int fd;
	int nread, cnt=0, errcnt=0;
	char ch;
	char text[TEXTSIZE] = "It is only the heart that one can see rightly, what is essential is invisible to the eye. It's the miracle that the person who i like likes me. No one is ever satisfied where he is.";
	struct termios init_attr, new_attr;
	fd = open(ttyname(fileno(stdin)), O_RDWR);
	tcgetattr(fd, &init_attr);
	new_attr = init_attr;
	new_attr.c_lflag &= ~ICANON;
	new_attr.c_lflag &= ~ECHO; /* ~(ECHO | ECHOE | ECHOK | ECHONL); */
	new_attr.c_cc[VMIN] = 1;
	new_attr.c_cc[VTIME] = 0;
	if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
		fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
	}
	printf("다음 문장을 그대로 입력하세요.\n%s\n", text);
	time(&tStart);
	while ((nread=read(fd, &ch, 1)) > 0 && ch != '\n') {
		if (ch == text[cnt++])
			write(fd, &ch, 1);
		else {
			write(fd, "*", 1);
			errcnt++;
		}
	}
	time(&tEnd);
	elapsed_time = difftime(tEnd, tStart);//타이핑하는데 걸린 시간
	printf("\n타이핑 오류의 횟수: %d\n", errcnt);
	tcsetattr(fd, TCSANOW, &init_attr);
	printf("\n분당 타자수: %.0f\n", cnt*60/elapsed_time);
	close(fd);
	return 0;
}

