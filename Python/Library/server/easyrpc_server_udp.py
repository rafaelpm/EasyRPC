import socket
import sys
sys.path.append('../')
from easyrpc_package_server import EasyRPCPackageServer

class EasyRPCServerUDP:

    port = 2000
    sock = None
    close_server = False
    methods = []
    count_null_data = 0

    def __init__(self, port) -> None:
        self.port = port

    def process_data(self, conn, data):
        #print("process_data: ",data)
        answer = self.check_methods(conn, data)        
        if len(answer) == 0:
            return
        #print("Answer: ",answer)
        conn.sendall(bytes(answer))

    def check_methods(self, conn, data):        
        pkt = EasyRPCPackageServer()
        pkt.params.clear()
        if pkt.set_data_from_client(data) == False:
            #print("check_methods: fail package")
            return []
        
        ack = EasyRPCPackageServer()
        conn.sendall(bytes(ack.build_ack()))
        
        #print("method name: ",pkt.method_name)
        found = False
        for m in self.methods:     
            if m.exist_method(pkt.method_name):
                found = True                
                pkt.return_info.value = m.run_method(pkt.method_name, pkt.params)
                break

        if found == False:            
            return []
        #print("Res: ",pkt.return_info.value)
        data_return = pkt.get_data_to_client()
        del pkt
        return data_return

    def finish(self):
        if len(self.methods) == 0:
            print("Please add methods!")
            return

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(('0.0.0.0', self.port))
        #self.sock.settimeout(1)
        
        print("EasyRPC waiting connection at port: ",self.port)
        self.close_server = False
        while self.close_server == False:            
            try:
                conn, addr = self.sock.accept()
                if conn == None:
                    continue                
                print("New connection from: ",addr)
                self.count_null_data = 0
                conn_id = self.sock.fileno()                
                while self.sock.fileno() == conn_id:
                    data = conn.recv(1024)
                    if self._is_disconnected(data):
                        break
                    
                    self.process_data(conn, data)
                    data = []
                    
                print("Finish connection: ",addr)
                conn.close()
            except socket.error:                
                continue
            except KeyboardInterrupt:                
                self.close_server = True
                break

        self.sock.close()

    def _is_disconnected(self, data):
        if len(data) == 0:
            self.count_null_data+=1
            if self.count_null_data >= 100:
                return True
        else:
            self.count_null_data = 0
        return False