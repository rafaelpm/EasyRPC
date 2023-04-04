#include "../FunctionsBuilder/FunctionsBuilder.h"
#include "../FunctionsBuilder/BuildFunction.h"

using namespace std;

int main() {
	printf("TestBuildFunction\n");

	ParserFunctions* helperFunction = new ParserFunctions();
	helperFunction->ParserContent("\nint sum(int a,int b){\n    return a + b;\n}\n");

	BuildFunction* sut = new BuildFunction();

	return 0;
}
