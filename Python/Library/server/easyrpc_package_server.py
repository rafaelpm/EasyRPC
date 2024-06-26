import sys
sys.path.append('../')
from easyrpc_package_base import EasyRPCPackageBase
from easyrpc_serialize import EasyRPCSerialize
from easyrpc_datainfo import EasyRPCDataInfo
from easyrpc_typedata import EasyRPCTypeData

class EasyRPCPackageServer(EasyRPCPackageBase):
    return_info = None
    
    method_name = '';
    params = []

    def build_ack(self):
        self.reserved = 0x01
        data = self.wrapData([])
        self.reserved = 0
        return data

    def set_data_from_client(self, data):
        self.method_name = ''
        self.set_data(data)
        if self._mount_position < self._max_positions:
            return False
        self.method_name = self.streamData.readString()

        self.return_info = EasyRPCDataInfo()
        self.return_info.type = self.streamData.readByte()

        total_params = self.streamData.readByte()

        #print("total_params: ", total_params)

        for i in range(total_params):
            if self.streamData.is_end_read_len(3) == True:
                return False

            param = EasyRPCDataInfo()
            param.type = self.streamData.readByte()
            if param.type == EasyRPCTypeData.BinaryArray:
                param.value_bin = self.streamData.readBinaryArray()
            else:
                param.value = self.streamData.readString()
            
            #print(param.value)
            self.params += [param]

    def get_data_to_client(self):
        if self.return_info == None:
            self.return_info = EasyRPCDataInfo()
            self.return_info.type = EasyRPCTypeData.Void

        streamPackage = EasyRPCSerialize()
        streamPackage.writeString(self.method_name)
        streamPackage.writeByte(self.return_info.type)
        if self.return_info.type == EasyRPCTypeData.BinaryArray:
            streamPackage.writeBinaryArray(self.return_info.value_bin)
        elif self.return_info.type != EasyRPCTypeData.Void:
            streamPackage.writeString(self.return_info.get_value_return())

        return self.wrapData(streamPackage.data_write)
        
            