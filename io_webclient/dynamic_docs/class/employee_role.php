<?

if (!defined('EMPLOYEE_ROLE')) {
    define('EMPLOYEE_ROLE', true);

class employee_role extends base
{
    protected $_v = array(
                            'role_id'           => 0,
                            'employee_id'  => 0
                         );

    protected $_enable_get = array(
                                    'role_id',
                                    'employee_id'
                                  );

    protected $_enable_set = array(
                                    'role_id',
                                    'employee_id'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'role_id'           => 0,
                        'employee_id'  => 0
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

  		$query = 'INSERT INTO "'.TABLE_EMPLOYEE_ROLE.'"';

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
        if ($this->_v['role_id']>0 && $this->_v['employee_id']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_EMPLOYEE_ROLE.'" WHERE "role_id"=\''.$db->str2base($this->_v['role_id']).'\' AND "employee_id"=\''.$db->str2base($this->_v['employee_id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();

        if (!is_numeric($this->_v['role_id'])) $errors[] = '[SYSTEM] role_id должно быть числом';
        if (!is_numeric($this->_v['employee_id'])) $errors[] = '[SYSTEM] employee_id должно быть числом';

        return $errors;
    }

    // TOOL FUNCTIONS
}

}