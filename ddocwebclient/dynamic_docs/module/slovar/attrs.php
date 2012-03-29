<?
$enable_sort_fields = array(
    'id'
);


$from = intval(get_request_variable('from', 0));
$id = intval(get_request_variable('id', 0));
$sort = get_request_variable('sort', 'date');


$sort_dir = get_request_variable('sort_dir', 'asc');

if (array_search($sort_dir, $convert_sort_dirs)===false) $sort_dir = 'asc';
$additional_vars = 'sort/'.urlencode($sort).'/sort_dir/'.urlencode($sort_dir);

$pattern = get_request_variable('pattern', '');


if($pattern != '')
        {
        	$query = 'SELECT COUNT(*) AS num FROM iogetobjectattrsall('.$id.');';
        }
        else 
        {

			$query = 'SELECT COUNT(*) AS num  FROM iogetobjectattrsall('.$id.');';
        }

$db->query($query);
//echo $query;

$all_num = $db->value[0]['num'];

$coms = array();
if ($all_num>0) {
    
	if($pattern != '')
        {
			
        	$query = 'SELECT * FROM iogetobjectattrsall('.$id.') ORDER BY 1;';
        }
        else 
        {
        	$query = 'SELECT * FROM iogetobjectattrsall('.$id.') ORDER BY 1;';
        }
	
	//echo $query;
	$db->query($query);
    $coms = $db->value;
}


$cats = new collection();
$cats->Load(TABLE_ATYPES, false, $where, 'name ASC');
$cats = $cats->_collection;


$names = new collection();
$names->Load(TABLE_ATTRIBUTES, false, $where, 'name ASC');
$names = $names->_collection;

$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_COMPANY_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars);


include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/attrs.php';
include SITE_FILE_ROOT.'template/footer.php';
