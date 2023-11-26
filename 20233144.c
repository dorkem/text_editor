#include <curses.h>

int main() {
    // curses ��� ����
    initscr();          

    // Ű �Է��� �ٷ� ����ϵ��� ����
    cbreak();           
    noecho();           // �Էµ� Ű�� ȭ�鿡 ǥ������ ����
    keypad(stdscr, TRUE); // Ư�� Ű�� ����� �� �ֵ��� ����

    // ȭ�鿡 �޽��� ���
    printw("Hello, World! Press any key to continue...");

    // ����ڷκ��� Ű �Է� ����
    int ch = getch();   

    // �Է¹��� Ű�� ȭ�鿡 ���
    printw("\nYou pressed: %c", ch);

    // ȭ�鿡 ���� ������ �ݿ�
    refresh();          

    // ����ڷκ��� �ٽ� Ű �Է��� ��ٸ�
    getch();             

    // curses ��� ����
    endwin();           

    return 0;
}

