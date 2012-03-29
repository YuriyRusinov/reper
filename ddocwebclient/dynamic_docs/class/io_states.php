<?
//обьекты
if (!defined('IO_STATES')) {
    define('IO_STATES', true);

class io_states extends base
{
    protected $_v = array(
    
                            'id'                	=> 0,
                            'name'                  => '',
                            'description'			=> ''
                            
							                           
                         );

    protected $_enable_get = array(
                            'id'                	,
                            'name'                 ,
                            'description'		
                                              	
                                  );

    protected $_enable_set = array(
                                    'id'	                ,
                            		'name'       ,          	
                            		'description'             	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					'id'     			=> 0,
                            'name'                	=> '',
                            'description'			=> ''
                            
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        
        return false;
    }

    public function Save()
    {
        $db = db_class::get_instance();
        if ($this->_v['id']>0 ) 
            {
        	
            $query = 'INSERT INTO "'.TABLE_IO_STATES.'"';
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
        if ($this->_v['id']>0 ) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_IO_STATES.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
            
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
         if (!trim($this->_v['name'])) $errors[] = '[SYSTEM] name должно быть заполнено';
         
         

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