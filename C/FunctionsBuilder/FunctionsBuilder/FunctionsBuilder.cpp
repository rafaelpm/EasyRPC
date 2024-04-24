#include "FunctionsBuilder.h"
#include "BuildServerFunctions.h"
#include "BuildClientFunctions.h"
#include "InfoParams.h"
#include "SaveContent.h"
#include "BuildSamples.h"
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
	printVersion();
	ParserFunctions *sut = new ParserFunctions();

	if(argc <= 1){
		printParams();
		return 0;
	}

	if(!loadParams(argc, argv)){
		printParams();
		return 0;
	}

	//printf("FileIn: %s\n",Params.fileIn);

	SaveContent *readContent = new SaveContent();
	
	if(!readContent->open(Params.fileIn)){		
		return 0;
	}
	BuildSamples *buildSamples = new BuildSamples();
	BuildServerFunctions *buildServerFunctions;
	BuildClientFunctions *buildClientFunctions;
	ParserFunctions *parserFunctions = new ParserFunctions();
	parserFunctions->ParserContent(readContent->contentRead);

	switch(Params.mode){
		case Both:
		case Server:
			buildServerFunctions = new BuildServerFunctions();
			buildServerFunctions->buildToContent(parserFunctions);
			buildServerFunctions->saveFileOnPath(Params.pathOut);
			delete buildServerFunctions;
			break;
	}

	switch(Params.mode){
		case Both:
		case Client:
			buildClientFunctions = new BuildClientFunctions();
			buildClientFunctions->buildToContent(parserFunctions);
			buildClientFunctions->saveFileOnPath(Params.pathOut);
			delete buildClientFunctions;
			break;
	}

	buildSamples->saveFileOnPath(Params.pathOut);
	delete buildSamples;

	//printf("Content:\n%s\n", readContent->contentRead.c_str());

	return 0;
}
