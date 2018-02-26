#include <iostream>
#include <string>
#include <stack>
using namespace std;

int StrtoInt(string);
string InttoStr(int);
string Calc(string, string, string);

int main() {
	string input, tmpStr;
	int priority, idx, tmp, neg;
	
	while (getline(cin, input)) {
		stack<string> numStack, symStack;
		tmp = 0; 
		priority = 0;
		neg = 0;
		tmpStr = "";
		
		for (int i=0; i<input.length(); i++) {
			if (isdigit(input[i])) { //number
				if (tmpStr.length() == 0) { //0~9
					if (neg == 1) tmpStr = "-" + input.substr(i, 1);
					else tmpStr = input.substr(i, 1);
					numStack.push(tmpStr);
					
					neg = 0;
				} else { // > 10
					tmpStr += input[i];
					numStack.pop();
					numStack.push(tmpStr);
				}
				
				//cout << numStack.top() << endl;
			} else { //symbol
				tmpStr = "";
				if (input[i] == ' ');
				else if (input[i] == '+' || input[i] == '-'){
					if (priority == 0 || priority == 3) { //previous symbol is '(' or none
						if (input[i-1] == '(' && input[i] == '-') neg = 1;
						else if (i == 0 && input[i] == '-') neg = 1;
						else {
							symStack.push(input.substr(i, 1));
							priority = 1;
							//cout << symStack.top() << endl;
							//cout << numStack.top() << endl;
						}
						
					} else { //previous symbol is '+', '-', '*', '/' or '%', need to calculate
						string num2 = numStack.top();
						numStack.pop();
						string num1 = numStack.top();
						numStack.pop();
						string symbol = symStack.top();
						symStack.pop();
						 
						symStack.push(input.substr(i, 1));
						tmpStr = Calc(num1, num2, symbol);
						numStack.push(tmpStr);
						
						tmpStr = "";	
					}
				} else if (input[i] == '*' || input[i] == '/' || input[i] == '%') {
					if (priority < 2 || priority == 3) { //previous symbol is '+', '-', '(' or none
						symStack.push(input.substr(i, 1));
						priority = 2;
					} else { //previous symbol is '*', '/' or '%', need to calculate
						string num2 = numStack.top();
						numStack.pop();
						string num1 = numStack.top();
						numStack.pop();
						string symbol = symStack.top();
						symStack.pop();
						 
						symStack.push(input.substr(i, 1));
						tmpStr = Calc(num1, num2, symbol);
						numStack.push(tmpStr);
						
						tmpStr = "";
					}
				} else if (input[i] == '(') {
					symStack.push(input.substr(i, 1));
					priority = 3; //to mention that there's a pair of bracket
				} else if (input[i] == ')') {
					while (symStack.top() != "(") { //keep calculating until meet '('
						string num2 = numStack.top();
						numStack.pop();
						string num1 = numStack.top();
						numStack.pop();
						string symbol = symStack.top();
						symStack.pop();
						
						tmpStr = Calc(num1, num2, symbol);
						numStack.push(tmpStr);
						tmpStr = "";
					}
					
					symStack.pop(); //pop '('
					
					//set priority based on the previous symbol
					if (symStack.empty()) priority = 0;
					else if (symStack.top() == "+" || symStack.top() == "-") priority = 1;
					else if (symStack.top() == "*" || symStack.top() == "/" 
					|| symStack.top() == "%") priority = 2;
				}
			}
		}
		
		while (!symStack.empty()) { //clear the remaining symbol
			string num2 = numStack.top();
			numStack.pop();
			string num1 = numStack.top();
			numStack.pop();
			string symbol = symStack.top();
			symStack.pop();
						
			tmpStr = Calc(num1, num2, symbol);
			numStack.push(tmpStr);
			tmpStr = "";
		}
		cout << numStack.top() << endl;
	}
}

int StrtoInt(string num) { //convert string to integer
	int result = 0;
	
	if (num[0] == '-') {
		for (int i=1; i<num.length(); i++) {
			result *= 10;
			result += num[i]-48;
		}
		
		result *= -1;
	} else {
		for (int i=0; i<num.length(); i++) {
			result *= 10;
			result += num[i]-48;
		}
	}
	
	return result;
} 

string InttoStr(int num) { //convert integer to string
	string result = "";
	/*int temp[30] = {0};
	int len = 0;
	
	if (num == 0) return "0";
	if (num < 0) {
		result = "-";
		num *= -1;
	}
	
	while (num > 0) {
		temp[len++] = num % 10;
		num /= 10;
	}
	
	for (int i=len-1; i>=0; i--) {
		char c = temp[i] + 48;
		result = result + c;
	}*/
	
	result = to_string(num); //C++ 11 required
	
	return result;
}

string Calc(string num1, string num2, string symbol) {
	int result;
	if (symbol == "+") result = StrtoInt(num1) + StrtoInt(num2);
	else if (symbol == "-") result = StrtoInt(num1) - StrtoInt(num2);
	else if (symbol == "*") result = StrtoInt(num1) * StrtoInt(num2);
	else if (symbol == "/") result = StrtoInt(num1) / StrtoInt(num2);
	else if (symbol == "%") result = StrtoInt(num1) % StrtoInt(num2);
		
	return InttoStr(result);
}
