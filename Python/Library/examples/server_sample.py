import sys
sys.path.append('../')

from server.easyrpc_base import EasyRPCBase
from server.easyrpc_server import EasyRPCServer

class Calcule(EasyRPCBase):
    def add(self,a,b):
        return a+b

    def sub(self,a,b):
        return a-b

server = EasyRPCServer()
server.methods += [Calcule()]