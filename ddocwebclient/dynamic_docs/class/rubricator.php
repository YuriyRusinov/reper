<?
//
if (!defined('RUBRICATOR')) {
	define('RUBRICATR', true);


	class rubricator extends base
	{
		protected $_v = array(

		'id'                  			=> 0,
		'id_parent'                     => '',
		'id_io_object'                 	=> '',
		'id_search_template'			=> '',
		'id_io_category'			    => '',
		'name'               		    => '',
		'code'                			=> '',
		'description'   				=> ''

		);

		protected $_enable_get = array(
		'id'                  			,
		'id_parent'                     ,
		'id_io_object'                 	,
		'id_search_template'			,
		'id_io_category'			    ,
		'name'               		    ,
		'code'                			,
		'description'

		);

		protected $_enable_set = array(
		'id'                  			,
		'id_parent'                     ,
		'id_io_object'                 	,
		'id_search_template'			,
		'id_io_category'	     		,
		'name'               		    ,
		'code'                			,
		'description'

		);

		public function __construct()
		{
			$this->Init();
		}

		public function Init() {
			$this->_v = array(
			'id'                  			=> 0,
			'id_parent'                     => '',
			'id_io_object'                 	=> '',
			'id_search_template'			=> '',
			'id_io_category'			    => '',
			'name'               		    => '',
			'code'                			=> '',
			'description'   				=> ''

			);
		}

		// MAIN DATA MANIPULATION FUNCTIONS
		public function Load() {
			if ($this->_v['id']>0) {
				$db = db_class::get_instance();
				$query = 'SELECT * FROM "'.TABLE_RUBRICATOR.'" WHERE "id"=\''.$db->str2base($this->_v['id']).'\'';
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
				$query = 'UPDATE "'.TABLE_RUBRICATOR.'" SET ';
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
					$query .= ' WHERE id='.$this->_v['id'];
				}



			} else {


				$query = 'select getnextseq(\''.TABLE_RUBRICATOR.'\',\'id\')';
					
				$db->query($query);

				$id = '';
				if (count($db->value)>0) {
					$id = $db->value[0]['getnextseq'];
				}

				
				$query = 'INSERT INTO "'.TABLE_RUBRICATOR.'"';
				reset($this->_v);
				$data_val = array();
				
				if ($this->_v['id']==0) {
				$this->_v['id'] = $id;
			}
				
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


			}
			//echo $query;
			$db->query($query);			

			return true;
		}

		public function Delete()
		{
			if ($this->_v['id']>0) {
				$db = db_class::get_instance();
				$query = 'SELECT  deleterubric('.$db->str2base($this->_v['id']).')';
				//echo $query;
				$db->query($query);

				return true;
			}

			return false;
		}

		public function IsValidData()
		{
			$errors = array();

			if (!is_numeric($this->_v['id'])) $errors[] = '[SYSTEM] id должно быть числом';

			//if (trim($this->_v['id_io_object'])) $errors[] = '[SYSTEM] id_io_object должно быть числом';

			if($this->_v['id_parent'] != 0)
			{
				if (!is_numeric($this->_v['id_parent'])) $errors[] = '[SYSTEM] id_parent должно быть числом';
			}
			if (!trim($this->_v['name'])) $errors[] = '[SYSTEM] name должно быть заполнено ';
			//if (!trim($this->_v['code'])) $errors[] = '[SYSTEM] code должно быть заполнено';

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

		public function ChildExists() {
			$db = db_class::get_instance();

			if (is_null($this->_v['id'])) {
				$query = 'SELECT COUNT(*) AS num FROM '.TABLE_RUBRICATOR.' WHERE id_parent IS NULL';
			} else {
				$query = 'SELECT COUNT(*) AS num FROM '.TABLE_RUBRICATOR.' WHERE id_parent='.intval($this->_v['id']);
			}
			$db->query($query);

			if ($db->value[0]['num']>0) {
				return true;
			}

			return false;
		}
		
		public function ObjectExists() {
			$db = db_class::get_instance();

			if (is_null($this->_v['id'])) {
				$query = 'SELECT COUNT(*) AS num FROM '.TABLE_IO_RUBRICATOR.' WHERE id_rubric IS NULL';
			} else {
				$query = 'SELECT COUNT(*) AS num FROM '.TABLE_IO_RUBRICATOR.' WHERE id_rubric='.intval($this->_v['id']);
			}
			$db->query($query);

			if ($db->value[0]['num']>0) {
				return true;
			}

			return false;
		}
		
	}

}