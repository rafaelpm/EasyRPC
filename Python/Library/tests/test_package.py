import sys
sys.path.append('../')

from easyrpc_package_base import EasyRPCPackageBase
from easyrpc_serialize import EasyRPCSerialize

sut_write = EasyRPCSerialize()
sut_write.writeString("Rafael")

sut_send = EasyRPCPackageBase()
pkt = sut_send.wrapData(sut_write.data_write)

sut_received = EasyRPCPackageBase()
sut_received.set_data(pkt)
if sut_received.error == True:
    raise Exception("Houve erro")

sut_read = EasyRPCSerialize()
sut_read.data_read = sut_received.data
value = sut_read.readString()
if value != "Rafael":
    raise Exception("Expected Rafael but received ",value)

print("OK")