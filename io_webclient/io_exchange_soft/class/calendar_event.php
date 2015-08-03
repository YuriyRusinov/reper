<?
//события
if (!defined('CALENDAR_EVENT')) {
    define('CALENDAR_EVENT', true);

class calendar_event extends base
{

    protected $_v = array(
                            'id_event'          => 0,
                            'calendar_id'       => 0,
                            'what'              => '',
                            'where'             => '',
                            'description'       => '',
                            'start_date'        => '0000-00-00',
                            'start_time'        => '',
                            'stop_date'         => '0000-00-00',
                            'stop_time'         => '',
                            'completed'         => 'no',
                            'notice'         => 'no',
                            'mark'         => 'no'
                         );

    protected $_enable_get = array(
                                    'id_event',
                                    'calendar_id',
                                    'what',
                                    'where',
                                    'description',
                                    'start_date',
                                    'start_time',
                                    'stop_date',
                                    'stop_time',
                                    'completed',
                                    'notice',
                                    'mark'
                                  );

    protected $_enable_set = array(
                                    'id_event',
                                    'calendar_id',
                                    'what',
                                    'where',
                                    'description',
                                    'start_date',
                                    'start_time',
                                    'stop_date',
                                    'stop_time',
                                    'completed',
                                    'notice',
                                    'mark'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
                            'id_event'          => 0,
                            'calendar_id'       => 0,
                            'what'              => '',
                            'where'             => '',
                            'description'       => '',
                            'start_date'        => '0000-00-00',
                            'start_time'        => '',
                            'stop_date'         => '0000-00-00',
                            'stop_time'         => '',
                            'completed'         => 'no',
                            'notice'         => 'no',
                            'mark'         => 'no'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id_event']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_CALENDAR_EVENT.'" WHERE "id_event"=\''.$db->str2base($this->_v['id_event']).'\'';
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
        if ($this->_v['id_event']>0) {
            $query = 'UPDATE "'.TABLE_CALENDAR_EVENT.'" SET ';
            
            reset($this->_v);
        	$data = array();
        	while(list($key, $val)=each($this->_v)) {
            	if ($key=='id_event') continue;
            	if (is_null($val)) {
                	$data[] = '"'.$key.'"=NULL';
            		} else {
                		$data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            				}
        	}
        
        	$query .= join(', ', $data);

        	if ($this->_v['id_event']>0) {
            	$query .= ' WHERE id_event='.$this->_v['id_event'];
        	}    
            
        } else {
        	
        	$query = 'select getnextseq(\'calendar_event\',\'id_event\')';
        	
        	$db->query($query);
        	
        	$id = '';
        	if (count($db->value)>0) {
                $id = $db->value[0]['getnextseq'];
            }       	
        	echo "ID:".$id;
            
            $query = 'INSERT INTO "'.TABLE_CALENDAR_EVENT.'"';
            
            
            reset($this->_v);
            $data_val = array(); 
            
        while(list($key, $val)=each($this->_v)) {
        	if($key == 'id_event') $val = $id;
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

        if ($this->_v['id_event']==0) {
            $this->_v['id_event'] = $id;
            //to do 2010
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id_event']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_CALENDAR_EVENT.'" WHERE "id_event"=\''.$db->str2base($this->_v['id_event']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if ($this->_v['completed']!='yes' && $this->_v['completed']!='no') $errors[] = '[SYSTEM] completed должно быть yes/no';        
        if ($this->_v['notice']!='yes' && $this->_v['notice']!='no') $errors[] = '[SYSTEM] notice должно быть yes/no';
        if ($this->_v['mark']!='yes' && $this->_v['mark']!='no') $errors[] = '[SYSTEM] mark должно быть yes/no';
        if (!is_numeric($this->_v['id_event'])) $errors[] = '[SYSTEM] id_event должно быть числом';
        
                           
        if (!trim($this->_v['start_date'])) {
            $errors[] = 'Поле дата объявления обязателено к заполнению';
        } else {
        	if(!preg_match('/\d{2,4}-\d{1,2}-\d{1,2}/', $this->_v['start_date']))
        	{
        		 $errors[] = 'Неверный формат даты начала события';
        	}
        	
        }

        if (!trim($this->_v['stop_date'])) {
            $errors[] = 'Поле дата объявления обязателено к заполнению';
        } else {
        	if(!preg_match('/\d{2,4}-\d{1,2}-\d{1,2}/', $this->_v['stop_date']))
        	{
        		 $errors[] = 'Неверный формат даты окончания события';
        	}
        	
        }

        if (trim($this->_v['start_date']) && trim($this->_v['stop_date']) && ($this->_v['stop_date'].' '.$this->_v['stop_time'] < $this->_v['start_date'].' '.$this->_v['start_time'])){
            $errors[] = 'Дата окончания события не может быть раньше даты его начала'; 
        }
        
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
