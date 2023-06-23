#include "../FunctionsBuilder/FunctionsBuilder.h"
#include "../FunctionsBuilder/BuildClientFunctions.h"
#include "../FunctionsBuilder/SaveContent.h"
using namespace std;

int main() {
	printf("TestBuildClientFunctions\n");

	string content = "\nint sum(int a,int b);\n";
	content += "float subtract(float a, float b);\n";
	content += "void print(char *message);\n";
	content += "void binaryData(byte *data);\n";	

	ParserFunctions* helperFunction = new ParserFunctions();
	helperFunction->ParserContent(content);

	BuildClientFunctions* sut = new BuildClientFunctions();
	content = sut->buildToContent(helperFunction);

	//sut->saveFile("../../../../../EasyRPC/Tests/easy_rpc_remote_client.h");
	sut->saveFile("../../../../../EasyRPC/Tests/easy_rpc_remote_client.h");
	
	return 0;
}
