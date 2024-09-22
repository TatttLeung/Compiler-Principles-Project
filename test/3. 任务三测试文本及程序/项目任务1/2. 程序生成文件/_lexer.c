#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
const char* keywordSet[9] = {"do","double","else","float","if","int","return","void","while" };
static struct
{
	const char* str;
	const char* value;
} reservedWords[20] = {{"+","PLUS"},{"(","LLM"},{")","RLM"},{"*","MULTIPLY"},{"[","LMM"},{"]","RMM"},{"!=","NE"},{"%","MOD"},{",","DOU"},{"-","MINUS"},{"/","DIVIDE"},{";","SEMI"},{"<","LT"},{"<=","LTEQ"},{"=","ASSIGN"},{"==","EQ"},{">","RT"},{">=","RTEQ"},{"{","LBM"},{"}","RBM"} };
void concat(char str[], char tmp) {
	size_t len = strlen(str);

	str[len] = tmp;

	str[len + 1] = '\0';
}

bool findKeyWord(const char* str) {
	int i = 0;
	for (i = 0; i < 9; i++) {
		if (strcmp(str,keywordSet[i]) == 0) {
			return true;
		}
	}
	return false;
}

const char* getValue(const char* str) {
	int i = 0;
	for (i = 0; i < 20; ++i) {
		if (strcmp(reservedWords[i].str, str) == 0) {
			return reservedWords[i].value;
		}
	}
	return NULL;
}
void coding(FILE* input_fp,FILE* output_fp) {
	char tmp = fgetc(input_fp);
	if (tmp == ' ' || tmp == '\n' || tmp == '\t'){
		return;
	}
	ungetc(tmp, input_fp);
	int state = 0;
	bool flag = false;
	bool isIdentifier = false;
	bool isDigit = false;
	bool isAnnotation = false;
	char value[1024];
	value[0] = '\0';
	while (!flag) {
		tmp = fgetc(input_fp);
		switch (state) {
		case 0: {
			switch (tmp) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':isDigit = true; state = 1; break;
			case 'a':
			case 'A':
			case 'b':
			case 'B':
			case 'c':
			case 'C':
			case 'd':
			case 'D':
			case 'e':
			case 'E':
			case 'f':
			case 'F':
			case 'g':
			case 'G':
			case 'h':
			case 'H':
			case 'i':
			case 'I':
			case 'j':
			case 'J':
			case 'k':
			case 'K':
			case 'l':
			case 'L':
			case 'm':
			case 'M':
			case 'n':
			case 'N':
			case 'o':
			case 'O':
			case 'p':
			case 'P':
			case 'q':
			case 'Q':
			case 'r':
			case 'R':
			case 's':
			case 'S':
			case 't':
			case 'T':
			case 'u':
			case 'U':
			case 'v':
			case 'V':
			case 'w':
			case 'W':
			case 'x':
			case 'X':
			case 'y':
			case 'Y':
			case 'z':
			case 'Z':isIdentifier = true; state = 5; break;
			case '+':state = 4; break;
			case '(':state = 4; break;
			case ')':state = 4; break;
			case '*':state = 4; break;
			case '[':state = 4; break;
			case ']':state = 4; break;
			case '!':state = 2; break;
			case '%':state = 4; break;
			case ',':state = 4; break;
			case '-':state = 4; break;
			case '/':state = 8; break;
			case ';':state = 4; break;
			case '<':state = 9; break;
			case '=':state = 9; break;
			case '>':state = 9; break;
			case '_':state = 5; break;
			case '{':state = 4; break;
			case '}':state = 4; break;
			}
			break;
		}
		case 1: {
			switch (tmp) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':isDigit = true; state = 1; break;
			case '.':state = 3; break;
			}
			break;
		}
		case 2: {
			switch (tmp) {
			case '=':state = 4; break;
			}
			break;
		}
		case 3: {
			switch (tmp) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':isDigit = true; state = 6; break;
			}
			break;
		}
		case 4: {
			switch (tmp) {
			}
			break;
		}
		case 5: {
			switch (tmp) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':isDigit = true; state = 5; break;
			case 'a':
			case 'A':
			case 'b':
			case 'B':
			case 'c':
			case 'C':
			case 'd':
			case 'D':
			case 'e':
			case 'E':
			case 'f':
			case 'F':
			case 'g':
			case 'G':
			case 'h':
			case 'H':
			case 'i':
			case 'I':
			case 'j':
			case 'J':
			case 'k':
			case 'K':
			case 'l':
			case 'L':
			case 'm':
			case 'M':
			case 'n':
			case 'N':
			case 'o':
			case 'O':
			case 'p':
			case 'P':
			case 'q':
			case 'Q':
			case 'r':
			case 'R':
			case 's':
			case 'S':
			case 't':
			case 'T':
			case 'u':
			case 'U':
			case 'v':
			case 'V':
			case 'w':
			case 'W':
			case 'x':
			case 'X':
			case 'y':
			case 'Y':
			case 'z':
			case 'Z':isIdentifier = true; state = 5; break;
			case '_':state = 5; break;
			}
			break;
		}
		case 6: {
			switch (tmp) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':isDigit = true; state = 6; break;
			}
			break;
		}
		case 7: {
			switch (tmp) {
			case '\n':state = 4; break;
			default:state = 7; isAnnotation = true; break;
			}
			break;
		}
		case 8: {
			switch (tmp) {
			case '/':state = 7; break;
			}
			break;
		}
		case 9: {
			switch (tmp) {
			case '=':state = 4; break;
			}
			break;
		}
		}
		concat(value, tmp);
		if (state ==1) {
			tmp = fgetc(input_fp);
			switch (tmp) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':break;
			case '.':break;
			default: {
				flag=true;
			}
			}
			ungetc(tmp, input_fp);
		}
		else if (state ==4) {
			tmp = fgetc(input_fp);
			switch (tmp) {
			default: {
				flag=true;
			}
			}
			ungetc(tmp, input_fp);
		}
		else if (state ==5) {
			tmp = fgetc(input_fp);
			switch (tmp) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':break;
			case 'a':
			case 'A':
			case 'b':
			case 'B':
			case 'c':
			case 'C':
			case 'd':
			case 'D':
			case 'e':
			case 'E':
			case 'f':
			case 'F':
			case 'g':
			case 'G':
			case 'h':
			case 'H':
			case 'i':
			case 'I':
			case 'j':
			case 'J':
			case 'k':
			case 'K':
			case 'l':
			case 'L':
			case 'm':
			case 'M':
			case 'n':
			case 'N':
			case 'o':
			case 'O':
			case 'p':
			case 'P':
			case 'q':
			case 'Q':
			case 'r':
			case 'R':
			case 's':
			case 'S':
			case 't':
			case 'T':
			case 'u':
			case 'U':
			case 'v':
			case 'V':
			case 'w':
			case 'W':
			case 'x':
			case 'X':
			case 'y':
			case 'Y':
			case 'z':
			case 'Z':break;
			case '_':break;
			default: {
				flag=true;
			}
			}
			ungetc(tmp, input_fp);
		}
		else if (state ==6) {
			tmp = fgetc(input_fp);
			switch (tmp) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':break;
			default: {
				flag=true;
			}
			}
			ungetc(tmp, input_fp);
		}
		else if (state ==8) {
			tmp = fgetc(input_fp);
			switch (tmp) {
			case '/':break;
			default: {
				flag=true;
			}
			}
			ungetc(tmp, input_fp);
		}
		else if (state ==9) {
			tmp = fgetc(input_fp);
			switch (tmp) {
			case '=':break;
			default: {
				flag=true;
			}
			}
			ungetc(tmp, input_fp);
		}
	}
	if (findKeyWord(value)) {
		fprintf(output_fp, "%s:%s \n", value, value);
		printf("%s:%s \n", value, value);
		return;
	}
	if (isIdentifier) {
		fprintf(output_fp, "ID:%s \n", value);
		printf("ID:%s \n", value);
	return;
	}
	if (isDigit) {
		fprintf(output_fp, "NUMBER:%s \n", value);
		printf("NUMBER:%s \n", value);
		return;
	}
	if (!isAnnotation) {
		fprintf(output_fp, "%s:%s \n", getValue(value), value);
		printf("%s:%s \n", getValue(value), value);
	}
};
int main(int argc, char* argv[]) {
	FILE* input_fp = fopen("F:/个人资料/学习/计算机大三/2024 编译原理课程项目/上交材料/2. 测试文件夹/3. 任务三测试文本及程序/项目任务1/2. 程序生成文件/_sample.tny", "r");
	if (input_fp == NULL) {
		printf("Failed to open input file");
		return 1;
	}
	FILE* output_fp = fopen("F:/个人资料/学习/计算机大三/2024 编译原理课程项目/上交材料/2. 测试文件夹/3. 任务三测试文本及程序/项目任务1/2. 程序生成文件/output.lex", "w");
	if (output_fp == NULL) {
		printf("Failed to open output file");
		fclose(input_fp);
		return 1;
	}
	char c;
	while ((c=fgetc(input_fp)) != EOF) {
		ungetc(c, input_fp);
		coding(input_fp, output_fp);
	}
	fprintf(output_fp, "EOF:EOF");
	printf("EOF:EOF");
	fclose(input_fp);
	fclose(output_fp);
	return 0;
}