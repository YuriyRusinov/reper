<?

if (!defined('VARIABLE')) {
    define('VARIABLE', true);

class variable extends base
{
    protected $_types = array(
                                'input' => 'Текстовое поле'
                             );

    protected $_modules = array(
                                    'contact-person'    => 'Контактное лицо',
                                    'company'           => 'Контрагент'
                               );

    protected $_v = array(
                            'id_variable'   => 0,
                            'module'        => 'contact-person',
                            'name'          => '',
                            'show_name'     => '',
                            'required'      => 'no',
                            'type'          => 'input',
                            'format'        => '.*',
                            'error_message' => '',
                            'order'         => 0,
                            'list_show'     => 'no'
                         );

    protected $_enable_get = array(
                                    'id_variable',
                                    'module',
                                    'name',
                                    'show_name',
                                    'required',
                                    'type',
                                    'format',
                                    'error_message',
                                    'order',
                                    'list_show'
                                  );

    protected $_enable_set = array(
                                    'id_variable',
                                    'module',
                                    'name',
                                    'show_name',
                                    'required',
                                    'type',
                                    'format',
                                    'error_message',
                                    'order',
                                    'list_show'
                                  );

    private $_wrong_variable_names = array(
                                            'id_company',
                                            'id_contact_person',
                                            'name'
                                          );

	public function __construct()
	{
	    asort($this->_types);
	    asort($this->_modules);
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'id_variable'   => 0,
                        'module'        => 'user',
                        'name'          => '',
                        'show_name'     => '',
                        'required'      => 'no',
                        'type'          => 'input',
                        'format'        => '.*',
                        'error_message' => '',
                        'order'         => 0,
                        'list_show'     => 'no'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id_variable']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_VARIABLE.'" WHERE "id_variable"=\''.$db->str2base($this->_v['id_variable']).'\'';
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
        if ($this->_v['id_variable']>0) {
            $query = 'UPDATE "'.TABLE_VARIABLE.'" SET ';
        } else {
            $query = 'INSERT INTO "'.TABLE_VARIABLE.'" SET "id_variable"=\''.$db->str2base($this->_v['id_variable']).'\',';
        }

        reset($this->_v);
        $data = array();
        while(list($key, $val)=each($this->_v)) {
            if ($key=='id_variable') continue;
            if (is_null($val)) {
                $data[] = '"'.$key.'"=NULL';
            } else {
                $data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            }
        }
        $query .= join(', ', $data);

        if ($this->_v['id_variable']>0) {
            $query .= ' WHERE id_variable='.$this->_v['id_variable'];
        }
        $db->query($query);

        if ($this->_v['id_variable']==0) {
            $this->_v['id_variable'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id_variable']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_VARIABLE.'" WHERE "id_variable"=\''.$db->str2base($this->_v['id_variable']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }


    public static function CheckRegexpHandler($errno, $errstr)
    {
        $GLOBALS['REGEXP_CHECK_ERROR'] = $errstr;
    }

    public function IsValidData()
    {
        $errors = array();

        if (array_key_exists($this->_v['module'], $this->_modules)===false) $errors[] = 'Установлен не существующий раздел';

        if (!trim($this->_v['name'])) {
            $errors[] = 'Название поля обязательно к заполнению';
        } else {
            if (!preg_match('/[a-z0-9_]+/i', $this->_v['name'])) {
                $errors[] = 'Название поля должно состоять из латинских букв, цифр и знака подчеркивания';
            } else {
                if (array_search($this->_v['name'], $this->_wrong_variable_names)!==false) {
                    $errors[] = 'Имя дополнительного поля не может носить данное служебное имя';
                } else {
                    // Check variable of this module already exist
                    $db = db_class::get_instance();
                    $query = 'SELECT "id_variable" FROM '.TABLE_VARIABLE.' WHERE ';
                    if ($this->_v['id_variable']>0) $query.=' id_variable!='.$this->_v['id_variable'].' AND ';
                    $query.=' "module"=\''.$db->str2base($this->_v['module']).'\' AND ';
                    $query.=' "name"=\''.$db->str2base($this->_v['name']).'\'';

                    $db->query($query);
                    if (count($db->value)>0) $errors[] = 'Дополнительное поле с указанным именем уже существует';
                }
            }
        }

        if (trim($this->_v['show_name']=='')) $errors[] = 'Надпись не может быть пустой';
        if (!preg_match('/[\-+]?\d+/', $this->_v['order'])) $errors[] = 'Номер по порядку должен состоять из цифр и знака минуса';

        if (array_key_exists($this->_v['type'], $this->_types)===false) $errors[] = 'Установлен не существующий тип';

        if (trim($this->_v['format']=='')) {
            $errors[] = 'Формат не может быть пустым';
        } else {
            $GLOBALS['REGEXP_CHECK_ERROR'] = '';
            $orig_hndl = set_error_handler(array('variable', 'CheckRegexpHandler'), E_ALL);
            $code = 'preg_match("/'.$this->_v['format'].'/i", "test string");';
            eval($code);
            restore_error_handler();
            if ($GLOBALS['REGEXP_CHECK_ERROR']) $errors[] = 'Регулярное выражение для формата не валидно: '.$GLOBALS['REGEXP_CHECK_ERROR'];
        }

        return $errors;
    }

    public function CheckValidData($value) {
        $errors = array();

        if ($this->_v['required']=='yes') {
            if (is_null($value) || (!is_null($value) && $value=='')) $errors[] = 'Поле \''.$this->_v['show_name'].'\' обязательно для заполнения' ;
        }

        if (!$errors) {
            if (!preg_match('/'.$this->_v['format'].'/is', $value)) {
               if ($this->_v['error_message']!='') {
                   $errors[] = $this->_v['error_message'];
               } else {
                   $errors[] = 'Поле \''.$this->_v['show_name'].'\' неверно заполнено и несоответствует формату: '.$this->_v['format'];
               }
            }
        }

        return $errors;
    }

    // TOOL FUNCTIONS
    public function GetNextOrder() {
        $db = db_class::get_instance();
        $query = 'SELECT MAX(order) AS next FROM "'.TABLE_VARIABLE.'" WHERE "module"=\''.$db->str2base($this->_v['module']).'\'';
        $db->query($query);

        return intval($db->value[0]['next']) + 1;
    }

    public function GetModules()
    {
        return $this->_modules;
    }

    public function GetModuleName($module=null)
    {
        if (is_null($module)) {
            $module = $this->_v['module'];
        }

        if (array_key_exists($this->_v['module'], $this->_modules)) {
            return $this->_modules[$this->_v['module']];
        }

        return false;
    }

    public function GetTypes()
    {
        return $this->_types;
    }

    public function GetTypeName($type=null)
    {
        if (is_null($type)) {
            $type = $this->_v['type'];
        }

        if (array_key_exists($type, $this->_types)) {
            return $this->_types[$type];
        }

        return false;
    }
}

}