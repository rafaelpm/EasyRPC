
class EasyRPCSerialize:

    index_write = 0
    index_read = 0
    data_write = []
    data_read = []

    def __init__(self) -> None:
        self.reset()

    def reset(self):
        self.index_write = 0
        self.index_read = 0
        self.data_write = []
        self.data_read = []

    def writeByte(self, value):
        self.data_write += [value]
        self.index_write += 1

    def writeShort(self, value):
        self.writeByte((value & 0xFF00) >> 8)
        self.writeByte((value & 0xFF))

    def writeInt(self, value):
        self.writeByte((value & 0xFF000000) >> 24)
        self.writeByte((value & 0x00FF0000) >> 16)
        self.writeByte((value & 0x0000FF00) >> 8)
        self.writeByte((value & 0xFF))

    def writeBinaryArray(self, data):
        self.writeShort(len(data))
        self.data_write += data
        self.index_write += len(data)

    def writeString(self, txt):
        #if len(txt) == 1:
        #    txt += "\0"
        value = bytearray(txt.encode())
        self.writeByte(len(value))
        self.data_write += value        
        self.index_write += len(value)

    def is_end_read_len(self, more):
        return ((self.index_read + (more-1)) >= len(self.data_read))

    def is_end_read(self):
        return self.is_end_read_len(0)

    def readString(self):        
        if(self.is_end_read()):
            return ""
        size = self.readByte()
        
        value = ''
        for _ in range(0,size):            
            value += chr(self.data_read[self.index_read])            
            self.index_read += 1
            if self.index_read > len(self.data_read):
                break
        return value

    def readByte(self):
        if(self.is_end_read()):
            return 0
        value = self.data_read[self.index_read]
        self.index_read += 1
        return value

    def readShort(self):
        if(self.is_end_read_len(2)):
            return 0
        value = self.data_read[self.index_read] << 8
        self.index_read += 1
        value |= self.data_read[self.index_read]
        self.index_read += 1
        return value

    def readInt(self):
        if(self.is_end_read_len(4)):
            return 0
        value = self.data_read[self.index_read] << 24
        self.index_read += 1
        value |= self.data_read[self.index_read] << 16
        self.index_read += 1
        value |= self.data_read[self.index_read] << 8
        self.index_read += 1
        value |= self.data_read[self.index_read]
        self.index_read += 1
        return value

    def readBinaryArray(self):
        if(self.is_end_read_len(2)):
            return []
        size = self.readShort()
        if(self.is_end_read_len(size)):
            return []
        value = self.data_read[self.index_read:(self.index_read+size)]
        return value
    
    def readArray(self, size):
        if(self.is_end_read_len(size)):
            return 0        
        value = self.data_read[self.index_read:(self.index_read+size)]
        return value