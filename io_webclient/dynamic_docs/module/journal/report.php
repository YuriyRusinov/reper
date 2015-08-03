<?
$enable_sort_fields = array(
    'id_comandirovka',
    'name'
);

$from = intval(get_request_variable('from', 0));
$sort = get_request_variable('sort', 'date');



$start_date = get_request_variable('start_date', date('Y-m-d', mktime(0,0,0,date('m'),1,date('Y'))));
$stop_date = get_request_variable('stop_date', date('Y-m-d', mktime(0,0,0,date('m'),date('d'),date('Y'))));

$report_employees = get_request_variable('report_employees', array());
$report = get_request_variable('report', 0);

$errors = array();

if ($report) {
    $start_date = parse_date('{Y}-{m}-{d}', $start_date, false);
    $stop_date = parse_date('{Y}-{m}-{d}', $stop_date, false);

 }


$sort_dir = get_request_variable('sort_dir', 'asc');
if (array_search($sort, $enable_sort_fields)===false) $sort = 'date';
if (array_search($sort_dir, $convert_sort_dirs)===false) $sort_dir = 'asc';
$additional_vars = 'sort/'.urlencode($sort).'/sort_dir/'.urlencode($sort_dir);
  
$where = array();
$arr = array();

if($start_date)
{
	$where[] = ' date > \''.$start_date.'\'';
}
if($stop_date)
{
	$where[] = 'date < \''.$stop_date.'\'';
}


if (count($report_employees)>0) 
{
	
	

	for($i=0;$i<count($report_employees);$i++)
	{
		
		$adr = $report_employees[$i];
		$pieces = explode("-", $adr);
		$arr[] = $pieces[0];
			
	}	
		
	$where[] = 'employee_id IN ('.join(',', $arr).')';
}

$report_employees = array_flip($report_employees);
$arr = array_flip($arr);



if(count($where)>0)
{   
   
	$query = 'SELECT COUNT(*) AS num FROM `'.TABLE_COMANDIROVKA.'`';
	$query.=' WHERE '.join(' AND ', $where);
 	
}
else 
{
	$query = 'SELECT COUNT(*) AS num FROM `'.TABLE_COMANDIROVKA.'`';
}

$db->query($query);


$all_num = $db->value[0]['num'];

$coms = array();
if ($all_num>0) {
	
	if(count($where)>0)
	{	
    	$query = 'SELECT * FROM `'.TABLE_COMANDIROVKA.'`';
    	$query.=' WHERE '.join(' AND ', $where);
    	
    	$query.=' ORDER BY '.$sort.' '.$sort_dir;
        $query.=' LIMIT '.$from.','.LIMIT_COMPANY_ON_PAGE;
	}
	else 
	{
		$query = 'SELECT * FROM `'.TABLE_COMANDIROVKA.'`  ORDER BY `'.$sort.'` '.$sort_dir.' LIMIT '.$from.','.LIMIT_COMPANY_ON_PAGE;
	}
    
    $db->query($query);
    $coms = $db->value;
}


$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_COMPANY_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars);


$all_employees = new collection();
$all_employees->Load(TABLE_EMPLOYEE, false, 'active_flag=\'yes\'');


include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/footer.php';