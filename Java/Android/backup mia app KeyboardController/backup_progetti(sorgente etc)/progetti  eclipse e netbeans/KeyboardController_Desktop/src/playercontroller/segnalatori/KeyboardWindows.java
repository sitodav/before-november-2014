package playercontroller.segnalatori;

import java.awt.*;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.ClipboardOwner;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.event.*;
import java.io.IOException;


/**
 *
 * @author hisheephowareu
 */

public class KeyboardWindows extends SegnalatoreGenerico{
    Robot robot;
    public KeyboardWindows() 
    {   
        try{ robot=new Robot(); } catch(Exception ex){/*TO-DO COSA DEVO FARE ?*/}
         
    }
    
    @Override
    public void launchSignal(String s)throws AWTException,IOException
    {
        if(!s.equals("alttab") && !s.equals("tab"))
            robot.keyRelease(KeyEvent.VK_ALT);
        switch(s)
        {
            
            
            case "up" : robot.keyPress(KeyEvent.VK_UP); robot.delay(500); robot.keyRelease(KeyEvent.VK_UP); break;
            case "down" : robot.keyPress(KeyEvent.VK_DOWN); robot.delay(500); robot.keyRelease(KeyEvent.VK_DOWN); break;
            case "right" : robot.keyPress(KeyEvent.VK_RIGHT); robot.delay(500); robot.keyRelease(KeyEvent.VK_RIGHT);break; 
            case "left" : robot.keyPress(KeyEvent.VK_LEFT); robot.delay(500); robot.keyRelease(KeyEvent.VK_LEFT);break;
            case "enter" : robot.keyPress(KeyEvent.VK_ENTER); robot.delay(500); robot.keyRelease(KeyEvent.VK_ENTER);break;
            case "esc" : robot.keyPress(KeyEvent.VK_ESCAPE);robot.delay(500); robot.keyRelease(KeyEvent.VK_ESCAPE); break;
            case "altf4" : 
                    robot.keyPress(KeyEvent.VK_ALT);
                    robot.keyPress(KeyEvent.VK_F4);
                    robot.delay(500);
                    robot.keyRelease(KeyEvent.VK_ALT);
                    robot.keyRelease(KeyEvent.VK_F4);
                    break; 
            case "backspace" : robot.keyPress(KeyEvent.VK_BACK_SPACE); break;    
            case "windowsexplorer" : 
                    robot.keyPress(KeyEvent.VK_WINDOWS); 
                    robot.keyPress(KeyEvent.VK_E);
                    robot.delay(500);
                    robot.keyRelease(KeyEvent.VK_WINDOWS);
                    robot.keyRelease(KeyEvent.VK_E);
                    break;
                    
            case "taskbar" : 
                    robot.keyPress(KeyEvent.VK_WINDOWS); 
                    robot.keyPress(KeyEvent.VK_TAB); 
                    robot.delay(500);
                    robot.keyRelease(KeyEvent.VK_WINDOWS);
                    robot.keyRelease(KeyEvent.VK_TAB);
                    break;  
            case "windows" : robot.keyPress(KeyEvent.VK_WINDOWS); robot.delay(100); robot.keyRelease(KeyEvent.VK_WINDOWS); break;
            case "tab" : robot.keyPress(KeyEvent.VK_TAB); robot.delay(100); robot.keyRelease(KeyEvent.VK_TAB); robot.keyRelease(KeyEvent.VK_ALT);break;
            case "alttab" :
                    robot.keyPress(KeyEvent.VK_ALT); 
                    robot.keyPress(KeyEvent.VK_TAB); robot.delay(0);
                    robot.keyRelease(KeyEvent.VK_TAB);break;
            case "minimizeall" : 
                    robot.keyPress(KeyEvent.VK_WINDOWS); 
                    robot.keyPress(KeyEvent.VK_M);robot.delay(500);  
                    robot.keyRelease(KeyEvent.VK_WINDOWS);
                    robot.keyRelease(KeyEvent.VK_M);break;
            case "aggiorna" : robot.keyPress(KeyEvent.VK_F5); robot.delay(500);robot.keyRelease(KeyEvent.VK_F5); break;
            case "backward" : 
                    robot.keyPress(KeyEvent.VK_ALT); 
                    robot.keyPress(KeyEvent.VK_LEFT); robot.delay(500);
                    robot.keyRelease(KeyEvent.VK_ALT);
                    robot.keyRelease(KeyEvent.VK_LEFT);break;
            case "forward" : 
                    robot.keyPress(KeyEvent.VK_ALT); 
                    robot.keyPress(KeyEvent.VK_RIGHT); robot.delay(500); 
                    robot.keyRelease(KeyEvent.VK_ALT);
                    robot.keyRelease(KeyEvent.VK_RIGHT);break;
            case "urlbar" : 
                    robot.keyPress(KeyEvent.VK_ALT); 
                    robot.keyPress(KeyEvent.VK_D); robot.delay(500); 
                    robot.keyRelease(KeyEvent.VK_ALT);
                    robot.keyRelease(KeyEvent.VK_D);break;
            //case "playvideo" : robot.keyPress(KeyEvent.VK_SPACE); robot.delay(500); robot.keyRelease(KeyEvent.VK_SPACE); break;
            case "property" : robot.keyPress(KeyEvent.VK_CONTEXT_MENU); robot.delay(100); robot.keyRelease(KeyEvent.VK_CONTEXT_MENU); break;
            //case "stop" : robot.keyPress(KeyEvent.VK_S); robot.delay(500);  robot.keyRelease(KeyEvent.VK_S);break;  
            //case "previous" : robot.keyPress(KeyEvent.VK_P);robot.delay(500);robot.keyRelease(KeyEvent.VK_P); break; 
            //case "next" : robot.keyPress(KeyEvent.VK_N); robot.delay(500); robot.keyRelease(KeyEvent.VK_N); break;
            case "fullscreen" : robot.keyPress(KeyEvent.VK_F11); robot.delay(500); robot.keyRelease(KeyEvent.VK_F11); break;    
            default:
                //qualunque altra stringa invece è un testo intero ...
                java.awt.datatransfer.Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
                StringSelection stringSelection = new StringSelection( s );
                clipboard.setContents(stringSelection, new ClipboardOwner(){

                @Override
                public void lostOwnership(Clipboard clipboard, Transferable contents) {
                    throw new UnsupportedOperationException("Not supported yet.");
                }
                    
                });
                robot.keyPress(KeyEvent.VK_CONTROL);
                robot.keyPress(KeyEvent.VK_V);
                robot.keyRelease(KeyEvent.VK_V);
                robot.keyRelease(KeyEvent.VK_CONTROL);
        }
        
    }
}