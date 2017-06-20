// The desk calulator

// reads from standard input or command line 
// uses namespaces and no exceptions

#include <map>
#include<iostream>
#include<cctype>
#include<string>

using namespace std;

namespace Error {

	struct Zero_divide { };

	struct Syntax_error {
		const char* p;
		Syntax_error(const char* q) { p = q; }
	};
}

namespace Lexer {

	enum Token_value {
		NAME, NUMBER, END,
		PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
		PRINT = ';', ASSIGN = '=', LP = '(', RP = ')'
	};

	Token_value curr_tok;
	double number_value;
	string string_value;

	Token_value get_token();
}

namespace Parser {
	double prim(bool get);		// handle primaries
	double term(bool get);		// multiply and divide
	double expr(bool get);		// and subtract

	using namespace Lexer;
	using namespace Error;
}

namespace Symbol_table {
	map<string, double> table;
}

namespace Driver {
	int no_of_errors;
	std::istream* input;

	void skip();
}

Lexer::Token_value Lexer::get_token()
{
	char ch;

	do {	// skip whitespace except '\n'
		if (!Driver::input->get(ch)) return curr_tok = END;
	} while (ch != '\n' && isspace(ch));

	switch (ch) {
	case 0:
		return END;
	case ';':
	case '\n':
		return curr_tok = PRINT;
	case '*':
	case '/':
	case '+':
	case '-':
	case '(':
	case ')':
	case '=':
		return curr_tok = Token_value(ch);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	case '.':
		Driver::input->putback(ch);
		*Driver::input >> number_value;
		return curr_tok = NUMBER;
	default:			// NAME, NAME =, or error
		if (isalpha(ch)) {
			string_value = ch;
			while (Driver::input->get(ch) && isalnum(ch))
				string_value += ch;	// string_value.push_back(ch);
									// to work around library bug
			Driver::input->putback(ch);
			return curr_tok = NAME;
		}
		throw Error::Syntax_error("bad token");
	}
}


double Parser::prim(bool get)		// handle primaries
{
	if (get) get_token();

	switch (curr_tok) {
	case Lexer::NUMBER:	// floating point constant
		get_token();
		return number_value;
	case Lexer::NAME:
	{	double& v = Symbol_table::table[string_value];
	if (get_token() == ASSIGN) v = expr(1);
	return v;
	}
	case Lexer::MINUS:		// unary minus
		return -prim(1);
	case Lexer::LP:
	{	double e = expr(1);
	if (curr_tok != RP) throw Error::Syntax_error("`)' expected");
	get_token();			  // eat ')'
	return e;
	}
	case Lexer::END:
		return 1;
	default:
		throw Error::Syntax_error("primary expected");
	}
}

double Parser::term(bool get)		// multiply and divide
{
	double left = prim(get);

	for (;;)			// ``forever''
		switch (curr_tok) {
		case Lexer::MUL:
			left *= prim(true);
			break;
		case Lexer::DIV:
			if (double d = prim(true)) {
				left /= d;
				break;
			}
			throw Error::Zero_divide();
		default:
			return left;
		}
}

double Parser::expr(bool get)		// add and subtract
{
	double left = term(get);

	for (;;)				// ``forever''
		switch (curr_tok) {
		case Lexer::PLUS:
			left += term(true);
			break;
		case Lexer::MINUS:
			left -= term(true);
			break;
		default:
			return left;
		}
}

void Driver::skip()
{
	no_of_errors++;

	while (*input) {	// discard characters until newline or semicolon
		char ch;
		input->get(ch);

		switch (ch) {
		case '\n':
		case ';':
		return;
		}
	}
}

int fib(int n) //fibonacci formula
{
	// Declare an array to store Fibonacci numbers. 
	int f[1000]; //can't inpute [n+1]
	int i;

	// 0th and 1st number of the series are 0 and 1
	f[0] = 0;
	f[1] = 1;

	for (i = 2; i <= n; i++)
	{
		// Add the previous 2 numbers in the series and store it 
		f[i] = f[i - 1] + f[i - 2];
	}

	return f[n];
}

#include <vector>
#include <algorithm>

int median(int iArray[]) 
{
	vector<int> mArray(iArray, iArray + 14); //set this number to match the number of items in your int array
	vector<int>::iterator it;

	sort(mArray.begin(), mArray.end());

	// Find the median.
	float median, middle, middleh;
	float middlel;
	vector<int>::iterator z;
	switch (mArray.size() % 2) {
	case(0): // even
		z = mArray.begin();
		middlel = mArray.size() / 2;
		z += middlel;
		middleh = (*z + *(--z)) / 2;
		cout << "Median is: " << middleh << endl;
		break;
	case(1): // odd
		z = mArray.begin();
		middle = mArray.size() / 2;
		cout << "Median is : " << *(z += middle) << endl;
		break;
	}
	// display the sorted array.
	for (it = mArray.begin(); it != mArray.end(); it++) {
		cout << *it << " ";
	}
	return *it;
}

#include <fstream>

void load_data()
{
	string load;

	getline(cin, load);

	if (load == "load") { //to load saved file

		ifstream loadFile;

		loadFile.open("Save.txt", ifstream::in);

		cout << "The file contained: ";

		while (loadFile.good()) {

			cout << (char)loadFile.get();

		}

		cout << " " << endl;

		loadFile.close();

	}
}


#include <strstream>


int main(int argc, char* argv[])
{
	using namespace Driver;

	switch (argc) {
	case 1:			 // read from standard input
		input = &cin;
		break;
	case 2:			 // read argument string
		input = new istrstream(argv[1], strlen(argv[1]));
		break;	
	default:
		cerr << "too many arguments\n";
		return 1;
	}

	// insert pre-defined names:
	Symbol_table::table["pi"] = 3.1415926535897932385;
	Symbol_table::table["e"] = 2.7182818284590452354;

	while (*input) {
		cout << "new expression:\n";
		try {
			Lexer::get_token();
			if (Lexer::curr_tok == Lexer::END) break;
			if (Lexer::curr_tok == Lexer::PRINT) continue;
			cout << Parser::expr(false) << '\n';

			string textToSave;           //block to save all inpute
			getline(cin, textToSave);
			ofstream saveFile("Save.txt");
			saveFile << textToSave;

			saveFile.close();
		}
		catch (Error::Zero_divide) {
			cerr << "attempt to divide by zero\n";
			skip();
		}
		catch (Error::Syntax_error e) {
			cerr << "syntax error:" << e.p << "\n";
			skip();
		}
	}
	string ld = "load";
	if (cin >> ld) load_data();
	if (input != &std::cin) delete input;
	return no_of_errors;
}