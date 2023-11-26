#include <curses.h>

int main() {
    // curses 모드 시작
    initscr();          

    // 키 입력을 바로 사용하도록 설정
    cbreak();           
    noecho();           // 입력된 키를 화면에 표시하지 않음
    keypad(stdscr, TRUE); // 특수 키를 사용할 수 있도록 설정

    // 화면에 메시지 출력
    printw("Hello, World! Press any key to continue...");

    // 사용자로부터 키 입력 받음
    int ch = getch();   

    // 입력받은 키를 화면에 출력
    printw("\nYou pressed: %c", ch);

    // 화면에 변경 사항을 반영
    refresh();          

    // 사용자로부터 다시 키 입력을 기다림
    getch();             

    // curses 모드 종료
    endwin();           

    return 0;
}

