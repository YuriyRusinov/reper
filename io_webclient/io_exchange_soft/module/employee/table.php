<?
$ajax_request = (intval(get_request_variable('ajax-request', 0))===1 ? true:false);
$include_header_footer = !$ajax_request;

$enable_sort_fields = array(
'id_employee',
'active_flag',
'photo_flag',
'name',
'department',
'post',
'email',
'office_phone',
'room'
);

$from = intval(get_request_variable('from', 0));
$sort = get_request_variable('sort', 'department');
$sort_dir = get_request_variable('sort_dir', 'asc');
$department_id = get_request_variable('department', 0);
$parent = ($department_id!=0);

if (array_search($sort, $enable_sort_fields)===false) $sort = 'department';
if (array_search($sort_dir, $convert_sort_dirs)===false) $sort_dir = 'asc';
$additional_vars = 'department/'.$department_id.'/sort/'.urlencode($sort).'/sort_dir/'.urlencode($sort_dir).'/';

if($parent)
{
	$query = 'SELECT COUNT(*) AS num FROM '.VIEW_EMPLOYEE.' WHERE  department_id='.$department_id;
}
else
{
	$query = 'SELECT COUNT(*) AS num FROM '.VIEW_EMPLOYEE;
}
$db->query($query);
$all_num = $db->value[0]['num'];


$employees = array();
if($sort!='department' && $all_num>0){
	if($parent)
	{
		$query = 'SELECT * FROM "'.VIEW_EMPLOYEE.'" WHERE department_id='.$department_id.' ORDER BY "'.$sort.'" '.$sort_dir.' LIMIT '.$from.','.LIMIT_EMPLOYEE_ON_PAGE;
	}
	else
	{
		$query = 'SELECT * FROM "'.VIEW_EMPLOYEE.'"  ORDER BY "'.$sort.'" '.$sort_dir.' LIMIT '.$from.','.LIMIT_EMPLOYEE_ON_PAGE;
	}
	$db->query($query);
	$employees = $db->value;
	$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_EMPLOYEE_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars, 'Aclick');
}
else
{
	function get_employee_in_dep($department_id,$sort_dep_dir='asc'){
		global $db;
		$where=($department_id > 0 ? '"department_id"='.$department_id : 'department_id IS NULL');
		$query = 'SELECT * FROM "'.VIEW_EMPLOYEE.'" WHERE '.$where.'  ORDER BY name ASC';
		$db->query($query);
		$employees = $db->value;
		$where=($department_id > 0 ? 'parent_id='.$department_id : 'parent_id IS NULL');
		$query = 'SELECT * FROM "'.TABLE_DEPARMENT.'" WHERE '.$where.' ORDER BY name '.$sort_dep_dir;
		$db->query($query);
		$departments = $db->value;
		for ($i=0;$i<count($departments);$i++) {
			$employees = array_merge ($employees,get_employee_in_dep($departments[$i]['id_department'],$sort_dep_dir));
		}
		return $employees;
	}

	$employees = get_employee_in_dep($department_id,$sort_dir);
	$employees = array_slice($employees,$from,LIMIT_EMPLOYEE_ON_PAGE);
	$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_EMPLOYEE_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars, 'Aclick');
}

$dep = new department();
$emp = new employee();
for ($i=0; $i<count($employees); $i++) {
    $dep->Init();
    $dep->id_department=$employees[$i]['department_id'];
    $dep->Load();
	$n=0;
	$employees[$i]['department']=array();
	$employees[$i]['department'][$n]['dep']=$dep->name;
	if($dep->head_id!=0){
		$emp->id_employee=$dep->head_id;
		$emp->Load();
		$employees[$i]['department'][$n]['head']=$emp->name;
	}
	else
	{
		$employees[$i]['department'][$n]['head']='';
	}
	while($dep->parent_id!=0){
		$n++;
		$dep->id_department=$dep->parent_id;
		$dep->Load();
		$employees[$i]['department'][$n]['dep']=$dep->name;
		if($dep->head_id!=0){
			$emp->id_employee=$dep->head_id;
			$emp->Load();
			$employees[$i]['department'][$n]['head']=$emp->name;
		}
		else
		{
			$employees[$i]['department'][$n]['head']='';
		};
	}
}


$rows_conf = new rows_conf;
$rows_conf->id_employee = $STORAGE['current_employee']->id_employee;
$rows_conf->table_name = 'employee';
$rows_conf->Load();

$columns = $rows_conf->columns;

$id = 'yes';
$active_flag = 'yes';
$photo = 'yes';
$name = 'yes';
$department = 'yes';
$post = 'yes';
$email = 'yes';
$office_phone = 'yes';
$room = 'yes';

if(strlen($columns) > 0)
{
	if(substr_count($columns,'id') > 0)
	{
		$id = 'no';
	}
	if(substr_count($columns,'active_flag') > 0)
	{
		$active_flag = 'no';
	}
	if(substr_count($columns,'photo') > 0)
	{
		$photo = 'no';
	}
	if(substr_count($columns,'name') > 0)
	{
		$name = 'no';
	}
	if(substr_count($columns,'department') > 0)
	{
		$department = 'no';
	}
	if(substr_count($columns,'post') > 0)
	{
		$post = 'no';
	}
	if(substr_count($columns,'email') > 0)
	{
		$email = 'no';
	}
	if(substr_count($columns,'office_phone') > 0)
	{
		$office_phone = 'no';
	}
	if(substr_count($columns,'room') > 0)
	{
		$room = 'no';
	}
}


$is_enabled_employee_edit=access_enable($STORAGE, $STORAGE['module'], 'edit');
$is_enabled_employee_delete=access_enable($STORAGE, $STORAGE['module'], 'delete');
$is_enabled_rows_conf=access_enable($STORAGE, $STORAGE['module'], 'rows_conf');

if ($include_header_footer) include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
if ($include_header_footer) include SITE_FILE_ROOT.'template/footer.php';
