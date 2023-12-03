#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
    #include <curses.h>
#else
    #include <ncurses.h>
#endif

typedef struct line{
	char* text;
	struct line* prev;
	struct line* next;
}line;

line *head = NULL, *tail = NULL; // 전역 변수로 head와 tail 선언

void init() {
    head = (line*)malloc(sizeof(line));
    tail = (line*)malloc(sizeof(line));
    head->text = strdup(""); // 빈 문자열로 초기화
    head->prev = NULL;
    head->next = tail;
    tail->prev = head;
    tail->next = NULL;
}

line* addLine(line *tail, const char *text) {
    line *newLine = (line *)malloc(sizeof(line));
    newLine->text = strdup(text); // 문자열 복사
    newLine->next = NULL;
    newLine->prev = tail;

    if (tail) {
        tail->next = newLine;
    }

    return newLine;
}

void printFileContent(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("파일을 열 수 없습니다");
        return;
    }

    line *head = NULL, *tail = NULL;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file)) {
        if (!head) {
            head = addLine(NULL, buffer);
            tail = head;
        } else {
            tail = addLine(tail, buffer);
        }
    }

    fclose(file);

    // 화면에 출력
    for (line *current = head; current != NULL; current = current->next) {
        printw("%s", current->text);
    }

    // 리스트 해제
    while (head) {
        line *temp = head;
        head = head->next;
        free(temp->text);
        free(temp);
    }
}


void processKeyInput() {
    int ch;
    int x, y;
    getyx(stdscr, y, x);
    move(y, x);
    line *currentLine = head;

    while (1) {
    	
        ch = getch(); // 키 입력 받기
		
        switch (ch) {
            case KEY_UP:
    if (y > 0) {
        currentLine = currentLine->prev; // 이전 라인으로 이동
        y--;
        x = strlen(currentLine->text);
        move(y, x);
    }
    break;

case KEY_DOWN:
    if (currentLine->next && currentLine->next != tail) {
        currentLine = currentLine->next; // 다음 라인으로 이동
        y++;
        x = strlen(currentLine->text);
        move(y, x);
    }
    break;
            case KEY_LEFT:
                if (x > 0) {
                    x--;
                    move(y, x);
                }
                break;
            case KEY_RIGHT:
                if (x < strlen(currentLine->text)) {
                    x++;
                    move(y, x);
                }
                break;
            case KEY_HOME:
                printw("Home key\n");
                break;
            case KEY_END:
                printw("End key\n");
                break;
            case KEY_PPAGE:
                printw("Page Up\n");
                break;
            case KEY_NPAGE:
                printw("Page Down\n");
                break;
            case '\n': // Enter key
                // 새 라인 추가
                currentLine = addLine(currentLine, "");
                y++; // y 값을 업데이트
                move(y, 0); // 다음 라인으로 커서 이동
                break;
            case KEY_BACKSPACE:
            case '\b':
            case 127: // 백스페이스 키 처리
                if (x > 0) {
                    int len = strlen(currentLine->text);
                    if (len > 0) {
                        currentLine->text[len - 1] = '\0';
                        move(y, --x);
                        delch();
                    }
                } else if (y > 0) {
                    // 이전 라인으로 이동
                    currentLine = currentLine->prev;
                    y = y - 1;
                    x = strlen(currentLine->text);
                    move(y, x);
                }
            
                break;


//            case 27: // ESC key
//                break;
            default:
                // 현재 라인에 문자 추가
                int len = strlen(currentLine->text);
                currentLine->text = realloc(currentLine->text, len + 2);
                currentLine->text[len] = ch;
                currentLine->text[len + 1] = '\0';
                printw("%c", ch); // 화면에 문자 출력
                break;
            
        	move(y, x);
        }
    }
}

int main(int argc, char*argv[]){
	
	int x=0,y=0;
	FILE *file;
	init();
	initscr();            // curses 모드 시작
    cbreak();             // 버퍼링 없이 각 키 입력을 즉시 받음
    noecho();
    resize_term(0, 0);     // PDCurses 윈도우 크기 조
    keypad(stdscr, TRUE); // 특수 키 입력 활성화
	if(argc==1){
		
		processKeyInput(); 
	}else if(argc==2){
		printFileContent(argv[1]);
        refresh();
        getch();
        if (file == NULL) {
    	printw("파일을 열 수 없습니다: %s\n", strerror(errno)); // 오류 메시지 출력
        refresh();
        endwin();
        exit(EXIT_FAILURE);
        }
		processKeyInput(); 
        // 파일 내용을 읽고 처리하는 코드를 여기에 추가하세요.
        fclose(file);
	}
    endwin();         // curses 모드 종료
}
