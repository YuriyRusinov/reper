<?
$enable_sort_fields = array(
    'id',
    'date'
);


$from = intval(get_request_variable('from', 0));
$sort = get_request_variable('sort', 'date');


$id = intval(get_request_variable('id', 0));

$sort_dir = get_request_variable('sort_dir', 'asc');
if (array_search($sort, $enable_sort_fields)===false) $sort = 'date';
if (array_search($sort_dir, $convert_sort_dirs)===false) $sort_dir = 'asc';
$additional_vars = 'sort/'.urlencode($sort).'/sort_dir/'.urlencode($sort_dir);

$pattern = get_request_variable('pattern', '');


if($pattern != '')
        {
        	$query = 'SELECT COUNT(*) AS num FROM iogetlog('.$id.') WHERE date(insert_time) \''.db_class::escape_like($pattern).'\';';
        }
        else 
        {

			$query = 'SELECT COUNT(*) AS num  FROM iogetlog('.$id.');';
        }

$db->query($query);


$all_num = $db->value[0]['num'];

$coms = array();
if ($all_num>0) {
    
	if($pattern != '')
        {
			
        	$query = 'SELECT * FROM iogetlog('.$id.') WHERE date(insert_time) = \''.db_class::escape_like($pattern).'\';';
        }
        else 
        {
        	$query = 'SELECT * FROM iogetlog('.$id.') ;';
        }
	
	
	$db->query($query);
    $coms = $db->value;
}

//TO DO - load operation type from bd
$cats = new collection();
$cats->Load(TABLE_CATEGORIES, false, $where, 'name ASC');
$cats = $cats->_collection;




$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_COMPANY_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars);


include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/logs.php';
include SITE_FILE_ROOT.'template/footer.php';
