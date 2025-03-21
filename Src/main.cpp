#include "calculate.h"
#include "initialize.h"

string s;        // 输入的中缀表达式

int main() {
    while (1) {
        Init();                 // 初始化
        getline(cin, s);        // 输入计算式
        if(s=="exit")
            break;
        else if(s=="clear")
            system("cls");
        else
            handleInput(s);         // 处理输入
    }
    return 0;
}
