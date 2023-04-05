# CMakeList.txt: projeto de CMake para FunctionsBuilder, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# Adicione a origem ao executável deste projeto.
add_executable (FunctionsBuilder "FunctionsBuilder.cpp" "FunctionsBuilder.h" "ParserFunction.h" "Trim.h" "BuildServerFunctions.h" "TypeData.h" "BuildClientFunctions.h" "SaveContent.h")

#add_executable (TestCheckContent "../Tests/TestParserContent.cpp" "FunctionsBuilder.h" "ParserFunction.h" "Trim.h" "BuildFunction.h")
add_executable (TestBuildServerFunctions "../Tests/TestBuildServerFunctions.cpp" "FunctionsBuilder.h" "ParserFunction.h" "Trim.h" "BuildServerFunctions.h" "TypeData.h" )
#add_executable (TestBuildClientFunctions "../Tests/TestBuildClientFunctions.cpp" "FunctionsBuilder.h" "ParserFunction.h" "Trim.h" "BuildClientFunctions.h" "TypeData.h" "SaveContent.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FunctionsBuilder PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.
