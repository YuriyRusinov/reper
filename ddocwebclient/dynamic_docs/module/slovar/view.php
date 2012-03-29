<?
$enable_sort_fields = array(
    'id',
    'date'
);


$from = intval(get_request_variable('from', 0));
$sort = get_request_variable('sort', 'id');


$sort_dir = get_request_variable('sort_dir', 'asc');
if (array_search($sort, $enable_sort_fields)===false) $sort = 'id';
if (array_search($sort_dir, $convert_sort_dirs)===false) $sort_dir = 'asc';
$additional_vars = 'sort/'.urlencode($sort).'/sort_dir/'.urlencode($sort_dir);

$pattern = get_request_variable('pattern', '');


$pattern_flag = true;

 //ГГГГ-ММ-ДД


if(empty($pattern_flag))
{
 $pattern_flag = false;
}


if(!ereg("[0-9]{4}-[0-9]{2}-[0-9]{2}",$pattern))
{
	$pattern_flag = false;
}


if($pattern != '' && $pattern_flag == true)
        {
        	$query = 'SELECT COUNT(*) AS num FROM io_objects WHERE date(insert_time) =  \''.db_class::escape_like($pattern).'\';';
        }
        else 
        {

			$query = 'SELECT COUNT(*) AS num  FROM io_objects;';
        }
//echo $query;
$db->query($query);


$all_num = $db->value[0]['num'];

$coms = array();
if ($all_num>0) {
    
	if($pattern != '' && $pattern_flag == true)
        {
			
        	$query = 'SELECT * FROM io_objects WHERE date(insert_time) = \''.db_class::escape_like($pattern).'\' ORDER BY "'.$sort.'" '.$sort_dir.' LIMIT '.LIMIT_COMPANY_ON_PAGE.';';
        }
        else 
        {
        	$query = 'SELECT * FROM io_objects ORDER BY "'.$sort.'" '.$sort_dir.' LIMIT '.LIMIT_COMPANY_ON_PAGE.' OFFSET '.$from.';';
        }
	
	
	$db->query($query);
    $coms = $db->value;
}


$cats = new collection();
$cats->Load(TABLE_CATEGORIES, false, $where, 'name ASC');
$cats = $cats->_collection;


$users = new collection();
$users->Load(TABLE_USERS, false, $where, 'fio ASC');
$users = $users->_collection;

$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_COMPANY_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars);


include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/view.php';
include SITE_FILE_ROOT.'template/footer.php';
