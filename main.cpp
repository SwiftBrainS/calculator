#include <iostream>
#include <stack>
#include <valarray>
'5';
using namespace std;

int Priority(char operation);
double getSingleNum(const string s,int& i);
double Opera(double num1, double num2, char op);
double Calculator(string s);
double UnaryOpera(double num1, char op);

int main() {
    //system("chcp 65001");
    string s;
    getline(cin,s);
    cout<<"���:"<<Calculator(s)<<endl;
    return 0;
}
//���õ����ݼ�ջ���м���
double Calculator(string s){
    int len=s.size(),i=0;
    stack<double> value;
    stack<char> ope;
    while (i<len) {
        //�����֣�������ֵջ
        if (isdigit(s[i])) {
            value.push(getSingleNum(s,i));//ָ�������ֺ�һλ;
            i--;
        }
        //�����������װ����������ж��Ƿ����
        else{
            //cout << "��ǰ�����: " << s[i] << endl;
            //cout << "����ջ״̬: " << (ope.empty() ? "��" : "����") << endl;
            //�жϵ���(inv)
            if (s[i]=='i'){
                ope.push('i');//���ȼ���ߣ�ֱ��push
                i+=2;
            }
            //�жϿ�����(sqrt)
            else if (s[i]=='s'){
                ope.push('s');
                i+=3;
                //��ֹ����ʹ��������ǰ����
                if (s[i+1]=='-'){
                    cout<<"����: �����޷������ţ�"<<endl;
                    return 0;
                }
            }
            //��������+����
            else{
                //���㲿�֣�whileѭ����ؼӲ�Ϊ�յ����������򳬷�Χ
                while(!ope.empty() && Priority(ope.top())>= Priority(s[i])){
                    //cout<<s[i]<<"������"<<endl;
                    //һԪ����
                    if (ope.top()=='s' || ope.top()=='i'){
                        double num1=value.top();
                        value.pop();
                        char op=ope.top();
                        ope.pop();
                        value.push(UnaryOpera(num1,op));
                    }
                    //��Ԫ����
                    else{
                        double num1=value.top();
                        value.pop();
                        double num2=value.top();
                        value.pop();
                        char op=ope.top();
                        ope.pop();
                        value.push(Opera(num2,num1,op));//ע�����Ĵ���Ƿ��ģ�������2��ǰ��1�ں�
                    }
                }
                //�������������Ϻ󣬷�����ջ,һ��Ҫ��ջ�������������ջ������ջ�ᳬ��Χ
                ope.push(s[i]);

            }
        }
        i++;
    }
    //ȫ����ջ��������Ϻ�������������û�㣬��ô�ͽ������ļ���
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
            value.push(Opera(num2,num1,op));//ע�����Ĵ���Ƿ��ģ�������2��ǰ��1�ں�
        }
    }
    return value.top();

}
//��һ�������м��㣬����sqrt,inv
double UnaryOpera(double num1, char op) {
    switch (op) {
        case 's':
            if (num1<0){
                cout<<"����: �����޷������ţ�"<<endl;
                return 0;
            }
            return sqrt(num1);
        case 'i':
            if (num1==0){
                cout<<"����: �޷�����ȡ������"<<endl;
                return 0;
            }
            return 1.0/num1;
    }
    return 0;
}

//���������������㣬����+,-,*,/,%
double Opera(double num1, double num2, char op) {
    switch (op) {
        case '+':return num1+num2;
        case '-':return num1-num2;
        case '*':return num1*num2;
        case '/':
            if (num2 == 0) {
                cout << "����: ��������Ϊ�㣡" << endl;
                return 0;
            }
            return num1 / num2;
        case '%':
            if ((int)num2 == 0) {
                cout << "����: ģ��������Ϊ�㣡" << endl;
                return 0;
            }
            return (int)num1 % (int)num2;  // ֻ����������ģ����

    }
    return 0;
}

int Priority(char operation){
    if (operation=='+' || operation=='-') return 1;
    else if(operation=='*' || operation=='/' || operation=='%') return 2;
    else if(operation=='s' || operation=='i' ) return 3;
    else return -1;
}
//ʶ��һ����������=��������+С������
double getSingleNum(const string s,int& i){
    double num=0;
    int len=s.size();
    while (i<len && (isdigit(s[i]) || s[i]=='.')){
        //����С������
        if (s[i]=='.'){
            double fraction=0.1;
            i++;
            while (i<len && isdigit(s[i])){
                num+=(s[i]-'0')*fraction;
                fraction/=10;//ʹλ���½�
                i++;
            }
            i--;//��������
        }
        //������������
        else{
            num=num*10+(s[i]-'0');
        }
        i++;
    }
    return num;
}