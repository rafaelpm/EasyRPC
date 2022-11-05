# EasyRPC
Easy library RPC for all.
I started that library to easily my life of developer.
The first language is Java, but I pretend do in Python and C/C++.
I'm a embedded developer to, so connect between libraries will be TCP, UDP, RS232, SPI, etc.
If you has a suggest or would contribue please contact me: rafael.p.monteiro@gmail.com


<a href="https://www.buymeacoffee.com/rafaelpmonE"><img src="https://img.buymeacoffee.com/button-api/?text=Buy me a coffee&emoji=&slug=rafaelpmonE&button_colour=FFDD00&font_colour=000000&font_family=Cookie&outline_colour=000000&coffee_colour=ffffff" /></a>

## Java Example:

For server mode, follow steps:
1) Import release JAR in your project
2) Write code below
```
System.out.println("Starting server");
try {
    EasyRPCServerTCP.build()
            .setPort(2000)
            .addClass(new Calcule())
            .finish();            
} catch (Exception ex) {
    Logger.getLogger(Server.class.getName()).log(Level.SEVERE, null, ex);
    System.exit(0);
}
```
3) Calcule is a class like below:
```
public class Calcule extends EasyRPCBaseBindClass {
    
    public Calcule() throws Exception {
        
    }
    
    public int sum(int a, int b){
        return a+b;
    }
    
    public int subtract(int a, int b){
        return a-b;
    }
}
```
4) Let's build client, so import library JAR in your project.
```
System.out.println("Starting client");        
EasyRPCBaseCall easyRPCBaseCall = new EasyRPCBaseCall();
easyRPCBaseCall.connection = new EasyRPCClientTCP("127.0.0.1", 2000);
try {
    boolean isOK = easyRPCBaseCall.setFunctionName("subtract")
            .setReturnType(TypeData.Integer)
            .setParams(10, 3)
            .send();

    if(isOK){
        System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
    }

    isOK = easyRPCBaseCall.setFunctionName("sum")
            .setReturnType(TypeData.Integer)
            .setParams(2, 3)
            .send();

    if(isOK){
        System.out.println(easyRPCBaseCall.easyRPCPackageReceived.returnInfo.value);
    }
} catch (Exception ex) {
    Logger.getLogger(Client.class.getName()).log(Level.SEVERE, null, ex);
    System.exit(0);
}
```

## Python Example

For server mode, follow steps:
Import EasyRPCBase and EasyRPCServer
```
from server.easyrpc_base import EasyRPCBase
from server.easyrpc_server import EasyRPCServer
```
Create your class and extend EasyRPCBase
```
class Calcule(EasyRPCBase):
   def sum(self,a,b):
      return int(a) + int(b)
```
Start server and add your class in:
```
server = EasyRPCServer(2000)
server.methods += [Calcule()]
server.finish()
```

For client mode, follow steps:
Import EasyRPCClient and EasyRPCTypeData
```
from easyrpc_typedata import EasyRPCTypeData
from easyrpc_client import EasyRPCClient
```
Instance EasyRPCClient and set host and port.
```
sut = EasyRPCClient()
sut.connect("127.0.0.1",2000)
```
Call function with parameters
```
res = sut.call(EasyRPCTypeData.Integer,"sum",[1,2])
```
Check answer
```
res.get_value_param()
```

## Enjoy!
