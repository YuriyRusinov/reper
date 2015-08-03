<?
$page_title = 'Выбор должностей';
include SITE_FILE_ROOT.'template/simple_header.php';

$sort = 'name';
$sort_dir = 'asc';
$pattern = get_request_variable('pattern', '');
$letter = get_request_variable('letter', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$where = array();
if ($pattern!='') {
    $where[] = '"name" LIKE \'%'.db_class::escape_like($pattern).'%\'';
    $letter = '';
} else {
    if ($letter!='') $where[] = '"name" LIKE \''.db_class::escape_like($letter).'%\'';
}

$offices = new collection();
$offices->Load(TABLE_OFFICE, false, join(' AND ', $where), '"'.$sort.'" '.$sort_dir);
$offices = $offices->_collection;

$additional_vars = ($letter!='' ? 'letter/'.urlencode($letter).'/':'').($pattern!='' ? 'letter/'.urlencode($pattern).'/':'').'callback/'.urlencode($_REQUEST['callback']);


include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
