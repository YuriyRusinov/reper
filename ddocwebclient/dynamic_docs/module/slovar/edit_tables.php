<?
$page_title = 'Редактирование атрибута';

$id = intval(get_request_variable('id', 0));
$rec_id = intval(get_request_variable('rec_id', 0));
$name = get_request_variable('name', '');
$save = get_request_variable('save', 0);
$value = get_request_variable('value','');

$set_empty = false;

$_REQUEST['callback'] = get_request_variable('callback', '');

$io_object = new io_objects();
$io_object->id = $id;

if ($id > 0 ) {
    if (!$io_object->Load()) {
        return;
    }
}

//echo "tst";
     
	if($rec_id > 0)
	{
	 	$table_name = $io_object->table_name;
	 	$column_name = $name;
	 
	 	$query = 'SELECT '.$name.' AS value FROM '.$table_name.' WHERE id='.$rec_id;
	 	
	 	//echo $query;
	 	$db->query($query);
     	$val = $db->value[0]['value'];
	}
	else {
		
	 if($io_object->id > 0)
	 {
		 $query = 'SELECT * FROM ioinsertemptyrecord('.$io_object->id.',\''.$io_object->table_name.'\');';
		
		 $db->query($query);
		 //echo $query;
		 $set_empty = true;
	 }
		
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
		
        $db = db_class::get_instance();
        $db->begin();

        $query = 'UPDATE '.$table_name.' SET '.$name.' = \''.$value.'\' WHERE id='.$rec_id;
        
        $db->query($query);
        $db->commit();     
        
    
} 

$names = new collection();
$names->Load(TABLE_ATTRIBUTES, false, $where, 'name ASC');
$names = $names->_collection;


include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
