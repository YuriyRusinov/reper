<?

if (!defined('MODULE_ACTION')) {
    define('MODULE_ACTION', true);

class module_action extends base
{
    protected $_v = array(
                            'id_module_action'       => 0,
                            'module_id'              => 0,
                            'action'                 => '',
                            'name'                   => '',
                            'public_flag'            => 'yes'
                         );

    protected $_enable_get = array(
                                    'id_module_action',
                                    'module_id',
                                    'action',
                                    'name',
                                    'public_flag'
                                  );

    protected $_enable_set = array(
                                    'id_module_action',
                                    'module_id',
                                    'action',
                                    'name',
                                    'public_flag'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'id_module_action'       => 0,
                        'module_id'              => 0,
                        'action'                 => '',
                        'name'                   => '',
                        'public_flag'            => 'yes'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id_module_action']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_MODULE_ACTION.'" WHERE "id_module_action"=\''.$db->str2base($this->_v['id_module_action']).'\'';
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
        if ($this->_v['id_module_action']>0) {
            $query = 'UPDATE "'.TABLE_MODULE_ACTION.'" SET ';
        } else {
            $query = 'INSERT INTO "'.TABLE_MODULE_ACTION.'" SET "id_module_action"=\''.$db->str2base($this->_v['id_module_action']).'\',';
        }

        reset($this->_v);
        $data = array();
        while(list($key, $val)=each($this->_v)) {
            if ($key=='id_module_action') continue;
            if (is_null($val)) {
                $data[] = '"'.$key.'"=NULL';
            } else {
                $data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            }
        }
        $query .= join(', ', $data);

        if ($this->_v['id_module_action']>0) {
            $query .= ' WHERE id_module_action='.$this->_v['id_module_action'];
        }
        $db->query($query);

        if ($this->_v['id_module_action']==0) {
            $this->_v['id_module_action'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id_module_action']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_MODULE_ACTION.'" WHERE "id_module_action"=\''.$db->str2base($this->_v['id_module_action']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();

        if ($this->_v['public_flag']!='yes' && $this->_v['public_flag']!='no') $errors[] = '[SYSTEM] public_flag должно быть yes/no';
        if (!is_numeric($this->_v['id_module_action'])) $errors[] = '[SYSTEM] id_module_action должно быть числом';
        if (!is_numeric($this->_v['module_id'])) $errors[] = '[SYSTEM] module_id должно быть числом';
        if (!trim($this->_v['action'])) {
            $errors[] = 'Название действия обязательно к заполнению';
        } else {
            if (!preg_match('/[a-z0-9_]+/i', $this->_v['action'])) $errors[] = 'Название действия должно состоять из латинских букв, цифр и знака подчеркивания';
        }
        if (!trim($this->_v['name'])) $errors[] = 'Надпись названия действия обязательно к заполнению';

        return $errors;
    }

    // TOOL FUNCTIONS
}

}