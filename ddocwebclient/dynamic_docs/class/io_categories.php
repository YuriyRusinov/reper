<?
//Категория
if (!defined('CATEGORIES')) {
    define('CATEGORIES', true);

class io_categories extends base
{
    protected $_v = array(
    
                            'id'                  			=> 0,
                            'id_io_type'                	=> 0,
                            'id_child'                 		=> 0,
                            'is_main'               		=> 'true',
                            'name'                			=> '',
                            'code'              			=> '',
                            'description'   				=> '',
                            'is_system'                  	=> 'false'
							                           
                         );

    protected $_enable_get = array(
                            'id'                  		,
                            'id_io_type'                	,
                            'id_child'                 		,
                            'is_main'               		,
                            'name'                			,
                            'code'              			,
                            'description'   				,
                            'is_system'                  	
                                  );

    protected $_enable_set = array(
                                    'id'                  		,
                            		'id_io_type'                ,
                            		'id_child'                 	,
                            		'is_main'               	,
                            		'name'                		,
                            		'code'              		,
                            		'description'   			,
                            		'is_system'                  	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					  'id'                  			=> 0,
                            'id_io_type'                	=> 0,
                            'id_child'                 		=> 0,
                            'is_main'               		=> 'true',
                            'name'                			=> '',
                            'code'              			=> '',
                            'description'   				=> '',
                            'is_system'                  	=> 'false'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_CATEGORIES.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
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
        
        if($this->_v['id_child'] == 0)
             $this->_v['id_child'] = NULL;
        
        if ($this->_v['id']>0) {
            $query = 'UPDATE "'.TABLE_CATEGORIES.'" SET ';
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
        	
           /* $query = 'INSERT INTO "'.TABLE_CATEGORIES.'"';
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
            
            
        }*/

        if($this->_v['id_child'] === 0)
        {
        	$idChild = 'NULL';
        }
        
       		$query = "SELECT   cinsert('".$this->_v['name']."','".$this->_v['code']."','".$this->_v['description']."',".$this->_v['id_io_type'].", $idChild, FALSE, NULL)";
        
        }
        
       //echo $query; 
        
       $db->query($query);
 
       $id = $db->value[0]['idCategory'];
       
        if ($this->_v['id']==0) {
            $this->_v['id'] = $db->last_insert_id();
        }
        

        
        return true;
    }

    public function Delete()
    {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_CATEGORIES.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
         if (!is_numeric($this->_v['id_io_type'])) $errors[] = '[SYSTEM] id_io_type должно быть числом';
        
         if (!trim($this->_v['name'])) $errors[] = '[SYSTEM] name должно быть числом';
         if (!trim($this->_v['code'])) $errors[] = '[SYSTEM] code должно быть задано';
               
          /*      
          is_system = FALSE , так как оператор не имеет права создавать системные категории
                     	
            */  
         
        return $errors;
    }

 public function ChildExists() {
        
        if (is_null($this->_v['id'])) {
           
        	return false;
            
        }
        else {
        	if($this->_v['is_main'] == TRUE)
        	{
        		return true;
        	}
        
        }
       
        return false;
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
