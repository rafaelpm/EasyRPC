import socket
from easyrpc_package_client import EasyRPCPackageClient

class EasyRPCClient:

    sock = None
    close_client = False
    methods = []
    count_null_data = 0
    conn = None
    addr = None
    sequence = 0
    
    def connect(self, host, port):        
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((host, port))
        #self.sock.listen()
        #(self.conn, self.addr) = self.sock.accept()
         
    def call(self, typeReturn, name, params):
        pkt_send = EasyRPCPackageClient()
        pkt_send.params.clear()
        pkt_send.set_return(typeReturn)
        pkt_send.method_name = name
        for param in params:
            pkt_send.add_param(param)
        data = pkt_send.get_data_to_server()  
        #print("=> ",data)
        self.sock.sendall(bytes(data))

        del pkt_send

        # Read ACK
        data = self.sock.recv(1024)
        #print("<= ",data)
        pkt_read = EasyRPCPackageClient()
        if pkt_read.set_data_from_server(data) == False:
            return
        
        if pkt_read.isAck() == False:
            return

        # Read Answer
        data = self.sock.recv(1024)
        #print("<= ",data)
        pkt_read = EasyRPCPackageClient()
        if pkt_read.set_data_from_server(data) == False:
            return

        #print("Return data: ",pkt_read.return_data)
        self.sequence += 1
        
        return pkt_read.return_info

    def _is_disconnected(self, data):
        if len(data) == 0:
            self.count_null_data+=1
            if self.count_null_data >= 100:
                return True
        else:
            self.count_null_data = 0
        return False