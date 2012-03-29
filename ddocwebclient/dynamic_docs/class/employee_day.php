<?

if (!defined('EMPLOYEE_DAY')) {
    define('EMPLOYEE_DAY', true);

class employee_day extends base
{

    private $_statuses = array(
                                //'' => 'не указан',
                                'work' => 'на работе',
                                'compensatory_holiday' => 'отгул',
                                'ill' => 'болезнь',
                                'business_trip' => 'командировка',
                                'holiday' => 'отпуск',
                                'shift' => 'прогул'
                              );

    protected $_v = array(
                            'date' => '0000-00-00',
                            'employee_id' => '',
                            'status' => '',
                            'start_work' => '00:00:00',
                            'stop_work' => '00:00:00',
                            'last_active' => '00:00:00',
                            'description' => ''
                         );

    protected $_enable_get = array(
                                    'date',
                                    'employee_id',
                                    'status',
                                    'start_work',
                                    'stop_work',
                                    'last_active',
                                    'description'
                                  );

    protected $_enable_set = array(
                                    'date',
                                    'employee_id',
                                    'status',
                                    'start_work',
                                    'stop_work',
                                    'last_active',
                                    'description'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
                        'date' => '0000-00-00',
                        'employee_id' => '',
                        'status' => '',
                        'start_work' => '00:00:00',
                        'stop_work' => '00:00:00',
                        'last_active' => '00:00:00',
                        'description' => ''
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['employee_id']!='' && $this->_v['date']!='' && $this->_v['date']!='0000-00-00') {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_EMPLOYEE_DAY.'" WHERE "date"=\''.$db->str2base($this->_v['date']).'\' AND "employee_id"=\''.$this->_v['employee_id'].'\'';
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
        
        $this->Delete();
        
        $query = 'INSERT INTO "'.TABLE_EMPLOYEE_DAY.'"';

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
        if ($this->_v['date']!='' && $this->_v['date']!='0000-00-00' && $this->_v['employee_id']!='') {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_EMPLOYEE_DAY.'" WHERE "date"=\''.$db->str2base($this->_v['date']).'\' AND "employee_id"=\''.$db->str2base($this->_v['employee_id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }
    
    public function IsValidData()
    {
        $errors = array();

        if (trim($this->_v['date'])!='' && !preg_match('/\d{2,4}-\d{1,2}-\d{1,2}/', $this->_v['date'])) $errors[] = '[SYSTEM] date должно быть датой';
        if (trim($this->_v['employee_id'])) $errors[] = '[SYSTEM] employee_id должно быть заполнено';
        if (array_key_exists($this->_v['status'], $this->_statuses)===false) $errors[] = '[SYSTEM] status должно быть одно из: '.join(',', array_keys($this->_statuses));
        if (trim($this->_v['start_work'])!='' && !preg_match('/\d{1,2}\:\d{1,2}:\d{1,2}/', $this->_v['start_work'])) $errors[] = '[SYSTEM] start_work должно быть временем';
        if (trim($this->_v['stop_work'])!='' && !preg_match('/\d{1,2}\:\d{1,2}:\d{1,2}/', $this->_v['stop_work'])) $errors[] = '[SYSTEM] stop_work должно быть временем';

        if ($this->_v['status']!='' && $this->_v['status']!='work' && empty($this->_v['description'])) $errors[] = 'При статусе дня: '.@$this->_statuses[$this->_v['status']].' поле комментария обязательно к заполнению';

        return $errors;
    }

    // TOOL FUNCTIONS
    public function EmployeeDayExist()
    {
        if ($this->_v['date']!='' && $this->_v['date']!='0000-00-00' && $this->_v['employee_id']!='') {
            $db = db_class::get_instance();
            $query = 'SELECT COUNT(*) AS num FROM "'.TABLE_EMPLOYEE_DAY.'" WHERE "date"=\''.$db->str2base($this->_v['date']).'\' AND "employee_id"=\''.$db->str2base($this->_v['employee_id']).'\'';
            $db->query($query);

            if ($db->value[0]['num']>0) return true;
        }

        return false;
    }

    public function UpdateLastActive(){
        if($this->EmployeeDayExist()){
            $db = db_class::get_instance();
            $query = 'UPDATE "'.TABLE_EMPLOYEE_DAY.'" '.
                     'SET "last_active"=\''.$db->str2base($this->_v['last_active']).'\''.
                     'WHERE "employee_id"=\''.$this->_v['employee_id'].'\' '.
                       'AND "date"=\''.$db->str2base($this->_v['date']).'\'' ;        
            $db->query($query);
        }
    }

    public function GetStatuses()
    {
        return $this->_statuses;
    }

    public function GetStatusName($status=null)
    {
        if (is_null($status)) {
            $status = $this->_v['status'];
        }

        if (array_key_exists($status, $this->_statuses)) {
            return $this->_statuses[$status];
        }

        return false;
    }

}

}