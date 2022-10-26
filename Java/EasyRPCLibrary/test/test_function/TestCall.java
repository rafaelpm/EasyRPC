package test_function;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.junit.Assert;
import org.junit.Test;
import rafaelpm.easyrpc.client.EasyRPCBaseCall;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.entities.TypeData;
import rafaelpm.easyrpc.server.EasyRPCBaseBindClass;
import test_package.HelperFunction;

/**
 *
 * @author Rafael
 */
public class TestCall {
    @Test
    public void test(){        
        EasyRPCBaseCall sut = new EasyRPCBaseCall();
        try {
            sut.setFunctionName("sum")
                .setReturnType(TypeData.Integer)
                .setParams(2, 3);            
            
            EasyRPCBaseBindClass bind = new Calc();
            Calc sum = new Calc();
            Assert.assertTrue(bind.process(sut.easyRPCPackageSend, sum));
            Assert.assertTrue(sut.easyRPCPackageSend.returnInfo.value.equals("5"));
        } catch (Exception ex) {
            Logger.getLogger(TestSumServer.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
