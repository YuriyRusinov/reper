<?
include SITE_FILE_ROOT.'template/header.php';

$enable_sort_fields = array(
    'id_role',
    'name',
);

$from = intval(get_request_variable('from', 0));
$sort = get_request_variable('sort', 'name');
$sort_dir = get_request_variable('sort_dir', 'asc');
if (array_search($sort, $enable_sort_fields)===false) $sort = 'name';
if (array_search($sort_dir, $convert_sort_dirs)===false) $sort_dir = 'asc';
$additional_vars = 'sort/'.urlencode($sort).'/sort_dir/'.urlencode($sort_dir);

$roles = new collection();
$all_num = $roles->Load(TABLE_ROLE, true, '', '"'.$sort.'" '.$sort_dir, $from, LIMIT_ROLES_ON_PAGE);

$page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, LIMIT_ROLES_ON_PAGE, LIMIT_PAGE_ON_PAGE, $additional_vars);

include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/footer.php';
