<?

if (!defined('MODULE')) {
    define('MODULE', true);

class module extends base
{
    protected $_v = array(
                            'id_module'       => 0,
                            'name'          => '',
                            'super_user'    => 'no'
                         );

    protected $_enable_get = array(
                                    'id_module',
                                    'name',
                                    'super_user'
                                  );

    protected $_enable_set = array(
                                    'id_module',
                                    'name'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'id_module'       => 0,
                        'name'          => '',
                        'super_user'    => 'no'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id_module']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_MODULE.'" WHERE "id_module"=\''.$db->str2base($this->_v['id_module']).'\'';
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
        if ($this->_v['id_module']>0) {
            $query = 'UPDATE "'.TABLE_MODULE.'" SET ';
        } else {
            $query = 'INSERT INTO "'.TABLE_MODULE.'" SET "id_module"=\''.$db->str2base($this->_v['id_module']).'\',';
        }

        reset($this->_v);
        $data = array();
        while(list($key, $val)=each($this->_v)) {
            if ($key=='id_module') continue;
            if (is_null($val)) {
                $data[] = '"'.$key.'"=NULL';
            } else {
                $data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            }
        }
        $query .= join(', ', $data);

        if ($this->_v['id_module']>0) {
            $query .= ' WHERE id_module='.$this->_v['id_module'];
        }
        $db->query($query);

        if ($this->_v['id_module']==0) {
            $this->_v['id_module'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id_module']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_MODULE.'" WHERE "id_module"=\''.$db->str2base($this->_v['id_module']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();

        if (!is_numeric($this->_v['id_module'])) $errors[] = '[SYSTEM] id_module должно быть числом';

        if (!trim($this->_v['name'])) {
            $errors[] = 'Название роли обязательно к заполнению';
        } else {
            $db = db_class::get_instance();

            $query = 'SELECT "id_module" FROM "'.TABLE_MODULE.'" WHERE ';
            if ($this->_v['id_module']>0) $query .= ' id_module!='.$this->_v['id_module'].' AND ';
            $query.=' "name"=\''.$this->_v['name'].'\'';
            $db->query($query);
            if (count($db->value)>0) $errors[] = 'Роль с таким названием уже существует';
        }

        return $errors;
    }

    // TOOL FUNCTIONS
}

}