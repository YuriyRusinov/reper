<?
if (!defined("LOGGER")) {
    define ("LOGGER",true);

    define('LOGGER_WARNING', LOG_WARNING);
    define('LOGGER_ERROR', LOG_ERR);
    define('LOGGER_NOTICE', LOG_NOTICE);

class logger
{
	var $_init_flag;	// Init logger flag
	var $_name;	// Logger name

	// Constructor
	function logger($name = 'logger')
	{
		$this->_init_flag = false;
		$this->_name = $name;
	}

	// Write message to syslog
	function write($msg, $priority = LOGGER_ERROR)
	{
		if (!$this->_init_flag) $this->_init();
		syslog($priority, $msg);
		echo '<br>', $msg, '<br>';
	}

	// Init syslog
	function _init()
	{
		define_syslog_variables();
		openlog($this->_name, LOG_PID | LOG_PERROR, LOG_USER);
	}

	// Close syslog
	function destroy()
	{
		if ($this->_init_flag) closelog();
	}
}

}
