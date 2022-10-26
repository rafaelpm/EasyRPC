package rafaelpm.easyrpc.entities;

/**
 *
 * @author Rafael
 */
public enum EasyRPCError {
    Success(0,"Success"),
    NameFunctionEmpty(1,"Name function is empty"),
    LimitParams(2,"The function is above limit parameters, max is %d."),
    TypeNotFound(3,"Type not implemented.");
    
    public int id;
    public String description;

    EasyRPCError(int id, String description){
        this.id = id;
        this.description = description;
    }
    
}
