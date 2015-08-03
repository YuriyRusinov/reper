<?
$page_title = 'Редактирование данных подразделения';

$id = intval(get_request_variable('id', 0));
$parent_id = intval(get_request_variable('parent_id', 0));
$head_id = intval(get_request_variable('head_id', 0));
$prefix = get_request_variable('prefix', '');
$description = get_request_variable('description', '');
$name = get_request_variable('name', '');
$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$department = new department();
$department->id_department = $id;
if ($id > 0) {
    if (!$department->Load()) {
        $department->id_department = 0;
    }
}
$department->parent_id = $parent_id;

if ($save) {
	$department->head_id = $head_id;
    if ($department->parent_id === 0) $department->parent_id = null;
    if ($department->head_id === 0) $department->head_id = null;
    $department->name = $name;
    $department->prefix = $prefix;
    $department->description = $description;

    $errors = $department->IsValidData();
    if (count($errors)==0) {
        $department->Save();
    }
}

$employees = new collection();
$employees->Load(TABLE_EMPLOYEE, false, '', 'name ASC');
$employees = $employees->_collection;

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
