<?
if (!defined("DB_CLASS")) {
    define ("DB_CLASS",true);
    define ("CLIENT_MULTI_STATEMENTS", 65536); /* Enable/disable multi-stmt support */

//*******************************************************************
//* db_class.php - класс работы с множеством БД
//*******************************************************************
//* Класс реализует основные функции для работы с БД
//*
//*******************************************************************
class db_class
{
    var $db_host;           // Адрес хоста базы данных
    var $db_user;           // Имя пользователя базы данных
    var $db_passw;          // Пароль базы данных
    var $db_name;           // Имя базы данных
    var $is_debug_mode;     // Флаг режима отладки
    var $is_connected;      // Флаг коннекта к базе данных
    var $db_link;           // Идентификатор соединения с базой данных
    var $value;             // Массив возвращаемых значений
    var $terminal_report;   // Сообщение при ошибке работы с базой данных
    var $finalprocedure;    // Процедура вызывающаяся при некорректной работе с базой данных
    var $is_selected;       // Флаг выбора базы данных
    var $is_transaction;    // Флаг транзакций
    var $logger;            // Объект для сохранения логов
    var $last_query;        // Последний выполненный запрос

    private static $instance;

    private function __construct()
    {
    }

    // Инициализация параметров доступа к БД
    public function init($db_host="localhost",$db_user="user",$db_passw="",$db_name="beta",$is_transaction=false,$terminal_report="Error work with DB",$finalprocedure="",$is_debug_mode=false,$logger=null)
    {
        $this->db_host=$db_host;
        $this->db_user=$db_user;
        $this->db_passw=$db_passw;
        $this->db_name=$db_name;
        $this->terminal_report=$terminal_report;
        $this->finalprocedure=$finalprocedure;
        $this->is_debug_mode=$is_debug_mode;
        $this->is_transaction=$is_transaction;
        $this->logger = $logger;
    }

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

    // Функция соединения с базой данных
    function connect()
    {
    	$errors = array();
        if (!$this->is_connected) {
   
            $this->db_link = @pg_connect("host=$this->db_host port=5432 dbname=$this->db_name user=$this->db_user password=$this->db_passw");
            
            if (!$this->db_link) {
                if ($this->is_debug_mode)
                 
                
                $errors[] =  "<b><font color=\"red\"> Ошибка БД: </font></b> Неудачная попытка соединения с параметрам HOST=".$this->db_host.", USER=".$this->db_user."<br>";
                 
                 
                if ($this->finalprocedure) {
                    $func = $this->finalprocedure;
                    $func($this);
                } else {
                    if ($this->finalprocedure) {
                        $func = $this->finalprocedure;
                        $func($this);
                    } //else {
               	       // die($this->terminal_report);
               	    //}
                }
            } else {
            	
            	pg_set_client_encoding ( $this->db_link, "UNICODE" );
            	pg_query($this->db_link, 'SET datestyle = Euro');
            	
                //pg_query($this->db_link,'SET NAMES "utf8"');
                //pg_query($this->db_link,'SET collation_connection="utf8_general_ci"');
                //pg_query($this->db_link,'SET collation_server="utf8_general_ci"');
                //pg_query($this->db_link,'SET character_set_connection="utf8"');
               // pg_query($this->db_link,'SET character_set_results="utf8"');
               // pg_query($this->db_link,'SET character_set_client="utf8"');
               // pg_query($this->db_link,'SET character_set_server="utf8"');
                $this->is_connected=true;
                $this->is_selected=true;
            }
        }
        return $errors;
    }

    
    // Функция формирования строки со слешами для заноса в базу
    // Проверяет наличие включенного добавления слешей
    static function str2base($str)
    {
        if(!get_magic_quotes_runtime()) {
            return pg_escape_string($str);
        }
        return $str;
    }

    // Функция формирования строки полученной из базы с убранными слешами
    // Проверяет наличие включенного добавления слешей
    static function base2string($str)
    {
        if(!get_magic_quotes_runtime()) {
            return stripslashes($str);
        }
        return $str;
    }

    // Функция формирования строки для работы с SQL оператором LIKE
    // Заменяет % и _ на \% и \_
    static function escape_like($str)
    {
        $str = str_replace('%', '\\%', $str);
        $str = str_replace('_', '\\_', $str);

        return $str;
    }

    // Функция выполнения запроса
    function query($query)
    {
    	//echo $query."<br>";
        if ($this->is_connected) {
            if ($this->is_selected) {
                $this->last_query = $query;
                $result=@pg_query($this->db_link, $query);
                if (!$result) {
                    if ($this->is_debug_mode && $this->logger) $this->write_log(@pg_last_error($this->db_link));
                    if ($this->finalprocedure) {
                        $func = $this->finalprocedure;
                        $func($this);
                    } else {
                        $this->rollback();
                  	    die($this->terminal_report);
                    }
                } else {
                    unset($this->value);
                    $this->value=array();
                    while($row = @pg_fetch_assoc($result)) {
                        $this->value[]=$row;
                        unset($row);
                    }
                    @pg_free_result($result);
                }
            } else {
                //$this->select();
                $this->query($query);
            }
        } else {
            $this->connect();
            //$this->select();
            $this->query($query);
        }
    }

    // Функция получения идентификатора последней занесенной строки
    function last_insert_id()
    {
    	 $query = "SELECT lastval();";
    	
    	 $result=@pg_query($this->db_link, $query);
    	
    	 $id = 0;
    	 
         while($row = @pg_fetch_row($result)) {
                        $id=$row[0];
                        unset($row);
                        break;
                    }
          @pg_free_result($result);
           	     	
        return $id;
    }

    // Функция начала транзакции (BEGIN)
    function begin()
    {
        if ($this->is_transaction) {
            $this->query("BEGIN");
        }
    }

    // Функция подтверждения транзакции (COMMIT)
    function commit()
    {
        if ($this->is_transaction) $this->query("COMMIT");
    }

    // Функция отката транзакции (ROLLBACK)
    function rollback()
    {
        if ($this->is_transaction) $this->query("ROLLBACK");
    }

    // Функция закрытия соединения с базой данных
    function disconnect()
    {
        if ($this->is_connected) {
            if (pg_close($this->db_link)) {
                $this->db_link=0;
                $this->is_connected=false;
                unset($this->value);
            } else {
                if ($this->is_debug_mode) $this->write_log(@pg_last_error($this->db_link));
                if ($this->finalprocedure) {
                    $func = $this->finalprocedure;
                    $func($this);
                } else {
                    die($this->terminal_report);
                }
            }
        }
    }

    function write_log($msg)
    {
    	$log_obj = &$this->logger;
    	if ($log_obj) $log_obj->write($msg);
    }
}

}