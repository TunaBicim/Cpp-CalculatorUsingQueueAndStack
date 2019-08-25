/* This calculator checks if the user inputed argument is valid or not
and if valid compute the result. Only 32 bit positive numbers are supported
The calculations are done from right to left in the precedence order
which is
^
%
*
/
+ -
By changing the precedence of multiplication we can get correct results
even with operation done right to left.
Stack and Queue classes were taken as templates from cprogramming.com
*/
#include <iostream>
#include "string.h"
#include "Queue.cpp"
#include "Stack.cpp"
#include <math.h> //used for finding number of digits of an integer

using namespace std;

void enterExpression(char* expression, int &valid);
void checkExpression(char *expression , int &valid);
int evaluateExpression(char* expression, int &valid);
void resetCalculator(char* expression ,int &valid,int &result);
void transferIntQueue (Queue<int>* CharQueue,Queue<int>* IntQueue);
void transferCharQueue (Queue<char>* CharQueue,Queue<char>* IntQueue);
int power(int operand1, int operand2); // for calculating the result of ^ operator
int mode(int operand1, int operand2); // for calculating the result of % operator

int main()
{
	int option = 0;	//variable for choosing different options
	int valid = -2; //variable for validity
	int result = -1; //variable for the result
	char Expression[50]; //char array for input expression
	Expression[0] = '\0';
	cout << "Queue and Stack Based Calculator" << endl << endl; //Printout options
	cout << "Menu:" << endl;
	cout << "1) Enter an Expression" << endl;
	cout << "2) Check the Expression" << endl;
	cout << "3) Calculate" << endl;
	cout << "4) Reset" << endl;
	cout << "5) Exit" << endl;

	while(1)
	{
		cout << endl << "Enter your option: ";
		if (!(cin  >> option)) //If the input is faulty clear and ignore (ie. a letter is recieved instead of number)
        {
            cin.clear();
            cin.ignore();
            cout << "Please enter a valid option" << endl;
        }
		switch(option)
		{
			case 1:
                enterExpression(Expression, valid);
				break;

			case 2:
				checkExpression(Expression, valid);
				break;

			case 3:
                result = evaluateExpression(Expression, valid);
                if (result != -1)
                    cout << "Result of the expression is " << result <<'.' << endl;
				break;

			case 4:
				resetCalculator(Expression, valid, result);
				break;

			case 5:

				return 0;

			default:

				option = 0;
				break;
		}
	}
}


void enterExpression(char* expression, int &valid) //reads the input and writes it to expression array
{
	char input[50];
	cout << "Enter an expression: ";
	cin  >> input;
	strcpy(expression,input);
	valid = -1;
	return;
}

void checkExpression(char *expression, int &valid) //checks if the expression is valid or not
{
	Stack<char> *ExpressionStack;
	ExpressionStack = new Stack<char>;
	char check[50];
	strcpy(check,expression);
	int length = strlen(check);
	if (length ==0)
    {
        cout <<  "Please enter an expresion first!" << endl;
        return;
    }
	int i;
	valid=1;
	for (i = 0; i < length ; i++)
    {
		if ((check[i] == '(') || (check[i] == '[')) //push open parantheses to stack
		{
			ExpressionStack->push(check[i]);
		}
		else if (check[i]==')') //if the expression is not an open parantheses check if it a closing parantheses
		{
			if (ExpressionStack->pop()!='(') //if ')' is recieved pop stack and check if the previous parantheses is '('
			{
				cout<<"Invalid Expression!" <<endl; // if there is no match give error and exit
				i = strlen(check);
				valid = 0;
				delete ExpressionStack;
				return;
			}
		}
		else if (check[i]==']') //if ']' is recieved pop stack and check if the previous parantheses is '['
		{
			if (ExpressionStack->pop()!='[')
			{
				cout<<"Invalid Expression!" <<endl; // if there is no match give error and exit
				i = strlen(check);
				valid = 0;
				delete ExpressionStack;
				return;
			}
		} //if the character is not a parantheses check the other viable options
		else if (check[i]!='0' && check[i]!='1' && check[i]!='2' && check[i]!='3' && check[i]!='4' &&
				 check[i]!='5' && check[i]!='6' && check[i]!='7' && check[i]!='8' && check[i]!='9' &&
				 check[i]!='^' && check[i]!='%' && check[i]!='*' && check[i]!='/' && check[i]!='+' && check[i]!='-')
		{
			cout<<"Invalid Expression!" <<endl; // if input doesn't match any of these invalid char.
			i = length;
			valid = 0;
			delete ExpressionStack;
			return;

		}
		}
	if (ExpressionStack->isEmpty() == 1) //when all of the array items are check there might be unclosed parantheses so check stack
	{
		cout<<"Valid Expression!" <<endl; //if stack is empty then the expression is valid
	}
	else
	{
		cout<<"Invalid Expression!" <<endl; //if there is still items in the stack the expression is invalid
		valid = 0;
	}
	delete ExpressionStack;
    return;
}

