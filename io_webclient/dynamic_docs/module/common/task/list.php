<?

$enable_sort_fields = array(
    'name',
    'stop_date',
    'priority',
    'responsible_name'
);


$from = get_request_variable('from', 0);

$sort = get_request_variable('sort', null);
$sort_dir = get_request_variable('sort_dir', null);
if (is_null($sort)) $sort = $default_sort;
if (is_null($sort_dir)) $sort_dir = $default_sort_dir;

$ajax_request = get_request_variable('ajax-request', 0);
if ($ajax_request) {
    $from = 0;
    $limit = 5;
}

$db = db_class::get_instance();

$query = 'SELECT '.VIEW_TASK.'.*';
$query .=' FROM '.VIEW_TASK;
if (isset($join) && is_array($join)) $query.=' '.join(' ', $join);
if (is_array($where) && count($where)>0) $query.=' WHERE '.join(' AND ', $where);

$query_count = str_replace('SELECT '.VIEW_TASK.'.*', 'SELECT COUNT(*) AS num', $query);
$query_count = $db->query($query_count);
$all_num = $db->value[0]['num'];

if ($show_get_in_work_check) {
    if (isset($show_spent_hour) && $show_spent_hour) {
        $query = str_replace('SELECT '.VIEW_TASK.'.*', 'SELECT '.VIEW_TASK.'.*, IF ((SELECT task_id FROM '.TABLE_TASK_WORK.' WHERE task_id=id_task AND employee_id='.$in_work_employee.' AND date=\''.$in_work_date.'\') IS NULL, 0, 1) AS in_work, '.TABLE_TASK_WORK.'.spent_hour', $query);
    } else {
        $query = str_replace('SELECT '.VIEW_TASK.'.*', 'SELECT '.VIEW_TASK.'.*, IF ((SELECT task_id FROM '.TABLE_TASK_WORK.' WHERE task_id=id_task AND employee_id='.$in_work_employee.' AND date=\''.$in_work_date.'\') IS NULL, 0, 1) AS in_work', $query);
    }
}
//$query = str_replace('SELECT '.VIEW_TASK.'.*', 'SELECT '.VIEW_TASK.'.*, IF ('.TABLE_TASK_WORK.'.task_id IS NULL, 0, 1) AS in_work, '.TABLE_TASK_WORK.'.spent_hour', $query);

$additional_vars = '';
if (isset($sort) && isset($enable_sort_fields) && is_array($enable_sort_fields) && array_search($sort, $enable_sort_fields)!==false) {
    $query.=' ORDER BY '.$sort;
    if (isset($sort_dir) && array_search($sort_dir, $convert_sort_dirs)!==false) $query.=' '.$sort_dir;
    $additional_vars = 'sort/'.urlencode($sort).'/sort_dir/'.urlencode($sort_dir);
} else {
    if (isset($default_sort)) $query.=' ORDER BY '.$default_sort;
    if (isset($default_sort_dir)) $query.=' '.$default_sort_dir;
}

$page_break = '';
if (isset($from)) {
    $query.=' LIMIT '.$from;
    if (isset($limit)) {
        $query.=','.$limit;
        $page_break = get_page_break(SITE_URI_ROOT.$STORAGE['module'].'/'.$STORAGE['action'].'/', $all_num, $from, $limit, LIMIT_PAGE_ON_PAGE, $additional_vars);
    }
}

if ($all_num>0) {
    $db->query($query);
} else {
    $db->value = array();
}
$tasks = $db->value;

if (!$ajax_request) {
    include SITE_FILE_ROOT.'template/header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
    include SITE_FILE_ROOT.'template/footer.php';
} else {
    //include SITE_FILE_ROOT.'template/simple_header.php';
    include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/simple-'.$STORAGE['action'].'.php';
    //include SITE_FILE_ROOT.'template/simple_footer.php';
}

