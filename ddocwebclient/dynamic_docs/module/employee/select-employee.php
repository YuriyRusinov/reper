<?
$page_title = 'Выбор сотрудника';
include SITE_FILE_ROOT.'template/simple_header.php';

$sort = 'name';
$sort_dir = 'asc';
$pattern = get_request_variable('pattern', '');
$letter = get_request_variable('letter', '');
$dept_id = get_request_variable('dept_id','');

$_REQUEST['callback'] = get_request_variable('callback', '');

$where = array();
if ($pattern!='') {
    $where[] = '"name" LIKE \'%'.db_class::escape_like($pattern).'%\'';
    $letter = '';
    
	} 
	elseif ($letter!='') 
	{
		$where[] = '"name" LIKE \''.db_class::escape_like($letter).'%\'';
	}
	elseif ($dept_id!='')
	{
		$where[] = '"department_id" = '.$dept_id;
		$letter = '';
		
	}

$where[]='"active_flag"=\'yes\'';

$contact_persons = new collection();
$contact_persons->Load(TABLE_EMPLOYEE, false, join(' AND ', $where), '"'.$sort.'" '.$sort_dir);
$contact_persons = $contact_persons->_collection;

$additional_vars = ($letter!='' ? 'letter/'.urlencode($letter).'/':'').($pattern!='' ? 'letter/'.urlencode($pattern).'/':'').'callback/'.urlencode($_REQUEST['callback']);


include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';

