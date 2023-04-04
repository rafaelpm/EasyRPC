#include "../FunctionsBuilder/FunctionsBuilder.h"
#include "../FunctionsBuilder/ParserFunction.h"

using namespace std;

int main() {
	printf("TestParserContent\n");

	//Test Trim
	char msg1[] = "Rafael ";
	char msg2[] = " Rafael";
	string msg3 = " Rafael    ";
	string msg4 = trim_str(msg3);

	printf("msg1: %s|\n", trim(msg1));
	printf("msg2: %s|\n", trim(msg2));
	printf("msg3: %s|\n", msg4.c_str());

	ParserFunctions* sut = new ParserFunctions();
	sut->ParserContent("\nint sum(int a,int b){\n    return a + b;\n}\n");

	return 0;
}
