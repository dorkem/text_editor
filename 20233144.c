#include <stdio.h>
#include <stdlib.h>

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

void init(){
	line* head = (line*)malloc(sizeof(line));
	line* tail = (line*)malloc(sizeof(line));
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
}

void processKeyInput() {
    int ch;
    int done = 0;

    while (!done) {
        ch = getch(); // 키 입력 받기

        switch (ch) {
            case KEY_UP:
                printw("Up arrow\n");
                break;
            case KEY_DOWN:
                printw("Down arrow\n");
                break;
            case KEY_LEFT:
                printw("Left arrow\n");
                break;
            case KEY_RIGHT:
                printw("Right arrow\n");
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
                printw("Enter key\n");
                break;
            case KEY_BACKSPACE:
            case 127: // Backspace key
                printw("Backspace\n");
                break;
            case 27: // ESC key
                done = 1; // 종료 플래그 설정
                break;
            default:
                printw("Character pressed: %c\n", ch);
                break;
        }
    }
}

void insertnode(){
	line* newnode;
	newnode = (line*)malloc(sizeof(line));
	//scanf("%s", &newnode->text);
	//newnode->next=tail;
	
//	if(head->next==tail){
//		head->next=newnode;
//		newnode->prev=head;
//		newnode->next=tail;
//		tail->prev=newnode;
//		return;
//	}	
}

int main(){
	init();
	initscr();            // curses 모드 시작
    cbreak();             // 버퍼링 없이 각 키 입력을 즉시 받음
    noecho();             // 입력된 키를 화면에 표시하지 않음
    keypad(stdscr, TRUE); // 특수 키 입력 활성화
    processKeyInput();    // 키 입력 처리 함수 호출
    endwin();             // curses 모드 종료
	
}
