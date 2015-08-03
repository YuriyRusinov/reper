<?
//объявления
if (!defined('DLS')) {
    define('DLS', true);

class position extends base
{

    protected $_v = array(
                            'id'                   	=> 0,
                            'id_positon'           	=> 0,
                            'id_unit'              	=> 0,
                            'id_user'              	=> 0,
                            'is_io'                	=> '',
                            'name'              	=> '',
                            'address'   			=> ''
                            
                         );

    protected $_enable_get = array(
                                    'id' ,
    								'id_position' ,
    								'id_unit' ,
    								'id_user' ,
    								'is_io' ,
    								'name' ,
    								'address' 
                                  );

    protected $_enable_set = array(
                                    'id' ,
    								'id_position' ,
    								'id_unit' ,
    								'id_user' ,
    								'is_io' ,
    								'name' ,
    								'address' 
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        				
                            'id'                   	=> 0,
                            'id_positon'           	=> 0,
                            'id_unit'              	=> 0,
                            'id_user'              	=> 0,
                            'is_io'                	=> '',
                            'name'              	=> '',
                            'address'   			=> ''
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_DLS.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
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
            $query = 'UPDATE "'.TABLE_DLS.'" SET ';
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
        	
            $query = 'INSERT INTO "'.TABLE_DLS.'"';
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
            $query = 'DELETE FROM "'.TABLE_DLS.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
         if (!is_numeric($this->_v['id_position'])) $errors[] = '[SYSTEM] id_position должно быть числом';
         if (!is_numeric($this->_v['id_unit'])) $errors[] = '[SYSTEM] id_unit должно быть числом';
         if (!is_numeric($this->_v['id_user'])) $errors[] = '[SYSTEM] id_user должно быть числом';
        
           
            if (!trim($this->_v['is_io'])) $errors[] = '[SYSTEM] is_io должно быть числом';
            
            if (!trim($this->_v['name'])) $errors[] = '[SYSTEM] name должно быть числом';
            
            if (!trim($this->_v['address'])) $errors[] = '[SYSTEM] address должно быть числом';
            
       

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
