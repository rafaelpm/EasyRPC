from easyrpc_typedata import EasyRPCTypeData

class EasyRPCDataInfo:
    type = EasyRPCTypeData.Void
    value = None
    value_bin = None

    def set_param(self, v):
        if type(v) is int:
            self.type = EasyRPCTypeData.Integer
            self.value = str(v)
        elif type(v) is float:
            self.type = EasyRPCTypeData.Float
            self.value = str(v)
        elif type(v) is bool:
            self.type = EasyRPCTypeData.Boolean
            self.value = str(v)
        elif type(v) is str:
            self.type = EasyRPCTypeData.String
            self.value = v
        else:
            self.type = EasyRPCTypeData.BinaryArray
            self.value_bin = v
            

    def set_int(self, v):
        self.type = EasyRPCTypeData.Integer
        self.value = str(v)

    def get_value_param(self):
        if self.type == EasyRPCTypeData.Integer:
            return int(self.value)
        elif self.type == EasyRPCTypeData.Float:
            return float(self.value)
        elif self.type == EasyRPCTypeData.Boolean:
            return bool(self.value)
        elif self.type == EasyRPCTypeData.BinaryArray:
            return self.value_bin
        else:
            return str(self.value)

    def get_value_return(self):
        return str(self.value)