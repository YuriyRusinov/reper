<?
$page_title = 'Редактирование атрибута';

$id = intval(get_request_variable('id', 0));
$attr_id = intval(get_request_variable('attr_id', 0));
$value = get_request_variable('value', '');
$save = get_request_variable('save', 0);

$_REQUEST['callback'] = get_request_variable('callback', '');

$attr = new attrs_values();
$attr->id_io_object = $id;
$attr->id_io_attribute = $attr_id;
if ($id > 0 && $attr_id > 0) {
    if (!$attr->Load()) {
        $attr->id_io_category = 0;
    }
}

$attr_desc = new attributes();
$attr_desc->id = $attr_id;

if($attr_id > 0 && $id >0 )
{
	if (!$attr_desc->Load()) {
        $attr_desc->id = 0;
    }
	
}

$type_attribute = $attr_desc->id_a_type;

$list_values = array();

if($type_attribute == 2)
{
	
	 $table_name = $attr_desc->table_name;
	 $column_name = $attr_desc->column_name;
	 
	 $query = 'SELECT * FROM '.$table_name.' ORDER BY 1;';
	 
	 $db->query($query);
     $list_values = $db->value;
	 
}



//echo "TYPE ATR: ".$type_attribute;

//types attributes
//id value description
//1 't' 'f' logical
//2 int4 element of list - use table_name and column_name - receive id
//3 int4 element of list - use table samesalfe
//4 date - ex 1999-01-08   YYYY-MM-DD
//5 timestamp - ex 1999-01-08 04:05:08
//6 float - ex 8.999
//7 dont do - infirm user!!!
//8 int4 - min int - max int

if ($save) {
	
	
	$attr_errors = false;
	
	if($type_attribute == 8)//int type
	
	{
		$findme = '.';		
		if (is_numeric($value) === false || strpos($value, $findme) !== false) 
		{ 
			$errors[] = '[SYSTEM] значение должно быть числом';
			$attr_errors = true;
		}
		
	}
		
	
	if($type_attribute == 2)//id
	
	{
		$findme = '.';		
		if (is_numeric($value) === false || strpos($value, $findme) !== false) 
		{ 
			$errors[] = '[SYSTEM] значение должно быть числом';
			$attr_errors = true;
		}
		
	}
	
	if($type_attribute == 6)//float
	
	{
		$findme = '.';		
		if (is_numeric($value) === false || strpos($value, $findme) == false) 
		{ 
			$errors[] = '[SYSTEM] значение должно быть числом c плавающей точкой 00.00';
			$attr_errors = true;
		}
		
	}
	
	
	if($type_attribute == 14)//string fix size
	
	{
		$len = $attr_desc->def_width;		
		if (strlen($value) > $len) 
		{ 
			$errors[] = '[SYSTEM] длинна строки не должна превышать '.$len.' !!!';
			$attr_errors = true;
		}
		
	}
	
	
	
	
	if($attr_errors === false)
	{
    	$attr->value = $value;
    	$errors = $attr->IsValidData();
	}
	
    if (count($errors)==0 && $attr_errors === false) {
        $db = db_class::get_instance();
        $db->begin();

        $attr->Save();
        $db->commit();
        
        
       
        
        
        
    }
} 

$names = new collection();
$names->Load(TABLE_ATTRIBUTES, false, $where, 'name ASC');
$names = $names->_collection;


include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
