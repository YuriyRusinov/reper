<?

if (!defined('FILE_ATTACH')) {
    define('FILE_ATTACH', true);

class file_attach extends base
{
    protected $_types = array(
                                'project',
                                'stage',
                                'task'
                             );

    protected $_v = array(
                            'item_id'  => 0,
                            'type'     => '',
                            'file_id'  => 0
                         );

    protected $_enable_get = array(
                                    'item_id',
                                    'type',
                                    'file_id'
                                  );

    protected $_enable_set = array(
                                    'item_id',
                                    'type',
                                    'file_id'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'item_id'  => 0,
                        'type'     => '',
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
        
        $query = 'INSERT INTO "'.TABLE_FILE_ATTACH.'"';

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
        if ($this->_v['item_id']>0 && $this->_v['file_id']>0 && $this->_v['type']!='') {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_FILE_ATTACH.'" WHERE "item_id"=\''.$db->str2base($this->_v['item_id']).'\' AND "file_id"=\''.$db->str2base($this->_v['file_id']).'\' AND "type"=\''.$db->str2base($this->_v['type']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();

        if (!is_numeric($this->_v['item_id'])) $errors[] = '[SYSTEM] item_id должно быть числом';
        if (array_key_exists($this->_v['type'], $this->_types)===false) $errors[] = '[SYSTEM] type должно быть одно из: '.join(',', array_keys($this->_types));
        if (!is_numeric($this->_v['file_id'])) $errors[] = '[SYSTEM] file_id должно быть числом';

        return $errors;
    }

    // TOOL FUNCTIONS
}

}