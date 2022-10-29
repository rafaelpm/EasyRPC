package rafaelpm.easyrpc.entities;

/**
 *
 * @author Rafael
 */
public enum TypeData {
    Void(0,"Void"),
    Integer(1,"Integer"),
    Float(2,"Float"),
    Boolean(3,"Boolean"),
    String(4,"String"),
    BinaryArray(5,"BinaryArray");
    
    public int id;
    public String name;

    TypeData(int id, String name){
        this.id = id;
        this.name = name;
    }
    
}
