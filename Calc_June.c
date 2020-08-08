#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#pragma warning(disable : 4996)

//Read only integers from the input string, put them into an array
double* IntStringParse(char* line, int* n) {
	//max buffer length 
	int max_len = 10;
	//the size of the chunk in which memory is allocated
	int size = 2;
	//individual character when reading string character-by-character
	char c;
	int i;
	//Allocation of memory for the array of numbers
	double* NumInt = (double*)malloc(size * sizeof(double));
	unsigned int k;
	//Array of numbers stored as characters
	char* NumChar;
	//Temporary variable for reallocation
	double* tmp;
	int j = 0;
	for (k = 0; k < strlen(line);) {
		NumChar = (char*)malloc(max_len * sizeof(char));
		i = 0;
		//Read each character
		c = line[k++];
		//Skip each chracter which isn't a number
		while ((c != '+') && (c != '-') && (c != '/') && (c != '*') && (c != '\0') && (c != '(') && (c != ')')) {
			NumChar[i++] = (char)c;
			c = line[k++];
		}
		NumChar[i] = '\0';

		//Check that you aren't allocating NULL
		if (NumInt != NULL) {
			//Convert character array into a number array
			NumInt[j++] = atof(NumChar);
			size += 1;
			//temporary reallocation variable
			tmp = (double*)realloc(NumInt, size * sizeof(double));
			if (tmp != NULL) {
				NumInt = tmp;
			}
		}

		free(NumChar);
	}


	*n = j;
	return NumInt;
}



char* SymbolReturn(char* line, int* n) {
	//size of input string in chars
	int size = 2;
	char c;
	char* tmp;
	char* SymChar = (char*)malloc(size * sizeof(char));
	unsigned int k;
	int  j = 0;
	for (k = 0; k < strlen(line); k++) {
		c = line[k];
		//Check if character is a symbol
		if ((c == '+') || (c == '-') || (c == '/') || (c == '*') || (c == '(') || (c == ')')) {
			if (SymChar != NULL) {
				SymChar[j++] = (char)c;
				size += 2;
				tmp = (char*)realloc(SymChar, size * sizeof(char));
				if (tmp != NULL) {
					SymChar = tmp;
				}
			}
		}
	}
	*n = j;
	return SymChar;
}

//Function that looks for brackets
//char* BracketSearch(double* NumInt, char* SymChar, int size_n, int size_c) {

//}
//Split Calc into two functions. 1 - what to call, Calc or Bracket search. 2 - The rest
//Calculation function. Works only with 2 (or 3) of the numbers, getting an answer for them and then putting it back into the array
double Calc(double* NumInt, char* SymChar, int size_n, int size_c) {
	double num1, num2, num3 = 0, ans = 0;
	char sym;
	char sym2 = NULL;
	char sym3 = NULL;
	int i = 0;
	int j = 0;
	int k = 0;

	
	//Check that there is an array to read
	while (size_n > 1) {
		num1 = NumInt[0];
		num2 = NumInt[1];
		sym = SymChar[0];
		//Check if the array has 3 numbers in the first place
		if (size_n > 2) {
			num3 = NumInt[2];
			sym2 = SymChar[1];
		}
		if (size_c > 3) {
			sym3 = SymChar[2];
		}



		//Simple switch for calculation, works on the 2nd and 3rd numbers
		if (sym2 && (sym == '+' || sym == '-') && (sym2 == '*' || sym2 == '/')) {
			switch (sym2) {
			case('*'):
				ans = num2 * num3;
				break;
			case('/'):
				ans = num2 / num3;
				break;
			}
			//We get an answer for the 2nd and 3rd number, replace the 2nd one with the answer
			NumInt[1] = ans;
			//Removing the unnecesary, third element from an array
			for (i = 2; i < size_n; i++) {
				NumInt[i] = NumInt[i + 1];
			}
			size_n--;
			//Removing the symbol from the array
			for (i = 1; i < size_c; i++) {
				SymChar[i] = SymChar[i + 1];
			}
			size_c--;
		}

		//When the amount of numebers in the array is 2 or less
		else {
			switch (sym) {
			case('+'):
				ans = num1 + num2;
				break;
			case('-'):
				ans = num1 - num2;
				break;
			case('/'):
				ans = num1 / num2;
				break;
			case('*'):
				ans = num1 * num2;
				break;
			}
			NumInt[0] = ans;
			//Remove the 2nd number from the array
			for (i = 1; i < size_n; i++) {
				NumInt[i] = NumInt[i + 1];
			}
			size_n--;
			//Remove
			for (i = 0; i < size_c; i++) {
				SymChar[i] = SymChar[i + 1];
			}
			size_c--;
		}
		sym2 = NULL;
	}
	return NumInt[0];

}