int evaluateExpression(char* expression, int &valid)
{
	if (valid == -2)
	{
		cout << "Please enter an expression first!" << endl;
		return -1;
	}
	else if (valid == -1)
	{
		cout << "Please check the validity of the expression first!" << endl;
		return -1;
	}
	else if (valid == 0)
	{
		cout << "Input expression is not valid! Please enter a valid expression to calculate!" << endl;
		return -1;
	}
	char evaluate[50];
	strcpy(evaluate,expression);  //get the copy of the input so that if the calculation is done again it will trace the same steps
	Stack<char> *ExpressionStack;
	ExpressionStack = new Stack<char>; //Stack to pass the expression on
	Queue<char> *CharTempQueue;
	CharTempQueue = new Queue<char>;  //Temp queue to hold the operations during midsteps
	Queue<char> *CharQueue;
	CharQueue = new Queue<char>; //Queue to keep the operators
	Queue<int> *IntQueue;
	IntQueue = new Queue<int>; //Queue to keep the numbers
	Queue<int> *IntTempQueue;
	IntTempQueue = new Queue<int>; //Temp queue to hold the numbers during midsteps
	int operand1,operand2;
	int intTemp = 0; //temp to hold the numbers during transition from char to int
	int input; //variable to read the input in the for loop
	int length = strlen(evaluate); //length of the input
	int digit = 1; //if an input number has more digits than 1 this temp is used for calculations
	char charTemp= '\0'; //temp to hold the input characters during transition
	int result; //variable to hold the result
	int priority; //variable to keep track of precedence during calculations
	bool closed; //to check if the parantheses has been closed

	for (input = 0; input <= length ; input++)
	{
		if (evaluate[input]!=')' && evaluate[input]!=']' && evaluate[input] != '\0') // push inputs until a closed parantheses or null is received
		{
			ExpressionStack->push(evaluate[input]);
		}
		else //when the closing parantheses is recieved start computation
		{
		    closed = 0;
			while (ExpressionStack->isEmpty() != 1 && closed == 0)
			{
				charTemp = ExpressionStack->pop(); //pop the operations and numbers from the stack
				switch (charTemp)
				{
					case '^': //If an operation came store it in the operation queue
					case '%':
					case '*':
					case '/':
					case '+':
					case '-':
						CharQueue->enQueue(charTemp);
						IntQueue->enQueue(intTemp); //the number might have more than 1 digit so store it after recieving an operator
						intTemp = 0; //reset the variables for the next number
						digit  = 1;
						break;

					case '(': //do not store the parantheses but store the previous number
						closed = 1;
						IntQueue->enQueue(intTemp);
						intTemp = 0;
						digit  = 1;
						break;

					case '[': //do not store the parantheses but store the previous number
						closed = 1;
						IntQueue->enQueue(intTemp);
						intTemp = 0;
						digit = 1;
						break;

					default: //if a number digit is recieved store it and increment digit by 10
                        intTemp += ((int)charTemp - 48)*digit ; //this way next number corresponds to next digit
                        digit *= 10;
                        break;
				}
			}
			if (ExpressionStack->isEmpty()==1 && closed == 0) //if no open parantheses is received and stack is empty store the first number
			{
			    IntQueue->enQueue(intTemp);
				intTemp = 0;
				digit  = 1;

			}
            for (priority = 4; priority >= 0 ; priority--) //check for operations starting from highest precedence
            {
                while (CharQueue->isEmpty() != 1) //check until operation queue is empty
                {
                    charTemp = CharQueue->deQueue();
                    if ((priority == 4) && (charTemp == '^')) //power operation
                    {   operand2 = IntQueue->deQueue(); //get the operands. Since the first item in queue is the last number get them in reverse order
                        operand1 = IntQueue->deQueue();
                        result = power(operand1,operand2); //calculate result
                        IntTempQueue->enQueue(result); //keep the result in the temp queue
                        transferIntQueue(IntQueue,IntTempQueue); //transfer the operations and numbers to temp and back
                        transferIntQueue(IntTempQueue,IntQueue); //so that their order is preserved
                        transferCharQueue(CharQueue,CharTempQueue);
                        transferCharQueue(CharTempQueue,CharQueue);
                    }
                    else if ((priority == 3) && (charTemp == '%')) //mode operator
                    {
                        operand2 = IntQueue->deQueue();
                        operand1 = IntQueue->deQueue();
                        result = mode(operand1,operand2);
                        IntTempQueue->enQueue(result);
                        transferIntQueue(IntQueue,IntTempQueue);
                        transferIntQueue(IntTempQueue,IntQueue);
                        transferCharQueue(CharQueue,CharTempQueue);
                        transferCharQueue(CharTempQueue,CharQueue);
                    }
                    else if ((priority == 2) && (charTemp == '*')) //multiplication operator
                    {
                        operand2 = IntQueue->deQueue();
                        operand1 = IntQueue->deQueue();
                        result = operand1 * operand2;
                        IntTempQueue->enQueue(result);
                        transferIntQueue(IntQueue,IntTempQueue);
                        transferIntQueue(IntTempQueue,IntQueue);
                        transferCharQueue(CharQueue,CharTempQueue);
                        transferCharQueue(CharTempQueue,CharQueue);
                    }
                    else if ((priority == 1) && (charTemp == '/')) //division operator
                    {
                        operand2 = IntQueue->deQueue();
                        operand1 = IntQueue->deQueue();
                        result = operand1 / operand2;
                        IntTempQueue->enQueue(result);
                        transferIntQueue(IntQueue,IntTempQueue);
                        transferIntQueue(IntTempQueue,IntQueue);
                        transferCharQueue(CharQueue,CharTempQueue);
                        transferCharQueue(CharTempQueue,CharQueue);
                    }
                    else if ((priority == 0) && (charTemp == '+')) //addition operator
                    {
                        operand2 = IntQueue->deQueue();
                        operand1 = IntQueue->deQueue();
                        result = operand1 + operand2;
                        IntTempQueue->enQueue(result);
                        transferIntQueue(IntQueue,IntTempQueue);
                        transferIntQueue(IntTempQueue,IntQueue);
                        transferCharQueue(CharQueue,CharTempQueue);
                        transferCharQueue(CharTempQueue,CharQueue);
                    }
                    else if ((priority == 0) && (charTemp == '-')) //substraction operator
                    {
                        operand2 = IntQueue->deQueue();
                        operand1 = IntQueue->deQueue();
                        result = operand1 - operand2;
                        IntTempQueue->enQueue(result);
                        transferIntQueue(IntQueue,IntTempQueue);
                        transferIntQueue(IntTempQueue,IntQueue);
                        transferCharQueue(CharQueue,CharTempQueue);
                        transferCharQueue(CharTempQueue,CharQueue);
                    }
                    else //during the current priority if the operation doesn't match push it to temp and push its' second operand also to temp
                    {
                        CharTempQueue->enQueue(charTemp);
                        IntTempQueue->enQueue(IntQueue->deQueue());
                    }
                }
                transferCharQueue(CharTempQueue,CharQueue); //After checking all operations with this priority transfer the lower priority ones back
                transferIntQueue(IntQueue,IntTempQueue); //also restore the numbers to correct order
                transferIntQueue(IntTempQueue,IntQueue);
            }
            if (ExpressionStack->isEmpty() == 0) //after doing all the operations check if the stack is empty. If not the calculation continious
            {
                intTemp = IntQueue->deQueue(); //get the result
                int mode = int(ceil(log10(intTemp))); //calculate the number of digits
                do {
                digit = intTemp % 10; //find the smallest digit
                charTemp = (char)(digit+48);
                ExpressionStack->push(charTemp); //put it on the stack
                intTemp /= 10;
                } while (intTemp > 0); //do this until all of the digits are found
                int mode2 = mode;
                for (; mode>0 ; mode--) //the digits are in reverse order so fetch them
                {
                    CharTempQueue->enQueue(ExpressionStack->pop());
                }
                for (; mode2>0 ; mode2--) //return them so that they are in order now
                {
                    ExpressionStack->push(CharTempQueue->deQueue());
                }
                intTemp = 0; //reset variables
                closed  = 0;
                digit = 1;
            } else //if stack is empty the calculation is done
            {
                result = IntQueue->deQueue();
				delete ExpressionStack;
				delete IntQueue;
				delete IntTempQueue;
				delete CharQueue;
				delete CharTempQueue;
                return  result;//return result
            }
		}
	} //if all the expressions are evaluated and stack is not empty then the result should be in there so return it
	digit = 1;
	while (ExpressionStack->isEmpty()== 0) //calculate the number in integers
    {
        charTemp = ExpressionStack->pop();
        intTemp += ((int)charTemp - 48)*digit ;
        digit *= 10;
    }
	result = intTemp;
	delete ExpressionStack;
	delete IntQueue;
	delete IntTempQueue;
	delete CharQueue;
	delete CharTempQueue;
    return result; //return result
}

void resetCalculator(char* expression ,int &valid, int &result)
{
	expression[0] = '\0';
	valid = -2;
	result = -1;
	cout << "Reset is successful!" << endl;
	return;
}

void transferIntQueue (Queue<int>* CharQueue,Queue<int>* IntQueue) //function for transferring all elements of an int queue to another queue
{
	while (CharQueue->isEmpty() != 1)
	{
		IntQueue->enQueue(CharQueue->deQueue());
	}
	return;
}

void transferCharQueue (Queue<char>* CharQueue,Queue<char>* IntQueue) //function for transferring all elements of a char queue to another queue
{
	while (CharQueue->isEmpty() != 1)
	{
		IntQueue->enQueue(CharQueue->deQueue());
	}
	return;
}

int power(int operand1, int operand2) //power operation calculation
{
    int result = 1;
	for (int i = operand2;i>0;i--)
	{
		result *= operand1;
	}
	return result;
}

int mode(int operand1, int operand2) //mode operation calculation
{
    int result = operand1;
    while (result > operand2)
    {
        result -= operand2;
    }
    return result;
}
