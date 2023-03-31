#include "../FunctionsBuilder/FunctionsBuilder.h"
#include "../FunctionsBuilder/ParserFunction.h"

using namespace std;

int main() {
	printf("TestParserContent");
	ParserFunctions* sut = new ParserFunctions();
	sut->ParserContent("\nint sum(int a, int b){\n    return a + b;\n}\n");

	return 0;
}
