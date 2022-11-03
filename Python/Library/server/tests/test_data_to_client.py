import sys
sys.path.append('../')
sys.path.append('../../')
sys.path.append('../../client/')

from easyrpc_package_server import EasyRPCPackageServer
from easyrpc_datainfo import EasyRPCDataInfo
from easyrpc_typedata import EasyRPCTypeData
from easyrpc_package_client import EasyRPCPackageClient


sut_server = EasyRPCPackageServer()
sut_server.method_name = "sum"
sut_server.return_info = EasyRPCDataInfo()
sut_server.return_info.set_int(30)
data = sut_server.get_data_to_client()

#print(data)

sut_client = EasyRPCPackageClient()
sut_client.set_data_from_server(data)
if sut_client.method_name != "sum":
    raise Exception("Expected sum but received ",sut_client.method_name)

if sut_client.return_info.value != "30":
    raise Exception("Expected 3 but received ",sut_client.return_info.value)

print("OK")