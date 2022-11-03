import sys
sys.path.append('../')

from server.easyrpc_base import EasyRPCBase

class Calcule(EasyRPCBase):
    def add(self,a,b):
        return a+b

    def sub(self,a,b):
        return a-b

calcule = Calcule()
methods = calcule.get_methods()

res = ''
for method in methods:        
    if method == 'add':
        res = calcule.run_method(method,[1,4])
        break

if res != 5:
    raise Exception("Expected 5 but received ",res)

print("OK")