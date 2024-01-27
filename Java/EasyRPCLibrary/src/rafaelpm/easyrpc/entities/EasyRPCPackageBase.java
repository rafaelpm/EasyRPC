package rafaelpm.easyrpc.entities;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import rafaelpm.easyrpc.packages_build.StatePackage;

/**
 *
 * @author Rafael
 */
public class EasyRPCPackageBase {
    protected int header;
    protected int version = 1;
    protected int sequence = 1;
    protected int checksum;
    protected int reserved;
    protected int size;
    
    protected int mountPosition;
    protected final int maxPositions = 6;
    protected final int headerSize = 8;
    
    protected byte[] bufferData;
    
    private ByteArrayOutputStream cache;
        
    protected byte[] getBinary(DataInputStream dis) throws IOException{
        int size = dis.readShort();
        size &= 0xFFFF;
        byte[] buffer = new byte[size];
        dis.read(buffer);
        return buffer;
    }
    
    protected void setBinary(byte[] buffer, DataOutputStream dos) throws IOException{
        dos.writeShort(buffer.length);
        dos.write(buffer);
    }
    
    protected String getBool(DataInputStream dis) throws IOException{
        byte size = dis.readByte();
        byte data = dis.readByte();
        return data == 't' ? "t":"f";
    }
    
    protected String getString(DataInputStream dis) throws IOException{
        int sizeName = dis.readByte();
        sizeName &= 0xFF;
        byte[] bufferName = new byte[sizeName];
        dis.read(bufferName);
        return (new String(bufferName)).trim();
    }
    
    protected void setString(String str, DataOutputStream dos) throws IOException{
        byte[] buffer = str.getBytes();
        if(buffer.length > 255){
            byte[] temp = new byte[255];
            for(int i=0; i < temp.length; i++){
                temp[i] = buffer[i];
            }
            buffer = new byte[255];
            for(int i=0; i < temp.length; i++){
                buffer[i] = temp[i];
            }
        }
        dos.writeByte(buffer.length);
        dos.write(buffer);
    }
    
    protected byte[] wrapData(byte[] data){
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        try{
            dos.writeByte(0x55);
            dos.writeByte(version);
            dos.writeByte(sequence);
            dos.writeShort(checksum);
            dos.writeByte(reserved);
            dos.writeShort(data.length);
            if(data.length > 0){
                dos.write(data);
            }
            
        }catch(Exception e){
            e.printStackTrace();
        }
        return baos.toByteArray();
    }
    
    public boolean setData(byte[] data) throws Exception {
        return setData(new DataInputStream(new ByteArrayInputStream(data)));
    }
    
    public StatePackage statePackage = StatePackage.None;
    private int countEndData = 0;
    private int maxEmptyBufferTest = 3;
    
    public void setMaxEmptyBufferTest(int max){
        maxEmptyBufferTest = max;
    }
    
    public boolean setData(DataInputStream dis) throws Exception {
        if(dis.available() <= headerSize){ 
            countEndData++;
            if(countEndData >= maxEmptyBufferTest){
                countEndData = 0;
                statePackage = StatePackage.Error;
            }else{
                statePackage = StatePackage.Incomplete;
            }
            saveCache(dis);
            return false;
        }
        while(mountPosition <= maxPositions){
            
            try{     
                if(dis.available() <= 0){
                    countEndData++;
                    if(countEndData >= 3){
                        countEndData = 0;
                        statePackage = StatePackage.Error;
                    }else{
                        statePackage = StatePackage.Incomplete;
                    }
                    return false;
                }
                switch(mountPosition){
                    case 0:                        
                        do{
                            if(dis.available() == 0){
                                break;
                            }
                            header = dis.readByte();                            
                        }while(header != 0x55);
                        if(header == 0x55){
                            countEndData = 0;
                            statePackage = StatePackage.Incomplete;
                            mountPosition++;
                        }
                        break;
                    case 1:
                        if(dis.available() < 1){
                            break;
                        }
                        version = dis.readByte();
                        version &= 0xFF;
                        mountPosition++;
                        break;
                    case 2:
                        if(dis.available() < 1){
                            break;
                        }
                        sequence = dis.readByte();
                        sequence &= 0xFF;
                        mountPosition++;
                        break;
                    case 3:
                        if(dis.available() < 2){
                            break;
                        }
                        checksum = dis.readShort();
                        checksum &= 0xFFFF;
                        mountPosition++;
                        break;
                    case 4:
                        if(dis.available() < 1){
                            break;
                        }
                        reserved = dis.readByte();
                        reserved &= 0xFF;
                        mountPosition++;
                        break;
                    case 5:
                        if(dis.available() < 2){
                            break;
                        }
                        size = dis.readShort();
                        size &= 0xFFFF;
                        mountPosition++;
                        if(size == 0){
                            statePackage = StatePackage.Complete;
                            mountPosition++;
                            return true;
                        }
                        break;
                    case 6:
                        if(dis.available() < size){
                            break;
                        }
                        
                        /*bufferData = new byte[dis.available()];
                        dis.read(bufferData);                        
                        dis = new DataInputStream(new ByteArrayInputStream(bufferData));*/
                                                
                        if(!processData(dis)){
                            return false;
                        }
                        
                        statePackage = StatePackage.Complete;
                                                                        
                        mountPosition++;
                        break;
                }
            }catch(Exception e){
                e.printStackTrace();
            }
        }
                
        return mountPosition >= maxPositions;
    }
    
    public boolean isACK(){
        return (reserved & 0x01) == 0x01;
    }
    
    protected boolean processData(DataInputStream dis) throws Exception {
        return false;
    }
    
    private void saveCache(DataInputStream dis) throws Exception {
        dis = loadCache(dis);
        
        byte[] buffer = new byte[dis.available()];
        dis.read(buffer);
        
        cache = new ByteArrayOutputStream();
        cache.write(buffer);
    }
    
    private DataInputStream loadCache(DataInputStream dis) throws Exception {
        if(cache == null){
            return dis;
        }
        byte[] bufferIn = new byte[dis.available()];
        dis.read(bufferIn);
        cache.write(bufferIn);
        
        dis = new DataInputStream(new ByteArrayInputStream(cache.toByteArray()));
        cache = null;
        return dis;
    }
        
    public void printHex(String title, byte[] data){
        String out = "";
        for(int i=0; i < data.length; i++){
            if(i > 0){
                out += ",";
            }
            out += "0x"+String.format("%02X", data[i]);
        }
        System.out.println(title+out);
    }
}
