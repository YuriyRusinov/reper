<?
//объявления
if (!defined('ATTRS')) {
	define('ATTRS', true);


	class attrs_values extends base
	{
		protected $_v = array(

		'id_io_object'         			=> 0,
		'id_io_category'                => 0,
		'id_io_attribute'              	=> 0,
		'value'               		    => ''



		);

		protected $_enable_get = array(
		'id_io_object'         			,
		'id_io_category'                ,
		'id_io_attribute'              	,
		'value'
		);

		protected $_enable_set = array(
		'id_io_object'         			,
		'id_io_category'                ,
		'id_io_attribute'              	,
		'value'
		);

		public function __construct()
		{
			$this->Init();
		}

		public function Init() {
			$this->_v = array(
			'id_io_object'                 => 0,
			'id_io_category'                => 0,
			'id_io_attribute'               => 0,
			'value'               		    => ''

			);
		}

		// MAIN DATA MANIPULATION FUNCTIONS
		public function Load() {
			if ($this->_v['id_io_object']>0 && $this->_v['id_io_attribute'] >0) {
				$db = db_class::get_instance();
				$query = 'SELECT * FROM "'.TABLE_ATTRS.'" WHERE "id_io_object"=\''.$db->str2base($this->_v['id_io_object']).'\' AND "id_io_attribute"=\''.$db->str2base($this->_v['id_io_attribute']).'\'';


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


			if ($this->_v['id_io_object']>0 && $this->_v['id_io_attribute']>0 ) {
				$query .= ' SELECT ioupdateattr('.$this->_v['id_io_object'].','.$this->_v['id_io_attribute'].',\''.$this->_v['value'].'\');';

				
				$db->query($query);

			}
			return true;
		}

		public function Delete()
		{
			if ($this->_v['id_io_object']>0 && $this->_v['id_io_attribute']>0) {
				$db = db_class::get_instance();
				$query = 'DELETE FROM "'.TABLE_ATTRS.'" WHERE "id_io_object"=\''.$db->str2base($this->_v['id_io_object']).'\' ANd "id_io_attribute"=\''.$db->str2base($this->_v['id_io_oattribute']).'\'';
				$db->query($query);

				return true;
			}

			return false;
		}

		public function IsValidData()
		{
			$errors = array();

			if (!is_numeric($this->_v['id_io_object'])) $errors[] = '[SYSTEM] id_io_object должно быть числом';

			if (!is_numeric($this->_v['id_io_category'])) $errors[] = '[SYSTEM] id_io_type должно быть числом';
			if (!trim($this->_v['id_io_attribute'])) $errors[] = '[SYSTEM] id_io_attribute должно быть числом';

			return $errors;
		}



		public function CheckActionEnable(&$STORAGE, $action) {


			if (check_current_employee_admin($STORAGE)) return true;


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
