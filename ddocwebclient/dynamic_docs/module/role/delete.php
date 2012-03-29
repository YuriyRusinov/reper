<?
$page_title = 'Удаление роли';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$role = new role();
$role->id_role = $id;
if ($id > 0) {
    if ($role->Load()) {
        $errors = $role->IsValidData();
        if (!$errors) {
            $role->Delete();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
