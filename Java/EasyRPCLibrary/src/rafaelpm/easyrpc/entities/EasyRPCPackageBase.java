package rafaelpm.easyrpc.entities;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
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
    protected int criptoType;
    protected int size;
    
    protected int mountPosition;
    protected final int maxPositions = 6;
    protected final int headerSize = 8;
    
    private ByteArrayOutputStream cache;
    
    protected byte[] getBinary(DataInputStream dis) throws IOException{
        int size = dis.readShort();
        byte[] buffer = new byte[size];
        dis.read(buffer);
        return buffer;
    }
    
    protected void setBinary(byte[] buffer, DataOutputStream dos) throws IOException{
        dos.writeShort(buffer.length);
        dos.write(buffer);
    }
    
    protected String getString(DataInputStream dis) throws IOException{
        int sizeName = dis.readByte();        
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
            dos.writeByte(criptoType);
            dos.writeShort(data.length);
            dos.write(data);
            
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
    
    public boolean setData(DataInputStream dis) throws Exception {
        if(dis.available() <= headerSize){ 
            countEndData++;
            if(countEndData >= 3){
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
                        criptoType = dis.readByte();
                        criptoType &= 0xFF;
                        mountPosition++;
                        break;
                    case 5:
                        if(dis.available() < 2){
                            break;
                        }
                        size = dis.readShort();
                        size &= 0xFFFF;
                        mountPosition++;
                        break;
                    case 6:
                        if(dis.available() < size){
                            break;
                        }
                                                
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
}
