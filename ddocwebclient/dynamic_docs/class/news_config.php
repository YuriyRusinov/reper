<?

if (!defined('NEWS_CONFIG')) {
    define('NEWS_CONFIG', true);

class news_config extends base
{

    protected $_v = array(
                            'id'                  	    => 1,
                            'am_news'          		 	=> 10,
                            'PreLengthC'                => 100,
                            'tab_width'      			=> '100%',
                            'p_time'                    => 1,
                            'p_date'                	=> 1,
                            'img_height'                => 20
                         );

    protected $_enable_get = array(
                                 	'id',
                            		'am_news',
                           			'PreLengthC',
                            		'tab_width',
                            		'p_time',
                            		'p_date',
                            		'img_height'	
                                  );

    protected $_enable_set = array(
                                    'id',
                            		'am_news',
                           			'PreLengthC',
                            		'tab_width',
                            		'p_time',
                            		'p_date',
                            		'img_height'
                                  );

	public function __construct()
	{
	    $this->Init();
	}

	public function Init() {
        $this->_v = array(
                            'id'                  	    => 1,
                            'am_news'          		 	=> 10,
                            'PreLengthC'                => 100,
                            'tab_width'      			=> '100%',
                            'p_time'                    => 1,
                            'p_date'                	=> 1,
                            'img_height'                => 20
                         );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load() {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            
            
            $query = 'SELECT * FROM "'.TABLE_NEWS_CONFIG.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\';';
            
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
        if ($this->_v['id']>0) {
            $query = 'UPDATE "'.TABLE_NEWS_CONFIG.'" SET ';
        } else {
            $query = 'INSERT INTO "'.TABLE_NEWS_CONFIG.'" SET "id"=\''.$db->str2base($this->_v['id']).'\',';
        }

        reset($this->_v);
        $data = array();
        while(list($key, $val)=each($this->_v)) {
            if ($key=='id') continue;
            if (is_null($val)) {
                $data[] = '"'.$key.'"=NULL';
            } else {
                $data[] = '"'.$key.'"=\''.$db->str2base($val).'\'';
            }
        }
        $query .= join(', ', $data);

        if ($this->_v['id']>0) {
            $query .= ' WHERE "id"=\''.$this->_v['id'].'\'';
        }
        
        $db->query($query);

        if ($this->_v['id']==0) {
            $this->_v['id'] = $db->last_insert_id();
        }

        return true;
    }

    public function Delete()
    {
        if ($this->_v['id']>0) {
            $db = db_class::get_instance();
            $query = 'DELETE FROM "'.TABLE_NEWS_CONFIG.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
            $db->query($query);

            return true;
        }

        return false;
    }

    public function IsValidData()
    {
        $errors = array();  
        
        
        if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';
        
        if (!trim($this->_v['tab_width'])) {
            $errors[] = 'Поле ширина таблицы обязательно к заполнению';
        }
        
        if (!trim($this->_v['p_time'])) {
            $errors[] = 'Флаг показа времени обязателен к заполнению';
        } else {
        	if(!is_numeric($this->_v['p_time']))
        	{
        		 $errors[] = 'Неверный формат флага показа времени';
        	}
        	
        }
        
        if (!trim($this->_v['img_height'])) {
            $errors[] = 'Поле ширина картинки обязателено к заполнению';
        } else {
        	if(!is_numeric($this->_v['img_height']))
        	{
        		 $errors[] = 'Неверный формат ширины картинки';
        	}
        	
        }
        
        if (!trim($this->_v['p_date'])) {
            $errors[] = 'Поле флаг даты объявления обязателено к заполнению';
        } else {
        	if(!is_numeric($this->_v['p_date']))
        	{
        		 $errors[] = 'Неверный формат флаг даты объявления';
        	}
        	
        }
        
        if (!trim($this->_v['am_news'])) {
            $errors[] = 'Поле количества обьявлений обязательно к заполнению';
        } else {
        	if(!is_numeric($this->_v['am_news']))
        	{
        		 $errors[] = 'Неверный формат поля количества обьявлений';
        	}
        	
        }
        
        if (!trim($this->_v['PreLengthC'])) {
            $errors[] = 'Поле ширина предпросмотра объявления обязателено к заполнению';
        } else {
        	if(!is_numeric($this->_v['PreLengthC']))
        	{
        		 $errors[] = 'Неверный формат ширины предпросмотра';
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
