<?
$enable_sort_fields = array(
    'id',
    'date'
);



$from = intval(get_request_variable('from', 0));
$sort = get_request_variable('sort', 'date');

$cur_dls = $_SESSION['PST'];

$sort_dir = get_request_variable('sort_dir', 'asc');
if (array_search($sort, $enable_sort_fields)===false) $sort = 'date';
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
        	$query =  'select count(*) as num from jGetOutMsgs('.$cur_dls.',\''.db_class::escape_like($pattern).'\');';
        }
        else 
        {

			$query = 'select count(*) as num from jGetOutMsgs('.$cur_dls.', NULL);';
        }

$db->query($query);


$all_num = $db->value[0]['num'];

$coms = array();
if ($all_num>0) {
    
	if($pattern != '' && $pattern_flag == true)
        {
			$query =  'SELECT * FROM jGetOutMsgs('.$cur_dls.', \''.db_class::escape_like($pattern).'\');';
        }
        else 
        {
        	$query = 'SELECT * FROM jGetOutMsgs( '.$cur_dls.', NULL )';
        }	
	
	$db->query($query);
    $coms = $db->value;
}

	$empl = new collection();
	$empl->Load(TABLE_DLS, false, $where, 'name ASC');
	$empl = $empl->_collection;
		   
	$cats = new collection();
	$cats->Load(TABLE_OBJECTS, false, $where, 'name ASC');
	$cats = $cats->_collection;


$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_COMPANY_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars);


include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/view_out.php';
include SITE_FILE_ROOT.'template/footer.php';
