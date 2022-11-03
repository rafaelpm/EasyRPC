import inspect

class EasyRPCBase:
    ignore_methods = ['run_method','get_methods', 'get_method_obj','get_total_params','ignore_methods','exist_method']

    def run_method(self, method_name, params):
        obj = self.get_method_obj(method_name)
        total = self.get_total_params(method_name)
        res = 0
        # Please, help me do better that statements
        if total == 0:
            res = obj()
        elif total == 1:
            res = obj(params[0])
        elif total == 2:
            res = obj(params[0],params[1])
        elif total == 3:
            res = obj(params[0],params[1],params[2])
        elif total == 4:
            res = obj(params[0],params[1],params[2],params[3])
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