//function that categorizes strings - whether they are comments, expressions or blank
void StringChange(char* line) {
	double* NumInt;
	char* SymChar;
	int size = 2;
	unsigned int i = 0;
	char* line2;
	unsigned int length = strlen(line);
	double result = 0;
	int size_NumInt, size_SymChar;
	//if the user presses "Enter"
	if (line[0] == '\0') {
		return;
	}
	//If the line is blank, print out a blank
	else if (isspace(line[0])) {
		for (i = 0; i < length; i++) {

			if (isspace(line[i]) && line[i] != '\n') {
				printf("%c", line[i]);
			}
			else
				break;
		}

	}

	//if the line is a comment, print out the comment with "//"
	if (strstr(line, "//") != NULL) {
		line2 = strstr(line, "//");
		printf("%s", line2);
		return;
	}
	else if (i == length)
		return;
	//this is an expression, parse it into numbers and symbols, call necessary functions
	else {
		NumInt = IntStringParse(line, &size_NumInt);
		SymChar = SymbolReturn(line, &size_SymChar);
		result = Calc(NumInt, SymChar, size_NumInt, size_SymChar);
		printf("%s == %.2f", line + i, result);
	}

}





//function that scans every individual string in stdin
bool StringScan() {
	//max buffer length
	int max_len = 128;
	//size of input string in chars
	int size = 0;
	//temporary variable for realloc
	char* tmp, * line = (char*)malloc(max_len);
	size = max_len;
	//first check for possible memory error
	if (line == NULL)
	{
		return false;
	}
	while (line != NULL && !feof(stdin))
	{
		int c = EOF;
		int i = 0;
		while ((c = getchar()) != '\n' && c != EOF)
		{

			line[i++] = (char)c;

			//allocation of extra memory if line is longer than max_size
			if (i == size)
			{
				size = i + max_len;
				tmp = (char*)realloc(line, size);
				if (tmp != NULL) {
					line = tmp;
				}
				//if there is a problem with memory allocation
				else {
					return false;
				}
			}
		}

		line[i] = '\0';
		if (c == EOF && line[0] == '\0')
			break;

		StringChange(line);
		if (c == EOF) {
			int length = strlen(line);
			for (i = 0; i < length; i++) {
				if (!isspace(line[i]))
					break;
			}
			if (i == length) {
				break;
			}
		}
		printf("\n");
	}
	free(line);
	return true;

}

//function that scans a files contents, similar to one above
bool FileScan(char* name) {
	FILE* f;
	f = fopen(name, "r");
	int i, max_len = 128;
	int size = 0;
	char* tmp, * line = (char*)malloc(max_len);
	size = max_len;
	if (line == NULL)
	{
		return false;
	}
	while (line != NULL && !feof(f))
	{
		int c = EOF;
		i = 0;
		while ((c = fgetc(f)) != '\n' && c != EOF)
		{
			line[i++] = (char)c;

			if (i == size)
			{
				size = i + max_len;
				tmp = (char*)realloc(line, size);
				if (tmp != NULL) {
					line = tmp;
				}
				else {
					return false;
				}
			}
		}

		line[i] = '\0';
		if (c == EOF && line[0] == '\0')
			break;

		StringChange(line);
		if (c == EOF) {
			int length = strlen(line);
			for (i = 0; i < length; i++) {
				if (!isspace(line[i]))
					break;
			}
			if (i == length) {
				break;
			}
		}
		printf("\n");

	}
	fclose(f);
	free(line);
	return true;
}

int main(int argc, char* argv[]) {
	//Checks for number command line arguments
	if (argc > 2) {
		printf("ERROR: too many arguments");
		return 1;
	}
	if (argv[1] != NULL) {
		FILE* f;
		f = fopen(argv[1], "r");
		if (!f) {
			//check if the file indicated by user exists at all
			fprintf(stderr, "ERROR: Such file does not exist");
			return 2;
		}
		//error output if memory allocations fails in file
		if (!FileScan(argv[1])) {
			printf("ERROR: Memory");
			return 3;
		}
	}
	else {
		//error output if memory allocations fails in console
		if (!StringScan()) {
			printf("ERROR: Memory");
			return 3;
		}
	}

	return 0;

}

