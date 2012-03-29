<?
$page_title = 'Изменение данных подразделения';

$id = intval(get_request_variable('id', 0));
$parent_id = intval(get_request_variable('parent_id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$department = new department();
$department->id_department = $id;
if ($id > 0) {
    if ($department->Load()) {
        if ($parent_id===0) $parent_id = null;
        $department->parent_id = $parent_id;
        $errors = $department->IsValidData();
        if (!$errors) {
            $department->Save();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
