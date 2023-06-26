#include "../FunctionsBuilder/FunctionsBuilder.h"
#include "../FunctionsBuilder/BuildServerFunctions.h"

using namespace std;

int main() {
	printf("TestBuildServerFunctions\n");

	string content = "\nint sum(int a,int b);\n";
	content += "float subtract(float a, float b);\n";
	content += "void print(char *message);\n";	
	content += "void binaryData(byte *data);\n";
	//Extra
	content += "string get_name();\n";
	content += "set_name(string name);\n";

	//content = "void binaryData(byte *data);\n";

	ParserFunctions* helperFunction = new ParserFunctions();
	helperFunction->ParserContent(content);

	BuildServerFunctions* sut = new BuildServerFunctions();
	sut->buildToContent(helperFunction);

	//sut->saveFileOnPath("../../../../../EasyRPC/TestServer/");
	sut->saveFileOnPath("/");

	return 0;
}
