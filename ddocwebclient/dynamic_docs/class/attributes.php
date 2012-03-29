<?
//объявления
if (!defined('ATTRIBUTES')) {
    define('ATTRIBUTES', true);
    

class attributes extends base
{
    protected $_v = array(
    
                            'unique_id'         		   => '',
                            'id'                           => 0,
                            'id_a_type'              	   => 0,
                            'code'               		   => '',
                            'name'               		   => '',
							'title'               		   => '',
							'table_name'           		   => '',
							'column_name'          		   => '',            
							'def_width'            		   => '',
							'is_system'               		=> ''
							
							
                         );

    protected $_enable_get = array(
                            'unique_id'         		   ,
                            'id'                           ,
                            'id_a_type'              	   ,
                            'code'               		   ,
                            'name'               		   ,
							'title'               		   ,
							'table_name'           		   ,
							'column_name'          		   ,            
							'def_width'            		   ,
							'is_system'               			     
                                  );

    protected $_enable_set = array(
                            'unique_id'         		   ,
                            'id'                           ,
                            'id_a_type'              	   ,
                            'code'               		   ,
                            'name'               		   ,
							'title'               		   ,
							'table_name'           		   ,
							'column_name'          		   ,            
							'def_width'            		   ,
							'is_system'               			               		    	         	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					 'unique_id'         		   => '',
                            'id'                           => 0,
                            'id_a_type'              	   => 0,
                            'code'               		   => '',
                            'name'               		   => '',
							'title'               		   => '',
							'table_name'           		   => '',
							'column_name'          		   => '',            
							'def_width'            		   => '',
							'is_system'               		=> ''
                           
                   );	
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_ATTRIBUTES.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\' ';
            $db->query($query);

            if (count($db->value)>0) {
                $this->_v = $db->value[0];
                return true;
            }
        }

        return false;
    }

    public function Save()
    {
        $db = db_class::get_instance();
        if ($this->_v['id']>0 ) {
            $query = 'UPDATE "'.TABLE_ATTRIBUTES.'" SET ';
            reset($this->_v);
        $data = array();
        while(list($key, $val)=each($this->_v)) {
            if ($key=='id') continue;
            
            if (is_null($val)) {
                $data[] = '"'.$key.'"=NULL';
            } else {
                $data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            }
        }
        $query .= join(', ', $data);

        if ($this->_v['id']>0) {
            $query .= ' WHERE id='.$this->_v['id'];
        }
            
            
            
        } else {
        	
            $query = 'INSERT INTO "'.TABLE_ATTRIBUTES.'"';
            reset($this->_v);
            $data_val = array(); 
            
        while(list($key, $val)=each($this->_v)) {
        
            if (is_null($val)) {
                $data[] = '"'.$key.'"';
                $data_val[] = 'NULL';
                
            } else {
                $data[] = '"'.$key.'"';
                $data_val[] = '\''.$db->str2base($val).'\'';
            }
        }
        $query .= "(";
        $query .= join(', ', $data);
        
        
        $query .= ") VALUES (";
        
        $query .= join(', ', $data_val);
        $query .= ")";
            
            
        }

        
        
        $db->query($query);

        

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id']>0 ) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_ATTRIBUTES.'" WHERE "id"=\''.$db->str2base($this->_v['id_io_object']).'\' ';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
        if (!is_numeric($this->_v['id_a_type'])) $errors[] = '[SYSTEM] id_io_type должно быть числом';
        if (!trim($this->_v['unique_id'])) $errors[] = '[SYSTEM] unique_id должно быть числом';
         
        return $errors;
    }

 
    
public function CheckActionEnable(&$STORAGE, $action) {
    	
    	
        if (check_current_employee_admin($STORAGE)) return true;
        
        
        $db = db_class::get_instance();

        switch ($action) {
            case 'view':
            case 'edit':   
            case 'delete':
            	     return true; 

            default: return false;
        }

        return false;
    }

}

}
