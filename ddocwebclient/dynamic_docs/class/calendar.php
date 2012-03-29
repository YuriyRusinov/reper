<?
//календарь
if (!defined('CALENDAR')) {
    define('CALENDAR', true);

class calendar extends base
{

    protected $_v = array(
                            'id_calendar'       => 0,
                            'author_id'         => '',
                            'name'              => '',
                            'description'       => '',
                            'type'              => 'default'
                         );

    protected $_enable_get = array(
                                    'id_calendar',
                                    'author_id',
                                    'name',
                                    'description',
                                    'type'
                                  );

    protected $_enable_set = array(
                                    'id_calendar',
                                    'author_id',
                                    'name',
                                    'description',
                                    'type'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
                            'id_calendar'       => 0,
                            'author_id'         => '',
                            'name'              => '',
                            'description'       => '',
                            'type'              => 'default'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id_calendar']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_CALENDAR.'" WHERE "id_event"=\''.$db->str2base($this->_v['id_calendar']).'\'';
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
        if ($this->_v['id_calendar']>0) {
            $query = 'UPDATE "'.TABLE_CALENDAR.'" SET ';
             reset($this->_v);
        $data = array();
        while(list($key, $val)=each($this->_v)) {
            if ($key=='id_calendar') continue;
            if (is_null($val)) {
                $data[] = '"'.$key.'"=NULL';
            } else {
                $data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            }
        }
        $query .= join(', ', $data);

        if ($this->_v['id_calendar']>0) {
            $query .= ' WHERE id_calendar='.$this->_v['id_calendar'];
        }
        
            
            
            
        } else {
        	
        	$query = 'select getnextseq(\'calendar\',\'id_calendar\')';
        	
        	$db->query($query);
        	
        	$id = '';
        	if (count($db->value)>0) {
                $id = $db->value[0]['getnextseq'];
            }
        	
            $query = 'INSERT INTO "'.TABLE_CALENDAR.'"';
            
             reset($this->_v);
            $data_val = array(); 
            
        while(list($key, $val)=each($this->_v)) {
        	if($key == 'id_calendar') $val = $id;
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

        if ($this->_v['id_calendar']==0) {
            $this->_v['id_calendar'] = $id; 
            //2010 comment $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id_calendar']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_CALENDAR.'" WHERE "id_calendar"=\''.$db->str2base($this->_v['id_calendar']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function loadDefault($id_author){
        if ($id_author!='') {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_CALENDAR.'" WHERE "author_id"=\''.$db->str2base($id_author).'\' and type=\'default\'';
            //echo $query;
            $db->query($query);

            if (count($db->value)>0) {
                $this->_v = $db->value[0];
                return true;
            }
            else{                                
                    $this->Init();
                    $this->_v['author_id']=$id_author;
                    $this->_v['name']=$id_author;
                    $this->_v['type']='default';
                    $this->Save();
                                        
                    $query = 'INSERT INTO "'.TABLE_EMPLOYEE_CALENDAR.'" (employee_id, calendar_id) VALUES (\''.$db->str2base($this->_v['author_id']).'\','.$db->str2base($this->_v['id_calendar']).'); ';
                    
					//echo $query;						
                    $db->query($query);
                    return true;                                  
            }
        }

        return false;        
    }
    
   /*  public function loadSales($id_author){
        if ($id_author>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_CALENDAR.'" WHERE "author_id"=\''.$db->str2base($id_author).'\' and type=\'sales\'';
            $db->query($query);

            if (count($db->value)>0) {
                $this->_v = $db->value[0];
                return true;
            }
            else{
                $query = 'SELECT * FROM "'.TABLE_EMPLOYEE.'" WHERE "id_employee"=\''.$db->str2base($id_author).'\' ';
                $db->query($query);
                if (count($db->value)>0) {
                    $this->Init();
                    $this->_v['author_id']=$id_author;
                    $this->_v['name']='sales';
                    $this->_v['type']='sales';
                    $this->Save();
                    $query = 'INSERT INTO "'.TABLE_EMPLOYEE_CALENDAR.'" SET "employee_id"=\''.$db->str2base($this->_v['author_id']).'\', "calendar_id"=\''.$db->str2base($this->_v['id_calendar']).'\'';
                    $db->query($query);
                    return true;                    
                }                
            }
        }

        return false;        
    }*/
    

    public function getIdsForAuthor($id_author){
        $db = db_class::get_instance();
        $query = 'SELECT id_calendar FROM "'.TABLE_CALENDAR.'" WHERE "author_id"=\''.$db->str2base($id_author).'\' ';
        //echo $query;
        $db->query($query);
        $ids=array();
        for($i=0;$i<count($db->value);$i++){
            $ids[]=$db->value[$i]['id_calendar'];
        }
        return $ids;
    }

    public function getIdsForEmployee($id_employee){
        $db = db_class::get_instance();
        $query = 'SELECT calendar_id FROM "'.TABLE_EMPLOYEE_CALENDAR.'" WHERE "employee_id"=\''.$db->str2base($id_employee).'\' ';
		//echo $query."<br>";
        $db->query($query);
        $ids=array();
        for($i=0;$i<count($db->value);$i++){
            $ids[]=$db->value[$i]['calendar_id'];
        }
        return $ids;
    }
    
    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['id_calendar'])) $errors[] = '[SYSTEM] id_calendar должно быть числом';
        if (!is_numeric($this->_v['author_id'])) $errors[] = '[SYSTEM] author_id должно быть числом';
                
        return $errors;
    }

 

    public function CheckActionEnable(&$STORAGE, $action) {
    	
    	
        if (check_current_employee_admin($STORAGE)) return true;
        if (!access_enable($STORAGE, 'organizer', $action)) return false;
        
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
