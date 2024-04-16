import sys
sys.path.append('../../')
sys.path.append('../../client/')

from easyrpc_typedata import EasyRPCTypeData
from easyrpc_client import EasyRPCClient

sut = EasyRPCClient()
sut.connect("127.0.0.1",2000)
res = sut.call(EasyRPCTypeData.Integer,"sum",[1,2])
if res.get_value_param() != 3:
    raise Exception("Expected 3 but received ",res.value)
print("sum(1,2) = ",res.get_value_param())

res = sut.call(EasyRPCTypeData.Float,"subtract",[9.5,4.5])
if res.get_value_param() != 5:
    raise Exception("Expected 5 but received ",res.value)
print("subtract(9.5,4.5) = ",res.get_value_param())

res = sut.call(EasyRPCTypeData.Void,"print",["Ready in python to!"])
res = sut.call(EasyRPCTypeData.Void,"binaryData",[[0x0A,0x0B,0x0C,0x0D]])


print("OK")