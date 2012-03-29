<?
//объявления
if (!defined('USERS')) {
    define('USERS', true);
    
class users extends base
{
    protected $_v = array(
    
                            'id'                  			=> 0,
                            'id_rank'                		=> 0,
                            'role_name'                 	=> '',
                            'lastname'               		=> '',
                            'firstname'                		=> '',
                            'sirname'              			=> '',
                            'fio'   						=> '',
                            'insert_time'              		=> ''                           
                         );

    protected $_enable_get = array(
                            'id'                  		    ,
                            'id_rank'                		,
                            'role_name'                 	,
                            'lastname'               		,
                            'firstname'                		,
                            'sirname'              			,
                            'fio'   						,
                            'insert_time'              		
                                  );

    protected $_enable_set = array(
                            'id'                  		    ,
                            'id_rank'                		,
                            'role_name'                 	,
                            'lastname'               		,
                            'firstname'                		,
                            'sirname'              			,
                            'fio'   						,
                            'insert_time'              		          		         	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					'id'                  			=> 0,
                            'id_rank'                		=> 0,
                            'role_name'                 	=> '',
                            'lastname'               		=> '',
                            'firstname'                		=> '',
                            'sirname'              			=> '',
                            'fio'   						=> '',
                            'insert_time'              		=> ''
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_USERS.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
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
        if ($this->_v['id']>0) {
            $query = 'UPDATE "'.TABLE_USERS.'" SET ';
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
        	
            $query = 'INSERT INTO "'.TABLE_USERS.'"';
            reset($this->_v);
            $data_val = array(); 
            
        while(list($key, $val)=each($this->_v)) {
        	if($key == 'id') continue;
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

        if ($this->_v['id']==0) {
            $this->_v['id'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_USERS.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
         if (!is_numeric($this->_v['id_rank'])) $errors[] = '[SYSTEM] id_rank должно быть числом';
         if (!trim($this->_v['role_name'])) $errors[] = '[SYSTEM] role_name должно быть числом';
         if (!trim($this->_v['lastname'])) $errors[] = '[SYSTEM] lastname должно быть числом';
         if (!trim($this->_v['firstname'])) $errors[] = '[SYSTEM] firstname должно быть числом';
         if (!trim($this->_v['fio'])) $errors[] = '[SYSTEM] fio должно быть числом';
               
      

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
