<?
//поисковые запросы 
if (!defined('SEARCH_TEMPLATES')) {
	define('SEARCH_TEMPLATES', true);


	class search_templates extends base
	{
		protected $_v = array(

		'id'         			  => 0,
		'id_group'                => 0,
		'author'                  => 0,
		'name'                    => '',
		'description'             => ''


		);

		protected $_enable_get = array(
		'id'         			,
		'id_group'              ,
		'author'              	,
		'name' 					,
		'description'
		);

		protected $_enable_set = array(
		'id'         			,
		'id_group'              ,
		'author'              	,
		'name'					,
		'description'
		);

		public function __construct()
		{
			$this->Init();
		}

		public function Init() {
			$this->_v = array(
			'id'                 			=> 0,
			'id_group'               	    => 0,
			'author'                        => 0,
			'name'               		    => '',
			'description'                   => ''

			);
		}

		// MAIN DATA MANIPULATION FUNCTIONS
		public function Load() {
			if ($this->_v['id']>0) {
				$db = db_class::get_instance();
				$query = 'SELECT * FROM "'.TABLE_SEARCH_TEMPLATES.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\' ';


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


			if ($this->_v['id']>0 && $this->_v['id_group']>0 ) {
			//	$query .= ' SELECT ioupdateattr('.$this->_v['id_io_object'].','.$this->_v['id_io_attribute'].',\''.$this->_v['value'].'\');';

				
			//	$db->query($query);

			}
			return true;
		}

		public function Delete()
		{
			if ($this->_v['id']>0 && $this->_v['id_group']>0) {
				$db = db_class::get_instance();
				$query = 'DELETE FROM "'.TABLE_SEARCH_TEMPLATES.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\' ANd "id_group"=\''.$db->str2base($this->_v['id_group']).'\'';
				$db->query($query);

				return true;
			}

			return false;
		}

		public function IsValidData()
		{
			$errors = array();

			if (!is_numeric($this->_v['id_group'])) $errors[] = '[SYSTEM] id_group должно быть числом';

			if (!is_numeric($this->_v['author'])) $errors[] = '[SYSTEM] author должно быть числом';
			//if (!trim($this->_v['id_io_attribute'])) $errors[] = '[SYSTEM] id_io_attribute должно быть числом';

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
