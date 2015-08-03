<?
$page_title = 'Удаление подразделения';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$department = new department();
$department->id_department = $id;
if ($id > 0) {
    if ($department->Load()) {
        $errors = $department->IsValidData();
        if (!$errors) {
            $department->Delete();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
