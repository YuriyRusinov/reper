<?
//объявления
if (!defined('MESSAGE_JOURNAL')) {
    define('MESSAGE_JOURNAL', true);
    
class message_journal extends base
{
    protected $_v = array(
                            'id'                  			=> 0,
                            'id_dl_sender'                	=> 0,
                            'id_dl_receiver'              	=> 0,
                            'id_io_object'               	=> 0,
                            'insert_time'				    => '',
                            'sent_datetime'                 => '',
                            'receive_datetime'              => '',
                            'read_datetime'   				=> '',
							'message_body'                  => '',
							'is_outed'						=> 'true'
							
                            
                         );

    protected $_enable_get = array(
                            'id'                  			,
                            'id_dl_sender'                	,
                            'id_dl_receiver'              	,
                            'id_io_object'               	,
                            'insert_time'				,
                            'sent_datetime'                 ,
                            'receive_datetime'              ,
                            'read_datetime'   				,
							'message_body'                  ,
							'is_outed'						            
                                  );

    protected $_enable_set = array(
                              'id'                  		,
                            'id_dl_sender'                	,
                            'id_dl_receiver'              	,
                            'id_io_object'               	,
                            'insert_time'				,
                            'sent_datetime'                 ,
                            'receive_datetime'              ,
                            'read_datetime'   				,
							'message_body'                  ,
							'is_outed'						
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					  'id'                  		=> 0,
                            'id_dl_sender'                	=> 0,
                            'id_dl_receiver'              	=> 0,
                            'id_io_object'               	=> 0,
                            'insert_time'			    	=> '',
                            'sent_datetime'                 => '',
                            'receive_datetime'              => '',
                            'read_datetime'   				=> '',
							'message_body'                  => '',
							'is_outed'						=> 'true'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_MESSAGE_JOURNAL.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
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
            $query = 'UPDATE "'.TABLE_MESSAGE_JOURNAL.'" SET ';
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
        	
            $query = 'INSERT INTO "'.TABLE_MESSAGE_JOURNAL.'"';
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
            $query = 'DELETE FROM "'.TABLE_MESSAGE_JOURNAL.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
          
                
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
         if (!is_numeric($this->_v['id_dl_sender'])) $errors[] = '[SYSTEM] id_dl_sender должно быть числом';
         if (!is_numeric($this->_v['id_dl_receiver'])) $errors[] = '[SYSTEM] id_dl_receiver должно быть числом';
         
         if($this->_v['id_dl_sender'] <= 0) $errors[] = '[SYSTEM] id_dl_sender должно быть числом';
         if($this->_v['id_dl_receiver'] <= 0) $errors[] = '[SYSTEM] id_dl_receiver должно быть числом';
          
         if (!is_numeric($this->_v['id_io_object'])) $errors[] = '[SYSTEM] id_io_object должно быть числом';
         if (!trim($this->_v['sent_datetime'])) $errors[] = '[SYSTEM] send_datetime должно быть заполнено';
         
         if (!trim($this->_v['message_body'])) $errors[] = '[SYSTEM] message_body должно быть заполнено';
        
               
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
