import sys
sys.path.append('../')
from easyrpc_package_base import EasyRPCPackageBase
from easyrpc_serialize import EasyRPCSerialize
from easyrpc_datainfo import EasyRPCDataInfo
from easyrpc_typedata import EasyRPCTypeData

class EasyRPCPackageClient(EasyRPCPackageBase):
    return_info = None
    
    method_name = '';
    params = []

    def get_data_to_server(self):
        if self.return_info == None:
            self.return_info = EasyRPCDataInfo()
            self.return_info.type = EasyRPCTypeData.Void
            
        streamPackage = EasyRPCSerialize()
        streamPackage.writeString(self.method_name)
        streamPackage.writeByte(self.return_info.type)

        streamPackage.writeByte(len(self.params))

        for param in self.params:
            streamPackage.writeByte(param.type)
            if param.type == EasyRPCTypeData.BinaryArray:
                streamPackage.writeBinaryArray(param.value_bin)
            elif param.type != EasyRPCTypeData.Void:
                streamPackage.writeString(param.value)

        return self.wrapData(streamPackage.data_write)

    def set_data_from_server(self, data):
        self.method_name = ''
        self.set_data(data)
        if self._mount_position < self._max_positions:            
            return False
        
        if self.isAck() == True:            
            return True
        
        self.method_name = self.streamData.readString()

        self.return_info = EasyRPCDataInfo()
        self.return_info.type = self.streamData.readByte()
        #print("Type: ", self.return_info.type)
        if self.return_info.type == EasyRPCTypeData.BinaryArray:
            self.return_info.value_bin = self.streamData.readBinaryArray()
        elif self.return_info.type != EasyRPCTypeData.Void:
            self.return_info.value = self.streamData.readString()

        return True

    def set_return(self, type):
        self.return_info = EasyRPCDataInfo()
        self.return_info.type = type

    def add_param(self, value):
        param = EasyRPCDataInfo()
        param.set_param(value)
        self.params += [param]