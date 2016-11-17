#include <iostream>
#include <string>
#include <stack>

using namespace std;

stack<char> stackOut, stackTemp; 

void pushFromTempToOut()
{
	stackOut.push(stackTemp.top());
	stackOut.push(',');
	stackTemp.pop();
}

int priority(char op) 
{
	if ((op == '*')||(op == '/'))
		return 2;
	if (op == '-')
		return 1;
	if (op == '+')
		return 0;
	else
		return -1;
}

double solve(char * in, int length)
{
	for (int pos = 0; pos < length; pos++)
	{
		if (isdigit(in[pos])) 
		{
			stackOut.push(in[pos]);
			if (!isdigit(in[pos+1]))
				stackOut.push(',');
		}
		else
			if ((in[pos] == '(')||(in[pos] == ')')) 
			{
				if (in[pos] == '(') 
					stackTemp.push(in[pos]);
				else
				{
					while (stackTemp.top() != '(') 
						pushFromTempToOut();
					stackTemp.pop();
				}
			}
			else
			{
				if (stackTemp.size() == 0) 
					stackTemp.push(in[pos]);
				else
				if (priority(in[pos]) <= priority(stackTemp.top())) 
				{
					pushFromTempToOut();                                               
					while (stackTemp.size() > 0 && priority(in[pos]) <= priority(stackTemp.top()))   
					    pushFromTempToOut();
					stackTemp.push(in[pos]);                            
				}
				else 
					stackTemp.push(in[pos]);                    
			}


	}
	while (stackTemp.size() > 0)   
		pushFromTempToOut();
	stackOut.push('\0');
	// reverse Polish ready
	                                          
	string expr;
	while (stackOut.size() > 0)
	{
		expr += stackOut.top();
		stackOut.pop();
	}
	int size = expr.size();
	for (int i = 0; i < (size + 1) / 2; i++) 
	{
		char temp = expr[i];
		expr[i] = expr[size - 1 - i];
		expr[size - 1 - i] = temp;
	}
	double a[1000] = {0};
	int i = 0, numOfNumbers = 0; 
	while (i < size)
	{
		double num = 0;
		if (isdigit(expr[i]))  
        {
			while (isdigit(expr[i]))
			    num = num*10.0 + (double)(expr[i++] - '0');   
		    a[numOfNumbers++] = num;
			i++;               
		}
		else 
			if (!isdigit(expr[i])) 
			{
    			switch (expr[i]) {
	    		case '+': 
					{
						a[numOfNumbers-2]+=a[numOfNumbers-1];
						numOfNumbers--;
					}
		 	    	break;
		    	case '-': 
					{
						a[numOfNumbers-2]-=a[numOfNumbers-1];
						numOfNumbers--;
					}
		    	 	break;
		        case '*': 
					{
						a[numOfNumbers-2]*=a[numOfNumbers-1];
						numOfNumbers--;
					}
	    			break;
			    case '/': 
					{
						a[numOfNumbers-2]/=a[numOfNumbers-1];
						numOfNumbers--;
					}
    				break;
				}
				i += 2;   
			}
	}
	return a[0];   
}

int main()
{
	char * expr = ("(2+3)*10-9*(7-5)/12");
	cout << expr << " = " << solve(expr, 21) << endl;
	return 0;
}