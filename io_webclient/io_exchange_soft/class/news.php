<?
//объявления
if (!defined('NEWS')) {
    define('NEWS', true);

class news extends base
{

    protected $_v = array(
                            'news_id'                   => 0,
                            'news_stamp'                => 0,
                            'news_time'                 => '00:00',
                            'news_date'               	=> '0000-00-00',
                            'news_title'                => '',
                            'news_content'              => '',
                            'news_img'   				=> 'n',
                            'news_exp'                  => '.jpg'
                         );

    protected $_enable_get = array(
                                    'news_id',
                            		'news_stamp',
                            		'news_time',
                            		'news_date',
                            		'news_title',
                            		'news_content',
                            		'news_img',
                            		'news_exp'	
                                  );

    protected $_enable_set = array(
                                    'news_id',
                            		'news_stamp',
                            		'news_time',
                            		'news_date',
                            		'news_title',
                            		'news_content',
                            		'news_img',
                            		'news_exp'	
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
        					'news_id'                   => 0,
                            'news_stamp'                => 0,
                            'news_time'                 => '00:00',
                            'news_date'               	=> '0000-00-00',
                            'news_title'                => '',
                            'news_content'              => '',
                            'news_img'   				=> 'n',
                            'news_exp'                  => '.jpg'
                   );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['news_id']>0) {
            $db = db_class::get_instance();
            $query = 'SELECT * FROM "'.TABLE_NEWS.'" WHERE "news_id"=\''.$db->str2base($this->_v['news_id']).'\'';
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
        if ($this->_v['news_id']>0) {
            $query = 'UPDATE "'.TABLE_NEWS.'" SET ';
            reset($this->_v);
        $data = array();
        while(list($key, $val)=each($this->_v)) {
            if ($key=='news_id') continue;
            if (is_null($val)) {
                $data[] = '"'.$key.'"=NULL';
            } else {
                $data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            }
        }
        $query .= join(', ', $data);

        if ($this->_v['news_id']>0) {
            $query .= ' WHERE news_id='.$this->_v['news_id'];
        }
            
            
            
        } else {
        	
            $query = 'INSERT INTO "'.TABLE_NEWS.'"';
            reset($this->_v);
            $data_val = array(); 
            
        while(list($key, $val)=each($this->_v)) {
        	if($key == 'news_id') continue;
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

        if ($this->_v['news_id']==0) {
            $this->_v['news_id'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['news_id']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_NEWS.'" WHERE "news_id"=\''.$db->str2base($this->_v['news_id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        if (!is_numeric($this->_v['news_id'])) $errors[] = '[SYSTEM] news_id должно быть числом';
        
              
         if (!trim($this->_v['news_title'])) {
            $errors[] = 'Поле заголовок объявления обязательно к заполнению';
        }
          
        
        if (!trim($this->_v['news_content'])){ 
        	$errors[] = 'Нет текста объявления';
        }
             
         if (!trim($this->_v['news_date'])) {
            $errors[] = 'Поле дата объявления обязателено к заполнению';
        } else {
        	if(!preg_match('/\d{2,4}-\d{1,2}-\d{1,2}/', $this->_v['news_date']))
        	{
        		 $errors[] = 'Неверный формат даты объявления';
        	}
        	
        }

        return $errors;
    }

 

public function CheckActionEnable(&$STORAGE, $action) {
    	
    	
        if (check_current_employee_admin($STORAGE)) return true;
        if (!access_enable($STORAGE, 'news', $action)) return false;
        
        $db = db_class::get_instance();

        switch ($action) {
            case 'view':
            case 'edit':   
            case 'delete':
            	     return true; 

            default: return false;
        }

        return false;
    }

}

}
