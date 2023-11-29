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

void init(){
	line* head = (line*)malloc(sizeof(line));
	line* tail = (line*)malloc(sizeof(line));
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
}

void processKeyInput(int *x, int *y) {
    int ch;
    int done = 0;

    while (!done) {
        ch = getch(); // Ű �Է� �ޱ�

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
                done = 1; // ���� �÷��� ����
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

int main(int argc, char*argv[]){
	int x,y;
	init();
	initscr();            // curses ��� ����
    cbreak();             // ���۸� ���� �� Ű �Է��� ��� ����
    noecho();    
    keypad(stdscr, TRUE); // Ư�� Ű �Է� Ȱ��ȭ
    move(x,y);
    processKeyInput(&x, &y);
	if(argc==1){
		
	}else if(argc==2){
		fopen
	}
    processKeyInput();    // Ű �Է� ó�� �Լ� ȣ��
    endwin();             // curses ��� ����
	
}
