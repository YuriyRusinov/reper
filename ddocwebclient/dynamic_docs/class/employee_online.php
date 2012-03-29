<?

if (!defined('EMPLOYEE_ONLINE')) {
    define('EMPLOYEE_ONLINE', true);

class employee_online extends base
{
    protected $_v = array(
                            'last_active_date'           => 0,
                            'employee_id'  => ''
                         );

    protected $_enable_get = array(
                                    'last_active_date',
                                    'employee_id'
                                  );

    protected $_enable_set = array(
                                    'last_active_date',
                                    'employee_id'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'last_active_date' => '0000-00-00 00:00:00',
                        'employee_id'  => ''
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        return false;
    }

    public function Save()
    {
        $db = db_class::get_instance();
        
         $this->Delete();
        
        $query = 'INSERT INTO "'.TABLE_EMPLOYEE_ONLINE.'"';

        reset($this->_v);
        $data = array();
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
        
        $db->query($query);
     
        return true;
    }

    public function Delete()
    {
        if ($this->_v['last_active_date']>0 && $this->_v['employee_id']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_EMPLOYEE_ONLINE.'" WHERE "last_active_date"=\''.$db->str2base($this->_v['last_active_date']).'\' AND "employee_id"=\''.$db->str2base($this->_v['employee_id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();

        if (trim($this->_v['employee_id'])) $errors[] = '[SYSTEM] employee_id должно быть';
        if (trim($this->_v['last_active_date'])!='' && !preg_match('/\d{2,4}-\d{1,2}-\d{1,2} \d{1,2}:\d{1,2}:\d{1,2}/', $this->_v['last_active_date'])) $errors[] = '[SYSTEM] last_active_date должно быть датой длинного формата';

        return $errors;
    }

    // TOOL FUNCTIONS
}

}