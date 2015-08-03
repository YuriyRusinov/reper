<?

if (!defined('EMPLOYEE')) {
    define('EMPLOYEE', true);

class employee extends base
{
    protected $_v = array(
                            
                            'login'         => '',
                            'password'      => '',
                            'post'          => '',
                            'dostup'         => '',
                            'post_name' => '',
                            'dep' => '',
                            'dep_name' => ''
                            
                         );

    protected $_enable_get = array(
                                   
                                    'login',
                                    'password',
                                    'post',
                                    'dostup',
                                    'post_name',
                                    'dep',
                                    'dep_name'
                                   
                                  );

    protected $_enable_set = array(
                                   
                                    'login',
                                    'password',
                                    'post',
                                    'dostup',
                                    'post_name',
                                    'dep',
                                    'dep_name'
                                   
                                  );

	public function __construct()
	{
	    $this->Init();
	}
	
	
	private static $instance;
	  
	// Получение состояния синглтона
    public static function get_instance()
    {
        if (!isset(self::$instance)) {
            $c = __CLASS__;
            self::$instance = new $c;
        }

        return self::$instance;
    }

    // Запрет клонирования
    public function __clone()
    {
        die('Clone is not allowed.');
    }  

	public function Init() {
        $this->_v = array(
                          
                            'login'         => '',
                            'password'      => '',
                            'post'          => '',
                            'dostup' 		 => '',
                            'post_name' => '',
                            'dep' => '',
                            'dep_name' => ''
                          
                         );
	}

    // MAIN DATA MANIPULATION FUNCTIONS
    public function Load($USER, $PASSWD , $LEVEL, $PST, $PST_NAME, $DEP, $DEP_NAME) {
       
    	if($USER != '' && $PASSWD != '' && $LEVEL != '' && $PST != '')
    	{
    		reset($this->_v);
    		$this->_v['login'] =    $USER;
    		$this->_v['password'] = $PASSWD;
    		$this->_v['post'] =     $PST;
    		$this->_v['dostup'] =    $LEVEL;
    		$this->_v['post_name'] =     $PST_NAME;
    		$this->_v['dep'] =     $DEP;
    		$this->_v['dep_name'] =     $DEP_NAME;
    		 
    		return true;
    	}
    	
    	return false;
    }

    // TOOL FUNCTIONS
  

    public function GetRoles()
    {        
            $res = array(id_role => '1',
                         name=> 'Администратор');
            
            return $res;  
    }

    public function GetEmployeeRolesId()
    {
        $result = array();
        
        $result[] = '1';
        
        return $result;
    }
}

}