<?
$page_title = 'Выбор информационного объекта';
include SITE_FILE_ROOT.'template/simple_header.php';

$sort = 'name';
$sort_dir = 'asc';
$pattern = get_request_variable('pattern', '');
$letter = get_request_variable('letter', '');


$_REQUEST['callback'] = get_request_variable('callback', '');
$_REQUEST['cat_id'] = get_request_variable('cat_id','');
$io_id = get_request_variable('io_id','');


$save = get_request_variable('save', '');

$where = array();
if ($pattern!='') {
    $where[] = '"name" LIKE \'%'.db_class::escape_like($pattern).'%\'';
    $letter = '';
    
	} 
	elseif ($letter!='') 
	{
		$where[] = '"name" LIKE \''.db_class::escape_like($letter).'%\' OR  "name" LIKE \''.db_class::escape_like(_strtoupper($letter)).'%\' ';
	}
	

	
	if($save)
	{
		if($_REQUEST['io_id'] > 0 && $_REQUEST['cat_id'] > 0)
		{
			$io_rub = new io_rubricator();	
			$io_rub->id_rubric = $_REQUEST['cat_id'];
			$io_rub->id_io_object = $io_id;
	
			$io_rub->Save();
		}
	
		
	}
	
	
if($_REQUEST['cat_id'] > 0)
{
	
	$rubricator = new rubricator();
	$rubricator->id = $_REQUEST['cat_id'];
	$rubricator->Load();
	
	$db = db_class::get_instance();
	
	$query = 'SELECT id_io_object as id FROM '.TABLE_IO_RUBRICATOR.' WHERE id_rubric='.$rubricator->id;
					
	$db->query($query);

   $id = '';
	    
		for($i=0; $i < count($db->value); $i++ )
		{ 
					$id = $db->value[$i]['id'];
					$where[] = " ID !=".$id;
		}		
	
	
	if($rubricator->id_io_category > 0)
	{		
		$where[] = 'id_io_category = '.$rubricator->id_io_category.' ';				
	}
	
}	
	
$contact_persons = new collection();
$contact_persons->Load(TABLE_OBJECTS, false, join(' AND ', $where), '"'.$sort.'" '.$sort_dir);
$contact_persons = $contact_persons->_collection;

$additional_vars = ($letter!='' ? 'letter/'.urlencode($letter).'/':'').($pattern!='' ? 'letter/'.urlencode($pattern).'/':'').'callback/'.urlencode($_REQUEST['callback']);


include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';

