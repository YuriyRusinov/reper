<?

if (!defined('ROLE')) {
    define('ROLE', true);

class role extends base
{
    protected $_v = array(
                            'id_role'       => 0,
                            'name'          => '',
                            'super_user'    => 'no'
                         );

    protected $_enable_get = array(
                                    'id_role',
                                    'name',
                                    'super_user'
                                  );

    protected $_enable_set = array(
                                    'id_role',
                                    'name'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $_v = array(
                        'id_role'       => 0,
                        'name'          => '',
                        'super_user'    => 'no'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id_role']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_ROLE.'" WHERE "id_role"=\''.$db->str2base($this->_v['id_role']).'\'';
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
        if ($this->_v['id_role']>0) {
            $query = 'UPDATE "'.TABLE_ROLE.'" SET ';
            
              reset($this->_v);
        	$data = array();
        	while(list($key, $val)=each($this->_v)) {
            	if ($key=='id_role') continue;
            	if (is_null($val)) {
                	$data[] = '"'.$key.'"=NULL';
            	} else {
                	$data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            	}
        	}
        	$query .= join(', ', $data);

        	if ($this->_v['id_role']>0) {
            	$query .= ' WHERE id_role='.$this->_v['id_role'];
        	}
            
        } else {
            $query = 'INSERT INTO "'.TABLE_ROLE.'"';
            
             reset($this->_v);
            $data_val = array(); 
            
        while(list($key, $val)=each($this->_v)) {
        	if($key == 'id_role') continue;
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

      if ($this->_v['id_role']==0) {
            $this->_v['id_role'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id_role']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_ROLE.'" WHERE "id_role"=\''.$db->str2base($this->_v['id_role']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();

        if (!is_numeric($this->_v['id_role'])) $errors[] = '[SYSTEM] id_role должно быть числом';

        if (!trim($this->_v['name'])) {
            $errors[] = 'Название роли обязательно к заполнению';
        } else {
            $db = db_class::get_instance();

            $query = 'SELECT "id_role" FROM "'.TABLE_ROLE.'" WHERE ';
            if ($this->_v['id_role']>0) $query .= ' id_role!='.$this->_v['id_role'].' AND ';
            $query.=' "name"=\''.$this->_v['name'].'\'';
            $db->query($query);
            if (count($db->value)>0) $errors[] = 'Роль с таким названием уже существует';
        }

        return $errors;
    }

    // TOOL FUNCTIONS
    public function GetRoleActionsId() {
        $result = array();

        if($this->_v['id_role']>0) {
            $db = db_class::get_instance();

            $query = 'SELECT "module_action_id" FROM "'.TABLE_ROLE_ACTION.'" WHERE "role_id"='.$this->_v['id_role'];
            $db->query($query);

            for ($i=0;$i<count($db->value);$i++) {
                $result[] = $db->value[$i]['module_action_id'];
            }
        }

        return $result;
    }


    public function GetRoleVariablesId() {
        $result = array();

        if($this->_v['id_role']>0) {
            $db = db_class::get_instance();

            $query = 'SELECT "variable_id" FROM "'.TABLE_ROLE_VARIABLE.'" WHERE "role_id"='.$this->_v['id_role'];
            $db->query($query);

            for ($i=0;$i<count($db->value);$i++) {
                $result[] = $db->value[$i]['variable_id'];
            }
        }

        return $result;
    }


    public function DeleteRoleActions() {
        if ($this->_v['id_role']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_ROLE_ACTION.'" WHERE "role_id"='.$this->_v['id_role'];
            $db->query($query);

            return true;
        }

        return false;
    }

    public function DeleteRoleVariables() {
        if ($this->_v['id_role']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_ROLE_VARIABLE.'" WHERE "role_id"='.$this->_v['id_role'];
            $db->query($query);

            return true;
        }

        return false;
    }
}

}