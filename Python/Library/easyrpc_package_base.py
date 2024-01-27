import struct
from easyrpc_serialize import EasyRPCSerialize



class EasyRPCPackageBase:
    
    header = 0x55
    version = 1
    sequence = 1
    checksum = 1
    reserved = 0
    size = 0
    data = []
    
    _mount_position = 0
    _max_positions = 6
    _header_size = 8

    streamIn = None

    streamData = None

    error = False

    #from client
    def set_data(self, data):
        if self.streamIn == None:
            self._mount_position = 0
            self.error = False
            self.streamIn = EasyRPCSerialize()

        self.streamIn.data_read += data

        while(self._mount_position <= self._max_positions):
            #print("pos: ",self._mount_position)
            if(self.streamIn.is_end_read()):
                self.error = True
                break

            if(self._mount_position == 0):
                self.header = 0
                while(self.streamIn.is_end_read_len(1) == False):
                    self.header = self.streamIn.readByte()
                    if(self.header == 0x55):
                        self._mount_position += 1
                        break
            elif(self._mount_position == 1):
                self.version = self.streamIn.readByte()
                self._mount_position += 1
            elif(self._mount_position == 2):
                self.sequence = self.streamIn.readByte()
                self._mount_position += 1
            elif(self._mount_position == 3):
                if(self.streamIn.is_end_read_len(2)):
                    break
                self.checksum = self.streamIn.readShort()
                self._mount_position += 1
            elif(self._mount_position == 4):
                self.reserved = self.streamIn.readByte()
                self._mount_position += 1
            elif(self._mount_position == 5):
                if(self.streamIn.is_end_read_len(2)):
                    break
                self.size = self.streamIn.readShort()
                self._mount_position += 1

                if(self.size == 0):                    
                    self._mount_position += 1
                    break
                
            elif(self._mount_position == 6):
                #print("Pos 6: ",self.size, " / ",self.streamIn.index_read," x ",len(data))
                if(self.streamIn.is_end_read_len(self.size)):
                    break
                self.data = self.streamIn.readArray(self.size)
                self._mount_position += 1

                self.streamData = EasyRPCSerialize()                
                self.streamData.data_read += self.data
                self.streamData.index_read = 0
                #print("streamData: ",self.streamData.data_read)

                self.streamIn = None
                break

        #End while

    # is Ack?
    def isAck(self):
        return (self.reserved & 0x01) == 0x01

    # to client
    def wrapData(self, data):
        #print("wrapData: ",data)
        self.size = len(data)
        out = EasyRPCSerialize()        
        out.writeByte(0x55)
        out.writeByte(self.version)
        out.writeByte(self.sequence)
        out.writeShort(self.checksum)
        out.writeByte(self.reserved)
        if(self.size > 0):
            out.writeBinaryArray(data)
        else:
            out.writeShort(0)
        return out.data_write

        
        