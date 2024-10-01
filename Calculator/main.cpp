#include <iostream>
#include <stack>
#include <valarray>
#include <fcntl.h>

using namespace std;

int Priority(char operation);
double getSingleNum(const string s,int& i);
double Opera(double num1, double num2, char op);
double Calculator(string s);
double UnaryOpera(double num1, char op);

int main() {
    system("chcp 65001");
    string s;
    getline(cin,s);
    cout<<"结果:"<<Calculator(s)<<endl;
    return 0;
}
//运用单调递减栈进行计算
double Calculator(string s){
    int len=s.size(),i=0;
    stack<double> value;
    stack<char> ope;
    while (i<len) {
        //是数字，推入数值栈
        if (isdigit(s[i])) {
            value.push(getSingleNum(s,i));//指针在数字后一位;
            i--;
        }
        //是运算符，就装载运算符并判断是否计算
        else{
            cout << "当前运算符: " << s[i] << endl;
            cout << "操作栈状态: " << (ope.empty() ? "空" : "不空") << endl;
            //栈为空
            if (ope.empty()) ope.push(s[i]);
            //符号栈不为空
            else{
                //判断倒数(inv)
                if (s[i]=='i'){
                    ope.push('i');//优先级最高，直接push
                    i+=2;
                }
                //判断开根号(sqrt)
                else if (s[i]=='s'){
                    ope.push('s');
                    i+=3;
                }
                //其他符号+运算
                else{
                    while(Priority(ope.top())>= Priority(s[i])){
                        //一元运算
                        if (ope.top()=='s' || ope.top()=='i'){
                            double num1=value.top();
                            value.pop();
                            char op=ope.top();
                            ope.pop();
                            ope.push(s[i]);
                            value.push(UnaryOpera(num1,op));
                        }
                        //二元运算
                        else{
                            double num1=value.top();
                            value.pop();
                            double num2=value.top();
                            value.pop();
                            char op=ope.top();
                            ope.pop();
                            ope.push(s[i]);
                            value.push(Opera(num1,num2,op));
                        }
                    }
                    ope.push(s[i]);

                }
            }
        }
        i++;
    }
    //全部入栈并计算完毕后，如果还有运算符没算，那么就进行最后的计算
    while (!ope.empty()){
        if (ope.top()=='s' || ope.top()=='i'){
            char op=ope.top();
            ope.pop();
            double num1=value.top();
            value.pop();
            value.push(UnaryOpera(num1,op));
        }else{
            char op=ope.top();
            ope.pop();
            double num1=value.top();
            value.pop();
            double num2=value.top();
            value.pop();
            value.push(Opera(num1,num2,op));
        }
    }
    return value.top();

}
//对一个数进行计算，包括sqrt,inv
double UnaryOpera(double num1, char op) {
    switch (op) {
        case 's':
            if (num1<0){
                cout<<"错误: 负数无法开根号！"<<endl;
                return 0;
            }
            return sqrt(num1);
        case 'i':
            if (num1==0){
                cout<<"错误: 无法对零取倒数！"<<endl;
                return 0;
            }
            return 1.0/num1;
    }
    return 0;
}

//对两个数进行运算，包括+,-,*,/,%
double Opera(double num1, double num2, char op) {
    switch (op) {
        case '+':return num1+num2;
        case '-':return num1-num2;
        case '*':return num1*num2;
        case '/':
            if (num2 == 0) {
                cout << "错误: 除数不能为零！" << endl;
                return 0;
            }
            return num1 / num2;
        case '%':
            if ((int)num2 == 0) {
                cout << "错误: 模除数不能为零！" << endl;
                return 0;
            }
            return (int)num1 % (int)num2;  // 只处理整数的模运算

    }
    return 0;
}

int Priority(char operation){
    if (operation=='+' || operation=='-') return 1;
    else if(operation=='*' || operation=='/' || operation=='%') return 2;
    else if(operation=='s' || operation=='i' ) return 3;
    else return -1;
}
//识别一个完整的数=整数部分+小数部分
double getSingleNum(const string s,int& i){
    double num=0;
    int len=s.size();
    while (i<len && (isdigit(s[i]) || s[i]=='.')){
        //处理小数部分
        if (s[i]=='.'){
            double fraction=0.1;
            i++;
            while (i<len && isdigit(s[i])){
                num+=(s[i]-'0')*fraction;
                fraction/=10;//使位数下降
                i++;
            }
            i--;//调整索引
        }
        //处理整数部分
        else{
            num=num*10+(s[i]-'0');
        }
        i++;
    }
    return num;
}