<?
//обьекты, свяанные с рубриками
if (!defined('IO_RUBRICATOR')) {
    define('IO_RUBRICATOR', true);

class io_rubricator extends base
{
    protected $_v = array(
    
                            'id_io_object'                	=> 0,
                            'id_rubric'                     => 0
                            
							                           
                         );

    protected $_enable_get = array(
                            'id_io_object'                	,
                            'id_rubric'                 		
                                              	
                                  );

    protected $_enable_set = array(
                                    'id_io_object'                ,
                            		'id_rubric'                 	
                            		             	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					'id_io_object'     			=> 0,
                            'id_rubric'                	=> 0
                            
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        
        return false;
    }

    public function Save()
    {
        $db = db_class::get_instance();
        if ($this->_v['id_io_object']>0 && $this->_v['id_rubric'] > 0 ) 
            {
        	
            $query = 'INSERT INTO "'.TABLE_IO_RUBRICATOR.'"';
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

        
       
        @$db->query($query);

        if ($this->_v['id']==0) {
            $this->_v['id'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id_io_object']>0 && $this->_v['id_rubric']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_IO_RUBRICATOR.'" WHERE "id_io_object"=\''.$db->str2base($this->_v['id_io_object']).'\' AND "id_rubric" = \''.$db->str2base($this->_v['id_rubric']).'\'';
            
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['id_io_object'])) $errors[] = '[SYSTEM] id_io_object должно быть числом';
        
         if (!is_numeric($this->_v['id_rubric'])) $errors[] = '[SYSTEM] id_rubric должно быть числом';
         
         

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