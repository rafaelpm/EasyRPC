import sys
sys.path.append('../../')
sys.path.append('../../server/')

from server.easyrpc_base import EasyRPCBase
from server.easyrpc_server_udp import EasyRPCServerUDP

class Calcule(EasyRPCBase):
    def sum(self,a,b):
        return int(a) + int(b)

    def subtract(self,a,b):
        return float(a)-float(b)

    def binaryData(self,data):
        print("BinaryData => ",data)

    def print(self,msg):
        print("<= ",msg)

obj = Calcule()

server = EasyRPCServerUDP(2000)
server.methods += [obj]
server.finish()