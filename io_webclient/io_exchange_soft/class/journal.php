<?
//объявления
if (!defined('JOURNAL')) {
    define('JOURNAL', true);

class journal extends base
{
    protected $_v = array(
                            'id'                  			=> 0,
                            'id_dl_from'                	=> 0,
                            'id_dl_executor'                => 0,
                            'id_dl_to'               		=> 0,
                            'id_io_category'                => '',
                            'id_jr_state'              		=> '',
                            'insert_time'   				=> '',
                            'is_automated'                  => '',
							'exec_datetime'                 => '',
							'exec_period'                   => '',
							'message_body'                  => '',
							'is_archived'                   => '',
							'id_tabel'                  	=> '',
							'extra_id'                  	=> ''
			                            
                            
                         );

    protected $_enable_get = array(
                                   'id'                  	,
                            'id_dl_from'                	,
                            'id_dl_executor'                ,
                            'id_dl_to'               		,
                            'id_io_category'                ,
                            'id_jr_state'              		,
                            'insert_time'   				,
                            'is_automated'                  ,
							'exec_datetime'                 ,
							'exec_period'                   ,
							'message_body'                  ,
							'is_archived'                   ,
							'id_tabel'                  	,
							'extra_id'                  	
                                  );

    protected $_enable_set = array(
                                       'id'                  	,
                            'id_dl_from'                	,
                            'id_dl_executor'                ,
                            'id_dl_to'               		,
                            'id_io_category'                ,
                            'id_jr_state'              		,
                            'insert_time'   				,
                            'is_automated'                  ,
							'exec_datetime'                 ,
							'exec_period'                   ,
							'message_body'                  ,
							'is_archived'                   ,
							'id_tabel'                  	,
							'extra_id'                  	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					'id'                  			=> 0,
                            'id_dl_from'                	=> 0,
                            'id_dl_executor'                => 0,
                            'id_dl_to'               		=> 0,
                            'id_io_category'                => '',
                            'id_jr_state'              		=> '',
                            'insert_time'   				=> '',
                            'is_automated'                  => '',
							'exec_datetime'                 => '',
							'exec_period'                   => '',
							'message_body'                  => '',
							'is_archived'                   => '',
							'id_tabel'                  	=> '',
							'extra_id'                  	=> ''
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_JOURNAL.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
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
            $query = 'UPDATE "'.TABLE_JOURNAL.'" SET ';
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
        	
            $query = 'INSERT INTO "'.TABLE_JOURNAL.'"';
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

        
       //echo $query; 
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
            $query = 'DELETE FROM "'.TABLE_JOURNAL.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
         if (!is_numeric($this->_v['id_dl_from'])) $errors[] = '[SYSTEM] id_dl_from должно быть числом';
         if (!is_numeric($this->_v['id_dl_executor'])) $errors[] = '[SYSTEM] id_dl_executor должно быть числом';
         if (!is_numeric($this->_v['id_dl_to'])) $errors[] = '[SYSTEM] id_dl_to должно быть числом';
         if (!is_numeric($this->_v['id_io_category'])) $errors[] = '[SYSTEM] id_io_category должно быть числом';
         if (!is_numeric($this->_v['id_jr_state'])) $errors[] = '[SYSTEM] id_jr_state должно быть числом';
        
               
          /*      
                            'insert_time'   				,
                            'is_automated'                  ,
							'exec_datetime'                 ,
							'exec_period'                   ,
							'message_body'                  ,
							'is_archived'                   ,
							'id_tabel'                  	,
							'extra_id'                  	
            */  
         

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
