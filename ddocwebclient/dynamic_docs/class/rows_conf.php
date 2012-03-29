<?

if (!defined('ROWS_CONF')) {
    define('ROWS_CONF', true);

class rows_conf extends base
{

    protected $_v = array(
                            'id_employee'            => '',
                            'columns'                => '',
                            'table_name'             => ''
                         );

    protected $_enable_get = array(
                                     	'id_employee',
                            			'columns',	
    									'table_name'
                                  );

    protected $_enable_set = array(
                                      
                                     	'id_employee',
                            			'columns',
                            			'table_name'	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
        				    'id_employee'  => '',
                            'columns'      => '',
                            'table_name'   => ''
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if (($this->_v['id_employee']>'') && ($this->_v['table_name']>'') ) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_ROWS.'" WHERE "id_employee"='.$db->str2base($this->_v['id_employee']).' AND "table_name"= \''.$db->str2base($this->_v['table_name']).'\'';
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
    	$empl = "false";
        $db = db_class::get_instance();
        
        $query = 'SELECT columns FROM "'.TABLE_ROWS.'" WHERE "id_employee"='.$db->str2base($this->_v['id_employee']).' AND "table_name"=\''.$db->str2base($this->_v['table_name']).'\'';
         
        $db->query($query);

        if (count($db->value)>0) {
            $empl = "true";
        }
        
        if ($empl === "true") {
            $query = 'UPDATE "'.TABLE_ROWS.'" SET ';
            
             reset($this->_v);
        	$data = array();
        	while(list($key, $val)=each($this->_v)) {
            	if ($key=='id_employee') continue;
            	if ($key=='table_name') continue;
            	if (is_null($val)) {
                	$data[] = '"'.$key.'"=NULL';
            	} else {
                	$data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            	}
        	}
        		$query .= join(', ', $data);
        
         		$query .= ' WHERE "id_employee"='.$db->str2base($this->_v['id_employee']).' AND "table_name"=\''.$db->str2base($this->_v['table_name']).'\'';
        
        
            
        } else {
            $query = 'INSERT INTO "'.TABLE_ROWS.'"';
            
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
        if ($this->_v['id_employee']>'' & $this->_v['table_name']>'') {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_ROWS.'" WHERE "id_employee"='.$db->str2base($this->_v['id_employee']).' AND "table_name"=\''.$db->str2base($this->_v['table_name']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();
        
        if (false)//!trim($this->_v['columns']))
        { 
        	$errors[] = 'Выберите отображаемые столбцы';
        }
 
        return $errors;
    }

    // TOOL FUNCTIONS


  

    public function CheckActionEnable(&$STORAGE, $action) {
    	
    	
        if (check_current_employee_admin($STORAGE)) return true;
       // if (!access_enable($STORAGE, 'agreement', $action)) return false;
        
        
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
