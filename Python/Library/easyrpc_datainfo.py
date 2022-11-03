from easyrpc_typedata import EasyRPCTypeData

class EasyRPCDataInfo:
    type = EasyRPCTypeData.Void
    value = None
    value_bin = None

    def set_int(self, v):
        self.type = EasyRPCTypeData.Integer
        self.value = str(v)