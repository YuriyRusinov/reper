<?

if (!defined('FILE')) {
    define('FILE', true);

class file extends base
{
    protected $_v = array(
                            'id_file'       => 0,
                            'name'          => '',
                            'path'          => '',
                            'crc'           => 0
                         );

    protected $_enable_get = array(
                                    'id_file',
                                    'name',
                                    'path',
                                    'crc'
                                  );

    protected $_enable_set = array(
                                    'id_file',
                                    'name',
                                    'path'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'id_file'       => 0,
                        'name'          => '',
                        'path'          => '',
                        'crc'           => 0
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id_file']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_FILE.'" WHERE "id_file"=\''.$db->str2base($this->_v['id_file']).'\'';
            $db->query($query);

            if (count($db->value)>0) {
                $this->_v = $db->value[0];
                return true;
            }
        }

        return false;
    }

    public function LoadByCrc($crc) {
        if ($crc!=0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_FILE.'" WHERE "crc"=\''.$db->str2base($crc).'\'';
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
        if ($this->_v['id_file']>0) {
            $query = 'UPDATE "'.TABLE_FILE.'" SET ';
        } else {
            $query = 'INSERT INTO "'.TABLE_FILE.'" SET "id_file"=\''.$db->str2base($this->_v['id_file']).'\',';
        }

        $this->_v['path'] .= '/';
        $this->_v['path'] = preg_replace('/[\/]{2,}/', '/', $this->_v['path']);

        reset($this->_v);
        $data = array();
        while(list($key, $val)=each($this->_v)) {
            if ($key=='id_file') continue;

            if (is_null($val)) {
                $data[] = '"'.$key.'"=NULL';
            } else {
                $data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            }
        }
        $query .= join(', ', $data);

        if ($this->_v['id_file']>0) {
            $query .= ' WHERE id_file='.$this->_v['id_file'];
        }
        $db->query($query);

        if ($this->_v['id_file']==0) {
            $this->_v['id_file'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id_file']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_FILE.'" WHERE "id_file"=\''.$db->str2base($this->_v['id_file']).'\'';
            $db->query($query);

            $query = 'DELETE FROM "'.TABLE_FILE_ROLE.'" WHERE "file_id"=\''.$db->str2base($this->_v['id_file']).'\'';
            $db->query($query);
            

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();

        if (!is_numeric($this->_v['id_file'])) $errors[] = '[SYSTEM] id_file должно быть числом';
        if (!is_numeric($this->_v['crc'])) $errors[] = '[SYSTEM] crc должно быть числом';

        if (trim($this->_v['name'])=='') $errors[] = 'Название файла не должно быть пустым';
        if (preg_match('/[\/\\\]+/', $this->_v['name'])) {
            $errors[] = 'Название не может содержать слеши';
        }

        return $errors;
    }

    // TOOL FUNCTIONS
    public function GetFileRoleIds()
    {
        $return = array();

        if ($this->_v['id_file']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT "role_id" FROM '.TABLE_FILE_ROLE.' WHERE "file_id"='.$this->_v['id_file'];
            $db->query($query);

            for ($i=0;$i<count($db->value);$i++) {
                $return[] = $db->value[$i]['role_id'];
            }
        }

        return $return;
    }


    public function DeleteFileRoles()
    {
        if ($this->_v['id_file']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_FILE_ROLE.'" WHERE "file_id"='.$this->_v['id_file'];
            $db->query($query);

            return true;
        }

        return false;
    }

    function MoveSource($source, $destination)
    {
        $from = dirname(preg_replace('/[\/]{2,}/', '/', $source.'/')).'/';
        $to = preg_replace('/[\/]{2,}/', '/', $destination.'/');

        $db = db_class::get_instance();

        $query = '
                    UPDATE "'.TABLE_FILE.'"
                    SET
                        path = CONCAT(\''.$db->str2base($to).'\', SUBSTRING(path, CHAR_LENGTH(\''.$db->str2base($from).'\')+1))
                    WHERE LOCATE(\''.$db->str2base($from).'\', path)=1
                 ';
        $db->query($query);
    }

}

}