import sys
sys.path.append('../')

from easyrpc_serialize import EasyRPCSerialize

sut_write = EasyRPCSerialize()
sut_write.writeByte(1)
sut_write.writeShort(2)
sut_write.writeInt(3)
sut_write.writeString("Rafael")

sut_read = EasyRPCSerialize()
sut_read.data_read = sut_write.data_write
value = sut_read.readByte()
if value != 1:
    raise Exception("Expected 1 but received ",value)

value = sut_read.readShort()
if value != 2:
    raise Exception("Expected 2 but received ",value)

value = sut_read.readInt()
if value != 3:
    raise Exception("Expected 3 but received ",value)

value = sut_read.readString()
if value != "Rafael":
    raise Exception("Expected Rafael but received ",value)

print("OK")