/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package playercontroller;

import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

/**
 *
 * @author sitodskij
 */
public class MainApp extends Application {
    Stage main_stage;
    Scene main_scene;
    AnchorPane main_pane;
    ControllerView controller1;
    
    
    @Override
    public void start(Stage primaryStage) {
        main_stage=primaryStage;
        FXMLLoader loader=new FXMLLoader(MainApp.class.getResource("viewroot.fxml"));
        try {
            main_pane=(AnchorPane)loader.load();
            main_scene=new Scene(main_pane);
            primaryStage.setScene(main_scene);
            primaryStage.initStyle(StageStyle.TRANSPARENT);
            main_scene.setFill(null);
            primaryStage.show();
            
            controller1=(ControllerView)loader.getController();
            controller1.collegaAlMain(this);
            
        } catch (IOException ex) {
            Logger.getLogger(MainApp.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * The main() method is ignored in correctly deployed JavaFX application.
     * main() serves only as fallback in case the application can not be
     * launched through deployment artifacts, e.g., in IDEs with limited FX
     * support. NetBeans ignores main().
     *
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
}
