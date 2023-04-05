#include "../FunctionsBuilder/FunctionsBuilder.h"
#include "../FunctionsBuilder/BuildServerFunctions.h"

using namespace std;

int main() {
	printf("TestBuildServerFunctions\n");

	string content = "\nint func1(int a,int b);\n";
	content += "float func2(float a, bool b);\n";
	content += "bool func3();\n";
	content += "char* func4(byte *content);\n";

	ParserFunctions* helperFunction = new ParserFunctions();
	helperFunction->ParserContent(content);

	BuildServerFunctions* sut = new BuildServerFunctions();
	sut->buildToContent(helperFunction);

	return 0;
}
