import inspect
from easyrpc_ugly_set_param import EasyRPCUglySetParam

class EasyRPCBase:
    ignore_methods = ['run_method','get_methods', 'get_method_obj','get_total_params','ignore_methods','exist_method']

    def run_method_not_work(self, method_name, params):
        obj = self.get_method_obj(method_name)
        total = self.get_total_params(method_name)
        args = []
        for param in params:
            args.append(param.get_value_param())
        
        res = obj(args)
        
        return res

    def run_method(self, method_name, params):
        obj = self.get_method_obj(method_name)
        total = self.get_total_params(method_name)

        ugly = EasyRPCUglySetParam()
        res = ugly.set_param(total, obj, params)

        return res

    def run_method_v1(self, method_name, params):
        obj = self.get_method_obj(method_name)
        total = self.get_total_params(method_name)
        
        res = 0
        # Please, help me do better that statements
        if total == 0:
            res = obj()
        elif total == 1:            
            res = obj(params[0].get_value_param())
        elif total == 2:
            res = obj(params[0].get_value_param(),params[1].get_value_param())
        elif total == 3:
            res = obj(params[0].get_value_param(),params[1].get_value_param(),params[2].get_value_param())
        elif total == 4:
            res = obj(params[0].get_value_param(),params[1].get_value_param(),params[2].get_value_param(),params[3].get_value_param())
        elif total == 5:
            res = obj(params[0],params[1],params[2],params[3],params[4])
        else:
            #exception
            print("Exception: ",total)
            res = 0

        return res

    def get_method_obj(self, method_name):
        return getattr(self,method_name)

    def get_total_params(self, method_name):
        method_obj = self.get_method_obj(method_name)
        fullArgSpec = inspect.getfullargspec(method_obj)
        return (len(fullArgSpec.args)-1)

    def get_methods(self):
        methods = []
        for method in dir(self):
            if self.ignore_methods.count(method) > 0:
                continue
            if method.startswith('_'):
                continue            
            methods += [method]
        return methods

    def exist_method(self, name):
        methods = self.get_methods()
        for m in methods:            
            if m == name:
                return True

        return False