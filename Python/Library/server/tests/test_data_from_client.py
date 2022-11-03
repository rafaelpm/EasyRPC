import sys
sys.path.append('../')
sys.path.append('../../')
sys.path.append('../../client/')

from easyrpc_package_server import EasyRPCPackageServer
from easyrpc_datainfo import EasyRPCDataInfo
from easyrpc_typedata import EasyRPCTypeData
from easyrpc_package_client import EasyRPCPackageClient

sut_client = EasyRPCPackageClient()
sut_client.method_name = "sum"
sut_client.set_return(EasyRPCTypeData.Integer)
sut_client.add_param(4,EasyRPCTypeData.Integer)
sut_client.add_param(3,EasyRPCTypeData.Integer)
data = sut_client.get_data_to_server()

sut_server = EasyRPCPackageServer()
sut_server.set_data_from_client(data)

#print(data)

if sut_server.method_name != "sum":
    raise Exception("Expected sum but received ",sut_client.method_name)

total = len(sut_server.params)
if total != 2:    
    raise Exception("Expected 2 params but received ",total)

value = sut_server.params[0].value
if value != '4':    
    raise Exception("Expected 4 params but received ",value)

value = sut_server.params[1].value
if value != '3':    
    raise Exception("Expected 3 params but received ",value)

print("OK ")