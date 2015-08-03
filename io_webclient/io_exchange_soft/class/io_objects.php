<?
//объявления
if (!defined('OBJECTS')) {
    define('OBJECTS', true);
    

class io_objects extends base
{
    protected $_v = array(
    
                            'id'                  			=> 0,
                            'id_io_category'                => 0,
                            'id_io_state'                 	=> 0,
                            'name'                			=> '',
                            'author'              			=> '',
                            'description'   				=> '',
                            'table_name'                  	=> '',
							'information'              		=> '',
                            'is_system'              		=> '',
							'insert_time'              		=> '',
							'id_maclabel'					=> ''

							                           
                         );

    protected $_enable_get = array(
                            'id'                  			,
                            'id_io_category'                ,
                            'id_io_state'                 	,
                            'name'                			,
                            'author'              			,
                            'description'   				,
                            'table_name'                  	,
							'information'              		,
                            'is_system'              		,
							'insert_time'              		,
							'id_maclabel'				
                                  );

    protected $_enable_set = array(
                            'id'                  			,
                            'id_io_category'                ,
                            'id_io_state'                 	,
                            'name'                			,
                            'author'              			,
                            'description'   				,
                            'table_name'                  	,
							'information'              		,
                            'is_system'              		,
							'insert_time'              		,
							'id_maclabel'	         	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					 'id'                  			=> 0,
                            'id_io_category'                => 0,
                            'id_io_state'                 	=> 0,
                            'name'                			=> '',
                            'author'              			=> '',
                            'description'   				=> '',
                            'table_name'                  	=> '',
							'information'              		=> '',
                            'is_system'              		=> '',
							'insert_time'              		=> '',
							'id_maclabel'					=> ''
                   );	
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_OBJECTS.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
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
            $query = 'UPDATE "'.TABLE_OBJECTS.'" SET ';
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
        	
            $query = 'INSERT INTO "'.TABLE_OBJECTS.'"';
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
            
        $query .= " RETURNING id"; 
           
        }

        
        
       //echo $query;
        $db->query($query);

        if (count($db->value)>0) {
         $mt = $db->value[0]['id'];
        }
         
         
       
        if ($this->_v['id']==0) {
            $this->_v['id'] = $mt;
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT  iodelete('.$db->str2base($this->_v['id']).');';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if($this->_v['author'] === 0) $errors[] = 'Не указан автор информационного объекта';
        
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
         if (!is_numeric($this->_v['id_io_category'])) $errors[] = '[SYSTEM] id_io_type должно быть числом';
         if (!trim($this->_v['id_io_state'])) $errors[] = '[SYSTEM] id_io_state должно быть числом';
         
         if($this->_v['is_system'] === true)
         {
         	if (!trim($this->_v['table_name'])) $errors[] = '[SYSTEM] table_name должно быть заполнено';
         }
         
         if (!trim($this->_v['name'])) $errors[] = '[SYSTEM] name должно быть заполнено';
         
          if (!trim($this->_v['id_maclabel'])) $errors[] = '[SYSTEM] id_maclabel гриф секретности должен быть указан';
               
      

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
