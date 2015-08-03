<?

if (!defined('FILE_ROLE')) {
    define('FILE_ROLE', true);

class file_role extends base
{
    protected $_v = array(
                            'role_id'  => 0,
                            'file_id'  => 0
                         );

    protected $_enable_get = array(
                                    'role_id',
                                    'file_id'
                                  );

    protected $_enable_set = array(
                                    'role_id',
                                    'file_id'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'role_id'           => 0,
                        'file_id'  => 0
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
        
        $query = 'INSERT INTO "'.TABLE_FILE_ROLE.'"';

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
        if ($this->_v['role_id']>0 && $this->_v['file_id']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_FILE_ROLE.'" WHERE "role_id"=\''.$db->str2base($this->_v['role_id']).'\' AND "file_id"=\''.$db->str2base($this->_v['file_id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();

        if (!is_numeric($this->_v['role_id'])) $errors[] = '[SYSTEM] role_id должно быть числом';
        if (!is_numeric($this->_v['file_id'])) $errors[] = '[SYSTEM] file_id должно быть числом';

        return $errors;
    }

    // TOOL FUNCTIONS
}

}