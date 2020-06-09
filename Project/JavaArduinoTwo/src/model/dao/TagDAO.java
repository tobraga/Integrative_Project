package model.dao;

import connection.ConnectionFactory;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.logging.Level;
import java.util.logging.Logger;
import model.bean.Tag;

/**
 *
 * @author Max Braga
 */
public class TagDAO {
    
    public boolean save (Tag t){
        Connection con = ConnectionFactory.getConnection();
        PreparedStatement tag = null;
        
        try {
            
            tag = con.prepareStatement("INSERT INTO rfid (valortag) VALUES (?)");
            tag.setString(1, t.getValor());
            
            tag.executeUpdate();
            
            return true;
            
        } catch (SQLException ex) {
            
            System.err.println("Erro ao salvar: " + ex);
            
            return false;
            
        }finally{
            ConnectionFactory.closeConnection(con, tag);
        }
    }
    
    /*
    public static void main(String[] args) {
        Tag tagRfid = new Tag();
        tagRfid.setValor("AGSBVA45723");
        
        TagDAO dao = new TagDAO();
        if(dao.save(tagRfid)){
            System.out.println("Salvo com sucesso");
        }else{
            System.err.println("Erro ao salvar!");
        }
    }*/    
}
