package test_function;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.junit.Assert;
import org.junit.Test;
import org.junit.Test;
import rafaelpm.easyrpc.entities.EasyRPCPackage;
import rafaelpm.easyrpc.server.EasyRPCBaseBindClass;
import test_package.HelperFunction;

/**
 *
 * @author Rafael
 */
public class TestSumServer {
    
    @Test
    public void test(){        
        EasyRPCPackage pkg = HelperFunction.buildSum(2, 3);
        
        try {
            EasyRPCBaseBindClass sut = new Calc();
            Calc sum = new Calc();
            Assert.assertTrue(sut.process(pkg, sum));
            Assert.assertTrue(pkg.returnInfo.value.equals("5"));
        } catch (Exception ex) {
            Logger.getLogger(TestSumServer.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
}
