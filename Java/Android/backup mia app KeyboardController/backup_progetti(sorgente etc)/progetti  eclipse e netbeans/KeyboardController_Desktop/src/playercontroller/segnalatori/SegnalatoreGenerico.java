/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package playercontroller.segnalatori;

import java.awt.AWTException;
import java.io.IOException;

/**
 *
 * @author sitodskij
 */
public abstract class SegnalatoreGenerico {
    public abstract void launchSignal (String s) throws AWTException,IOException;
    
}
